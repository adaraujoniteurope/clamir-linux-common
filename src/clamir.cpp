#define SENSORV2

#include "Inicializacion.h"
#include "commandaddr.h"
#include "stdio.h"
#include "error.h"

// // Definiciones para imagen debug
// // #define MEM_IMG_DEBUG_ADDRESS 0x46000000  //Prueba NIT
// #define MEM_IMG_DEBUG_ADDRESS 0x41000000
// #define MEM_IMG_DEBUG_SIZE 8192

// // Definicion para el semaforo
// #define SEM_NAME "semaforo"

// // Definiciones de estados
// #define MANUAL 0
// #define IDLE 8
// #define MIDIENDO 9
// #define CONTROL 10
// #define PREHEATING 11

// #define WAIT_START_CALIBRATION 150
// #define WAIT_STOP_CALIBRATION 100 // 30
// #define WAIT_SECOND_APERTURE 400
// #define NAP_DURATION 750 // 300

// #ifdef DEMOSEMAFORO
// #define VERSION 0x0002 // Negated alarm digital output and ignore digital output 3 command for semaphore. Made for demo.
// #else
// // #ifdef SENSORV2
// // #define VERSION 0x0004 // 3 para sensor v1, 4 para sensor v2
// // #else
// // #define VERSION 0x0003 // 3 para sensor v1, 4 para sensor v2
// // #endif
// // #define VERSION 0x0005	// Corregido bug media movil
// // #define VERSION 0x0006	// Aumentados limites de power
// #define VERSION 0x0007 // Version DiscOVER
// #endif

// // Variables globales
// pthread_mutex_t lock;
// int logging;
// int stop_logging;

// /*
//  * Funcion de c�clculo de ancho, empleada por varios estados del automata de control
//  */
// double CalculateWidth(int metadatos[12])
// {
// 	double W = 0;
// 	double M00, M01, M10, M11, M02, M20;
// 	double X, Y;
// 	double u20, u11, u02;
// 	M00 = metadatos[1];
// 	M01 = metadatos[2];
// 	M10 = metadatos[3];
// 	M11 = metadatos[4];
// 	M02 = metadatos[5];
// 	M20 = metadatos[6];
// 	// Procesos de calculo
// 	if (M00 == 0)
// 		M00 = 1;
// 	X = M10 / M00;
// 	Y = M01 / M00;

// 	u20 = (M20 / M00) - (X * X);
// 	u11 = (M11 / M00) - (X * Y);
// 	u02 = (M02 / M00) - (Y * Y);

// 	W = sqrt(8 * (u20 + u02 - sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02))))); // Aqui se ha calculado el ancho
// 	return W;
// }

// /*
//  * Funcion que inicializa las memorias con la estructura de datos obtenida de un fichero.
//  * En caso de conflicto prioriza los Maximos a los Minimos.
//  */
// void InitializeMemory(sys_data data_struct, volatile int *shmemProcVar, volatile int *ptrAIMENCom, volatile int *shmem)
// {

// 	uint64_t time_track_aux = 0;

// 	// inicializacion de la memoria mapeada digitalmente shmemProcVar

// 	shmemProcVar[KI] = data_struct.conf_KI;
// 	shmemProcVar[KP] = data_struct.conf_KP;
// 	shmemProcVar[KD] = data_struct.conf_KD;
// 	if (data_struct.conf_MAX_POWER < data_struct.conf_MIN_POWER)
// 	{
// 		data_struct.conf_MIN_POWER = data_struct.conf_MAX_POWER - 1;
// 	}
// 	shmemProcVar[MAX_POWER] = data_struct.conf_MAX_POWER;
// 	shmemProcVar[MIN_POWER] = data_struct.conf_MIN_POWER;
// 	shmemProcVar[POWER_MAN] = data_struct.conf_POWER;
// 	if (data_struct.conf_MAX_POWER_LIMIT < data_struct.conf_MIN_POWER_LIMIT)
// 	{
// 		data_struct.conf_MIN_POWER_LIMIT = data_struct.conf_MAX_POWER_LIMIT - 1;
// 	}
// 	shmemProcVar[POWER_LIMIT_MAX] = data_struct.conf_MAX_POWER_LIMIT;
// 	ptrAIMENCom[PWM_LIMIT_MAX] = (unsigned int)((double)data_struct.conf_MAX_POWER_LIMIT - (double)shmemProcVar[MIN_POWER]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (double)shmemProcVar[MIN_POWER]));
// 	shmemProcVar[POWER_LIMIT_MIN] = data_struct.conf_MIN_POWER_LIMIT;
// 	ptrAIMENCom[PWM_LIMIT_MIN] = (unsigned int)((double)data_struct.conf_MIN_POWER_LIMIT - (double)shmemProcVar[MIN_POWER]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (double)shmemProcVar[MIN_POWER]));
// 	shmemProcVar[WIDTH_REF] = data_struct.conf_WIDTH_MANUAL;
// 	shmemProcVar[SET_REF_WIDTH] = 1; // Cuando comience el hilo de procesamiento de control se tomara el ancho de referencia
// 	shmemProcVar[PIXEL_MM_RATIO] = data_struct.conf_PIXEL_MM_RATIO;
// 	shmemProcVar[END_OF_PROCESS] = data_struct.conf_END_OF_PROCESS;
// 	shmemProcVar[LIMIT_INTEGRAL] = data_struct.conf_LIMIT_INTEGRAL;
// 	shmemProcVar[LIMIT_SLEW] = data_struct.conf_LIMIT_SLEW;
// 	shmemProcVar[BUFF_SIZE] = data_struct.conf_CIRCULAR_BUFFER_SIZE;
// 	shmemProcVar[ENABLE_ALARM] = data_struct.conf_ALARM_ENABLE;
// 	if (data_struct.conf_ALARM_MAX < data_struct.conf_ALARM_MIN)
// 	{
// 		data_struct.conf_ALARM_MIN = data_struct.conf_ALARM_MAX - 1;
// 	}
// 	shmemProcVar[ALARM_MAX] = data_struct.conf_ALARM_MAX;
// 	shmemProcVar[ALARM_MIN] = data_struct.conf_ALARM_MIN;
// 	shmemProcVar[ALARM_TIME] = data_struct.conf_ALARM_TIME;
// 	shmemProcVar[AUTOMEASURE] = data_struct.conf_AUTOMEASURE;
// 	shmemProcVar[AUTOSHUTTER_CONFIG] = data_struct.conf_CONF_AUTOSHUTTER;
// 	shmemProcVar[AUTOSHUTTER_TEMP] = data_struct.conf_DRIFT_TEMP_AUTOSHUTTER;
// 	shmemProcVar[AUTOSHUTTER_TIMER] = data_struct.conf_TIMER_AUTOSHUTTER;
// 	shmemProcVar[TRACK_REF_START] = data_struct.conf_TRACK_REF_START;
// 	shmemProcVar[LASER_EXTERAL_CONTROL] = data_struct.conf_LASER_EXTERNAL_CONTROL;
// 	shmemProcVar[DELAY_LASER_ON] = data_struct.conf_DELAY_LASER_ON;
// 	shmemProcVar[PREHEATING_ENA] = data_struct.conf_PREHEATING_ENA;
// 	shmemProcVar[PREHEATING_TIME] = data_struct.conf_PREHEATING_TIME;
// 	shmemProcVar[PREHEATING_POWER] = data_struct.conf_PREHEATING_POWER;

// 	// inicializacion de la memoria Baseaddress aimen command

// 	if (data_struct.conf_START_TRACK_MOM < data_struct.conf_STOP_TRACK_MOM)
// 	{
// 		data_struct.conf_STOP_TRACK_MOM = data_struct.conf_START_TRACK_MOM - 1;
// 	}
// 	ptrAIMENCom[START_TRACK_MOM_T] = data_struct.conf_START_TRACK_MOM;
// 	ptrAIMENCom[END_OF_TRACK] = data_struct.conf_STOP_TRACK_MOM;
// 	ptrAIMENCom[MODE] = data_struct.conf_MODE;
// 	ptrAIMENCom[REFERENCE_TRACK] = data_struct.conf_TRACK_REFERENCE;

// 	time_track_aux = (uint64_t)(((double)data_struct.conf_TRACK_LENGTH) * (CLK_100MHZ / 10));
// 	ptrAIMENCom[TIME_TRACK_LOW] = (unsigned int)(0x00000000FFFFFFFF & time_track_aux);
// 	ptrAIMENCom[TIME_TRACK_HIGH] = (unsigned int)((0xFFFFFFFF00000000 & time_track_aux) >> 32);

// 	ptrAIMENCom[THRESHOLD] = data_struct.conf_THRESHOLD;
// 	ptrAIMENCom[ROI_ROUND] = data_struct.conf_ROI_ROUND;
// 	ptrAIMENCom[ENABLE_ROI] = data_struct.conf_ROI_ENABLE;
// 	if (data_struct.conf_X2_PIXEL < data_struct.conf_X1_PIXEL)
// 	{
// 		data_struct.conf_X2_PIXEL = data_struct.conf_X1_PIXEL;
// 	}
// 	ptrAIMENCom[ROI_X1] = data_struct.conf_X1_PIXEL;
// 	ptrAIMENCom[ROI_X2] = data_struct.conf_X2_PIXEL;
// 	if (data_struct.conf_Y2_PIXEL < data_struct.conf_Y1_PIXEL)
// 	{
// 		data_struct.conf_Y2_PIXEL = data_struct.conf_Y1_PIXEL;
// 	}
// 	ptrAIMENCom[ROI_Y1] = data_struct.conf_Y1_PIXEL;
// 	ptrAIMENCom[ROI_Y2] = data_struct.conf_Y2_PIXEL;
// 	ptrAIMENCom[DIGITAL_OUT_CONF] = data_struct.conf_CONF_DIGITALIO;

