/*
 * clamir.cpp
 *
 *  Created on: 27 dec. 2024
 *      Author: arthur
 */

#include <exception>

#include "config.h"
#include "commandaddr.h"
#include "stdio.h"
#include "error.h"
#include "metadata.h"
#include "process_variables.h"

#include <iostream>
#include <thread>
#include <vector>

#include <gst/gst.h>

#define VERSION 0x0002 // Negated alarm digital output and ignore digital output 3 command for semaphore. Made for demo.

void SIGPIPE_handler(int s)
{
	printf("Caught SIGPIPE\n");
}

// Variables globales
pthread_mutex_t mutex;
int logging;
int stop_logging;

void memory_initialize(config_data_t config_data, process_variables_t *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_nit_command);
void controller_loop(metadata_t *metadata, process_variables_t *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_reset, volatile int *shm_nit_command);
int read_serial_number(const char *path, process_variables_t *process_variables);
int read_bpcc_table(const char *path);
void command_host(int newsockfd, volatile int *shm_nit_command, process_variables_t *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_reset);
void image_writer(int newsockimgfd, metadata_t *metadata, volatile int *shm_nit_command);

int main(int argc, char *argv[])
{
	// Shared memory para metadatos, memoria virtual no asociada a ninguna BRAM que no usa el driver de devmem
	//{Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20, Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status, Width}
	// volatile int *shm_metadata = NULL;
	// shm_metadata = (volatile int *)mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	metadata_t metadata;

	// Variables no escritas en la FPGA
	// volatile int *shm_proc_var = NULL;
	// shm_proc_var = (volatile int *)mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	process_variables_t process_variables;

	// Bancos de AIMEN
	int fd_dev_mem;
	fd_dev_mem = open("/dev/mem", O_RDWR | O_SYNC);

	if (fd_dev_mem < 0) {
		printf("Failed to open /dev/mem for %d\n", NIT_MB_CORE_BASE_ADDRESS);
	}

	volatile int *shm_ai_men_com = NULL;
	shm_ai_men_com = (volatile int *)mmap(NULL, 5242888, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dev_mem, NIT_MB_CORE_BASE_ADDRESS);

	// Shared memory, lee y escribe comandos de NIT en la FPGA de configuraciones relativas de la c�mara
	int fd_dev_mem_2;
	fd_dev_mem_2 = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd_dev_mem_2 < 0)
	{
		printf("Failed to open /dev/mem\n");
		exit(-1);
	}
	volatile int *shm_nit_command = NULL;
	shm_nit_command = (volatile int *)mmap(NULL, BRAM_NIT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dev_mem_2, NIT_CONTROL_UNIT_BASE_ADDRESS);

	/*
	 * Mapeado de memoria para reset
	 */
	int fd_dev_mem_3;
	fd_dev_mem_3 = open("/dev/mem", O_RDWR | O_SYNC);
	volatile int *shm_reset = NULL;
	shm_reset = (volatile int *)mmap(NULL, 131072, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dev_mem_3, NIT_ARM_CORE_BASE_ADDRESS);

	/*
	 * Load Serial Number
	 */
	if (!read_serial_number("/etc/clamir/serial_number.conf", &process_variables))
	{
		printf("Couldn't read serial number from file %s", "/etc/clamir/serial_number.conf");
	}

	/*
	 * Load BPC table
	 */
	if (read_bpcc_table("/etc/clamir/bpcc_table.conf"))
	{
		shm_nit_command[NIT_BPC_EN] = 1;
	}

	/*
	 * Inicializacion de los valores de CLAMIR
	 */
	config_data_t config_data;
	config_data = config_file_read(config_data);
	memory_initialize(config_data, &process_variables, shm_ai_men_com, shm_nit_command);

	signal(SIGPIPE, SIGPIPE_handler);

	auto log_thread_function = [&]()
	{
		for (;;)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	};

	auto control_thread_function = [&]()
	{
		for (;;)
		{
			controller_loop(&metadata, &process_variables, shm_ai_men_com, shm_reset, shm_nit_command);
			std::cout << "Closed Loop Control: Closed" << std::endl;
		}
	};

	auto server_command_host_thread_function = [&]()
	{
		int sockfd, newsockfd, port, sockimgfd, newsockimgfd, portimg;

		socklen_t clilen, cliimglen;
		struct sockaddr_in serv_addr, cli_addr, serv_addr_img, cli_addr_img;

		/*
		 * Inicializacion del host TCP
		 */
		int set_reuseaddress = 1;

		/*
		 *	Socket de Puerto de comunicacion de comandos
		 */

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			printf("ERROR opening socket");
			exit(-1);
		}

		bzero((char *)&serv_addr, sizeof(serv_addr));

		port = 4097;
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set_reuseaddress, sizeof(set_reuseaddress)) < 0)
		{
			printf("Unable to use REUSEADDR 1");
			exit(-1);
		}

		if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			printf("ERROR on binding 1");
			exit(-1);
		}

		listen(sockfd, 5);

		for (;;)
		{

			auto sock = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

			if (sock < 0)
			{
				std::cout << "Command Host: Couldn't Accept a New Connection." << std::endl;
				continue;
			}

			command_host(sock, shm_nit_command, &process_variables, shm_ai_men_com, shm_reset);
			close(sock);
		}
	};

	auto server_video_stream_host_thread_function = [&]()
	{
		int sockfd, port, portimg;

		socklen_t clilen, cliimglen;
		struct sockaddr_in serv_addr, cli_addr, serv_addr_img, cli_addr_img;

		/*
		 * Inicializacion del host TCP
		 */
		int set_reuseaddress = 1;

		/*
		 *	Socket de Puerto de comunicacion de comandos
		 */

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			printf("ERROR opening socket");
			exit(-1);
		}

		bzero((char *)&serv_addr, sizeof(serv_addr));

		port = 4096;
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set_reuseaddress, sizeof(set_reuseaddress)) < 0)
		{
			printf("Unable to use REUSEADDR 1");
			exit(-1);
		}

		if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			printf("ERROR on binding 1");
			exit(-1);
		}

		listen(sockfd, 5);

		for (;;)
		{

			auto sock = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

			if (sock < 0)
			{
				std::cout << "Video Stream: Couldn't Accept a New Connection." << std::endl;
				exit(-1);
			}

			image_writer(sock, &metadata, shm_nit_command);
		}
	};

	auto log_thread = std::thread(log_thread_function);
	auto control_thread = std::thread(control_thread_function);
	auto server_command_host_thread = std::thread(server_command_host_thread_function);
	auto server_video_stream_host_thread = std::thread(server_video_stream_host_thread_function);

	log_thread.join();
	control_thread.join();
	server_command_host_thread.join();
	server_video_stream_host_thread.join();
	return EXIT_SUCCESS;
}

