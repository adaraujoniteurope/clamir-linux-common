#include "application_legacy_control_loop.hpp"

#define MANUAL 0
#define IDLE 8
#define MIDIENDO 9
#define CONTROL 10
#define PREHEATING 11

#define THRESHOLD 0x4000 + 0
#define INTENSITY_MIN 0x4000 + 1
#define INTENSITY_MAX 0x4000 + 2
#define MOM_00 0x4000 + 3
#define MOM_01 0x4000 + 4
#define MOM_10 0x4000 + 5
#define MOM_11 0x4000 + 6
#define MOM_02 0x4000 + 7
#define MOM_20 0x4000 + 8
#define START_TRACK_MOM_T 0x4000 + 9
#define TRACK_NBR 0x4000 + 10
#define MODE 0x4000 + 11
#define TIME_TRACK_LOW 0x4000 + 12
#define TIME_TRACK_HIGH 0x4000 + 13
#define FRAME_MAX 0x4000 + 14
#define REFERENCE_TRACK 0x4000 + 15
#define END_OF_TRACK 0x4000 + 16

#define VERSION_ID 0x0000 + 0
#define ENABLE_MOM 0x0000 + 1
#define ENABLE_ROI 0x0000 + 2
#define ENABLE_PWM 0x0000 + 3
#define ENABLE_IRQ 0x0000 + 4
#define CHANGE_OP_MODE 0x0000 + 5
#define IRQ_MICROBLAZE 0x0000 + 6
#define DIGITAL_IN_0 0x0000 + 7
#define DIGITAL_IN_1 0x0000 + 8
#define DIGITAL_OUT_0 0x0000 + 9
#define DIGITAL_OUT_1 0x0000 + 10
#define DIGITAL_OUT_CONF 0x0000 + 11
#define DIGITAL_IN_2 0x0000 + 12
#define DIGITAL_IN_3 0x0000 + 13
#define DIGITAL_OUT_2 0x0000 + 14
#define DIGITAL_OUT_3 0x0000 + 15

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

#define PWM_LIMIT_MAX 0xC000 + 0
#define PWM 0xC000 + 1
#define POWER 0xC000 + 2
#define PWM_LIMIT_MIN 0xC000 + 3

#define WAIT_START_CALIBRATION 150
#define WAIT_STOP_CALIBRATION 100 // 30
#define WAIT_SECOND_APERTURE 400
#define NAP_DURATION 750 // 300

#define LED_R 0x4000 + 0
#define LED_B 0x4000 + 1
#define LED_G 0x4000 + 2

#define NIT_INT_TIME 0			  // 0
#define NIT_BIAS_V 1			  // 4
#define NIT_OFFSET_EN 2			  // 8
#define NIT_OFFSET_UPDATE 3		  // 12
#define NIT_SHUTTER 4			  // 16
#define NIT_BPC_EN 5			  // 20
#define NIT_BPC_MEM_WRITE 6		  // 24
#define NIT_BPC_IDENTIFY 7		  // 28
#define NIT_TEMP1 8				  // 32
#define NIT_TEMP2 9				  // 36
#define NIT_TEMP3 10			  // 40
#define NIT_TEMP4 11			  // 44
#define NIT_SHUTTER_RESET 12	  // 48
#define NIT_SINCRONIZATION 13	  // 52
#define NIT_SAVE_EMBEDDED_CONF 14 // 56
#define NIT_ARM_SW_VERSION 15	  // 60
#define NIT_DRIFT_ENABLE 16		  // 64
#define NIT_DRIFT_POSITION 17	  // 68
#define NIT_DRIFT_LEVEL 18		  // 72
#define NIT_FPGA_VERSION 19		  // 76
#define NIT_9 20				  // 80
#define NIT_10 21				  // 84
#define NIT_11 22				  // 88
#define NIT_12 23				  // 92
#define NIT_13 24				  // 96
#define NIT_14 25				  // 100
#define NIT_15 26				  // 104
#define NIT_16 27				  // 108
#define NIT_17 28				  // 112
#define NIT_18 29				  // 116
#define NIT_19 30				  // 120
#define NIT_20 31				  // 124
#define NIT_TRIGGER_USEC 35		  // 140
#define NIT_BLACK_LEVEL 36		  // 144
#define NIT_21 37				  // 152