// 	// Inicializacion de la memoria Baseaddress NIT command
// 	shmem[NIT_BLACK_LEVEL] = data_struct.conf_BLACK_LEVEL;
// 	shmem[NIT_BIAS_V] = data_struct.conf_BIAS_VOLTAGE;
// 	shmem[NIT_INT_TIME] = data_struct.conf_INTEGRATION_TIME;
// #ifdef SENSORV2
// 	// printf("Drift enable:		%d\n", data_struct.conf_ENA_DRIFT);
// 	// printf("Drift intensity:	%d\n", data_struct.conf_DRIFT_INTENSITY);
// 	shmem[NIT_DRIFT_ENABLE] = data_struct.conf_ENA_DRIFT;
// 	shmem[NIT_DRIFT_POSITION] = data_struct.conf_DRIFT_INTENSITY;
// #endif

// 	ptrAIMENCom[CHANGE_OP_MODE] = 1; // Indica a la memoria el cambio al nuevo modo al finalizar la inicializacion.
// 	shmemProcVar[AUTO_SHUTTER] = 1;	 // realizar� un autoshutter con la nueva configuracion en cuanto se inicie el proceso de control

// 	/*//Escribe en pantalla para comprobar inicializacion. en orden alfabetico de la estructura.
// 	//Solo en fase de desarrollo, quitar/comentar al pasar.
// 	printf("\n***SYSTEM INITIALIZATION VALUES***\n\n");
// 	printf("Alarm enable: 				%d \n", data_struct.conf_ALARM_ENABLE);
// 	printf("Alarm Max:			 	%d\n", data_struct.conf_ALARM_MAX);
// 	printf("Alarm Min:			 	%d\n", data_struct.conf_ALARM_MIN);
// 	printf("Alarm time:				%d\n", data_struct.conf_ALARM_TIME);
// 	printf("Automeasure:				%d\n", data_struct.conf_AUTOMEASURE);
// 	printf("BIAS:			 		%d\n", data_struct.conf_BIAS_VOLTAGE);
// 	printf("Blackevel:			 	%d\n", data_struct.conf_BLACK_LEVEL);
// 	printf("Circular buffer size:			%d\n", data_struct.conf_CIRCULAR_BUFFER_SIZE);
// 	printf("CONF Autoshutter:			%d\n", data_struct.conf_CONF_AUTOSHUTTER);
// 	printf("CONF digitalIO:				%d\n", data_struct.conf_CONF_DIGITALIO);
// 	printf("Laser ON delay: 			%d\n", data_struct.conf_DELAY_LASER_ON);
// 	printf("Temp autoshutter:	 		%d\n", data_struct.conf_DRIFT_TEMP_AUTOSHUTTER);
// 	printf("End of process:				%d\n", data_struct.conf_END_OF_PROCESS);
// 	printf("Integration time:			%d\n", data_struct.conf_INTEGRATION_TIME);
// 	printf("KD:			 		%d\n", data_struct.conf_KD);
// 	printf("KI:			 		%d\n", data_struct.conf_KI);
// 	printf("KP:			 		%d\n", data_struct.conf_KP);
// 	printf("Laser external: 			%d\n", data_struct.conf_LASER_EXTERNAL_CONTROL);
// 	printf("Limit Integral:				%d\n", data_struct.conf_LIMIT_INTEGRAL);
// 	printf("Limit Slew:			 	%d\n", data_struct.conf_LIMIT_SLEW);
// 	printf("Max Power:				%d\n", data_struct.conf_MAX_POWER);
// 	printf("Max power limit:			%d\n", data_struct.conf_MAX_POWER_LIMIT);
// 	printf("Min Power:			 	%d\n", data_struct.conf_MIN_POWER);
// 	printf("Min power limit:			%d\n", data_struct.conf_MIN_POWER_LIMIT);
// 	printf("MODE:					%d\n", data_struct.conf_MODE);
// 	printf("Pixel/mm ratio:				%d\n", data_struct.conf_PIXEL_MM_RATIO);
// 	printf("Power:			 		%d\n", data_struct.conf_POWER);
// 	printf("Enable preheating:  			%d\n", data_struct.conf_PREHEATING_ENA);
// 	printf("Preheating power:   			%d\n", data_struct.conf_PREHEATING_POWER);
// 	printf("Preheating time:    			%d\n", data_struct.conf_PREHEATING_TIME);
// 	printf("ROI enable:			 	%d\n", data_struct.conf_ROI_ENABLE);
// 	printf("Start track mom:			%d\n", data_struct.conf_START_TRACK_MOM);
// 	printf("Stop track mom:				%d\n", data_struct.conf_STOP_TRACK_MOM);
// 	printf("threshold:			 	%d\n", data_struct.conf_THRESHOLD);
// 	printf("Timer Autoshutter:			%d\n", data_struct.conf_TIMER_AUTOSHUTTER);
// 	printf("Track Length:				%d\n", data_struct.conf_TRACK_LENGTH);
// 	printf("Track Reference:			%d\n", data_struct.conf_TRACK_REFERENCE);
// 	printf("Track reference Start:			%d\n", data_struct.conf_TRACK_REF_START);
// 	printf("Width manual:				%d\n", data_struct.conf_WIDTH_MANUAL);
// 	printf("X1:			 		%d\n", data_struct.conf_X1_PIXEL);
// 	printf("X2:			 		%d\n", data_struct.conf_X2_PIXEL);
// 	printf("Y1:			 		%d\n", data_struct.conf_Y1_PIXEL);
// 	printf("Y2:			 		%d\n", data_struct.conf_Y2_PIXEL);*/
// }