/*
 * Funcion que inicializa las memorias con la estructura de datos obtenida de un fichero.
 * En caso de conflicto prioriza los Maximos a los Minimos.
 */
void memory_initialize(config_data_t config_data, process_variables_t *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_nit_command)
{

	uint64_t time_track_aux = 0;

	// inicializacion de la memoria mapeada digitalmente shm_proc_var

	shm_proc_var->ki = config_data.ki;
	shm_proc_var->kp = config_data.kp;
	shm_proc_var->kd = config_data.kd;
	if (config_data.max_power < config_data.min_power)
	{
		config_data.min_power = config_data.max_power - 1;
	}
	shm_proc_var->max_power = config_data.max_power;
	shm_proc_var->min_power = config_data.min_power;
	shm_proc_var->power_man = config_data.power;
	if (config_data.max_power_limit < config_data.min_power_limit)
	{
		config_data.min_power_limit = config_data.max_power_limit - 1;
	}
	shm_proc_var->power_limit_max = config_data.max_power_limit;
	shm_ai_men_com[PWM_LIMIT_MAX] = (unsigned int)((double)config_data.max_power_limit - (double)shm_proc_var->min_power) * (16383 / ((double)shm_proc_var->max_power - (double)shm_proc_var->min_power));
	shm_proc_var->power_limit_min = config_data.min_power_limit;
	shm_ai_men_com[PWM_LIMIT_MIN] = (unsigned int)((double)config_data.min_power_limit - (double)shm_proc_var->min_power) * (16383 / ((double)shm_proc_var->max_power - (double)shm_proc_var->min_power));
	shm_proc_var->width_ref = config_data.width_manual;
	shm_proc_var->set_ref_width = 1; // Cuando comience el hilo de procesamiento de control se tomara el ancho de referencia
	shm_proc_var->pixel_mm_ratio = config_data.pixel_mm_ratio;
	shm_proc_var->end_of_process = config_data.end_of_process;
	shm_proc_var->limit_integral = config_data.limit_integral;
	shm_proc_var->limit_slew = config_data.limit_slew;
	shm_proc_var->buff_size = config_data.circular_buffer_size;
	shm_proc_var->enable_alarm = config_data.alarm_enable;
	if (config_data.alarm_max < config_data.alarm_min)
	{
		config_data.alarm_min = config_data.alarm_max - 1;
	}
	shm_proc_var->alarm_max = config_data.alarm_max;
	shm_proc_var->alarm_min = config_data.alarm_min;
	shm_proc_var->alarm_time = config_data.alarm_time;
	shm_proc_var->automeasure = config_data.automeasure;
	shm_proc_var->autoshutter_config = config_data.autoshutter;
	shm_proc_var->autoshutter_temp = config_data.drift_temp_autoshutter;
	shm_proc_var->autoshutter_timer = config_data.timer_autoshutter;
	shm_proc_var->track_ref_start = config_data.track_ref_start;
	shm_proc_var->laser_exteral_control = config_data.laser_external_control;
	shm_proc_var->delay_laser_on = config_data.delay_laser_on;
	shm_proc_var->preheating_ena = config_data.preheating_ena;
	shm_proc_var->preheating_time = config_data.preheating_time;
	shm_proc_var->preheating_power = config_data.preheating_power;

	// inicializacion de la memoria Baseaddress aimen command

	if (config_data.start_track_mom < config_data.stop_track_mom)
	{
		config_data.stop_track_mom = config_data.start_track_mom - 1;
	}
	shm_ai_men_com[START_TRACK_MOM_T] = config_data.start_track_mom;
	shm_ai_men_com[END_OF_TRACK] = config_data.stop_track_mom;
	shm_ai_men_com[MODE] = config_data.mode;
	shm_ai_men_com[REFERENCE_TRACK] = config_data.track_reference;

	time_track_aux = (uint64_t)(((double)config_data.track_length) * (CLK_100MHZ / 10));
	shm_ai_men_com[TIME_TRACK_LOW] = (unsigned int)(0x00000000FFFFFFFF & time_track_aux);
	shm_ai_men_com[TIME_TRACK_HIGH] = (unsigned int)((0xFFFFFFFF00000000 & time_track_aux) >> 32);

	shm_ai_men_com[THRESHOLD] = config_data.threshold;
	shm_ai_men_com[ROI_ROUND] = config_data.roi_round;
	shm_ai_men_com[ENABLE_ROI] = config_data.roi_enable;
	if (config_data.x2_pixel < config_data.x1_pixel)
	{
		config_data.x2_pixel = config_data.x1_pixel;
	}
	shm_ai_men_com[ROI_X1] = config_data.x1_pixel;
	shm_ai_men_com[ROI_X2] = config_data.x2_pixel;
	if (config_data.y2_pixel < config_data.y1_pixel)
	{
		config_data.y2_pixel = config_data.y1_pixel;
	}
	shm_ai_men_com[ROI_Y1] = config_data.y1_pixel;
	shm_ai_men_com[ROI_Y2] = config_data.y2_pixel;
	shm_ai_men_com[DIGITAL_OUT_CONF] = config_data.digitalio;

	// Inicializacion de la memoria Baseaddress NIT command
	shm_nit_command[NIT_BLACK_LEVEL] = config_data.black_level;
	shm_nit_command[NIT_BIAS_V] = config_data.bias_voltage;
	shm_nit_command[NIT_INT_TIME] = config_data.integration_time;

	// printf("Drift enable:		%d\n", config_data.ena_drift);
	// printf("Drift intensity:	%d\n", config_data.drift_intensity);
	shm_nit_command[NIT_DRIFT_ENABLE] = config_data.ena_drift;
	shm_nit_command[NIT_DRIFT_POSITION] = config_data.drift_intensity;

	shm_ai_men_com[CHANGE_OP_MODE] = 1; // Indica a la memoria el cambio al nuevo modo al finalizar la inicializacion.
	shm_proc_var->auto_shutter = 1;		// realizará un autoshutter con la nueva configuracion en cuanto se inicie el proceso de control
}