#define BRAM_IMG_METADATOS 0x42002000

#define SEM_NAME "semaforo"

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <string.h>

#include <application.hpp>

double application::calculate_width(int metadatos[12])
{
	double W = 0;
	double M00, M01, M10, M11, M02, M20;
	double X, Y;
	double u20, u11, u02;
	M00 = metadatos[1];
	M01 = metadatos[2];
	M10 = metadatos[3];
	M11 = metadatos[4];
	M02 = metadatos[5];
	M20 = metadatos[6];
	// Procesos de calculo
	if (M00 == 0)
		M00 = 1;
	X = M10 / M00;
	Y = M01 / M00;

	u20 = (M20 / M00) - (X * X);
	u11 = (M11 / M00) - (X * Y);
	u02 = (M02 / M00) - (Y * Y);

	W = sqrt(8 * (u20 + u02 - sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02))))); // Aqui se ha calculado el ancho
	return W;
}

void application::legacy_control_function(volatile int *virtual_metadata_shm, volatile int *real_metadata_shm, volatile int *proc_var_shm, volatile int *mb_core_shm, volatile int *arm_core_shm, volatile int *control_unit_shm)
{
	int estadoAutomata = 0;
	int valor1 = 0;
	int valor2 = 0;
	int contadorFramesMidiendo = 0;

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
	// int fd_int = open("/dev/uio0", O_RDWR);
	// if (fd_int < 0)
	// 	printf("No se puede abrir el descriptor de uio0 para copntrol\n");

	// int fdMetadatos;
	// fdMetadatos = open("/dev/mem", O_RDWR | O_SYNC);
	// volatile int *real_metadata_shm = NULL;
	// real_metadata_shm = (volatile int *)mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fdMetadatos, BRAM_IMG_METADATOS);

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
	uint16_t cnt_aux_alarm = proc_var_shm[ALARM_TIME];
	uint16_t alarm;
	uint16_t automeasure_flag, autoshutter_config;
	uint32_t autoshutter_time_target;
	float autoshutter_temp_target;
	int resultado = 0;
	int cont = 0;
	int z = 0;
	int circ_buffer_size = 1;
	// write(fd_int, (void *)&enable, sizeof(int));
	derivative = 0;

	char auto_auto_shutter_enable;
	char auto_auto_shutter_enable_inprocess;
	char auto_auto_shutter_time_xtemp;
	float current_temperature;
	float temperature_last_autoshutter = ((float)virtual_metadata_shm[13]) / 10;
	int cnt_last_autoshutter = 0;
	char pending_autoshutter = 0;
	uint16_t laser_external = 0;
	int last_laser_status = 0;
	int delay_laser_on = 0;
	int cont_preheating = 0;
	while (1)
	{
		ki = proc_var_shm[KI];
		kp = proc_var_shm[KP];
		kd = proc_var_shm[KD];
		potenciaMax = (double)proc_var_shm[MAX_POWER];
		potenciaMin = (double)proc_var_shm[MIN_POWER];
		if (potenciaMin > 32767)
		{
			potenciaMin = potenciaMin - 65536;
		}
		limPotenciaMax = proc_var_shm[POWER_LIMIT_MAX];
		limPotenciaMin = proc_var_shm[POWER_LIMIT_MIN];
		pixel_mm_ratio = ((double)proc_var_shm[PIXEL_MM_RATIO] / 1000); // se reciben en micras
		proc_var_shm[PID_ERROR] = (int)100 * error_t1;
		limiteIntegral = (double)proc_var_shm[LIMIT_INTEGRAL];
		limiteSlew = ((double)proc_var_shm[LIMIT_SLEW] / 100);
		circ_buffer_size = proc_var_shm[BUFF_SIZE];

		alarm_enable = proc_var_shm[ENABLE_ALARM];
		alarm_max = ((double)proc_var_shm[ALARM_MAX]) / 100;
		alarm_min = ((double)proc_var_shm[ALARM_MIN]) / 100;
		alarm_time = proc_var_shm[ALARM_TIME];
		automeasure_flag = proc_var_shm[AUTOMEASURE];

		autoshutter_config = proc_var_shm[AUTOSHUTTER_CONFIG]; // booleano, comprobar flags
		// decode_config
		auto_auto_shutter_enable = autoshutter_config & 0x01;
		auto_auto_shutter_enable_inprocess = (autoshutter_config >> 1) & 0x01;
		auto_auto_shutter_time_xtemp = (autoshutter_config >> 3) & 0x01;
		// decodee config end
		autoshutter_temp_target = ((float)proc_var_shm[AUTOSHUTTER_TEMP]) / 10; // de int16 recibido a double o float, dividir entre 10
		autoshutter_time_target = proc_var_shm[AUTOSHUTTER_TIMER] * 10000;		//	de int16 recibido a int32, multiplicar por 10

		laser_external = proc_var_shm[LASER_EXTERAL_CONTROL];

		if (circ_buffer_size < 1)
		{
			circ_buffer_size = 1;
		}
		else if (circ_buffer_size > LMAX_CIRCULAR_BUFFER_SIZE)
		{
			circ_buffer_size = LMAX_CIRCULAR_BUFFER_SIZE;
		}
		if (mode != mb_core_shm[MODE])
		{
			mode = mb_core_shm[MODE];
			if (mode == 2)
			{
				estadoAutomata = MANUAL;
				printf("\nCambio a estado MANUAL\n");
				// if (logging == 1)
				// {
				// 	stop_logging = 1;
				// 	cont = 0;
				// 	pthread_mutex_unlock(&lock);
				// }
			}
			else
			{
				estadoAutomata = IDLE;
				printf("\nCambio a estado IDLE\n");
				mb_core_shm[CHANGE_OP_MODE] = 1;
			}
		}

		m_timer->wait();
		// read(fd_int, (int *)&pending, sizeof(int)); // Se bloquea hasta que sucede una interrupcion de uio0
		memcpy(metadatos, (void *)real_metadata_shm, 48);

		// Laser status
		if (laser_external)
		{
			laser_status = mb_core_shm[DIGITAL_IN_0];
			// laser_status = mb_core_shm[DIGITAL_IN_1];
		}
		else
		{
			if (laser_status)
			{
				if (metadatos[1] < mb_core_shm[END_OF_TRACK])
					laser_status = 0;
			}
			else
			{
				if (metadatos[1] >= mb_core_shm[START_TRACK_MOM_T])
					laser_status = 1;
			}
		}

		// write(fd_int, (void *)&enable, sizeof(int)); // Habilita la interrupcion de uio0

		/*
		 * Cambio a lectura buffer circular
		 */
		if (proc_var_shm[AUTO_SHUTTER])
		{

			proc_var_shm[AUTO_SHUTTER] = 0;
			if (!measurement_NAP)
			{
				measurement_NAP = NAP_DURATION;
				control_NAP = NAP_DURATION;
				// proc_var_shm[AUTO_SHUTTER] = 0;
				cont_calibration = WAIT_START_CALIBRATION + 1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE;
				// if(control_unit_shm[NIT_SHUTTER] == 0)
				//{
				control_unit_shm[NIT_SHUTTER] = 1; // cerrar shutter
										//}
				cnt_last_autoshutter = 0;
				temperature_last_autoshutter = current_temperature;
			}
			// control_unit_shm[NIT_SHUTTER_RESET]=1;
		}
		if (!measurement_NAP)
		{
			mb_core_shm[DIGITAL_OUT_3] = 0;
			if (delay_laser_on > 0)
			{
				delay_laser_on--;
			}
			z += 1;
			if (z >= circ_buffer_size)
			{
				z = 0;
			}
			w_circ_buffer[z] = pixel_mm_ratio * calculate_width(metadatos);
			W_circ_buff_aux = 0;
			for (int y = 0; y < circ_buffer_size; y++)
			{
				W_circ_buff_aux += w_circ_buffer[y];
			}
			W = W_circ_buff_aux / (circ_buffer_size);
		}
		else
		{ // siesta
			mb_core_shm[DIGITAL_OUT_3] = 1;
			W = W;
			if ((cont_calibration <= 0) && (control_unit_shm[NIT_SHUTTER] != 0))
			{
				control_unit_shm[NIT_SHUTTER] = 0; // open shutter
			}
			else
			{
				if (cont_calibration == (WAIT_SECOND_APERTURE))
				{
					//	control_unit_shm[NIT_SHUTTER] = 0;
				}
				if (cont_calibration == (1 + WAIT_STOP_CALIBRATION + WAIT_SECOND_APERTURE))
				{
					control_unit_shm[NIT_OFFSET_UPDATE] = 1;
					// control_unit_shm[NIT_SHUTTER_RESET]=1;
				}
				// cont_calibration--;
			}
			cont_calibration--;
			measurement_NAP--;
			control_NAP--;
		}

		if (proc_var_shm[SET_REF_WIDTH] > 0)
		{
			width_ref = ((double)proc_var_shm[WIDTH_REF]) / 100;
			proc_var_shm[SET_REF_WIDTH] = 0;
		}

		switch (estadoAutomata)
		{
		case MANUAL:
			potencia_t0 = proc_var_shm[POWER_MAN];
			if (mode != 2)
			{
				estadoAutomata = IDLE;
				printf("\nCambio a estado IDLE\n");
				mb_core_shm[CHANGE_OP_MODE] = 1;
			}
			track_cnt = 0;
			track = 0;
			delay_laser_on = 0;
			break;

		case IDLE:
			mb_core_shm[CHANGE_OP_MODE] = 1;
			width_aux = 0;
			contadorFramesStop = 0;
			if (laser_status)
			{
				if (proc_var_shm[PREHEATING_ENA])
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
					// pthread_mutex_unlock(&lock);
				}
				track_length = ((unsigned int)mb_core_shm[TIME_TRACK_HIGH]);
				track_length = (((track_length << 32) | ((unsigned int)mb_core_shm[TIME_TRACK_LOW])) * 1000) / CLK_100MHZ;
			}
			else
			{
				potencia_t0 = proc_var_shm[POWER_MAN];
				potencia_t1 = potencia_t0;
			}
			track_cnt = 0;
			track = 0;
			delay_laser_on = 0;
			break;

		case PREHEATING:
			potencia_t0 = proc_var_shm[PREHEATING_POWER];
			if (cont_preheating++ >= proc_var_shm[PREHEATING_TIME])
			{
				estadoAutomata = MIDIENDO;
				contadorFramesMidiendo = 0;
				// pthread_mutex_unlock(&lock);
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
						delay_laser_on = proc_var_shm[DELAY_LASER_ON];
					}
					else
					{
						if (!measurement_NAP)
						{
							track++;
							delay_laser_on = proc_var_shm[DELAY_LASER_ON];
						}
					}
				}
			}
			// if ((laser_status) & (metadatos[7] >= proc_var_shm[TRACK_REF_START])) //sustituido por cuenta de tracks en el ARM
			if ((laser_status) & (track >= proc_var_shm[TRACK_REF_START]))
			{
				width_aux = width_aux + W;
				contadorFramesMidiendo++;
			}
			potencia_t0 = proc_var_shm[POWER_MAN];
			potencia_t1 = potencia_t0;
			// if (metadatos[7] >= mb_core_shm[REFERENCE_TRACK]){
			if (track >= mb_core_shm[REFERENCE_TRACK])
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
					width_ref = ((double)proc_var_shm[WIDTH_REF]) / 100;
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
			endP = proc_var_shm[END_OF_PROCESS];
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
						delay_laser_on = proc_var_shm[DELAY_LASER_ON];
					}
					else
					{
						if (!measurement_NAP)
						{
							track++;
							delay_laser_on = proc_var_shm[DELAY_LASER_ON];
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
					potencia_t0 = proc_var_shm[POWER_MAN] + (kp * error_t0) + (ki * integral) + (kd * derivative);
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
		mb_core_shm[PWM] = (unsigned int)duty;
		last_laser_status = laser_status;

		memcpy((void *)&virtual_metadata_shm[0], &metadatos, 28);	  // Power, MOM00, MOM01, MOM10, MOM11, MOM02, MOM20
		virtual_metadata_shm[7] = resultado;						  // Width
		memcpy((void *)&virtual_metadata_shm[8], &metadatos[7], 20); // Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status

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

		// LEDS y autoshutter
		cnt_last_autoshutter++;
		current_temperature = ((float)virtual_metadata_shm[13]) / 10;
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
				proc_var_shm[AUTO_SHUTTER] = 1;
				pending_autoshutter = 0;
			}
			break;
		case IDLE:
			if (auto_auto_shutter_enable && pending_autoshutter)
			{
				proc_var_shm[AUTO_SHUTTER] = 1;
				pending_autoshutter = 0;
			}
			break;
		case MIDIENDO:
			if (auto_auto_shutter_enable && auto_auto_shutter_enable_inprocess && pending_autoshutter)
			{
				proc_var_shm[AUTO_SHUTTER] = 1;
				pending_autoshutter = 0;
			}

			break;

		case CONTROL:
			if (auto_auto_shutter_enable && auto_auto_shutter_enable_inprocess && pending_autoshutter)
			{
				proc_var_shm[AUTO_SHUTTER] = 1;
				pending_autoshutter = 0;
			}
			break;

		default:
			break;
		}

		if (alarm == 1)
		{
			// ROJO
			arm_core_shm[LED_R] = 0;
			arm_core_shm[LED_G] = 1;
			arm_core_shm[LED_B] = 1;
		}
		else
		{

			switch (estadoAutomata)
			{
			case MANUAL: // AMARILLO
				arm_core_shm[LED_R] = 0;
				arm_core_shm[LED_G] = 0;
				arm_core_shm[LED_B] = 1;

				break;
			case IDLE: // VERDE
				arm_core_shm[LED_R] = 1;
				arm_core_shm[LED_G] = 0;
				arm_core_shm[LED_B] = 1;

				break;

			case MIDIENDO: // MORADO
				arm_core_shm[LED_R] = 0;
				arm_core_shm[LED_G] = 1;
				arm_core_shm[LED_B] = 0;

				break;

			case CONTROL: // AZUL
				arm_core_shm[LED_R] = 1;
				arm_core_shm[LED_G] = 1;
				arm_core_shm[LED_B] = 0;

				break;
			default:
				break;
			}
		}
		mb_core_shm[DIGITAL_OUT_1] = 1; // naranja
		if (alarm_enable == 1)
		{ // Solo si la alarma est� habilitada

			if (alarm == 1)
			{
				mb_core_shm[DIGITAL_OUT_0] = 0; // asignacion de la digital out 1 a la alarma
				mb_core_shm[DIGITAL_OUT_2] = 1; // verde
			}
			else
			{
				mb_core_shm[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
				mb_core_shm[DIGITAL_OUT_2] = 0; // verde
			}
		}
		else
		{
			mb_core_shm[DIGITAL_OUT_0] = 1; // asignacion de la digital out 1 a la alarma
			mb_core_shm[DIGITAL_OUT_2] = 0; // verde
		}
		sem_getvalue(semaforo, &valor1);
		if (valor1 < 1)
		{ // permite al productor enviar hasta 2 imagenes por TCP al empezar la conexion
			sem_post(semaforo);
		}
		sem_getvalue(semaforo, &valor2);
	}

	munmap(((int *)real_metadata_shm), 64);
	real_metadata_shm = NULL;
	sem_close(semaforo);
	sem_unlink(SEM_NAME);
	exit(1);
}