// /*
//  * Salva la configuracion inicial en el fichero CONFIG.sys.
//  * Permite que la configuracion actual sea la de inicio del sistema embebido.
//  */
// int SaveConf(volatile int *shmemProcVar, volatile int *ptrAIMENCom, volatile int *shmem)
// {
// 	int result = 0;
// 	uint64_t time_track_aux = 0;
// 	FILE *fdCONF;
// 	fdCONF = fopen("/mnt/mmc/sys/CONFIG.sys", "w");
// 	if (fdCONF == NULL)
// 	{
// 		printf("\n*** UNABLE TO SAVE CONFIGURATION ***\n");
// 		return result;
// 	}
// 	else
// 	{
// 		fprintf(fdCONF, "KI %d\n", shmemProcVar[KI]);
// 		fprintf(fdCONF, "KP %d\n", shmemProcVar[KP]);
// 		fprintf(fdCONF, "KD %d\n", shmemProcVar[KD]);
// 		fprintf(fdCONF, "MAX_POWER %d\n", shmemProcVar[MAX_POWER]);
// 		fprintf(fdCONF, "MIN_POWER %d\n", shmemProcVar[MIN_POWER]);
// 		fprintf(fdCONF, "POWER %d\n", shmemProcVar[POWER_MAN]);
// 		fprintf(fdCONF, "MAX_POWER_LIMIT %d\n", shmemProcVar[POWER_LIMIT_MAX]);
// 		fprintf(fdCONF, "MIN_POWER_LIMIT %d\n", shmemProcVar[POWER_LIMIT_MIN]);
// 		fprintf(fdCONF, "WIDTH_MANUAL %.2f\n", ((float)shmemProcVar[WIDTH_REF] / 100));
// 		fprintf(fdCONF, "PIXEL_MM_RATIO %.3f\n", ((float)shmemProcVar[PIXEL_MM_RATIO] / 1000));
// 		fprintf(fdCONF, "END_OF_PROCESS %d\n", shmemProcVar[END_OF_PROCESS]);
// 		fprintf(fdCONF, "LIMIT_INTEGRAL %d\n", shmemProcVar[LIMIT_INTEGRAL]);
// 		fprintf(fdCONF, "LIMIT_SLEW %.2f\n", ((float)shmemProcVar[LIMIT_SLEW] / 100));
// 		fprintf(fdCONF, "CIRCULAR_BUFFER_SIZE %d\n", shmemProcVar[BUFF_SIZE]);
// 		fprintf(fdCONF, "ALARM_ENABLE %d\n", shmemProcVar[ENABLE_ALARM]);
// 		fprintf(fdCONF, "ALARM_MAX %.2f\n", ((float)shmemProcVar[ALARM_MAX] / 100));
// 		fprintf(fdCONF, "ALARM_MIN %.2f\n", ((float)shmemProcVar[ALARM_MIN] / 100));
// 		fprintf(fdCONF, "ALARM_TIME %d\n", shmemProcVar[ALARM_TIME]);
// 		fprintf(fdCONF, "AUTOMEASURE %d\n", shmemProcVar[AUTOMEASURE]);
// 		fprintf(fdCONF, "CONF_AUTOSHUTTER %d\n", shmemProcVar[AUTOSHUTTER_CONFIG]);
// 		fprintf(fdCONF, "DRIFT_TEMP_AUTOSHUTTER %.1f\n", ((float)shmemProcVar[AUTOSHUTTER_TEMP] / 10));
// 		fprintf(fdCONF, "TIMER_AUTOSHUTTER %d\n", (shmemProcVar[AUTOSHUTTER_TIMER] * 10));
// 		fprintf(fdCONF, "TRACK_REF_START %d\n", shmemProcVar[TRACK_REF_START]);
// 		fprintf(fdCONF, "LASER_EXTERNAL_CONTROL %d\n", shmemProcVar[LASER_EXTERAL_CONTROL]);
// 		fprintf(fdCONF, "DELAY_LASER_ON %d\n", shmemProcVar[DELAY_LASER_ON]);
// 		fprintf(fdCONF, "PREHEATING_ENA %d\n", shmemProcVar[PREHEATING_ENA]);
// 		fprintf(fdCONF, "PREHEATING_TIME %d\n", shmemProcVar[PREHEATING_TIME]);
// 		fprintf(fdCONF, "PREHEATING_POWER %d\n", shmemProcVar[PREHEATING_POWER]);
// 		fprintf(fdCONF, "START_TRACK_MOM %d\n", ptrAIMENCom[START_TRACK_MOM_T]);
// 		fprintf(fdCONF, "STOP_TRACK_MOM %d\n", ptrAIMENCom[END_OF_TRACK]);
// 		fprintf(fdCONF, "MODE %d\n", ptrAIMENCom[MODE]);
// 		fprintf(fdCONF, "TRACK_REFERENCE %d\n", ptrAIMENCom[REFERENCE_TRACK]);
// 		time_track_aux = ((unsigned int)ptrAIMENCom[TIME_TRACK_HIGH]);
// 		time_track_aux = ((time_track_aux << 32) | ((unsigned int)ptrAIMENCom[TIME_TRACK_LOW]));
// 		fprintf(fdCONF, "TRACK_LENGTH %.1f\n", (((float)time_track_aux / CLK_100MHZ)));
// 		fprintf(fdCONF, "THRESHOLD %d\n", ptrAIMENCom[THRESHOLD]);
// 		fprintf(fdCONF, "ROI_ROUND %d\n", ptrAIMENCom[ROI_ROUND]);
// 		fprintf(fdCONF, "ROI_ENABLE %d\n", ptrAIMENCom[ENABLE_ROI]);
// 		fprintf(fdCONF, "X1_PIXEL %d\n", ptrAIMENCom[ROI_X1]);
// 		fprintf(fdCONF, "X2_PIXEL %d\n", ptrAIMENCom[ROI_X2]);
// 		fprintf(fdCONF, "Y1_PIXEL %d\n", ptrAIMENCom[ROI_Y1]);
// 		fprintf(fdCONF, "Y2_PIXEL %d\n", ptrAIMENCom[ROI_Y2]);
// 		fprintf(fdCONF, "CONF_DIGITALIO %d\n", ptrAIMENCom[DIGITAL_OUT_CONF]);
// 		fprintf(fdCONF, "BLACK_LEVEL %d\n", shmem[NIT_BLACK_LEVEL]);
// 		fprintf(fdCONF, "BIAS_VOLTAGE %f\n", ((float)shmem[NIT_BIAS_V] * 0.00018311));
// 		fprintf(fdCONF, "INTEGRATION_TIME %d\n", shmem[NIT_INT_TIME]);
// #ifdef SENSORV2
// 		fprintf(fdCONF, "ENA_DRIFT %d\n", shmem[NIT_DRIFT_ENABLE]);
// 		fprintf(fdCONF, "DRIFT_INTENSITY %d\n", shmem[NIT_DRIFT_POSITION]);
// #endif
// 		result = 1;
// 		fclose(fdCONF);
// 	}
// 	return result;
// }

// /*
//  * Function to load a BPC table and write its contents into the CLAMIR system
//  */
// int LoadBPCtable(volatile int *shmemProcVar)
// {
// 	int resultado = 0;
// 	int fdBPCtable;
// 	int contadorBPCtable = 0;
// 	fdBPCtable = open("/dev/mem", O_RDWR | O_SYNC);
// 	volatile int *memBPC = NULL;
// 	memBPC = (volatile int *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fdBPCtable, BASEADDRESS_BPC_TABLE);
// 	int y_BPC, x_BPC;
// 	char SerialNumber[8];
// 	FILE *fdBPC = fopen("/mnt/mmc/sys/bpcc.sys", "r");
// 	if (fdBPC == NULL)
// 	{
// 		printf("\n Unable to load BPC table\n");
// 	}
// 	else
// 	{
// 		// printf("\n Reading BPC table\n");
// 		resultado = 1;
// 		if (fscanf(fdBPC, "%s", SerialNumber) != 0)
// 		{
// 			printf("*** Serial Number %s ***\n", SerialNumber);
// 			// memcpy((void *)shmemProcVar[SERIAL_NUMBER_HIGH], SerialNumber, 8);
// 			memcpy((void *)(((uint8_t *)shmemProcVar) + (SERIAL_NUMBER_LOW * 4)), SerialNumber, 8);
// 			while (fscanf(fdBPC, "%d %d", &y_BPC, &x_BPC) == 2)
// 			{
// 				// printf("Y: %d	X: %d\n", y_BPC, x_BPC);
// 				memBPC[contadorBPCtable] = (y_BPC << 8) | (x_BPC);
// 				contadorBPCtable++;
// 			}
// 		}
// 		else
// 		{
// 			// printf("Fichero BPC no valido\n");
// 			resultado = 0;
// 		}
// 		fclose(fdBPC);
// 	}

// 	close(fdBPCtable);
// 	munmap((int *)memBPC, 4096);
// 	memBPC = NULL;
// 	return resultado;
// }

// /*
//  * Funcion de guardado de ficheros LOG
//  */
// void *Logger(void *sharedMetadatos)
// {
// 	int *metaLog = (int *)sharedMetadatos;
// 	int bufferMeta[15];
// 	uint16_t header_v_id[2] = {1, 0};
// 	uint16_t num;
// 	uint16_t num_folder = 0;
// 	DIR *directory;
// 	struct dirent *dir;
// 	directory = opendir("/mnt/mmc/log");
// 	if (directory)
// 	{
// 		while ((dir = readdir(directory)) != NULL)
// 		{
// 			if ((dir->d_type != DT_UNKNOWN) && (dir->d_type == DT_DIR))
// 			{
// 				if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0))
// 				{
// 					continue;
// 				}
// 				else
// 				{
// 					sscanf(dir->d_name, "%hd", &num);
// 					if (num > num_folder)
// 					{
// 						num_folder = num;
// 					}
// 				}
// 			}
// 		}
// 		printf("Mayor identificador de directorio: %hd \n", num_folder);
// 	}
// 	else
// 	{
// 		printf("Fallo al abrir la direccion de log \n");
// 	}
// 	FILE *fdLOG = NULL;
// 	char logpath[30];
// 	memset(logpath, '\0', sizeof(logpath)); // inicializa el path a vacio
// 	num_folder++;
// 	sprintf(logpath, "/mnt/mmc/log/%05d", num_folder);
// 	mkdir(logpath, 0700);
// 	// log_version = 1;
// 	while (1)
// 	{
// 		pthread_mutex_lock(&lock);
// 		memcpy(bufferMeta, metaLog, 60);
// 		if (logging == 0)
// 		{
// 			header_v_id[1]++;
// 			sprintf(logpath, "/mnt/mmc/log/%05d/%05d.log", num_folder, header_v_id[1]);
// 			fdLOG = fopen(logpath, "w");
// 			if (fdLOG != NULL)
// 			{
// 				logging = 1;
// 				/*
// 				 * escritura de la cabecera
// 				 */
// 				fwrite(header_v_id, sizeof(uint16_t), 2, fdLOG); // Version & ID_PROC
// 				fwrite(bufferMeta + 11, sizeof(int), 2, fdLOG);	 // Frame Number and Timestamp
// 				// printf("Frame inicio: %d --- 0x%x\n",bufferMeta[11], bufferMeta[11]);
// 				/*
// 				 * escritura de la primera imagen
// 				 */
// 				fwrite(bufferMeta, sizeof(int), 2, fdLOG);		// Power y MOM00
// 				fwrite(bufferMeta + 7, sizeof(int), 3, fdLOG);	// Widths, track number y frame max
// 				fwrite(bufferMeta + 12, sizeof(int), 2, fdLOG); // IO status y Temp1(sin calcular)
// 			}
// 			else
// 			{
// 				header_v_id[1]--;
// 				printf("Unable to save log files\n");
// 			}
// 		}
// 		else
// 		{
// 			if (stop_logging == 1)
// 			{
// 				/*
// 				 * Finaliza el fichero de logging
// 				 */
// 				fclose(fdLOG);
// 				memset(logpath, '\0', sizeof(logpath));
// 				logging = 0;
// 				stop_logging = 0;
// 			}
// 			else
// 			{
// 				/*
// 				 * escritura de datos
// 				 */
// 				fwrite(bufferMeta, sizeof(int), 2, fdLOG);		// Power y MOM00
// 				fwrite(bufferMeta + 7, sizeof(int), 3, fdLOG);	// Widths, track number y frame max
// 				fwrite(bufferMeta + 12, sizeof(int), 2, fdLOG); // IO status y Temp1(sin calcular)
// 			}
// 		}
// 	}
// 	pthread_cancel(pthread_self());
// 	return NULL;
// }

