/*
 * clamir.cpp
 *
 *  Created on: 27 dec. 2024
 *      Author: arthur
 */

#include <exception>

#include "stdio.h"
#include "error.h"

#include <iostream>
#include <thread>
#include <vector>

#include <gst/gst.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <sched.h>
#include <semaphore.h>
#include <dirent.h>

#include "fpga_common_defs.h"
#include "arm_core.h"
#include "mb_core.h"
#include "control_unit_core.h"
#include "bpcc_core.h"
#include "framebuffer_core.h"
#include "framebuffer_metadata_core.h"

#define VERSION 0x0002 // Negated alarm digital output and ignore digital output 3 command for semaphore. Made for demo.

// Definiciones de Limites M�ximos
#define LMAX_KI 30000
#define LMAX_KP 30000
#define LMAX_KD 30000
#define LMAX_MAX_POWER 32000
#define LMAX_MIN_POWER 30000
#define LMAX_START_TRACK_MOM 2000
#define LMAX_POWER 32000
#define LMAX_STOP_TRACK_MOM 1000
#define LMAX_MODE 2
#define LMAX_TRACK_REFERENCE 100
#define LMAX_TRACK_LENGTH 10000
#define LMAX_WIDTH_MANUAL 65
#define LMAX_THRESHOLD 5000
#define LMAX_ROI_ROUND 3
#define LMAX_X2_PIXEL 63
#define LMAX_X2_PIXEL 63
#define LMAX_X1_PIXEL 62
#define LMAX_Y2_PIXEL 63
#define LMAX_Y1_PIXEL 62
#define LMAX_MAX_POWER_LIMIT 32000
#define LMAX_MIN_POWER_LIMIT 30000
#define LMAX_END_OF_PROCESS 30000
#define LMAX_PIXEL_MM_RATIO 10
#define LMAX_LIMIT_SLEW 300
#define LMAX_BLACK_LEVEL 10000
#define LMAX_ALARM_MAX 320
#define LMAX_ALARM_MIN 320
#define LMAX_ALARM_TIME 10000
#define LMAX_LIMIT_INTEGRAL 10000
#define LMAX_CIRCULAR_BUFFER_SIZE 512
#define LMAX_INTEGRATION_TIME 800
#define LMAX_CONF_DIGITALIO 2
#define LMAX_TRACK_REF_START 100
#define LMAX_DRIFT_TEMP_AUTOSHUTTER 50
#define LMAX_TIMER_AUTOSHUTTER 320000
#define LMAX_BIAS_VOLTAGE 2.5
#define LMAX_DELAY_LASER_ON 1000
#define LMAX_PREHEATING_TIME 30000
#define LMAX_PREHEATING_POWER 32000
#define LMAX_DRIFT_INTENSITY 15

// Definiciones de Limites M�nimos
#define LMIN_KI 0
#define LMIN_KP 0
#define LMIN_KD 0
#define LMIN_MAX_POWER 100
#define LMIN_MIN_POWER -30000
#define LMIN_START_TRACK_MOM 0
#define LMIN_POWER 0
#define LMIN_STOP_TRACK_MOM 0
#define LMIN_MODE 0
#define LMIN_TRACK_REFERENCE 0
#define LMIN_TRACK_LENGTH 0.1
#define LMIN_WIDTH_MANUAL 0
#define LMIN_THRESHOLD 0
#define LMIN_ROI_ROUND 0
#define LMIN_X2_PIXEL 1
#define LMIN_X1_PIXEL 0
#define LMIN_Y2_PIXEL 1
#define LMIN_Y1_PIXEL 0
#define LMIN_MAX_POWER_LIMIT 1
#define LMIN_MIN_POWER_LIMIT 0
#define LMIN_END_OF_PROCESS 500
#define LMIN_PIXEL_MM_RATIO 0.01
#define LMIN_LIMIT_SLEW 0.01
#define LMIN_BLACK_LEVEL 0
#define LMIN_ALARM_MAX 0
#define LMIN_ALARM_MIN 0
#define LMIN_ALARM_TIME 0
#define LMIN_LIMIT_INTEGRAL 0
#define LMIN_CIRCULAR_BUFFER_SIZE 1
#define LMIN_INTEGRATION_TIME 50
#define LMIN_CONF_DIGITALIO 0
#define LMIN_TRACK_REF_START 0
#define LMIN_DRIFT_TEMP_AUTOSHUTTER 0.1
#define LMIN_TIMER_AUTOSHUTTER 10
#define LMIN_BIAS_VOLTAGE 1
#define LMIN_DELAY_LASER_ON 0
#define LMIN_PREHEATING_TIME 0
#define LMIN_PREHEATING_POWER 0
#define LMIN_DRIFT_INTENSITY 0

/*
 * Offsets para memoria virtual empleada para comandos que no se escriben en la FPGA
 */
#define KI 0
#define KP 1
#define KD 2
#define MAX_POWER 3
#define MIN_POWER 4
#define POWER_MAN 5
#define POWER_LIMIT_MAX 6
#define POWER_LIMIT_MIN 7
#define SET_REF_WIDTH 8
#define WIDTH_REF 9
#define PIXEL_MM_RATIO 10 // llegan en micras, pasar a mm
#define PID_ERROR 11
#define END_OF_PROCESS 12 // imagenes sin se�al para considerar un proceso finalizado
#define LIMIT_INTEGRAL 13
#define LIMIT_SLEW 14
#define BUFF_SIZE 15
#define AUTO_SHUTTER 16
#define ENABLE_ALARM 17
#define ALARM_MAX 18
#define ALARM_MIN 19
#define ALARM_TIME 20
#define SERIAL_NUMBER_LOW 21
#define SERIAL_NUMBER_HIGH 22
#define AUTOMEASURE 23
#define AUTOSHUTTER_CONFIG 24
#define AUTOSHUTTER_TEMP 25
#define AUTOSHUTTER_TIMER 26
#define TRACK_REF_START 27
#define LASER_EXTERAL_CONTROL 28
#define DELAY_LASER_ON 29
#define PREHEATING_ENA 30
#define PREHEATING_TIME 31
#define PREHEATING_POWER 32

// Definicion para el semaforo
#define SEM_NAME "semaforo"

// Definiciones de estados
#define MANUAL 0
#define IDLE 8
#define MIDIENDO 9
#define CONTROL 10
#define PREHEATING 11

#define WAIT_START_CALIBRATION 150
#define WAIT_STOP_CALIBRATION 100 // 30
#define WAIT_SECOND_APERTURE 400
#define NAP_DURATION 750 // 300