int read_serial_number(const char *path, process_variables_t *process_variables)
{
	char serial_number[8];
	FILE *fd = nullptr;
	int retval = 0;

	fd = fopen(path, "r");

	if (fd == nullptr)
	{
		return retval;
	}

	if (fscanf(fd, "%s", serial_number) != 0)
	{
		printf("*** Serial Number %s ***\n", serial_number);
		process_variables->serial_number_low = 0x00000000;
		process_variables->serial_number_high = 0x00000000;
		retval = 1;
	}

	fclose(fd);

	return retval;
}

/*
 * Function to load a BPC table and write its contents into the CLAMIR system
 */
int read_bpcc_table(const char *path)
{
	int resultado = 0;
	int fd_bpcc_table_dev;
	volatile int *shm_bpcc_table_dev = nullptr;

	FILE *fd_bpcc_table_config = fopen(path, "r");
	if (fd_bpcc_table_config == nullptr)
	{
		return 0;
	}

	fd_bpcc_table_dev = open("/dev/mem", O_RDWR | O_SYNC);

	if (fd_bpcc_table_dev < 0) {
		fclose(fd_bpcc_table_config);
		return 0;
	}

	shm_bpcc_table_dev = (volatile int *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd_bpcc_table_dev, NIT_BPCC_TABLE_BASE_ADDRESS);

	if (shm_bpcc_table_dev == nullptr)
	{
		fclose(fd_bpcc_table_config);
		close(fd_bpcc_table_dev);
		return 0;
	}

	int bpcc_y, bpcc_x;

	int index = 0;
	while (fscanf(fd_bpcc_table_config, "%d %d", &bpcc_y, &bpcc_x) == 2)
	{
		shm_bpcc_table_dev[index] = (bpcc_y << 8) | (bpcc_x);
		index++;
	}

	close(fd_bpcc_table_dev);
	munmap((int *)shm_bpcc_table_dev, 4096);
	shm_bpcc_table_dev = NULL;
	return 1;
}

/*
 * Funcion de guardado de ficheros LOG
 */
void *Logger(void *sharedMetadatos)
{
	int *metaLog = (int *)sharedMetadatos;
	int bufferMeta[15];
	uint16_t header_v_id[2] = {1, 0};
	uint16_t num;
	uint16_t num_folder = 0;
	DIR *directory;
	struct dirent *dir;
	directory = opendir("/mnt/mmc/log");
	if (directory)
	{
		while ((dir = readdir(directory)) != NULL)
		{
			if ((dir->d_type != DT_UNKNOWN) && (dir->d_type == DT_DIR))
			{
				if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0))
				{
					continue;
				}
				else
				{
					sscanf(dir->d_name, "%hd", &num);
					if (num > num_folder)
					{
						num_folder = num;
					}
				}
			}
		}
		printf("Mayor identificador de directorio: %hd \n", num_folder);
	}
	else
	{
		printf("Fallo al abrir la direccion de log \n");
	}
	FILE *fdLOG = NULL;
	char logpath[30];
	memset(logpath, '\0', sizeof(logpath)); // inicializa el path a vacio
	num_folder++;
	sprintf(logpath, "/mnt/mmc/log/%05d", num_folder);
	mkdir(logpath, 0700);
	// log_version = 1;
	while (1)
	{
		pthread_mutex_lock(&mutex);
		memcpy(bufferMeta, metaLog, 60);
		if (logging == 0)
		{
			header_v_id[1]++;
			sprintf(logpath, "/mnt/mmc/log/%05d/%05d.log", num_folder, header_v_id[1]);
			fdLOG = fopen(logpath, "w");
			if (fdLOG != NULL)
			{
				logging = 1;
				/*
				 * escritura de la cabecera
				 */
				fwrite(header_v_id, sizeof(uint16_t), 2, fdLOG); // Version & ID_PROC
				fwrite(bufferMeta + 11, sizeof(int), 2, fdLOG);	 // Frame Number and Timestamp
				// printf("Frame inicio: %d --- 0x%x\n",bufferMeta[11], bufferMeta[11]);
				/*
				 * escritura de la primera imagen
				 */
				fwrite(bufferMeta, sizeof(int), 2, fdLOG);		// Power y MOM00
				fwrite(bufferMeta + 7, sizeof(int), 3, fdLOG);	// Widths, track number y frame max
				fwrite(bufferMeta + 12, sizeof(int), 2, fdLOG); // IO status y Temp1(sin calcular)
			}
			else
			{
				header_v_id[1]--;
				printf("Unable to save log files\n");
			}
		}
		else
		{
			if (stop_logging == 1)
			{
				/*
				 * Finaliza el fichero de logging
				 */
				fclose(fdLOG);
				memset(logpath, '\0', sizeof(logpath));
				logging = 0;
				stop_logging = 0;
			}
			else
			{
				/*
				 * escritura de datos
				 */
				fwrite(bufferMeta, sizeof(int), 2, fdLOG);		// Power y MOM00
				fwrite(bufferMeta + 7, sizeof(int), 3, fdLOG);	// Widths, track number y frame max
				fwrite(bufferMeta + 12, sizeof(int), 2, fdLOG); // IO status y Temp1(sin calcular)
			}
		}
	}
	pthread_cancel(pthread_self());
	return NULL;
}

/*
 * Funci�n de escritura de imagenes desde el CLAMIR
 */