// #include <exception>

// /*
//  * Funci�n de escritura de imagenes desde el CLAMIR
//  */
// // void ImageWriter(int newsockimgfd, volatile int16_t * imgmem)
// void ImageWriter(int newsockimgfd, volatile int *shmemMeta, volatile int *shmem)
// {

// 	unsigned char frame_sync[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

// 	// Inicializacion de variables de datos

// 	sem_t *semaforo;
// 	semaforo = sem_open(SEM_NAME, 0);

// 	int metadatos_cabecera[15]; //{Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20, Width, Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status, T1, T2}
// 	// Variable imagen
// 	int16_t image[4096];
// 	for (int16_t i = 0; i < 4096; i++)
// 	{
// 		image[i] = i * 2;
// 	}

// 	// int fdTraza1;
// 	// fdTraza1 = open("/dev/mem", O_RDWR | O_SYNC);
// 	// volatile int *traza1Ptr = NULL;
// 	// traza1Ptr = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdTraza1, 0x60001000);
// 	// int fdTraza2;
// 	// fdTraza2 = open("/dev/mem", O_RDWR | O_SYNC);
// 	// volatile int *traza2Ptr = NULL;
// 	// traza2Ptr = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdTraza2, 0x60002000);

// 	// Mapeado en memoria de la BRAM donde esta la imagen
// 	int fd_img = open("/dev/mem", O_RDWR | O_SYNC);
// 	volatile int16_t *imgmem = NULL;
// 	imgmem = (volatile int16_t *)mmap(NULL, BRAM_IMG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_img, BRAM_IMG_ADDRESS);

// 	int cont = 0;
// 	int status1 = 0;
// 	int status2 = 0;
// 	int nw = 0;
// 	float lVoltage, lResistance, auxTemp;

// 	/*Inicializacion de los metadatos de temperatura*/
// 	lVoltage = 3.0f / 4096.0f * (float)shmem[NIT_TEMP1]; // temp1
// 	lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
// 	auxTemp = (3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f; // Translate to deg C assuming B=3380
// 	metadatos_cabecera[13] = (int)(auxTemp * 10);

// 	lVoltage = 3.0f / 4096.0f * (float)shmem[NIT_TEMP2];
// 	lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
// 	auxTemp = (3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f; // Translate to deg C assuming B=3380
// 	metadatos_cabecera[14] = (int)(auxTemp * 10);

// 	while (nw >= 0)
// 	{


// 		sem_getvalue(semaforo, &status1);
// 		sem_wait(semaforo); // Bloqueo por semaforo, es el lazo de control el que lo desbloquea
// 		sem_getvalue(semaforo, &status2);
// 		// traza1Ptr[0] = 1;
// 		memcpy(metadatos_cabecera, (int *)shmemMeta, 52);
// 		cont++;
// 		if (cont >= 1000)
// 		{ // Se actualiza la temperatura cada 1000 imagenes
// 			cont = 0;
// 			lVoltage = 3.0f / 4096.0f * (float)shmem[NIT_TEMP1]; // temp1
// 			lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
// 			metadatos_cabecera[13] = (int)(10 * ((3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f)); // Translate to deg C assuming B=3380

// 			lVoltage = 3.0f / 4096.0f * (float)shmem[NIT_TEMP2];
// 			lResistance = 10000.f * ((3.3f / lVoltage) - 1.f);
// 			metadatos_cabecera[14] = (int)(10 * ((3380.0f / (log(lResistance) + 2.126235177f)) - 273.15f)); // Translate to deg C assuming B=3380
// 		}

// 		memcpy(image, (void *)imgmem, BRAM_IMG_SIZE);
// 		// traza2Ptr[0] = 1;
// 		shmemMeta[13] = metadatos_cabecera[13];
// 		shmemMeta[14] = metadatos_cabecera[14];
// #ifdef DEMOSEMAFORO
// 		if ((metadatos_cabecera[12] & 0x00000004) > 0)
// 		{
// 			metadatos_cabecera[12] = (metadatos_cabecera[12] & 0xFFFFFFFB);
// 		}
// 		else
// 		{
// 			metadatos_cabecera[12] = (metadatos_cabecera[12] | 0x00000004);
// 		}
// #endif
// 		nw = write(newsockimgfd, frame_sync, sizeof(frame_sync));
// 		if (nw < 0) {
// 			printf("ERROR writing image to socket (frame sync)\n");
// 			break;
// 		}

// 		nw = write(newsockimgfd, metadatos_cabecera, 60);
// 		// printf("Cabecera devuelta = %d", nw);
// 		if (nw < 0)
// 		{
// 			printf("ERROR writing image to socket (header)\n");
// 			break;
// 		}

// 		nw = write(newsockimgfd, image, 8192);
// 		// printf(" imagen = %d\n", nw);
// 		if (nw < 0)
// 		{
// 			printf("ERROR writing image to socket (image)\n");
// 			break;
// 		}
// 	}
// 	// close(fd_int);
// 	close(fd_img);
// 	munmap((int16_t *)imgmem, BRAM_NIT_SIZE);
// 	imgmem = NULL;

// 	// close(fdTraza1);
// 	// munmap(((int *)traza1Ptr), 64);
// 	// traza1Ptr = NULL;
// 	// close(fdTraza2);
// 	// munmap(((int *)traza2Ptr), 64);
// 	// traza2Ptr = NULL;
// }

// /*
//  * Función de gestion de comandos
//  */
// void CommandHost(int newsockfd, volatile int *shmem, volatile int *shmemProcVar, volatile int *ptrAIMENCom, volatile int *shmemReset)
// {
// 	int nr, nw = 1;
// 	uint64_t time_track_aux;
// 	int cont_tramas_vacias = 0;
// 	uint16_t buffer[2];
// 	gestion_comandos gest;

// 	/*
// 	 * Bucle de lectura
// 	 */
// 	while ((nr >= 0) && (cont_tramas_vacias < 3) && (nw >= 0))
// 	{

// 		bzero(buffer, 4);

// 		nr = recv(newsockfd, buffer, sizeof(buffer), MSG_WAITALL);