typedef struct __attribute__((packed)) process_variables_struct
{
	double ki;
	double kp;
	double kd;
	double derivative;
	double integral;
	double width_aux;
	double error_t0;
	double error_t1;
	double max_power;
	double min_power;
	double power_man;
	double power_limit_max;
	double power_limit_min;
	double set_ref_width;
	double width_ref;
	double pixel_mm_ratio;
	double pid_error;
	double end_of_process;
	double limit_integral;
	double limit_slew;
	unsigned int buff_size;
	double auto_shutter;
	double enable_alarm;
	double alarm_max;
	double alarm_min;
	double alarm_time;
	double serial_number_low;
	double serial_number_high;
	double automeasure;
	uint16_t autoshutter_config;
	double autoshutter_temp;
	double autoshutter_timer;
	double track_ref_start;
	bool laser_exteral_control;
	double delay_laser_on;
	double preheating_ena;
	double preheating_time;
	double preheating_power;
	double potencia_t0;
	double potencia_t1;
	double dt;
	double power_max;
	double power_min;
	double power_max_limit;
	double power_min_limit;
	double duty;
	double integral_limit;
	double integral_limit_scaled;
	double integral_limit_slew;
} process_variables_t;

typedef struct
{
	unsigned int rw;
	unsigned int base;
	unsigned int offset;
} tcp_command_host_action_t;

// Structure with all the values to initialize
typedef struct
{
	double ki;
	double kp;
	double kd;
	double max_power;
	double min_power;
	double start_track_mom;
	double power;
	double stop_track_mom;
	double mode;
	double track_reference;
	double track_length;
	double width_manual;
	double threshold;
	double roi_enable;
	double x2_pixel;
	double x1_pixel;
	double y2_pixel;
	double y1_pixel;
	double max_power_limit;
	double min_power_limit;
	double end_of_process;
	double pixel_mm_ratio;
	double limit_slew;
	double black_level;
	double alarm_enable;
	double alarm_max;
	double alarm_min;
	double alarm_time;
	double automeasure;
	double limit_integral;
	double circular_buffer_size;
	double integration_time;
	double digitalio;
	double track_ref_start;
	double autoshutter;
	double drift_temp_autoshutter;
	double timer_autoshutter;
	double bias_voltage;
	bool laser_external_control;
	double delay_laser_on;
	double preheating_ena;
	double preheating_time;
	double preheating_power;
	double roi_round;
	double ena_drift;
	double drift_intensity;
} config_data_t;

void SIGPIPE_handler(int s)
{
	printf("Caught SIGPIPE\n");
}

// Variables globales
pthread_mutex_t mutex;
int logging;
int stop_logging;

double metadata_get_width(metadata_t *metadata);
void memory_initialize(config_data_t config_data, process_variables_t *process_variables, mb_core_state_t *mb_core_state, control_unit_core_state_t *control_unit_state);
int read_serial_number(const char *path, process_variables_t *process_variables);
void tcp_command_host(int newsockfd, control_unit_core_state_t *control_unit_state, process_variables_t *process_variables, mb_core_state_t *mb_core_state, arm_core_state_t *arm_core_state);
void image_writer(int newsockimgfd, metadata_t *metadata, control_unit_core_state_t *control_unit_state);

config_data_t config_file_read(config_data_t initialization_data);
config_data_t config_initialize(config_data_t s_dat);
int config_save(const char *path, process_variables_t *process_variables, mb_core_state_t *mb_core_state, control_unit_core_state_t *control_unit_state);
double metadata_get_width(metadata_t *metadata);

int main(int argc, char *argv[])
{

	process_variables_t process_variables;

	/**
	 * Open MB Core
	 */
	mb_core_state_t mb_core_state;
	{

		auto retval = mb_core_open(&mb_core_state);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	/**
	 * Open Control Unit Core
	 */
	control_unit_core_state_t control_unit_state;
	{

		auto retval = control_unit_open(&control_unit_state);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	/**
	 * Open ARM Core
	 */
	arm_core_state_t arm_core_state;
	{

		auto retval = arm_core_open(&arm_core_state);

		if (retval < 0)
		{
			printf("Failed to open control unit core\n");
			exit(retval);
		}
	}

	/*
	 * Load Serial Number
	 */
	if (!read_serial_number("/etc/clamir/serial_number.conf", &process_variables))
	{
		printf("Couldn't read serial number from file %s", "/etc/clamir/serial_number.conf");
	}

	/*
	 * Inicializacion de los valores de CLAMIR
	 */
	config_data_t config_data;
	config_data = config_file_read(config_data);

	memory_initialize(config_data, &process_variables, &mb_core_state, &control_unit_state);

	bpcc_table_core_state_t bpcc_table_state;
	bpcc_table_core_open(&bpcc_table_state);

	if (bpcc_table_core_load_coefficients_from_file(&bpcc_table_state, BPCC_TABLE_COEFFICIENTS_TABLE_FILE_PATH))
	{
		control_unit_bpc_en_set(&control_unit_state, 1);
	}

	bpcc_table_core_close(&bpcc_table_state);

	framebuffer_metadata_core_state_t framebuffer_metadata_state;
	framebuffer_metadata_core_open(&framebuffer_metadata_state);
	metadata_t *metadata = (metadata_t *)framebuffer_metadata_core_memory_map_get(&framebuffer_metadata_state);

	signal(SIGPIPE, SIGPIPE_handler);

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

			tcp_command_host(sock, &control_unit_state, &process_variables, &mb_core_state, &arm_core_state);
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

			image_writer(sock, metadata, &control_unit_state);
		}
	};
	
	auto server_command_host_thread = std::thread(server_command_host_thread_function);
	auto server_video_stream_host_thread = std::thread(server_video_stream_host_thread_function);

	server_command_host_thread.join();
	server_video_stream_host_thread.join();

	return EXIT_SUCCESS;
}

/*
 * Funcion que inicializa las memorias con la estructura de datos obtenida de un fichero.
 * En caso de conflicto prioriza los Maximos a los Minimos.
 */