void image_writer(int newsockimgfd, metadata_t *metadata, volatile int *shm_nit_command)
{
	int frame_counter = 0;
	int cont = 0;
	int status1 = 0;
	int status2 = 0;
	int nw = 0;
	float lVoltage, lResistance, auxTemp;

	metadata_t _metadata;
	unsigned char frame_sync[] = {0x5f, 0x00, 0x48, 0xf6, 0x70, 0x44, 0x94, 0xee};

	sem_t *semaforo;

	// Mapeado en memoria de la BRAM donde esta la imagen
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
	volatile int16_t *img = NULL;
	img = (volatile int16_t *)mmap(NULL, NIT_IMAGE_BRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, NIT_IMAGE_BRAM_BASE_ADDRESS);

	semaforo = sem_open(SEM_NAME, 0);
	_metadata = *metadata;

	/*Inicializacion de los metadatos de temperatura*/
	lVoltage = 3.0f / 4096.0f * (float)shm_nit_command[NIT_TEMP1]; // temp1
	lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
	auxTemp = (3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f; // Translate to deg C assuming B=3380
	_metadata.t1 = (int)(auxTemp * 10);

	lVoltage = 3.0f / 4096.0f * (float)shm_nit_command[NIT_TEMP2];
	lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
	auxTemp = (3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f; // Translate to deg C assuming B=3380
	_metadata.t2 = (int)(auxTemp * 10);

	while (nw >= 0)
	{

		sem_getvalue(semaforo, &status1);
		sem_wait(semaforo); // Bloqueo por semaforo, es el lazo de control el que lo desbloquea
		sem_getvalue(semaforo, &status2);

		_metadata = *metadata;

		cont = 0;
		lVoltage = 3.0f / 4096.0f * (float)shm_nit_command[NIT_TEMP1]; // temp1
		lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
		_metadata.t1 = (int)(10 * ((3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f)); // Translate to deg C assuming B=3380

		lVoltage = 3.0f / 4096.0f * (float)shm_nit_command[NIT_TEMP2];
		lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
		_metadata.t2 = (int)(10 * ((3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f)); // Translate to deg C assuming B=3380

		if ((_metadata.io_status & 0x00000004) > 0)
		{
			_metadata.io_status = (_metadata.io_status & 0xFFFFFFFB);
		}
		else
		{
			_metadata.io_status = (_metadata.io_status | 0x00000004);
		}

		nw = write(newsockimgfd, frame_sync, sizeof(frame_sync));
		if (nw < 0)
		{
			printf("ERROR writing image to socket (frame sync)\n");
			break;
		}

		nw = write(newsockimgfd, (unsigned char *)metadata, sizeof(metadata_t));

		if (nw < 0)
		{
			printf("ERROR writing image to socket (header)\n");
			break;
		}

		nw = write(newsockimgfd, (unsigned char *)img, NIT_IMAGE_BRAM_SIZE);
		if (nw < 0)
		{
			printf("ERROR writing image to socket (image)\n");
			break;
		}
	}
	close(fd);
	munmap((int16_t *)img, BRAM_NIT_SIZE);
	img = NULL;
}

/*
 * Función de gestion de comandos
 */
void command_host(int newsockfd, volatile int *shm_nit_command, process_variables_t *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_reset)
{
	int nr, nw = 1;
	uint64_t time_track_aux;
	int cont_tramas_vacias = 0;
	uint16_t buffer[2];
	gestion_comandos gest;

	/*
	 * Bucle de lectura
	 */
	while ((nr >= 0) && (cont_tramas_vacias < 3) && (nw >= 0))
	{

		bzero(buffer, 4);

		nr = recv(newsockfd, buffer, sizeof(buffer), MSG_WAITALL);

		if (nr < 0)
			printf("ERROR reading from socket");
		if (nr <= 0)
		{
			cont_tramas_vacias++;
			// printf("nr: %d, contador: %d\n", nr, cont_tramas_vacias);
		}
		else
		{
			// printf("Recibido address-command 0x%x, data %d\n",buffer[0],buffer[1]);
			gest = command_manager(buffer[0]);
			if (gest.fpga_write == 0)
			{
				if (gest.lectura > 127)
				{ // lectura
					if (gest.offset == SERIAL_NUMBER_LOW)
					{
						buffer[1] = (uint16_t)(shm_proc_var->serial_number_low & 0x0000FFFF);
						nw = write(newsockfd, buffer, 4);
						buffer[1] = (uint16_t)((shm_proc_var->serial_number_low & 0xFFFF0000) >> 16);
						buffer[0] = 0x04AB;
						nw = write(newsockfd, buffer, 4);
						buffer[1] = (uint16_t)(shm_proc_var->serial_number_high & 0x0000FFFF);
						buffer[0] = 0x04AC;
						nw = write(newsockfd, buffer, 4);
						buffer[1] = (uint16_t)((shm_proc_var->serial_number_high & 0xFFFF0000) >> 16);
						buffer[0] = 0x04AD;
						nw = write(newsockfd, buffer, 4);
					}
					else
					{
						// this is the problem how am I am going to get the process offset from a packed struct
						buffer[1] = (uint16_t)(((int *)shm_proc_var)[gest.offset] & 0x0000FFFF);
						nw = write(newsockfd, buffer, 4);
					}
				}
				else
				{ // escritura de datos en memoria
					switch (gest.offset)
					{
					case POWER_LIMIT_MAX:
						((int *)shm_proc_var)[gest.offset] = buffer[1];
						shm_ai_men_com[PWM_LIMIT_MAX] = (unsigned int)((double)buffer[1] - (double)shm_proc_var->min_power) * (16383 / ((double)shm_proc_var->max_power - (double)shm_proc_var->min_power));
						break;
					case POWER_LIMIT_MIN:
						((int *)shm_proc_var)[gest.offset] = buffer[1];
						shm_ai_men_com[PWM_LIMIT_MIN] = (unsigned int)((double)buffer[1] - (double)shm_proc_var->min_power) * (16383 / ((double)shm_proc_var->max_power - (double)shm_proc_var->min_power));
						break;
					case MAX_POWER:
						((int *)shm_proc_var)[gest.offset] = buffer[1];
						shm_ai_men_com[PWM_LIMIT_MAX] = (unsigned int)((double)shm_proc_var->power_limit_max - (int16_t)shm_proc_var->min_power) * (16383 / ((double)shm_proc_var->max_power - (int16_t)shm_proc_var->min_power));
						shm_ai_men_com[PWM_LIMIT_MIN] = (unsigned int)((double)shm_proc_var->power_limit_min - (int16_t)shm_proc_var->min_power) * (16383 / ((double)shm_proc_var->max_power - (int16_t)shm_proc_var->min_power));
						break;
					case MIN_POWER:
						((int *)shm_proc_var)[gest.offset] = (int16_t)buffer[1];
						shm_ai_men_com[PWM_LIMIT_MAX] = (unsigned int)((double)shm_proc_var->power_limit_max - (int16_t)buffer[1]) * (16383 / ((double)shm_proc_var->max_power - (int16_t)buffer[1]));
						shm_ai_men_com[PWM_LIMIT_MIN] = (unsigned int)((double)shm_proc_var->power_limit_min - (int16_t)buffer[1]) * (16383 / ((double)shm_proc_var->max_power - (int16_t)buffer[1]));
						break;
					default:
						((int *)shm_proc_var)[gest.offset] = buffer[1];
						break;
					}
				}
			}
			else
			{
				switch (gest.baseaddress)
				{
				case NIT_CONTROL_UNIT_BASE_ADDRESS:
					if (gest.lectura > 127)
					{ // lectura
						if (gest.offset == NIT_SINCRONIZATION)
						{
							buffer[1] = 1;
							nw = write(newsockfd, buffer, 4); // Mero comando de sincronizacion, no se guarda en memoria
						}
						else if (gest.offset == NIT_ARM_SW_VERSION)
						{
							buffer[1] = (uint16_t)VERSION;
							nw = write(newsockfd, buffer, 4);
						}
						else if ((gest.offset == NIT_FPGA_VERSION) && (buffer[0] == 0x05A6))
						{
							buffer[1] = (uint16_t)((shm_nit_command[gest.offset] & 0xFFFF0000) >> 16);
							nw = write(newsockfd, buffer, 4);
						}
						else
						{
							buffer[1] = (uint16_t)(shm_nit_command[gest.offset] & 0x0000FFFF);
							nw = write(newsockfd, buffer, 4);
						}
						// printf("Respondiendo NIT lectura address-command 0x%x, data %d\n",buffer[0],buffer[1]);
					}
					else
					{ // escritura de datos en memoria
						if (gest.offset == NIT_SAVE_EMBEDDED_CONF)
						{
							if (config_save("/mnt/mmc/sys/CONFIG.sys", (volatile int *)shm_proc_var, shm_ai_men_com, shm_nit_command) == 0)
							{
								printf("Couldn't save current configuration\n");
							}
						}
						else
						{
							shm_nit_command[gest.offset] = buffer[1];
						}
					}
					break;

				case NIT_MB_CORE_BASE_ADDRESS:
					if (gest.lectura > 127)
					{
						if (gest.offset != TIME_TRACK_LOW)
						{
							buffer[1] = (uint16_t)(shm_ai_men_com[gest.offset] & 0x0000FFFF);
							nw = write(newsockfd, buffer, 4);
						}
						else
						{
							time_track_aux = ((unsigned int)shm_ai_men_com[TIME_TRACK_HIGH]);
							time_track_aux = ((time_track_aux << 32) | ((unsigned int)shm_ai_men_com[TIME_TRACK_LOW]));
							buffer[1] = ((uint16_t)(((double)time_track_aux / CLK_100MHZ) * 10));
							nw = write(newsockfd, buffer, 4);
						}
					}
					else
					{
						switch (gest.offset)
						{

						case TIME_TRACK_LOW:
							time_track_aux = (uint64_t)(((double)buffer[1]) * (CLK_100MHZ / 10));
							shm_ai_men_com[gest.offset] = (unsigned int)(0x00000000FFFFFFFF & time_track_aux);
							shm_ai_men_com[TIME_TRACK_HIGH] = (unsigned int)((0xFFFFFFFF00000000 & time_track_aux) >> 32);
							break;

						case MODE:
							shm_ai_men_com[gest.offset] = buffer[1];
							shm_ai_men_com[CHANGE_OP_MODE] = 1;
							break;

						default:
							shm_ai_men_com[gest.offset] = buffer[1];
							break;
						}
						// printf("escribiendo en  0x%x +  0x%x = 0x%x\n",gest.baseaddress,(gest.offset*4),gest.baseaddress+ (gest.offset*4));
					}
					break;
				case NIT_ARM_CORE_BASE_ADDRESS:
					shm_reset[gest.offset] = buffer[1];
					break;
				default:
					printf("Error in ADDRESS identification :%d \n", gest.baseaddress);
					break;
				}
			}
		}
	}
	if (cont_tramas_vacias == 3)
		printf("Error en lectura, recibiendo tramas vacias\n");
}

/*
 * Funcion de control de CLAMIR
 */

void controller_loop(metadata_t *metadata, process_variables_t *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_reset, volatile int *shm_nit_command)
{
	int estadoAutomata = 0;
	int valor1 = 0;
	int valor2 = 0;
	int contadorFramesMidiendo = 0;
	volatile int *shm_metadata = (volatile int *)metadata;

	// Variables calculo potencia
	double width_ref = 1;
	double error_t0 = 0;
	double error_t1 = 0;
	double potencia_t0 = 0;
	double potencia_t1 = 0;
	double dt = 0.001;
	double potenciaMax, potenciaMin, limPotenciaMax, limPotenciaMin;
	double ki, kp, kd;
	double duty;
	double pixel_mm_ratio;
	double integral = 0;
	double derivative, limiteIntegral, limit_integral_divided, limiteSlew;
	int mode = 0;
	int laser_status = 0; // 0 OFF, 1 ON
	int contadorFramesStop = 0;
	int endP = 1000;
	double delta_potencia = 0;
	int track_cnt = 0;
	int track = 0;
	uint64_t track_length = 2000;

	sem_t *semaforo;
	semaforo = sem_open(SEM_NAME, O_CREAT, 0644, 0);
	// File descriptor de UIO
	int pending = 0;
	int enable = 1; // Habilita interrupciones
	int fd_int = open("/dev/uio0", O_RDWR);
	if (fd_int < 0)
	{
		printf("No se puede abrir el descriptor de uio0 para copntrol\n");
	}

	int fdMetadatos;
	fdMetadatos = open("/dev/mem", O_RDWR | O_SYNC);
	volatile int *metaPtr = NULL;
	metaPtr = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdMetadatos, NIT_IMAGE_METADATA_BRAM_BASE_ADDRESS);

	// int fdTraza0;
	// fdTraza0 = open("/dev/mem", O_RDWR | O_SYNC);
	// volatile int *traza0Ptr = NULL;
	// traza0Ptr = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdTraza0, 0x60000000);
	int metadatos[12];
	double W = 0;
	double width_aux = 0;
	double W_circ_buff_aux = 0;
	double w_circ_buffer[512];
	for (int i = 0; i < 512; i++)
	{
		w_circ_buffer[i] = 0;
	}
	int control_NAP = 0;
	int measurement_NAP = 0;
	int cont_calibration = 0;
	uint16_t alarm_enable; // 1 alarm ON, 0 alarm OFF
	double alarm_min;
	double alarm_max;
	uint16_t alarm_time;
	uint16_t cnt_aux_alarm = shm_proc_var->alarm_time;
	uint16_t alarm;
	uint16_t automeasure_flag, autoshutter_config;
	uint32_t autoshutter_time_target;
	float autoshutter_temp_target;
	int resultado = 0;
	int cont = 0;
	int z = 0;
	int circ_buffer_size = 1;
	write(fd_int, (void *)&enable, sizeof(int));
	derivative = 0;

	char auto_auto_shutter_enable;
	char auto_auto_shutter_enable_inprocess;
	char auto_auto_shutter_time_xtemp;
	float current_temperature;
	float temperature_last_autoshutter = ((float)shm_metadata[13]) / 10;
	int cnt_last_autoshutter = 0;
	char pending_autoshutter = 0;
	uint16_t laser_external = 0;
	int last_laser_status = 0;
	int delay_laser_on = 0;
	int cont_preheating = 0;
	while (1)
	{
		ki = shm_proc_var->ki;
		kp = shm_proc_var->kp;
		kd = shm_proc_var->kd;
		potenciaMax = (double)shm_proc_var->max_power;
		potenciaMin = (double)shm_proc_var->min_power;
		if (potenciaMin > 32767)
		{
			potenciaMin = potenciaMin - 65536;
		}
		limPotenciaMax = shm_proc_var->power_limit_max;
		limPotenciaMin = shm_proc_var->power_limit_min;
		pixel_mm_ratio = ((double)shm_proc_var->pixel_mm_ratio / 1000); // se reciben en micras
		shm_proc_var->pid_error = (int)100 * error_t1;
		limiteIntegral = (double)shm_proc_var->limit_integral;
		limiteSlew = ((double)shm_proc_var->limit_slew / 100);
		circ_buffer_size = shm_proc_var->buff_size;

		alarm_enable = shm_proc_var->enable_alarm;
		alarm_max = ((double)shm_proc_var->alarm_max) / 100;
		alarm_min = ((double)shm_proc_var->alarm_min) / 100;
		alarm_time = shm_proc_var->alarm_time;
		automeasure_flag = shm_proc_var->automeasure;

		autoshutter_config = shm_proc_var->autoshutter_config; // booleano, comprobar flags
		// decode_config
		auto_auto_shutter_enable = autoshutter_config & 0x01;
		auto_auto_shutter_enable_inprocess = (autoshutter_config >> 1) & 0x01;
		auto_auto_shutter_time_xtemp = (autoshutter_config >> 3) & 0x01;
		// decodee config end
		autoshutter_temp_target = ((float)shm_proc_var->autoshutter_temp) / 10; // de int16 recibido a double o float, dividir entre 10
		autoshutter_time_target = shm_proc_var->autoshutter_timer * 10000;		//	de int16 recibido a int32, multiplicar por 10

		laser_external = shm_proc_var->laser_exteral_control;

		if (circ_buffer_size < 1)
		{
			circ_buffer_size = 1;
		}
		else if (circ_buffer_size > LMAX_CIRCULAR_BUFFER_SIZE)
		{
			circ_buffer_size = LMAX_CIRCULAR_BUFFER_SIZE;
		}
		if (mode != shm_ai_men_com[MODE])
		{
			mode = shm_ai_men_com[MODE];
			if (mode == 2)
			{
				estadoAutomata = MANUAL;
				printf("\nCambio a estado MANUAL\n");
				if (logging == 1)
				{
					stop_logging = 1;
					cont = 0;
					pthread_mutex_unlock(&mutex);
				}
			}
			else
			{
				estadoAutomata = IDLE;
				printf("\nCambio a estado IDLE\n");
				shm_ai_men_com[CHANGE_OP_MODE] = 1;
			}
		}

		read(fd_int, (int *)&pending, sizeof(int)); // Se bloquea hasta que sucede una interrupcion de uio0
		// traza0Ptr[0] = 1;
		memcpy(metadatos, (void *)metaPtr, 48);

		// Laser status
		if (laser_external)
		{
			laser_status = shm_ai_men_com[DIGITAL_IN_0];
			// laser_status = shm_ai_men_com[DIGITAL_IN_1];
		}
		else
		{
			if (laser_status)
			{
				if (metadatos[1] < shm_ai_men_com[END_OF_TRACK])
					laser_status = 0;
			}
			else
			{
				if (metadatos[1] >= shm_ai_men_com[START_TRACK_MOM_T])
					laser_status = 1;
			}
		}

		write(fd_int, (void *)&enable, sizeof(int)); // Habilita la interrupcion de uio0

		/*
		 * Cambio a lectura buffer circular
		 */
		if (shm_proc_var->auto_shutter)
		{

			shm_proc_var->auto_shutter = 0;
			if (!measurement_NAP)
			{
				measurement_NAP = NAP_DURATION;
				control_NAP = NAP_DURATION;
				// shm_proc_var->auto_shutter = 0;
				cont_calibration = WAIT_START_CALIBRATION + 1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE;
				// if(shm_nit_command[NIT_SHUTTER] == 0)
				//{
				shm_nit_command[NIT_SHUTTER] = 1; // cerrar shutter
												  //}
				cnt_last_autoshutter = 0;
				temperature_last_autoshutter = current_temperature;
			}
			// shm_nit_command[NIT_SHUTTER_RESET]=1;
		}
		if (!measurement_NAP)
		{
			shm_ai_men_com[DIGITAL_OUT_3] = 0;
			if (delay_laser_on > 0)
			{
				delay_laser_on--;
			}
			z += 1;
			if (z >= circ_buffer_size)
			{
				z = 0;
			}
			w_circ_buffer[z] = pixel_mm_ratio * metadata_get_width((metadata_t *)metadatos);
			W_circ_buff_aux = 0;
			for (int y = 0; y < circ_buffer_size; y++)
			{
				W_circ_buff_aux += w_circ_buffer[y];
			}
			W = W_circ_buff_aux / (circ_buffer_size);
		}
		else
		{ // siesta
			shm_ai_men_com[DIGITAL_OUT_3] = 1;
			W = W;
			if ((cont_calibration <= 0) && (shm_nit_command[NIT_SHUTTER] != 0))
			{
				shm_nit_command[NIT_SHUTTER] = 0; // open shutter
			}
			else
			{
				if (cont_calibration == (WAIT_SECOND_APERTURE))
				{
					//	shm_nit_command[NIT_SHUTTER] = 0;
				}
				if (cont_calibration == (1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE))
				{
					shm_nit_command[NIT_OFFSET_UPDATE] = 1;
					// shm_nit_command[NIT_SHUTTER_RESET]=1;
				}
				// cont_calibration--;
			}
			cont_calibration--;
			measurement_NAP--;
			control_NAP--;
		}

		if (shm_proc_var->set_ref_width > 0)
		{
			width_ref = ((double)shm_proc_var->width_ref) / 100;
			shm_proc_var->set_ref_width = 0;
		}

		switch (estadoAutomata)
		{
		case MANUAL:
			potencia_t0 = shm_proc_var->power_man;
			if (mode != 2)
			{
				estadoAutomata = IDLE;
				printf("\nCambio a estado IDLE\n");
				shm_ai_men_com[CHANGE_OP_MODE] = 1;
			}
			track_cnt = 0;
			track = 0;
			delay_laser_on = 0;
			break;

		case IDLE:
			shm_ai_men_com[CHANGE_OP_MODE] = 1;
			width_aux = 0;
			contadorFramesStop = 0;
			if (logging == 1)
			{
				stop_logging = 1;
				cont = 0;
				pthread_mutex_unlock(&mutex);
			}
			else
			{
				if (laser_status)
				{
					if (shm_proc_var->preheating_ena)
					{
						cont_preheating = 0;
						estadoAutomata = PREHEATING;
						printf("\nCambio a estado PREHEATING\n");
					}
					else
					{
						estadoAutomata = MIDIENDO;
						printf("\nCambio a estado MIDIENDO\n");
						contadorFramesMidiendo = 0;
						pthread_mutex_unlock(&mutex);
					}
					track_length = ((unsigned int)shm_ai_men_com[TIME_TRACK_HIGH]);
					track_length = (((track_length << 32) | ((unsigned int)shm_ai_men_com[TIME_TRACK_LOW])) * 1000) / CLK_100MHZ;
				}
				else
				{
					potencia_t0 = shm_proc_var->power_man;
					potencia_t1 = potencia_t0;
				}
			}
			track_cnt = 0;
			track = 0;
			delay_laser_on = 0;
			break;

		case PREHEATING:
			potencia_t0 = shm_proc_var->preheating_power;
			if (cont_preheating++ >= shm_proc_var->preheating_time)
			{
				estadoAutomata = MIDIENDO;
				contadorFramesMidiendo = 0;
				pthread_mutex_unlock(&mutex);
				printf("\nCambio a estado MIDIENDO\n");
			}
			break;

		case MIDIENDO:
			cont++;
			if (mode == 0)
			{
				if (track_cnt >= track_length)
				{
					track_cnt = 0;
					track++;
				}
				else
				{
					track_cnt++;
				}
			}
			else if (mode == 1)
			{
				if ((laser_status > 0) && (laser_status != last_laser_status))
				{
					if (laser_external)
					{
						track++;
						delay_laser_on = shm_proc_var->delay_laser_on;
					}
					else
					{
						if (!measurement_NAP)
						{
							track++;
							delay_laser_on = shm_proc_var->delay_laser_on;
						}
					}
				}
			}
			// if ((laser_status) & (metadatos[7] >= shm_proc_var->track_ref_start)) //sustituido por cuenta de tracks en el ARM
			if ((laser_status) & (track >= shm_proc_var->track_ref_start))
			{
				width_aux = width_aux + W;
				contadorFramesMidiendo++;
			}
			potencia_t0 = shm_proc_var->power_man;
			potencia_t1 = potencia_t0;
			// if (metadatos[7] >= shm_ai_men_com[REFERENCE_TRACK]){
			if (track >= shm_ai_men_com[REFERENCE_TRACK])
			{
				estadoAutomata = CONTROL;
				printf("\nCambio a estado CONTROL\n");
				if (contadorFramesMidiendo == 0)
				{
					contadorFramesMidiendo = 1;
				}
				if (automeasure_flag == 1)
				{
					width_ref = width_aux / contadorFramesMidiendo;
				}
				else
				{
					width_ref = ((double)shm_proc_var->width_ref) / 100;
				}
				width_aux = 0;
				error_t0 = 0;
				error_t1 = 0;
				integral = 0;
				derivative = 0;
				contadorFramesMidiendo = 0;
			}
			break;

		case CONTROL:
			endP = shm_proc_var->end_of_process;
			cont++;

			if (mode == 0)
			{
				if (track_cnt >= track_length)
				{
					track_cnt = 0;
					track++;
				}
				else
				{
					track_cnt++;
				}
			}
			else if (mode == 1)
			{
				if ((laser_status > 0) && (laser_status != last_laser_status))
				{
					if (laser_external)
					{
						track++;
						delay_laser_on = shm_proc_var->delay_laser_on;
					}
					else
					{
						if (!measurement_NAP)
						{
							track++;
							delay_laser_on = shm_proc_var->delay_laser_on;
						}
					}
				}
			}
			if (delay_laser_on > 0)
			{
				delay_laser_on--;
			}
			else
			{
				if ((!laser_status) || (control_NAP))
				{
					if (contadorFramesStop++ > endP)
					{
						estadoAutomata = IDLE;
						printf("\nCambio a estado IDLE\n");
						contadorFramesStop = 0;
						cont = 0;
						stop_logging = 1;
						pthread_mutex_unlock(&mutex);
					}
				}
				else
				{
					contadorFramesStop = 0;
					// Comienza el c�clculo de la potencia
					error_t0 = width_ref - W;
					integral = integral + (error_t0 * dt);

					if (ki > 0)
					{
						limit_integral_divided = limiteIntegral / ki;
					}
					else
					{
						limit_integral_divided = 1000;
					}
					if (fabs(integral) >= (limit_integral_divided))
					{
						if (integral < 0)
						{
							integral = 0 - limit_integral_divided;
						}
						else
						{
							integral = limit_integral_divided;
						}
					}

					//				derivative = (error_t0 - error_t1)/dt; //comentada por posibilidad de valor muy grande
					derivative = (error_t0 - error_t1);
					potencia_t0 = shm_proc_var->power_man + (kp * error_t0) + (ki * integral) + (kd * derivative);
					error_t1 = error_t0;

					delta_potencia = potencia_t1 - potencia_t0;
					if (abs(delta_potencia) > limiteSlew)
					{
						// printf("potencia_t0 : %f", potencia_t0);
						if (potencia_t0 < potencia_t1)
						{
							potencia_t0 = potencia_t1 - limiteSlew;
						}
						else
						{
							potencia_t0 = potencia_t1 + limiteSlew;
						}
						// printf(" potencia_t0 despues de limit : %f\n", potencia_t0);
					}
				}
			}

			break;
		default:
			break;
		}

		resultado = (((int)(100 * width_ref) << 16)) | ((int)(W * 100));

		if (potencia_t0 > limPotenciaMax)
		{
			potencia_t0 = limPotenciaMax;
		}
		else
		{
			if (potencia_t0 < limPotenciaMin)
			{
				potencia_t0 = limPotenciaMin;
			}
		}
		potencia_t1 = potencia_t0;
		metadatos[0] = potencia_t0;
		metadatos[7] = track;
		metadatos[11] = (estadoAutomata << 24) | (laser_status << 16) | metadatos[11];
		// Comienza el c�lculo del duty cycle
		// duty = (potencia_t0 - potenciaMin) * (1000/(potenciaMax - potenciaMin));
		duty = (potencia_t0 - potenciaMin) * (16383 / (potenciaMax - potenciaMin));
		shm_ai_men_com[PWM] = (unsigned int)duty;
		last_laser_status = laser_status;

		memcpy((void *)&shm_metadata[0], &metadatos, 28);	 // Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20
		shm_metadata[7] = resultado;						 // Width
		memcpy((void *)&shm_metadata[8], &metadatos[7], 20); // Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status

		// gestion de la alarma

		if ((alarm_enable == 1) && (laser_status))
		{
			if ((W > alarm_max) || (W < alarm_min))
			{
				if (cnt_aux_alarm > 0)
				{
					cnt_aux_alarm--;
					alarm = 0;
				}
				else
				{
					alarm = 1;
				}
			}
			else
			{
				cnt_aux_alarm = alarm_time;
				alarm = 0;
			}
		}
		else
		{
			cnt_aux_alarm = alarm_time;
			alarm = 0;
		}

		// Gestion de logging
		if (cont >= 100)
		{
			cont = 0;
			pthread_mutex_unlock(&mutex);
		}

		// LEDS y autoshutter
		cnt_last_autoshutter++;
		current_temperature = ((float)shm_metadata[13]) / 10;
		if (auto_auto_shutter_time_xtemp)
		{
			if (autoshutter_time_target < cnt_last_autoshutter)
			{
				pending_autoshutter = 1;
				cnt_last_autoshutter = 0;
			}
		}
		else
		{
			if (autoshutter_temp_target < (fabs(temperature_last_autoshutter - current_temperature)))
			{
				pending_autoshutter = 1;
				cnt_last_autoshutter = 0;
				temperature_last_autoshutter = current_temperature;
			}
		}

		switch (estadoAutomata)
		{
		case MANUAL:
			if (auto_auto_shutter_enable && pending_autoshutter)
			{
				shm_proc_var->auto_shutter = 1;
				pending_autoshutter = 0;
			}
			break;
		case IDLE:
			if (auto_auto_shutter_enable && pending_autoshutter)
			{
				shm_proc_var->auto_shutter = 1;
				pending_autoshutter = 0;
			}
			break;
		case MIDIENDO:
			if (auto_auto_shutter_enable && auto_auto_shutter_enable_inprocess && pending_autoshutter)
			{
				shm_proc_var->auto_shutter = 1;
				pending_autoshutter = 0;
			}

			break;

		case CONTROL:
			if (auto_auto_shutter_enable && auto_auto_shutter_enable_inprocess && pending_autoshutter)
			{
				shm_proc_var->auto_shutter = 1;
				pending_autoshutter = 0;
			}
			break;

		default:
			break;
		}

		if (alarm == 1)
		{
			// ROJO
			shm_reset[LED_R] = 0;
			shm_reset[LED_G] = 1;
			shm_reset[LED_B] = 1;
		}
		else
		{

			switch (estadoAutomata)
			{
			case MANUAL: // AMARILLO
				shm_reset[LED_R] = 0;
				shm_reset[LED_G] = 0;
				shm_reset[LED_B] = 1;

				break;
			case IDLE: // VERDE
				shm_reset[LED_R] = 1;
				shm_reset[LED_G] = 0;
				shm_reset[LED_B] = 1;

				break;

			case MIDIENDO: // MORADO
				shm_reset[LED_R] = 0;
				shm_reset[LED_G] = 1;
				shm_reset[LED_B] = 0;

				break;

			case CONTROL: // AZUL
				shm_reset[LED_R] = 1;
				shm_reset[LED_G] = 1;
				shm_reset[LED_B] = 0;

				break;
			default:
				break;
			}
		}

		shm_ai_men_com[DIGITAL_OUT_1] = 1; // naranja
		if (alarm_enable == 1)
		{ // Solo si la alarma est� habilitada

			if (alarm == 1)
			{
				shm_ai_men_com[DIGITAL_OUT_0] = 0; // asignacion de la digital out 1 a la alarma
				shm_ai_men_com[DIGITAL_OUT_2] = 1; // verde
			}
			else
			{
				shm_ai_men_com[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
				shm_ai_men_com[DIGITAL_OUT_2] = 0; // verde
			}
		}
		else
		{
			shm_ai_men_com[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
			shm_ai_men_com[DIGITAL_OUT_2] = 0; // verde
		}

		sem_getvalue(semaforo, &valor1);
		if (valor1 < 1)
		{ // permite al productor enviar hasta 2 imagenes por TCP al empezar la conexion
			sem_post(semaforo);
		}
		sem_getvalue(semaforo, &valor2);
	}
	close(fd_int);
	close(fdMetadatos);
	munmap(((int *)metaPtr), 64);
	metaPtr = NULL;
	sem_close(semaforo);
	sem_unlink(SEM_NAME);
	exit(1);
}