// 		if (nr < 0)
// 			printf("ERROR reading from socket");
// 		if (nr <= 0)
// 		{
// 			cont_tramas_vacias++;
// 			// printf("nr: %d, contador: %d\n", nr, cont_tramas_vacias);
// 		}
// 		else
// 		{
// 			// printf("Recibido address-command 0x%x, data %d\n",buffer[0],buffer[1]);
// 			gest = CommandManager(buffer[0]);
// 			if (gest.fpga_write == 0)
// 			{
// 				if (gest.lectura > 127)
// 				{ // lectura
// 					if (gest.offset == SERIAL_NUMBER_LOW)
// 					{
// 						buffer[1] = (uint16_t)(shmemProcVar[SERIAL_NUMBER_LOW] & 0x0000FFFF);
// 						nw = write(newsockfd, buffer, 4);
// 						buffer[1] = (uint16_t)((shmemProcVar[SERIAL_NUMBER_LOW] & 0xFFFF0000) >> 16);
// 						buffer[0] = 0x04AB;
// 						nw = write(newsockfd, buffer, 4);
// 						buffer[1] = (uint16_t)(shmemProcVar[SERIAL_NUMBER_HIGH] & 0x0000FFFF);
// 						buffer[0] = 0x04AC;
// 						nw = write(newsockfd, buffer, 4);
// 						buffer[1] = (uint16_t)((shmemProcVar[SERIAL_NUMBER_HIGH] & 0xFFFF0000) >> 16);
// 						buffer[0] = 0x04AD;
// 						nw = write(newsockfd, buffer, 4);
// 					}
// 					else
// 					{
// 						buffer[1] = (uint16_t)(shmemProcVar[gest.offset] & 0x0000FFFF);
// 						nw = write(newsockfd, buffer, 4);
// 					}
// 				}
// 				else
// 				{ // escritura de datos en memoria
// 					switch (gest.offset)
// 					{
// 					case POWER_LIMIT_MAX:
// 						shmemProcVar[gest.offset] = buffer[1];
// 						ptrAIMENCom[PWM_LIMIT_MAX] = (unsigned int)((double)buffer[1] - (double)shmemProcVar[MIN_POWER]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (double)shmemProcVar[MIN_POWER]));
// 						break;
// 					case POWER_LIMIT_MIN:
// 						shmemProcVar[gest.offset] = buffer[1];
// 						ptrAIMENCom[PWM_LIMIT_MIN] = (unsigned int)((double)buffer[1] - (double)shmemProcVar[MIN_POWER]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (double)shmemProcVar[MIN_POWER]));
// 						break;
// 					case MAX_POWER:
// 						shmemProcVar[gest.offset] = buffer[1];
// 						ptrAIMENCom[PWM_LIMIT_MAX] = (unsigned int)((double)shmemProcVar[POWER_LIMIT_MAX] - (int16_t)shmemProcVar[MIN_POWER]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (int16_t)shmemProcVar[MIN_POWER]));
// 						ptrAIMENCom[PWM_LIMIT_MIN] = (unsigned int)((double)shmemProcVar[POWER_LIMIT_MIN] - (int16_t)shmemProcVar[MIN_POWER]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (int16_t)shmemProcVar[MIN_POWER]));
// 						break;
// 					case MIN_POWER:
// 						shmemProcVar[gest.offset] = (int16_t)buffer[1];
// 						ptrAIMENCom[PWM_LIMIT_MAX] = (unsigned int)((double)shmemProcVar[POWER_LIMIT_MAX] - (int16_t)buffer[1]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (int16_t)buffer[1]));
// 						ptrAIMENCom[PWM_LIMIT_MIN] = (unsigned int)((double)shmemProcVar[POWER_LIMIT_MIN] - (int16_t)buffer[1]) * (16383 / ((double)shmemProcVar[MAX_POWER] - (int16_t)buffer[1]));
// 						break;
// 					default:
// 						shmemProcVar[gest.offset] = buffer[1];
// 						break;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				switch (gest.baseaddress)
// 				{
// 				case BASEADDRESS_NIT_COMMAND:
// 					if (gest.lectura > 127)
// 					{ // lectura
// 						if (gest.offset == NIT_SINCRONIZATION)
// 						{
// 							buffer[1] = 1;
// 							nw = write(newsockfd, buffer, 4); // Mero comando de sincronizacion, no se guarda en memoria
// 						}
// 						else if (gest.offset == NIT_ARM_SW_VERSION)
// 						{
// 							buffer[1] = (uint16_t)VERSION;
// 							nw = write(newsockfd, buffer, 4);
// 						}
// 						else if ((gest.offset == NIT_FPGA_VERSION) && (buffer[0] == 0x05A6))
// 						{
// 							buffer[1] = (uint16_t)((shmem[gest.offset] & 0xFFFF0000) >> 16);
// 							nw = write(newsockfd, buffer, 4);
// 						}
// 						else
// 						{
// 							buffer[1] = (uint16_t)(shmem[gest.offset] & 0x0000FFFF);
// 							nw = write(newsockfd, buffer, 4);
// 						}
// 						// printf("Respondiendo NIT lectura address-command 0x%x, data %d\n",buffer[0],buffer[1]);
// 					}
// 					else
// 					{ // escritura de datos en memoria
// 						if (gest.offset == NIT_SAVE_EMBEDDED_CONF)
// 						{
// 							if (SaveConf(shmemProcVar, ptrAIMENCom, shmem) == 0)
// 							{
// 								printf("Couldn't save current configuration\n");
// 							}
// 						}
// 						else
// 						{
// 							shmem[gest.offset] = buffer[1];
// 						}
// 					}
// 					break;

// 				case BASEADDRESS_AIMEN_COMMAND:
// 					if (gest.lectura > 127)
// 					{
// 						if (gest.offset != TIME_TRACK_LOW)
// 						{
// 							buffer[1] = (uint16_t)(ptrAIMENCom[gest.offset] & 0x0000FFFF);
// 							nw = write(newsockfd, buffer, 4);
// 						}
// 						else
// 						{
// 							time_track_aux = ((unsigned int)ptrAIMENCom[TIME_TRACK_HIGH]);
// 							time_track_aux = ((time_track_aux << 32) | ((unsigned int)ptrAIMENCom[TIME_TRACK_LOW]));
// 							buffer[1] = ((uint16_t)(((double)time_track_aux / CLK_100MHZ) * 10));
// 							nw = write(newsockfd, buffer, 4);
// 						}
// 					}
// 					else
// 					{
// 						switch (gest.offset)
// 						{

// 						case TIME_TRACK_LOW:
// 							time_track_aux = (uint64_t)(((double)buffer[1]) * (CLK_100MHZ / 10));
// 							ptrAIMENCom[gest.offset] = (unsigned int)(0x00000000FFFFFFFF & time_track_aux);
// 							ptrAIMENCom[TIME_TRACK_HIGH] = (unsigned int)((0xFFFFFFFF00000000 & time_track_aux) >> 32);
// 							break;

// 						case MODE:
// 							ptrAIMENCom[gest.offset] = buffer[1];
// 							ptrAIMENCom[CHANGE_OP_MODE] = 1;
// 							break;

// 						default:
// 							ptrAIMENCom[gest.offset] = buffer[1];
// 							break;
// 						}
// 						// printf("escribiendo en  0x%x +  0x%x = 0x%x\n",gest.baseaddress,(gest.offset*4),gest.baseaddress+ (gest.offset*4));
// 					}
// 					break;
// 				case BASEADDRESS_AIMEN_STARTUP:
// 					shmemReset[gest.offset] = buffer[1];
// 					break;
// 				default:
// 					printf("Error in ADDRESS identification :%d \n", gest.baseaddress);
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	if (cont_tramas_vacias == 3)
// 		printf("Error en lectura, recibiendo tramas vacias\n");
// }

// /*
//  * Funcion de control de CLAMIR
//  */

// void ClosedLoopControlFunct(volatile int *shmemMeta, volatile int *shmemProcVar, volatile int *ptrAIMENCom, volatile int *shmemReset, volatile int *shmem)
// {
// 	int estadoAutomata = 0;
// 	int valor1 = 0;
// 	int valor2 = 0;
// 	int contadorFramesMidiendo = 0;

// 	// Variables calculo potencia
// 	double width_ref = 1;
// 	double error_t0 = 0;
// 	double error_t1 = 0;
// 	double potencia_t0 = 0;
// 	double potencia_t1 = 0;
// 	double dt = 0.001;
// 	double potenciaMax, potenciaMin, limPotenciaMax, limPotenciaMin;
// 	double ki, kp, kd;
// 	double duty;
// 	double pixel_mm_ratio;
// 	double integral = 0;
// 	double derivative, limiteIntegral, limit_integral_divided, limiteSlew;
// 	int mode = 0;
// 	int laser_status = 0; // 0 OFF, 1 ON
// 	int contadorFramesStop = 0;
// 	int endP = 1000;
// 	double delta_potencia = 0;
// 	int track_cnt = 0;
// 	int track = 0;
// 	uint64_t track_length = 2000;

// 	sem_t *semaforo;
// 	semaforo = sem_open(SEM_NAME, O_CREAT, 0644, 0);
// 	// File descriptor de UIO
// 	int pending = 0;
// 	int enable = 1; // Habilita interrupciones
// 	int fd_int = open("/dev/uio0", O_RDWR);
// 	if (fd_int < 0)
// 	{
// 		printf("No se puede abrir el descriptor de uio0 para copntrol\n");
// 	}

// 	int fdMetadatos;
// 	fdMetadatos = open("/dev/mem", O_RDWR | O_SYNC);
// 	volatile int *metaPtr = NULL;
// 	metaPtr = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdMetadatos, BRAM_IMG_METADATOS);

// 	// int fdTraza0;
// 	// fdTraza0 = open("/dev/mem", O_RDWR | O_SYNC);
// 	// volatile int *traza0Ptr = NULL;
// 	// traza0Ptr = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdTraza0, 0x60000000);
// 	int metadatos[12];
// 	double W = 0;
// 	double width_aux = 0;
// 	double W_circ_buff_aux = 0;
// 	double w_circ_buffer[512];
// 	for (int i = 0; i < 512; i++)
// 	{
// 		w_circ_buffer[i] = 0;
// 	}
// 	int control_NAP = 0;
// 	int measurement_NAP = 0;
// 	int cont_calibration = 0;
// 	uint16_t alarm_enable; // 1 alarm ON, 0 alarm OFF
// 	double alarm_min;
// 	double alarm_max;
// 	uint16_t alarm_time;
// 	uint16_t cnt_aux_alarm = shmemProcVar[ALARM_TIME];
// 	uint16_t alarm;
// 	uint16_t automeasure_flag, autoshutter_config;
// 	uint32_t autoshutter_time_target;
// 	float autoshutter_temp_target;
// 	int resultado = 0;
// 	int cont = 0;
// 	int z = 0;
// 	int circ_buffer_size = 1;
// 	write(fd_int, (void *)&enable, sizeof(int));
// 	derivative = 0;

// 	char auto_auto_shutter_enable;
// 	char auto_auto_shutter_enable_inprocess;
// 	char auto_auto_shutter_time_xtemp;
// 	float current_temperature;
// 	float temperature_last_autoshutter = ((float)shmemMeta[13]) / 10;
// 	int cnt_last_autoshutter = 0;
// 	char pending_autoshutter = 0;
// 	uint16_t laser_external = 0;
// 	int last_laser_status = 0;
// 	int delay_laser_on = 0;
// 	int cont_preheating = 0;
// 	while (1)
// 	{
// 		ki = shmemProcVar[KI];
// 		kp = shmemProcVar[KP];
// 		kd = shmemProcVar[KD];
// 		potenciaMax = (double)shmemProcVar[MAX_POWER];
// 		potenciaMin = (double)shmemProcVar[MIN_POWER];
// 		if (potenciaMin > 32767)
// 		{
// 			potenciaMin = potenciaMin - 65536;
// 		}
// 		limPotenciaMax = shmemProcVar[POWER_LIMIT_MAX];
// 		limPotenciaMin = shmemProcVar[POWER_LIMIT_MIN];
// 		pixel_mm_ratio = ((double)shmemProcVar[PIXEL_MM_RATIO] / 1000); // se reciben en micras
// 		shmemProcVar[PID_ERROR] = (int)100 * error_t1;
// 		limiteIntegral = (double)shmemProcVar[LIMIT_INTEGRAL];
// 		limiteSlew = ((double)shmemProcVar[LIMIT_SLEW] / 100);
// 		circ_buffer_size = shmemProcVar[BUFF_SIZE];