void memory_initialize(config_data_t config_data, process_variables_t *process_variables, mb_core_state_t *mb_core_state, control_unit_core_state_t *control_unit_state)
{

	uint64_t time_track_aux = 0;

	// inicializacion de la memoria mapeada digitalmente process_variables

	process_variables->ki = config_data.ki;
	process_variables->kp = config_data.kp;
	process_variables->kd = config_data.kd;

	if (config_data.max_power < config_data.min_power)
	{
		config_data.min_power = config_data.max_power - 1;
	}

	process_variables->max_power = config_data.max_power;
	process_variables->min_power = config_data.min_power;
	process_variables->power_man = config_data.power;

	if (config_data.max_power_limit < config_data.min_power_limit)
	{
		config_data.min_power_limit = config_data.max_power_limit - 1;
	}
	process_variables->power_limit_max = config_data.max_power_limit;

	nit_mb_core_pwm_limit_max_set(mb_core_state, (unsigned int)((double)config_data.max_power_limit - (double)process_variables->min_power) * (16383 / ((double)process_variables->max_power - (double)process_variables->min_power)));

	process_variables->power_limit_min = config_data.min_power_limit;

	nit_mb_core_pwm_limit_min_set(mb_core_state, (unsigned int)((double)config_data.min_power_limit - (double)process_variables->min_power) * (16383 / ((double)process_variables->max_power - (double)process_variables->min_power)));

	process_variables->width_ref = config_data.width_manual;
	process_variables->set_ref_width = 1; // Cuando comience el hilo de procesamiento de control se tomara el ancho de referencia
	process_variables->pixel_mm_ratio = config_data.pixel_mm_ratio;
	process_variables->end_of_process = config_data.end_of_process;
	process_variables->limit_integral = config_data.limit_integral;
	process_variables->limit_slew = config_data.limit_slew;
	process_variables->buff_size = config_data.circular_buffer_size;
	process_variables->enable_alarm = config_data.alarm_enable;

	if (config_data.alarm_max < config_data.alarm_min)
	{
		config_data.alarm_min = config_data.alarm_max - 1;
	}
	process_variables->alarm_max = config_data.alarm_max;
	process_variables->alarm_min = config_data.alarm_min;
	process_variables->alarm_time = config_data.alarm_time;
	process_variables->automeasure = config_data.automeasure;
	process_variables->autoshutter_config = config_data.autoshutter;
	process_variables->autoshutter_temp = config_data.drift_temp_autoshutter;
	process_variables->autoshutter_timer = config_data.timer_autoshutter;
	process_variables->track_ref_start = config_data.track_ref_start;
	process_variables->laser_exteral_control = config_data.laser_external_control;
	process_variables->delay_laser_on = config_data.delay_laser_on;
	process_variables->preheating_ena = config_data.preheating_ena;
	process_variables->preheating_time = config_data.preheating_time;
	process_variables->preheating_power = config_data.preheating_power;

	// inicializacion de la memoria Baseaddress aimen command

	if (config_data.start_track_mom < config_data.stop_track_mom)
	{
		config_data.stop_track_mom = config_data.start_track_mom - 1;
	}

	time_track_aux = (uint64_t)(((double)config_data.track_length) * (CLK_100MHZ / 10));

	if (config_data.x2_pixel < config_data.x1_pixel)
	{
		config_data.x2_pixel = config_data.x1_pixel;
	}

	if (config_data.y2_pixel < config_data.y1_pixel)
	{
		config_data.y2_pixel = config_data.y1_pixel;
	}

	process_variables->auto_shutter = 1; // realizará un autoshutter con la nueva configuracion en cuanto se inicie el proceso de control

	nit_mb_core_start_track_mom_t_set(mb_core_state, config_data.start_track_mom);
	nit_mb_core_end_of_track_set(mb_core_state, config_data.stop_track_mom);
	nit_mb_core_mode_set(mb_core_state, config_data.mode);
	nit_mb_core_reference_track_set(mb_core_state, config_data.track_reference);

	nit_mb_core_time_track_low_set(mb_core_state, (unsigned int)(0x00000000FFFFFFFF & time_track_aux));
	nit_mb_core_time_track_high_set(mb_core_state, (unsigned int)((0xFFFFFFFF00000000 & time_track_aux) >> 32));

	nit_mb_core_threshold_set(mb_core_state, config_data.threshold);
	nit_mb_core_roi_round_set(mb_core_state, config_data.roi_round);
	nit_mb_core_enable_roi_set(mb_core_state, config_data.roi_enable);

	nit_mb_core_roi_x1_set(mb_core_state, config_data.x1_pixel);
	nit_mb_core_roi_x2_set(mb_core_state, config_data.x2_pixel);

	nit_mb_core_roi_y1_set(mb_core_state, config_data.y1_pixel);
	nit_mb_core_roi_y2_set(mb_core_state, config_data.y2_pixel);
	nit_mb_core_digital_out_conf_set(mb_core_state, config_data.digitalio);

	// Inicializacion de la memoria Baseaddress NIT command
	control_unit_black_level_set(control_unit_state, config_data.black_level);
	control_unit_bias_v_set(control_unit_state, config_data.bias_voltage);
	control_unit_int_time_set(control_unit_state, config_data.integration_time);

	// printf("Drift enable:		%d\n", config_data.ena_drift);
	// printf("Drift intensity:	%d\n", config_data.drift_intensity);
	control_unit_drift_enable_set(control_unit_state, config_data.ena_drift);

	control_unit_drift_position_set(control_unit_state, config_data.drift_intensity);

	nit_mb_core_change_op_mode_set(mb_core_state, 1); // Indica a la memoria el cambio al nuevo modo al finalizar la inicializacion).
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
 * Funcián de escritura de imagenes desde el CLAMIR
 */
void image_writer(int sock, metadata_t *metadata, control_unit_core_state_t *control_unit_state)
{
	int retval = 0;
	unsigned char frame_sync[] = {0x5f, 0x00, 0x48, 0xf6, 0x70, 0x44, 0x94, 0xee};

	int timer_fd = open("/dev/uio0", O_RDWR);

	int timer_ctrl = 1;
	int timer_status = 0;
	write(timer_fd, (void *)&timer_ctrl, sizeof(int));

	framebuffer_core_state_t framebuffer_core_state;
	framebuffer_core_open(&framebuffer_core_state);
	volatile uint16_t *buffer = framebuffer_get_memory_map(&framebuffer_core_state);

	const int shutter_enable_counter_max = 1000;
	int shutter_enable_counter = 0;

	control_unit_offset_update_set(control_unit_state, 1);

	while (retval >= 0)
	{

		read(timer_fd, (int *)&timer_status, sizeof(int));
		write(timer_fd, (void *)&timer_ctrl, sizeof(int));

		/**
		 * Enable shutter
		 */

		if (shutter_enable_counter-->0) {
			control_unit_shutter_set(control_unit_state, 0);
		} else {
			control_unit_shutter_set(control_unit_state, 1);
			shutter_enable_counter = shutter_enable_counter_max;
		}

		retval = write(sock, frame_sync, sizeof(frame_sync));

		metadata->frame_number++;

		if (retval < 0)
		{
			printf("ERROR writing image to socket (frame sync)\n");
			break;
		}

		retval = write(sock, (unsigned char *)metadata, sizeof(metadata_t));

		if (retval < 0)
		{
			printf("ERROR writing image to socket (header)\n");
			break;
		}

		retval = write(sock, (unsigned char *)buffer, NIT_FRAMEBUFFER_CORE_SIZE);
		if (retval < 0)
		{
			printf("ERROR writing image to socket (image)\n");
			break;
		}
	}

	framebuffer_core_close(&framebuffer_core_state);
	
}

int system_command_host_process_action(const tcp_command_host_action_t *action)
{
	return 0;
}

int tcp_command_host_buffer_dump(const unsigned char *buffer)
{
	return 0;
}

int tcp_command_host_action_dump(const tcp_command_host_action_t *action)
{
	return 0;
}

int tcp_command_host_decode_buffer(tcp_command_host_action_t *dest, const unsigned char *buffer)
{
	return 0;
}

void tcp_command_host(int newsockfd, control_unit_core_state_t *control_unit_state, process_variables_t *process_variables, mb_core_state_t *mb_core_state, arm_core_state_t *arm_core_state)
{

	uint64_t time_track_aux;
	int cont_tramas_vacias = 0;
	uint8_t buffer[4];
	tcp_command_host_action_t action;
	uint16_t bytes_read = 0;
	int result;

	while (true)
	{
		bytes_read = recv(newsockfd, buffer, sizeof(buffer), MSG_WAITALL);

		if (bytes_read < 0)
		{
			break;
		}

		if (bytes_read == 0)
		{
			continue;
		}

		result = tcp_command_host_decode_buffer(&action, buffer);

		if (result < 0)
		{
			printf("failed to decode TCP action %d", result);
			tcp_command_host_buffer_dump(buffer);
			tcp_command_host_action_dump(&action);
			continue;
		}

		system_command_host_process_action(&action);
	}
}

config_data_t get_value_of_key(FILE *fdCONFSYS, config_data_t s_dat, char *auxString);

config_data_t config_file_read(config_data_t initialization_data)
{
	char *auxstr;
	auxstr = (char *)malloc(30);
	initialization_data = config_initialize(initialization_data);
	FILE *config_file_stream = fopen("/mnt/mmc/sys/CONFIG.sys", "r");

	if (config_file_stream == NULL)
	{
		printf("\nUnable to read CONFIG.sys file. Loading default values.\n");
	}
	else
	{
		while (fscanf(config_file_stream, "%s", auxstr) == 1)
		{
			initialization_data = get_value_of_key(config_file_stream, initialization_data, auxstr);
		}
		if (fclose(config_file_stream) == 0)
		{
			printf("\nInit file closed successfully\n");
		}
	}
	// close(config_file_stream);

	return initialization_data;
}

// No he encontrado otra
config_data_t get_value_of_key(FILE *config_file_stream, config_data_t config_data, char *key)
{

	int auxInt;
	float auxDbl;

	if (strcmp("ALARM_ENABLE", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				config_data.alarm_enable = 1;
			else
				config_data.alarm_enable = 0;
		}
	}
	else if (strcmp("ALARM_MAX", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_ALARM_MAX)
			{
				config_data.alarm_max = (uint16_t)((float)LMIN_ALARM_MAX * 100);
			}
			else if (auxDbl > LMAX_ALARM_MAX)
			{
				config_data.alarm_max = (uint16_t)((float)LMAX_ALARM_MAX * 100);
			}
			else
			{
				config_data.alarm_max = (uint16_t)(auxDbl * 100);
			}
		}
	}
	else if (strcmp("ALARM_MIN", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_ALARM_MIN)
			{
				config_data.alarm_min = (uint16_t)((float)LMIN_ALARM_MIN * 100);
			}
			else if (auxDbl > LMAX_ALARM_MIN)
			{
				config_data.alarm_min = (uint16_t)((float)LMAX_ALARM_MIN * 100);
			}
			else
			{
				config_data.alarm_min = (uint16_t)(auxDbl * 100);
			}
		}
	}
	else if (strcmp("ALARM_TIME", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_ALARM_TIME)
			{
				config_data.alarm_time = LMIN_ALARM_TIME;
			}
			else if (auxInt > LMAX_ALARM_TIME)
			{
				config_data.alarm_time = LMAX_ALARM_TIME;
			}
			else
			{
				config_data.alarm_time = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("AUTOMEASURE", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				config_data.automeasure = 1;
			else
				config_data.automeasure = 0;
		}
	}
	else if (strcmp("BIAS_VOLTAGE", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_BIAS_VOLTAGE)
			{
				config_data.bias_voltage = (uint16_t)((float)LMIN_BIAS_VOLTAGE / 0.00018311);
			}
			else if (auxDbl > LMAX_BIAS_VOLTAGE)
			{
				config_data.bias_voltage = (uint16_t)((float)LMAX_BIAS_VOLTAGE / 0.00018311);
			}
			else
			{
				config_data.bias_voltage = (uint16_t)(auxDbl / 0.00018311);
			}
		}
	}
	else if (strcmp("BLACK_LEVEL", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_BLACK_LEVEL)
			{
				config_data.black_level = LMIN_BLACK_LEVEL;
			}
			else if (auxInt > LMAX_BLACK_LEVEL)
			{
				config_data.black_level = LMAX_BLACK_LEVEL;
			}
			else
			{
				config_data.black_level = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("CIRCULAR_BUFFER_SIZE", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_CIRCULAR_BUFFER_SIZE)
			{
				config_data.circular_buffer_size = LMIN_CIRCULAR_BUFFER_SIZE;
			}
			else if (auxInt > LMAX_CIRCULAR_BUFFER_SIZE)
			{
				config_data.circular_buffer_size = LMAX_CIRCULAR_BUFFER_SIZE;
			}
			else
			{
				config_data.circular_buffer_size = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("CONF_AUTOSHUTTER", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			config_data.autoshutter = (uint16_t)auxInt;
		}
	}
	else if (strcmp("CONF_DIGITALIO", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_CONF_DIGITALIO)
			{
				config_data.digitalio = LMIN_CONF_DIGITALIO;
			}
			else if (auxInt > LMAX_CONF_DIGITALIO)
			{
				config_data.digitalio = LMAX_CONF_DIGITALIO;
			}
			else
			{
				config_data.digitalio = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("DRIFT_TEMP_AUTOSHUTTER", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_DRIFT_TEMP_AUTOSHUTTER)
			{
				config_data.drift_temp_autoshutter = (uint16_t)((float)LMIN_DRIFT_TEMP_AUTOSHUTTER * 10);
			}
			else if (auxDbl > LMAX_DRIFT_TEMP_AUTOSHUTTER)
			{
				config_data.drift_temp_autoshutter = (uint16_t)((float)LMAX_DRIFT_TEMP_AUTOSHUTTER * 10);
			}
			else
			{
				config_data.drift_temp_autoshutter = (uint16_t)(auxDbl * 10);
			}
		}
	}
	else if (strcmp("END_OF_PROCESS", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_END_OF_PROCESS)
			{
				config_data.end_of_process = LMIN_END_OF_PROCESS;
			}
			else if (auxInt > LMAX_END_OF_PROCESS)
			{
				config_data.end_of_process = LMAX_END_OF_PROCESS;
			}
			else
			{
				config_data.end_of_process = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("INTEGRATION_TIME", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_INTEGRATION_TIME)
			{
				config_data.integration_time = LMIN_INTEGRATION_TIME;
			}
			else if (auxInt > LMAX_INTEGRATION_TIME)
			{
				config_data.integration_time = LMAX_INTEGRATION_TIME;
			}
			else
			{
				config_data.integration_time = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("KD", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_KD)
			{
				config_data.kd = LMIN_KD;
			}
			else if (auxInt > LMAX_KD)
			{
				config_data.kd = LMAX_KD;
			}
			else
			{
				config_data.kd = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("KI", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_KI)
			{
				config_data.ki = LMIN_KI;
			}
			else if (auxInt > LMAX_KI)
			{
				config_data.ki = LMAX_KI;
			}
			else
			{
				config_data.ki = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("KP", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_KP)
			{
				config_data.kp = LMIN_KP;
			}
			else if (auxInt > LMAX_KP)
			{
				config_data.kp = LMAX_KP;
			}
			else
			{
				config_data.kp = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("LIMIT_INTEGRAL", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_LIMIT_INTEGRAL)
			{
				config_data.limit_integral = LMIN_LIMIT_INTEGRAL;
			}
			else if (auxInt > LMAX_LIMIT_INTEGRAL)
			{
				config_data.limit_integral = LMAX_LIMIT_INTEGRAL;
			}
			else
			{
				config_data.limit_integral = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("LIMIT_SLEW", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_LIMIT_SLEW)
			{
				config_data.limit_slew = LMIN_LIMIT_SLEW;
			}
			else if (auxDbl > LMAX_LIMIT_SLEW)
			{
				config_data.limit_slew = LMAX_LIMIT_SLEW;
			}
			else
			{
				config_data.limit_slew = auxDbl;
			}
		}
	}
	else if (strcmp("MAX_POWER", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MAX_POWER)
			{
				config_data.max_power = LMIN_MAX_POWER;
			}
			else if (auxInt > LMAX_MAX_POWER)
			{
				config_data.max_power = LMAX_MAX_POWER;
			}
			else
			{
				config_data.max_power = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("MAX_POWER_LIMIT", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MAX_POWER_LIMIT)
			{
				config_data.max_power_limit = LMIN_MAX_POWER_LIMIT;
			}
			else if (auxInt > LMAX_MAX_POWER_LIMIT)
			{
				config_data.max_power_limit = LMAX_MAX_POWER_LIMIT;
			}
			else
			{
				config_data.max_power_limit = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("MIN_POWER", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MIN_POWER)
			{
				config_data.min_power = LMIN_MIN_POWER;
			}
			else if (auxInt > LMAX_MIN_POWER)
			{
				config_data.min_power = LMAX_MIN_POWER;
			}
			else
			{
				config_data.min_power = (int16_t)auxInt;
			}
		}
	}
	else if (strcmp("MIN_POWER_LIMIT", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MIN_POWER_LIMIT)
			{
				config_data.min_power_limit = LMIN_MIN_POWER_LIMIT;
			}
			else if (auxInt > LMAX_MIN_POWER_LIMIT)
			{
				config_data.min_power_limit = LMAX_MIN_POWER_LIMIT;
			}
			else
			{
				config_data.min_power_limit = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("MODE", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MODE)
			{
				config_data.mode = LMIN_MODE;
			}
			else if (auxInt > LMAX_MODE)
			{
				config_data.mode = LMAX_MODE;
			}
			else
			{
				config_data.mode = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("PIXEL_MM_RATIO", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_PIXEL_MM_RATIO)
			{
				config_data.pixel_mm_ratio = LMIN_PIXEL_MM_RATIO;
			}
			else if (auxDbl > LMAX_PIXEL_MM_RATIO)
			{
				config_data.pixel_mm_ratio = LMAX_PIXEL_MM_RATIO;
			}
			else
			{
				config_data.pixel_mm_ratio = auxDbl;
			}
		}
	}
	else if (strcmp("POWER", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_POWER)
			{
				config_data.power = LMIN_POWER;
			}
			else if (auxInt > LMAX_POWER)
			{
				config_data.power = LMAX_POWER;
			}
			else
			{
				config_data.power = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("ROI_ENABLE", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				config_data.roi_enable = 1;
			else
				config_data.roi_enable = 0;
		}
	}
	else if (strcmp("START_TRACK_MOM", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_START_TRACK_MOM)
			{
				config_data.start_track_mom = LMIN_START_TRACK_MOM;
			}
			else if (auxInt > LMAX_START_TRACK_MOM)
			{
				config_data.start_track_mom = LMAX_START_TRACK_MOM;
			}
			else
			{
				config_data.start_track_mom = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("STOP_TRACK_MOM", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_STOP_TRACK_MOM)
			{
				config_data.stop_track_mom = LMIN_STOP_TRACK_MOM;
			}
			else if (auxInt > LMAX_STOP_TRACK_MOM)
			{
				config_data.stop_track_mom = LMAX_STOP_TRACK_MOM;
			}
			else
			{
				config_data.stop_track_mom = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("THRESHOLD", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_THRESHOLD)
			{
				config_data.threshold = LMIN_THRESHOLD;
			}
			else if (auxInt > LMAX_THRESHOLD)
			{
				config_data.threshold = LMAX_THRESHOLD;
			}
			else
			{
				config_data.threshold = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("TIMER_AUTOSHUTTER", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_TIMER_AUTOSHUTTER)
			{
				config_data.timer_autoshutter = (uint16_t)(LMIN_TIMER_AUTOSHUTTER / 10);
			}
			else if (auxInt > LMAX_TIMER_AUTOSHUTTER)
			{
				config_data.timer_autoshutter = (uint16_t)(LMAX_TIMER_AUTOSHUTTER / 10);
			}
			else
			{
				config_data.timer_autoshutter = (uint16_t)(auxInt / 10);
			}
		}
	}
	else if (strcmp("TRACK_LENGTH", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_TRACK_LENGTH)
			{
				config_data.track_length = (uint16_t)(LMIN_TRACK_LENGTH * 10);
			}
			else if (auxDbl > LMAX_TRACK_LENGTH)
			{
				config_data.track_length = (uint16_t)(LMAX_TRACK_LENGTH * 10);
			}
			else
			{
				config_data.track_length = (uint16_t)(auxDbl * 10);
			}
		}
	}
	else if (strcmp("TRACK_REFERENCE", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_TRACK_REFERENCE)
			{
				config_data.track_reference = LMIN_TRACK_REFERENCE;
			}
			else if (auxInt > LMAX_TRACK_REFERENCE)
			{
				config_data.track_reference = LMAX_TRACK_REFERENCE;
			}
			else
			{
				config_data.track_reference = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("TRACK_REF_START", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_TRACK_REF_START)
			{
				config_data.track_ref_start = LMIN_TRACK_REF_START;
			}
			else if (auxInt > LMAX_TRACK_REF_START)
			{
				config_data.track_ref_start = LMAX_TRACK_REF_START;
			}
			else
			{
				config_data.track_ref_start = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("WIDTH_MANUAL", key) == 0)
	{
		if (fscanf(config_file_stream, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_WIDTH_MANUAL)
			{
				config_data.width_manual = (uint16_t)(LMIN_WIDTH_MANUAL * 100);
			}
			else if (auxDbl > LMAX_WIDTH_MANUAL)
			{
				config_data.width_manual = (uint16_t)(LMAX_WIDTH_MANUAL * 100);
			}
			else
			{
				config_data.width_manual = (uint16_t)(auxDbl * 100);
			}
		}
	}
	else if (strcmp("X1_PIXEL", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_X1_PIXEL)
			{
				config_data.x1_pixel = LMIN_X1_PIXEL;
			}
			else if (auxInt > LMAX_X1_PIXEL)
			{
				config_data.x1_pixel = LMAX_X1_PIXEL;
			}
			else
			{
				config_data.x1_pixel = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("X2_PIXEL", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_X2_PIXEL)
			{
				config_data.x2_pixel = LMIN_X2_PIXEL;
			}
			else if (auxInt > LMAX_X2_PIXEL)
			{
				config_data.x2_pixel = LMAX_X2_PIXEL;
			}
			else
			{
				config_data.x2_pixel = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("Y1_PIXEL", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_Y1_PIXEL)
			{
				config_data.y1_pixel = LMIN_Y1_PIXEL;
			}
			else if (auxInt > LMAX_Y1_PIXEL)
			{
				config_data.y1_pixel = LMAX_Y1_PIXEL;
			}
			else
			{
				config_data.y1_pixel = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("Y2_PIXEL", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_Y2_PIXEL)
			{
				config_data.y2_pixel = LMIN_Y2_PIXEL;
			}
			else if (auxInt > LMAX_Y2_PIXEL)
			{
				config_data.y2_pixel = LMAX_Y2_PIXEL;
			}
			else
			{
				config_data.y2_pixel = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("LASER_EXTERNAL_CONTROL", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				config_data.laser_external_control = 1;
			else
				config_data.laser_external_control = 0;
		}
	}
	else if (strcmp("DELAY_LASER_ON", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_DELAY_LASER_ON)
			{
				config_data.delay_laser_on = LMIN_DELAY_LASER_ON;
			}
			else if (auxInt > LMAX_DELAY_LASER_ON)
			{
				config_data.delay_laser_on = LMAX_DELAY_LASER_ON;
			}
			else
			{
				config_data.delay_laser_on = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("PREHEATING_ENA", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				config_data.preheating_ena = 1;
			else
				config_data.preheating_ena = 0;
		}
	}
	else if (strcmp("PREHEATING_TIME", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_PREHEATING_TIME)
			{
				config_data.preheating_time = LMIN_PREHEATING_TIME;
			}
			else if (auxInt > LMAX_PREHEATING_TIME)
			{
				config_data.preheating_time = LMAX_PREHEATING_TIME;
			}
			else
			{
				config_data.preheating_time = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("PREHEATING_POWER", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_PREHEATING_POWER)
			{
				config_data.preheating_power = LMIN_PREHEATING_POWER;
			}
			else if (auxInt > LMAX_PREHEATING_POWER)
			{
				config_data.preheating_power = LMAX_PREHEATING_POWER;
			}
			else
			{
				config_data.preheating_power = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("ROI_ROUND", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_ROI_ROUND)
			{
				config_data.roi_round = LMIN_ROI_ROUND;
			}
			else if (auxInt > LMAX_ROI_ROUND)
			{
				config_data.roi_round = LMAX_ROI_ROUND;
			}
			else
			{
				config_data.roi_round = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("ENA_DRIFT", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (fscanf(config_file_stream, "%d", &auxInt) == 1)
			{
				if (auxInt > 0)
					config_data.ena_drift = 1;
				else
					config_data.ena_drift = 0;
			}
		}
	}
	else if (strcmp("DRIFT_INTENSITY", key) == 0)
	{
		if (fscanf(config_file_stream, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_DRIFT_INTENSITY)
			{
				config_data.drift_intensity = LMIN_DRIFT_INTENSITY;
			}
			else if (auxInt > LMAX_DRIFT_INTENSITY)
			{
				config_data.drift_intensity = LMAX_DRIFT_INTENSITY;
			}
			else
			{
				config_data.drift_intensity = (uint16_t)auxInt;
			}
		}
	}
	else
	{
		printf("Bad line in the CONFIG.sys file\n");
	}

	return config_data;
}

int config_save(const char *path, process_variables_t *process_variables, mb_core_state_t *mb_core_state, control_unit_core_state_t *control_unit_state)
{
	int result = 0;
	uint64_t time_track_aux = 0;
	FILE *fd;
	fd = fopen(path, "w");
	if (fd == NULL)
	{
		printf("\n*** UNABLE TO SAVE CONFIGURATION ***\n");
		return result;
	}
	else
	{

		fprintf(fd, "KI %d\n", process_variables->ki);
		fprintf(fd, "KP %d\n", process_variables->kp);
		fprintf(fd, "KD %d\n", process_variables->kd);
		fprintf(fd, "MAX_POWER %d\n", process_variables->max_power);
		fprintf(fd, "MIN_POWER %d\n", process_variables->min_power);
		fprintf(fd, "POWER %d\n", process_variables->power_man);
		fprintf(fd, "MAX_POWER_LIMIT %d\n", process_variables->power_limit_max);
		fprintf(fd, "MIN_POWER_LIMIT %d\n", process_variables->power_limit_min);
		fprintf(fd, "WIDTH_MANUAL %.2f\n", ((float)process_variables->width_ref / 100));
		fprintf(fd, "PIXEL_MM_RATIO %.3f\n", ((float)process_variables->pixel_mm_ratio / 1000));
		fprintf(fd, "END_OF_PROCESS %d\n", process_variables->end_of_process);
		fprintf(fd, "LIMIT_INTEGRAL %d\n", process_variables->limit_integral);
		fprintf(fd, "LIMIT_SLEW %.2f\n", ((float)process_variables->limit_slew));
		fprintf(fd, "CIRCULAR_BUFFER_SIZE %d\n", process_variables->buff_size);
		fprintf(fd, "ALARM_ENABLE %d\n", process_variables->enable_alarm);
		fprintf(fd, "ALARM_MAX %.2f\n", ((float)process_variables->alarm_max / 100));
		fprintf(fd, "ALARM_MIN %.2f\n", ((float)process_variables->alarm_min / 100));
		fprintf(fd, "ALARM_TIME %d\n", process_variables->alarm_time);
		fprintf(fd, "AUTOMEASURE %d\n", process_variables->automeasure);
		fprintf(fd, "CONF_AUTOSHUTTER %d\n", process_variables->autoshutter_config);
		fprintf(fd, "DRIFT_TEMP_AUTOSHUTTER %.1f\n", ((float)process_variables->autoshutter_temp / 10));
		fprintf(fd, "TIMER_AUTOSHUTTER %d\n", (process_variables->autoshutter_timer * 10));
		fprintf(fd, "TRACK_REF_START %d\n", process_variables->track_ref_start);
		fprintf(fd, "LASER_EXTERNAL_CONTROL %d\n", process_variables->laser_exteral_control);
		fprintf(fd, "DELAY_LASER_ON %d\n", process_variables->delay_laser_on);
		fprintf(fd, "PREHEATING_ENA %d\n", process_variables->preheating_ena);
		fprintf(fd, "PREHEATING_TIME %d\n", process_variables->preheating_time);
		fprintf(fd, "PREHEATING_POWER %d\n", process_variables->preheating_power);

		fprintf(fd, "START_TRACK_MOM %d\n", nit_mb_core_start_track_mom_t_get(mb_core_state));
		fprintf(fd, "STOP_TRACK_MOM %d\n", nit_mb_core_end_of_track_get(mb_core_state));
		fprintf(fd, "MODE %d\n", nit_mb_core_mode_get(mb_core_state));
		fprintf(fd, "TRACK_REFERENCE %d\n", nit_mb_core_reference_track_get(mb_core_state));

		time_track_aux = ((unsigned int)nit_mb_core_time_track_high_get(mb_core_state));
		time_track_aux = ((time_track_aux << 32) | ((unsigned int)nit_mb_core_time_track_low_get(mb_core_state)));

		fprintf(fd, "TRACK_LENGTH %.1f\n", (((float)time_track_aux / CLK_100MHZ)));
		fprintf(fd, "THRESHOLD %d\n", nit_mb_core_threshold_get(mb_core_state));
		fprintf(fd, "ROI_ROUND %d\n", nit_mb_core_roi_round_get(mb_core_state));
		fprintf(fd, "ROI_ENABLE %d\n", nit_mb_core_enable_roi_get(mb_core_state));
		fprintf(fd, "X1_PIXEL %d\n", nit_mb_core_roi_x1_get(mb_core_state));
		fprintf(fd, "X2_PIXEL %d\n", nit_mb_core_roi_x2_get(mb_core_state));
		fprintf(fd, "Y1_PIXEL %d\n", nit_mb_core_roi_y1_get(mb_core_state));
		fprintf(fd, "Y2_PIXEL %d\n", nit_mb_core_roi_y2_get(mb_core_state));
		fprintf(fd, "CONF_DIGITALIO %d\n", nit_mb_core_digital_out_conf_get(mb_core_state));
		fprintf(fd, "BLACK_LEVEL %d\n", control_unit_black_level_get(control_unit_state));
		fprintf(fd, "BIAS_VOLTAGE %f\n", ((float)control_unit_bias_v_get(control_unit_state) * 0.00018311));
		fprintf(fd, "INTEGRATION_TIME %d\n", control_unit_int_time_get(control_unit_state));
		fprintf(fd, "ENA_DRIFT %d\n", control_unit_drift_enable_get(control_unit_state));
		fprintf(fd, "DRIFT_INTENSITY %d\n", control_unit_drift_position_get(control_unit_state));
		result = 1;
		fclose(fd);
	}
	return result;
}

config_data_t config_initialize(config_data_t config_data)
{

	config_data.alarm_enable = 0;
	config_data.alarm_max = 500; // 5,0
	config_data.alarm_min = 100; // 1,0
	config_data.alarm_time = 2000;
	config_data.automeasure = 1;
	config_data.bias_voltage = (uint16_t)((float)2 / 0.00018311);
	config_data.black_level = 1000;
	config_data.circular_buffer_size = 8;
	config_data.autoshutter = 8;
	config_data.digitalio = 0;
	config_data.drift_temp_autoshutter = 30; // equivalente a 3,0 en el fichero
	config_data.end_of_process = 5000;
	config_data.integration_time = 200;
	config_data.kd = 100;
	config_data.ki = 500;
	config_data.kp = 200;
	config_data.limit_integral = 5000;
	config_data.limit_slew = 100; // equivale a 1,00
	config_data.max_power = 5000;
	config_data.max_power_limit = 1500;
	config_data.min_power = 0;
	config_data.min_power_limit = 500;
	config_data.mode = 2;
	config_data.pixel_mm_ratio = 150; // Equivalente a 0,15 en el fichero
	config_data.power = 1000;
	config_data.roi_enable = 0;
	config_data.start_track_mom = 40;
	config_data.stop_track_mom = 30;
	config_data.threshold = 1200;
	config_data.timer_autoshutter = 18; // equivalente a 180 en fichero, se evia en decenas de segundos como unidad desde el PC
	config_data.track_length = 20;		// equivalente a 2,0 en el fichero
	config_data.track_reference = 3;
	config_data.track_ref_start = 0;
	config_data.width_manual = 100; // equivalente a 1,0 en el fichero
	config_data.roi_round = 0;
	config_data.x1_pixel = 2;
	config_data.x2_pixel = 61;
	config_data.y1_pixel = 2;
	config_data.y2_pixel = 61;
	config_data.laser_external_control = 0;
	config_data.delay_laser_on = 0;
	config_data.preheating_ena = 0;
	config_data.preheating_time = 0;
	config_data.preheating_power = 0;
	config_data.ena_drift = 1;
	config_data.drift_intensity = 14;
	return config_data;
}

enum tcp_protocol_header_command_target_enum
{
	COMMAND_TARGET_CONTROL_UNIT_CORE = 5,
	COMMAND_TARGET_MB_CORE_TYPE0 = 3,
	COMMAND_TARGET_MB_CORE_TYPE1 = 4,
};

enum tcp_protocol_header_command_control_unit_target_parameter_enum
{

	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_INT_TIME = 0x01,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BIAS_V = 0x02,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_OFFSET_EN = 0x03,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_OFFSET_UPDATE = 0x04,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_SHUTTER = 0x05,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BPC_EN = 0x06,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BPC_MEM_WRITE = 0x07,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BPC_IDENTIFY = 0x08,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP1 = 0x09,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP2 = 0x0A,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP3 = 0x0B,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TEMP4 = 0x0C,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_TRIGGER_USEC = 0x0D,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_BLACK_LEVEL = 0x0E,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_SINCRONIZATION = 0x0F,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_SAVE_EMBEDDED_CONF = 0x10,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_ARM_SW_VERSION = 0x11,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_DRIFT_ENABLE = 0x12,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_DRIFT_POSITION = 0x13,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_DRIFT_LEVEL = 0x14,
	COMMAND_TARGET_PARAMETER_CONTROL_UNIT_FPGA_VERSION = 0x15,

	COMMAND_TARGET_PARAMETER_MB_CORE_KI = 0x01,
	COMMAND_TARGET_PARAMETER_MB_CORE_KP = 0x02,
	COMMAND_TARGET_PARAMETER_MB_CORE_KD = 0x03,
	COMMAND_TARGET_PARAMETER_MB_CORE_MAX_POWER = 0x04,
	COMMAND_TARGET_PARAMETER_MB_CORE_MIN_POWER = 0x05,
	COMMAND_TARGET_PARAMETER_MB_CORE_POWER_MAN = 0x06,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTO_SHUTTER = 0x07,
	COMMAND_TARGET_PARAMETER_MB_CORE_SET_REF_WIDTH = 0x08,
	COMMAND_TARGET_PARAMETER_MB_CORE_POWER_LIMIT_MAX = 0x09,
	COMMAND_TARGET_PARAMETER_MB_CORE_POWER_LIMIT_MIN = 0x0A,
	COMMAND_TARGET_PARAMETER_MB_CORE_WIDTH_REF = 0x0B,
	COMMAND_TARGET_PARAMETER_MB_CORE_PIXEL_MM_RATIO = 0x0C,
	COMMAND_TARGET_PARAMETER_MB_CORE_PID_ERROR = 0x0D,
	COMMAND_TARGET_PARAMETER_MB_CORE_END_OF_PROCESS = 0x0E,
	COMMAND_TARGET_PARAMETER_MB_CORE_LIMIT_INTEGRAL = 0x0F,
	COMMAND_TARGET_PARAMETER_MB_CORE_LIMIT_SLEW = 0x10,
	COMMAND_TARGET_PARAMETER_MB_CORE_BUFF_SIZE = 0x11,
	COMMAND_TARGET_PARAMETER_MB_CORE_ENABLE_ALARM = 0x12,
	COMMAND_TARGET_PARAMETER_MB_CORE_ALARM_MAX = 0x13,
	COMMAND_TARGET_PARAMETER_MB_CORE_ALARM_MIN = 0x14,
	COMMAND_TARGET_PARAMETER_MB_CORE_ALARM_TIME = 0x15,
	COMMAND_TARGET_PARAMETER_MB_CORE_SERIAL_NUMBER_LOW = 0x16,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOMEASURE = 0x17,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOSHUTTER_CONFIG = 0x18,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOSHUTTER_TEMP = 0x19,
	COMMAND_TARGET_PARAMETER_MB_CORE_AUTOSHUTTER_TIMER = 0x1A,
	COMMAND_TARGET_PARAMETER_MB_CORE_TRACK_REF_START = 0x1B,
	COMMAND_TARGET_PARAMETER_MB_CORE_LASER_EXTERAL_CONTROL = 0x1C,
	COMMAND_TARGET_PARAMETER_MB_CORE_DELAY_LASER_ON = 0x1D,
	COMMAND_TARGET_PARAMETER_MB_CORE_PREHEATING_ENA = 0x1E,
	COMMAND_TARGET_PARAMETER_MB_CORE_PREHEATING_TIME = 0x1F,
	COMMAND_TARGET_PARAMETER_MB_CORE_PREHEATING_POWER = 0x20,

	COMMAND_TARGET_PARAMETER_MB_CORE_START_TRACK_MOM_T = 0x06,
	COMMAND_TARGET_PARAMETER_MB_CORE_END_OF_TRACK = 0x08,
	COMMAND_TARGET_PARAMETER_MB_CORE_MODE = 0x0A,
	COMMAND_TARGET_PARAMETER_MB_CORE_REFERENCE_TRACK = 0x0B,
	COMMAND_TARGET_PARAMETER_MB_CORE_TIME_TRACK_LOW = 0x0C,

	COMMAND_TARGET_PARAMETER_MB_CORE_THRESHOLD = 0x08F,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_ROUND = 0x090,
	COMMAND_TARGET_PARAMETER_MB_CORE_ENABLE_ROI = 0x091,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_X1 = 0x012,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_Y1 = 0x093,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_X2 = 0x094,
	COMMAND_TARGET_PARAMETER_MB_CORE_ROI_Y2 = 0x095,

	COMMAND_TARGET_PARAMETER_MB_CORE_PWM = 0x97,
	COMMAND_TARGET_PARAMETER_MB_CORE_PWM_LIMIT_MAX = 0x9D,
	COMMAND_TARGET_PARAMETER_MB_CORE_PWM_LIMIT_MIN = 0x9E,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_CONF = 0xB0,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_0 = 0xB9,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_1 = 0xBA,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_2 = 0xBB,
	COMMAND_TARGET_PARAMETER_MB_CORE_DIGITAL_OUT_3 = 0xBC,

	COMMAND_TARGET_PARAMETER_ARM_CORE_SOFT_RST = 0x0D,
	COMMAND_TARGET_PARAMETER_ARM_CORE_IP_OP_MODE = 0x16,
	COMMAND_TARGET_PARAMETER_ARM_CORE_LED_R = 0x9A,
	COMMAND_TARGET_PARAMETER_ARM_CORE_LED_G = 0x9B,
	COMMAND_TARGET_PARAMETER_ARM_CORE_LED_B = 0x9C,

};

struct __attribute__((packed)) tcp_protocol_header_struct
{
	uint8_t target_parameter : 7;
	uint8_t rw : 1;
	uint8_t reserved1 : 4;
	uint8_t target : 4;
};

typedef union tcp_protocol_header_union
{
	uint16_t val;
	struct tcp_protocol_header_struct obj;
} tcp_protocol_header_t;

/*
 * Funcion de cáclculo de ancho, empleada por varios estados del automata de control
 */
double metadata_get_width(metadata_t *metadata)
{
	double W = 0;
	double X, Y;
	double u20, u11, u02;

	// Procesos de calculo
	if (metadata->M00 == 0)
		metadata->M00 = 1;
	X = metadata->M10 / metadata->M00;
	Y = metadata->M01 / metadata->M00;

	u20 = (metadata->M20 / metadata->M00) - (X * X);
	u11 = (metadata->M11 / metadata->M00) - (X * Y);
	u02 = (metadata->M02 / metadata->M00) - (Y * Y);

	W = sqrt(8 * (u20 + u02 - sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02))))); // Aqui se ha calculado el ancho
	return W;
}