// 		alarm_enable = shmemProcVar[ENABLE_ALARM];
// 		alarm_max = ((double)shmemProcVar[ALARM_MAX]) / 100;
// 		alarm_min = ((double)shmemProcVar[ALARM_MIN]) / 100;
// 		alarm_time = shmemProcVar[ALARM_TIME];
// 		automeasure_flag = shmemProcVar[AUTOMEASURE];

// 		autoshutter_config = shmemProcVar[AUTOSHUTTER_CONFIG]; // booleano, comprobar flags
// 		// decode_config
// 		auto_auto_shutter_enable = autoshutter_config & 0x01;
// 		auto_auto_shutter_enable_inprocess = (autoshutter_config >> 1) & 0x01;
// 		auto_auto_shutter_time_xtemp = (autoshutter_config >> 3) & 0x01;
// 		// decodee config end
// 		autoshutter_temp_target = ((float)shmemProcVar[AUTOSHUTTER_TEMP]) / 10; // de int16 recibido a double o float, dividir entre 10
// 		autoshutter_time_target = shmemProcVar[AUTOSHUTTER_TIMER] * 10000;		//	de int16 recibido a int32, multiplicar por 10

// 		laser_external = shmemProcVar[LASER_EXTERAL_CONTROL];

// 		if (circ_buffer_size < 1)
// 		{
// 			circ_buffer_size = 1;
// 		}
// 		else if (circ_buffer_size > LMAX_CIRCULAR_BUFFER_SIZE)
// 		{
// 			circ_buffer_size = LMAX_CIRCULAR_BUFFER_SIZE;
// 		}
// 		if (mode != ptrAIMENCom[MODE])
// 		{
// 			mode = ptrAIMENCom[MODE];
// 			if (mode == 2)
// 			{
// 				estadoAutomata = MANUAL;
// 				printf("\nCambio a estado MANUAL\n");
// 				if (logging == 1)
// 				{
// 					stop_logging = 1;
// 					cont = 0;
// 					pthread_mutex_unlock(&lock);
// 				}
// 			}
// 			else
// 			{
// 				estadoAutomata = IDLE;
// 				printf("\nCambio a estado IDLE\n");
// 				ptrAIMENCom[CHANGE_OP_MODE] = 1;
// 			}
// 		}

// 		read(fd_int, (int *)&pending, sizeof(int)); // Se bloquea hasta que sucede una interrupcion de uio0
// 		// traza0Ptr[0] = 1;
// 		memcpy(metadatos, (void *)metaPtr, 48);

// 		// Laser status
// 		if (laser_external)
// 		{
// 			laser_status = ptrAIMENCom[DIGITAL_IN_0];
// 			// laser_status = ptrAIMENCom[DIGITAL_IN_1];
// 		}
// 		else
// 		{
// 			if (laser_status)
// 			{
// 				if (metadatos[1] < ptrAIMENCom[END_OF_TRACK])
// 					laser_status = 0;
// 			}
// 			else
// 			{
// 				if (metadatos[1] >= ptrAIMENCom[START_TRACK_MOM_T])
// 					laser_status = 1;
// 			}
// 		}

// 		write(fd_int, (void *)&enable, sizeof(int)); // Habilita la interrupcion de uio0

// 		/*
// 		 * Cambio a lectura buffer circular
// 		 */
// 		if (shmemProcVar[AUTO_SHUTTER])
// 		{

// 			shmemProcVar[AUTO_SHUTTER] = 0;
// 			if (!measurement_NAP)
// 			{
// 				measurement_NAP = NAP_DURATION;
// 				control_NAP = NAP_DURATION;
// 				// shmemProcVar[AUTO_SHUTTER] = 0;
// 				cont_calibration = WAIT_START_CALIBRATION + 1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE;
// 				// if(shmem[NIT_SHUTTER] == 0)
// 				//{
// 				shmem[NIT_SHUTTER] = 1; // cerrar shutter
// 										//}
// 				cnt_last_autoshutter = 0;
// 				temperature_last_autoshutter = current_temperature;
// 			}
// 			// shmem[NIT_SHUTTER_RESET]=1;
// 		}
// 		if (!measurement_NAP)
// 		{
// 			ptrAIMENCom[DIGITAL_OUT_3] = 0;
// 			if (delay_laser_on > 0)
// 			{
// 				delay_laser_on--;
// 			}
// 			z += 1;
// 			if (z >= circ_buffer_size)
// 			{
// 				z = 0;
// 			}
// 			w_circ_buffer[z] = pixel_mm_ratio * CalculateWidth(metadatos);
// 			W_circ_buff_aux = 0;
// 			for (int y = 0; y < circ_buffer_size; y++)
// 			{
// 				W_circ_buff_aux += w_circ_buffer[y];
// 			}
// 			W = W_circ_buff_aux / (circ_buffer_size);
// 		}
// 		else
// 		{ // siesta
// 			ptrAIMENCom[DIGITAL_OUT_3] = 1;
// 			W = W;
// 			if ((cont_calibration <= 0) && (shmem[NIT_SHUTTER] != 0))
// 			{
// 				shmem[NIT_SHUTTER] = 0; // open shutter
// 			}
// 			else
// 			{
// 				if (cont_calibration == (WAIT_SECOND_APERTURE))
// 				{
// 					//	shmem[NIT_SHUTTER] = 0;
// 				}
// 				if (cont_calibration == (1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE))
// 				{
// 					shmem[NIT_OFFSET_UPDATE] = 1;
// 					// shmem[NIT_SHUTTER_RESET]=1;
// 				}
// 				// cont_calibration--;
// 			}
// 			cont_calibration--;
// 			measurement_NAP--;
// 			control_NAP--;
// 		}

// 		if (shmemProcVar[SET_REF_WIDTH] > 0)
// 		{
// 			width_ref = ((double)shmemProcVar[WIDTH_REF]) / 100;
// 			shmemProcVar[SET_REF_WIDTH] = 0;
// 		}

// 		switch (estadoAutomata)
// 		{
// 		case MANUAL:
// 			potencia_t0 = shmemProcVar[POWER_MAN];
// 			if (mode != 2)
// 			{
// 				estadoAutomata = IDLE;
// 				printf("\nCambio a estado IDLE\n");
// 				ptrAIMENCom[CHANGE_OP_MODE] = 1;
// 			}
// 			track_cnt = 0;
// 			track = 0;
// 			delay_laser_on = 0;
// 			break;

// 		case IDLE:
// 			ptrAIMENCom[CHANGE_OP_MODE] = 1;
// 			width_aux = 0;
// 			contadorFramesStop = 0;
// 			if (logging == 1)
// 			{
// 				stop_logging = 1;
// 				cont = 0;
// 				pthread_mutex_unlock(&lock);
// 			}
// 			else
// 			{
// 				if (laser_status)
// 				{
// 					if (shmemProcVar[PREHEATING_ENA])
// 					{
// 						cont_preheating = 0;
// 						estadoAutomata = PREHEATING;
// 						printf("\nCambio a estado PREHEATING\n");
// 					}
// 					else
// 					{
// 						estadoAutomata = MIDIENDO;
// 						printf("\nCambio a estado MIDIENDO\n");
// 						contadorFramesMidiendo = 0;
// 						pthread_mutex_unlock(&lock);
// 					}
// 					track_length = ((unsigned int)ptrAIMENCom[TIME_TRACK_HIGH]);
// 					track_length = (((track_length << 32) | ((unsigned int)ptrAIMENCom[TIME_TRACK_LOW])) * 1000) / CLK_100MHZ;
// 				}
// 				else
// 				{
// 					potencia_t0 = shmemProcVar[POWER_MAN];
// 					potencia_t1 = potencia_t0;
// 				}
// 			}
// 			track_cnt = 0;
// 			track = 0;
// 			delay_laser_on = 0;
// 			break;

// 		case PREHEATING:
// 			potencia_t0 = shmemProcVar[PREHEATING_POWER];
// 			if (cont_preheating++ >= shmemProcVar[PREHEATING_TIME])
// 			{
// 				estadoAutomata = MIDIENDO;
// 				contadorFramesMidiendo = 0;
// 				pthread_mutex_unlock(&lock);
// 				printf("\nCambio a estado MIDIENDO\n");
// 			}
// 			break;

// 		case MIDIENDO:
// 			cont++;
// 			if (mode == 0)
// 			{
// 				if (track_cnt >= track_length)
// 				{
// 					track_cnt = 0;
// 					track++;
// 				}
// 				else
// 				{
// 					track_cnt++;
// 				}
// 			}
// 			else if (mode == 1)
// 			{
// 				if ((laser_status > 0) && (laser_status != last_laser_status))
// 				{
// 					if (laser_external)
// 					{
// 						track++;
// 						delay_laser_on = shmemProcVar[DELAY_LASER_ON];
// 					}
// 					else
// 					{
// 						if (!measurement_NAP)
// 						{
// 							track++;
// 							delay_laser_on = shmemProcVar[DELAY_LASER_ON];
// 						}
// 					}
// 				}
// 			}
// 			// if ((laser_status) & (metadatos[7] >= shmemProcVar[TRACK_REF_START])) //sustituido por cuenta de tracks en el ARM
// 			if ((laser_status) & (track >= shmemProcVar[TRACK_REF_START]))
// 			{
// 				width_aux = width_aux + W;
// 				contadorFramesMidiendo++;
// 			}
// 			potencia_t0 = shmemProcVar[POWER_MAN];
// 			potencia_t1 = potencia_t0;
// 			// if (metadatos[7] >= ptrAIMENCom[REFERENCE_TRACK]){
// 			if (track >= ptrAIMENCom[REFERENCE_TRACK])
// 			{
// 				estadoAutomata = CONTROL;
// 				printf("\nCambio a estado CONTROL\n");
// 				if (contadorFramesMidiendo == 0)
// 				{
// 					contadorFramesMidiendo = 1;
// 				}
// 				if (automeasure_flag == 1)
// 				{
// 					width_ref = width_aux / contadorFramesMidiendo;
// 				}
// 				else
// 				{
// 					width_ref = ((double)shmemProcVar[WIDTH_REF]) / 100;
// 				}
// 				width_aux = 0;
// 				error_t0 = 0;
// 				error_t1 = 0;
// 				integral = 0;
// 				derivative = 0;
// 				contadorFramesMidiendo = 0;
// 			}
// 			break;

// 		case CONTROL:
// 			endP = shmemProcVar[END_OF_PROCESS];
// 			cont++;

// 			if (mode == 0)
// 			{
// 				if (track_cnt >= track_length)
// 				{
// 					track_cnt = 0;
// 					track++;
// 				}
// 				else
// 				{
// 					track_cnt++;
// 				}
// 			}
// 			else if (mode == 1)
// 			{
// 				if ((laser_status > 0) && (laser_status != last_laser_status))
// 				{
// 					if (laser_external)
// 					{
// 						track++;
// 						delay_laser_on = shmemProcVar[DELAY_LASER_ON];
// 					}
// 					else
// 					{
// 						if (!measurement_NAP)
// 						{
// 							track++;
// 							delay_laser_on = shmemProcVar[DELAY_LASER_ON];
// 						}
// 					}
// 				}
// 			}
// 			if (delay_laser_on > 0)
// 			{
// 				delay_laser_on--;
// 			}
// 			else
// 			{
// 				if ((!laser_status) || (control_NAP))
// 				{
// 					if (contadorFramesStop++ > endP)
// 					{
// 						estadoAutomata = IDLE;
// 						printf("\nCambio a estado IDLE\n");
// 						contadorFramesStop = 0;
// 						cont = 0;
// 						stop_logging = 1;
// 						pthread_mutex_unlock(&lock);
// 					}
// 				}
// 				else
// 				{
// 					contadorFramesStop = 0;
// 					// Comienza el c�clculo de la potencia
// 					error_t0 = width_ref - W;
// 					integral = integral + (error_t0 * dt);

// 					if (ki > 0)
// 					{
// 						limit_integral_divided = limiteIntegral / ki;
// 					}
// 					else
// 					{
// 						limit_integral_divided = 1000;
// 					}
// 					if (fabs(integral) >= (limit_integral_divided))
// 					{
// 						if (integral < 0)
// 						{
// 							integral = 0 - limit_integral_divided;
// 						}
// 						else
// 						{
// 							integral = limit_integral_divided;
// 						}
// 					}

// 					//				derivative = (error_t0 - error_t1)/dt; //comentada por posibilidad de valor muy grande
// 					derivative = (error_t0 - error_t1);
// 					potencia_t0 = shmemProcVar[POWER_MAN] + (kp * error_t0) + (ki * integral) + (kd * derivative);
// 					error_t1 = error_t0;

// 					delta_potencia = potencia_t1 - potencia_t0;
// 					if (abs(delta_potencia) > limiteSlew)
// 					{
// 						// printf("potencia_t0 : %f", potencia_t0);
// 						if (potencia_t0 < potencia_t1)
// 						{
// 							potencia_t0 = potencia_t1 - limiteSlew;
// 						}
// 						else
// 						{
// 							potencia_t0 = potencia_t1 + limiteSlew;
// 						}
// 						// printf(" potencia_t0 despues de limit : %f\n", potencia_t0);
// 					}
// 				}
// 			}

// 			break;
// 		default:
// 			break;
// 		}

// 		resultado = (((int)(100 * width_ref) << 16)) | ((int)(W * 100));

// 		if (potencia_t0 > limPotenciaMax)
// 		{
// 			potencia_t0 = limPotenciaMax;
// 		}
// 		else
// 		{
// 			if (potencia_t0 < limPotenciaMin)
// 			{
// 				potencia_t0 = limPotenciaMin;
// 			}
// 		}
// 		potencia_t1 = potencia_t0;
// 		metadatos[0] = potencia_t0;
// 		metadatos[7] = track;
// 		metadatos[11] = (estadoAutomata << 24) | (laser_status << 16) | metadatos[11];
// 		// Comienza el c�lculo del duty cycle
// 		// duty = (potencia_t0 - potenciaMin) * (1000/(potenciaMax - potenciaMin));
// 		duty = (potencia_t0 - potenciaMin) * (16383 / (potenciaMax - potenciaMin));
// 		ptrAIMENCom[PWM] = (unsigned int)duty;
// 		last_laser_status = laser_status;

// 		memcpy((void *)&shmemMeta[0], &metadatos, 28);	  // Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20
// 		shmemMeta[7] = resultado;						  // Width
// 		memcpy((void *)&shmemMeta[8], &metadatos[7], 20); // Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status

// 		// gestion de la alarma

// 		if ((alarm_enable == 1) && (laser_status))
// 		{
// 			if ((W > alarm_max) || (W < alarm_min))
// 			{
// 				if (cnt_aux_alarm > 0)
// 				{
// 					cnt_aux_alarm--;
// 					alarm = 0;
// 				}
// 				else
// 				{
// 					alarm = 1;
// 				}
// 			}
// 			else
// 			{
// 				cnt_aux_alarm = alarm_time;
// 				alarm = 0;
// 			}
// 		}
// 		else
// 		{
// 			cnt_aux_alarm = alarm_time;
// 			alarm = 0;
// 		}

// 		// Gestion de logging
// 		if (cont >= 100)
// 		{
// 			cont = 0;
// 			pthread_mutex_unlock(&lock);
// 		}

// 		// LEDS y autoshutter
// 		cnt_last_autoshutter++;
// 		current_temperature = ((float)shmemMeta[13]) / 10;
// 		if (auto_auto_shutter_time_xtemp)
// 		{
// 			if (autoshutter_time_target < cnt_last_autoshutter)
// 			{
// 				pending_autoshutter = 1;
// 				cnt_last_autoshutter = 0;
// 			}
// 		}
// 		else
// 		{
// 			if (autoshutter_temp_target < (fabs(temperature_last_autoshutter - current_temperature)))
// 			{
// 				pending_autoshutter = 1;
// 				cnt_last_autoshutter = 0;
// 				temperature_last_autoshutter = current_temperature;
// 			}
// 		}

// 		switch (estadoAutomata)
// 		{
// 		case MANUAL:
// 			if (auto_auto_shutter_enable && pending_autoshutter)
// 			{
// 				shmemProcVar[AUTO_SHUTTER] = 1;
// 				pending_autoshutter = 0;
// 			}
// 			break;
// 		case IDLE:
// 			if (auto_auto_shutter_enable && pending_autoshutter)
// 			{
// 				shmemProcVar[AUTO_SHUTTER] = 1;
// 				pending_autoshutter = 0;
// 			}
// 			break;
// 		case MIDIENDO:
// 			if (auto_auto_shutter_enable && auto_auto_shutter_enable_inprocess && pending_autoshutter)
// 			{
// 				shmemProcVar[AUTO_SHUTTER] = 1;
// 				pending_autoshutter = 0;
// 			}

// 			break;

// 		case CONTROL:
// 			if (auto_auto_shutter_enable && auto_auto_shutter_enable_inprocess && pending_autoshutter)
// 			{
// 				shmemProcVar[AUTO_SHUTTER] = 1;
// 				pending_autoshutter = 0;
// 			}
// 			break;

// 		default:
// 			break;
// 		}

// 		if (alarm == 1)
// 		{
// 			// ROJO
// 			shmemReset[LED_R] = 0;
// 			shmemReset[LED_G] = 1;
// 			shmemReset[LED_B] = 1;
// 		}
// 		else
// 		{

// 			switch (estadoAutomata)
// 			{
// 			case MANUAL: // AMARILLO
// 				shmemReset[LED_R] = 0;
// 				shmemReset[LED_G] = 0;
// 				shmemReset[LED_B] = 1;

// 				break;
// 			case IDLE: // VERDE
// 				shmemReset[LED_R] = 1;
// 				shmemReset[LED_G] = 0;
// 				shmemReset[LED_B] = 1;

// 				break;

// 			case MIDIENDO: // MORADO
// 				shmemReset[LED_R] = 0;
// 				shmemReset[LED_G] = 1;
// 				shmemReset[LED_B] = 0;

// 				break;

// 			case CONTROL: // AZUL
// 				shmemReset[LED_R] = 1;
// 				shmemReset[LED_G] = 1;
// 				shmemReset[LED_B] = 0;

// 				break;
// 			default:
// 				break;
// 			}
// 		}

// 		// ptrAIMENCom[DIGITAL_OUT_0]=alarm; //asignacion de la digital out 1 a la alarma
// #ifdef DEMOSEMAFORO
// 		ptrAIMENCom[DIGITAL_OUT_1] = 1; // naranja
// 		if (alarm_enable == 1)
// 		{ // Solo si la alarma est� habilitada

// 			if (alarm == 1)
// 			{
// 				ptrAIMENCom[DIGITAL_OUT_0] = 0; // asignacion de la digital out 1 a la alarma
// 				ptrAIMENCom[DIGITAL_OUT_2] = 1; // verde
// 			}
// 			else
// 			{
// 				ptrAIMENCom[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
// 				ptrAIMENCom[DIGITAL_OUT_2] = 0; // verde
// 			}
// 		}
// 		else
// 		{
// 			ptrAIMENCom[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
// 			ptrAIMENCom[DIGITAL_OUT_2] = 0; // verde
// 		}

// #else
// 		if (alarm_enable == 1)
// 		{ // Solo si la alarma est� habilitada
// 			ptrAIMENCom[DIGITAL_OUT_0] = alarm;
// 		}
// #endif
// 		sem_getvalue(semaforo, &valor1);
// 		if (valor1 < 1)
// 		{ // permite al productor enviar hasta 2 imagenes por TCP al empezar la conexion
// 			sem_post(semaforo);
// 		}
// 		sem_getvalue(semaforo, &valor2);

// 		// traza0Ptr[0] = 0;
// 	}
// 	close(fd_int);
// 	close(fdMetadatos);
// 	munmap(((int *)metaPtr), 64);
// 	metaPtr = NULL;
// 	// close(fdTraza0);
// 	// munmap(((int *)traza0Ptr), 64);
// 	// traza0Ptr = NULL;
// 	sem_close(semaforo);
// 	sem_unlink(SEM_NAME);
// 	exit(1);
// }

// #include <iostream>
// #include <thread>
// #include <vector>

// void SIGPIPE_handler(int s) {
// 	printf("Caught SIGPIPE\n");
// }

// int main()
// {

// 	// Shared memory para metadatos, memoria virtual no asociada a ninguna BRAM que no usa el driver de devmem
// 	//{Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20, Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status, Width}
// 	volatile int *shmemMeta = NULL;
// 	shmemMeta = (volatile int *)mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

// 	// Variables no escritas en la FPGA
// 	volatile int *shmemProcVar = NULL;
// 	shmemProcVar = (volatile int *)mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

// 	// Bancos de AIMEN
// 	int fdProccommand;
// 	fdProccommand = open("/dev/mem", O_RDWR | O_SYNC);
// 	if (fdProccommand < 0)
// 		printf("Failed to open /dev/mem for %d\n", BASEADDRESS_AIMEN_COMMAND);
// 	volatile int *ptrAIMENCom = NULL;
// 	ptrAIMENCom = (volatile int *)mmap(NULL, 5242888, PROT_READ | PROT_WRITE, MAP_SHARED, fdProccommand, BASEADDRESS_AIMEN_COMMAND);

// 	// Shared memory, lee y escribe comandos de NIT en la FPGA de configuraciones relativas de la c�mara
// 	int fd;
// 	fd = open("/dev/mem", O_RDWR | O_SYNC);
// 	if (fd < 0)
// 	{
// 		printf("Failed to open /dev/mem\n");
// 		exit(-1);
// 	}
// 	volatile int *shmem = NULL;
// 	shmem = (volatile int *)mmap(NULL, BRAM_NIT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASEADDRESS_NIT_COMMAND);

// 	/*
// 	 * Mapeado de memoria para reset
// 	 */
// 	int fdR;
// 	fdR = open("/dev/mem", O_RDWR | O_SYNC);
// 	volatile int *shmemReset = NULL;
// 	shmemReset = (volatile int *)mmap(NULL, 131072, PROT_READ | PROT_WRITE, MAP_SHARED, fdR, BASEADDRESS_AIMEN_STARTUP);

// 	/*
// 	 * Load BPC table
// 	 */
// 	if (LoadBPCtable(shmemProcVar))
// 	{
// 		shmem[NIT_BPC_EN] = 1;
// 	}

// 	/*
// 	 * Inicializacion de los valores de CLAMIR
// 	 */
// 	sys_data data_struct;
// 	data_struct = ReadSysFile(data_struct);
// 	InitializeMemory(data_struct, shmemProcVar, ptrAIMENCom, shmem);

// 	signal(SIGPIPE, SIGPIPE_handler);

// 	auto log_thread_function = [&]()
// 	{
// 		for (;;)
// 		{
// 			std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 		}
// 	};

// 	auto control_thread_function = [&]()
// 	{
// 		for (;;)
// 		{
// 			ClosedLoopControlFunct(shmemMeta, shmemProcVar, ptrAIMENCom, shmemReset, shmem);
// 			std::cout << "Closed Loop Control: Closed" << std::endl;
// 		}
// 	};

// 	auto server_command_host_thread_function = [&]()
// 	{
// 		int sockfd, newsockfd, portcomm, sockimgfd, newsockimgfd, portimg;

// 		socklen_t clilen, cliimglen;
// 		struct sockaddr_in serv_addr, cli_addr, serv_addr_img, cli_addr_img;

// 		/*
// 		 * Inicializacion del host TCP
// 		 */
// 		int set_reuseaddress = 1;

// 		/*
// 		 *	Socket de Puerto de comunicacion de comandos
// 		 */

// 		sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 		if (sockfd < 0)
// 		{
// 			printf("ERROR opening socket");
// 			exit(-1);
// 		}

// 		bzero((char *)&serv_addr, sizeof(serv_addr));

// 		portcomm = 4097;
// 		serv_addr.sin_family = AF_INET;
// 		serv_addr.sin_addr.s_addr = INADDR_ANY;
// 		serv_addr.sin_port = htons(portcomm);

// 		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set_reuseaddress, sizeof(set_reuseaddress)) < 0)
// 		{
// 			printf("Unable to use REUSEADDR 1");
// 			exit(-1);
// 		}

// 		if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
// 		{
// 			printf("ERROR on binding 1");
// 			exit(-1);
// 		}

// 		listen(sockfd, 5);

// 		for (;;)
// 		{

// 			auto sock = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

// 			if (sock < 0)
// 			{
// 				std::cout << "Command Host: Couldn't Accept a New Connection." << std::endl;
// 				continue;
// 			}

// 			CommandHost(sock, shmem, shmemProcVar, ptrAIMENCom, shmemReset);
// 			close(sock);
// 		}
// 	};

// 	auto server_video_stream_host_thread_function = [&]()
// 	{
// 		int sockfd, portcomm, portimg;

// 		socklen_t clilen, cliimglen;
// 		struct sockaddr_in serv_addr, cli_addr, serv_addr_img, cli_addr_img;

// 		/*
// 		 * Inicializacion del host TCP
// 		 */
// 		int set_reuseaddress = 1;

// 		/*
// 		 *	Socket de Puerto de comunicacion de comandos
// 		 */

// 		sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 		if (sockfd < 0)
// 		{
// 			printf("ERROR opening socket");
// 			exit(-1);
// 		}

// 		bzero((char *)&serv_addr, sizeof(serv_addr));

// 		portcomm = 4096;
// 		serv_addr.sin_family = AF_INET;
// 		serv_addr.sin_addr.s_addr = INADDR_ANY;
// 		serv_addr.sin_port = htons(portcomm);

// 		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set_reuseaddress, sizeof(set_reuseaddress)) < 0)
// 		{
// 			printf("Unable to use REUSEADDR 1");
// 			exit(-1);
// 		}

// 		if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
// 		{
// 			printf("ERROR on binding 1");
// 			exit(-1);
// 		}

// 		listen(sockfd, 5);

// 		for (;;)
// 		{

// 			auto sock = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

// 			if (sock < 0)
// 			{
// 				std::cout << "Video Stream: Couldn't Accept a New Connection." << std::endl;
// 				exit(-1);
// 			}

// 			ImageWriter(sock, shmemMeta, shmem);
// 		}
// 	};

// 	auto log_thread = std::thread(log_thread_function);
// 	auto control_thread = std::thread(control_thread_function);
// 	auto server_command_host_thread = std::thread(server_command_host_thread_function);
// 	auto server_video_stream_host_thread = std::thread(server_video_stream_host_thread_function);

// 	log_thread.join();
// 	control_thread.join();
// 	server_command_host_thread.join();
// 	server_video_stream_host_thread.join();
// }