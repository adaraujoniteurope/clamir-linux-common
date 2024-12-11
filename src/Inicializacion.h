/*
 * Inicializacion.h
 *
 *  Created on: 7 ago. 2018
 *      Author: sfuentel
 */

#ifndef SRC_INICIALIZACION_H_
#define SRC_INICIALIZACION_H_

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

#endif /* SRC_INICIALIZACION_H_ */

// Definicion reloj
#define CLK_100MHZ 100000000

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

// Structure with all the values to initialize
typedef struct
{
    uint16_t conf_KI;
    uint16_t conf_KP;
    uint16_t conf_KD;
    uint16_t conf_MAX_POWER;
    int16_t conf_MIN_POWER;
    uint16_t conf_START_TRACK_MOM;
    uint16_t conf_POWER;
    uint16_t conf_STOP_TRACK_MOM;
    uint16_t conf_MODE;
    uint16_t conf_TRACK_REFERENCE;
    uint16_t conf_TRACK_LENGTH;
    uint16_t conf_WIDTH_MANUAL;
    uint16_t conf_THRESHOLD;
    uint16_t conf_ROI_ENABLE;
    uint16_t conf_X2_PIXEL;
    uint16_t conf_X1_PIXEL;
    uint16_t conf_Y2_PIXEL;
    uint16_t conf_Y1_PIXEL;
    uint16_t conf_MAX_POWER_LIMIT;
    uint16_t conf_MIN_POWER_LIMIT;
    uint16_t conf_END_OF_PROCESS;
    uint16_t conf_PIXEL_MM_RATIO;
    uint16_t conf_LIMIT_SLEW;
    uint16_t conf_BLACK_LEVEL;
    uint16_t conf_ALARM_ENABLE;
    uint16_t conf_ALARM_MAX;
    uint16_t conf_ALARM_MIN;
    uint16_t conf_ALARM_TIME;
    uint16_t conf_AUTOMEASURE;
    uint16_t conf_LIMIT_INTEGRAL;
    uint16_t conf_CIRCULAR_BUFFER_SIZE;
    uint16_t conf_INTEGRATION_TIME;
    uint16_t conf_CONF_DIGITALIO;
    uint16_t conf_TRACK_REF_START;
    uint16_t conf_CONF_AUTOSHUTTER;
    uint16_t conf_DRIFT_TEMP_AUTOSHUTTER;
    uint16_t conf_TIMER_AUTOSHUTTER;
    uint16_t conf_BIAS_VOLTAGE;
    uint16_t conf_LASER_EXTERNAL_CONTROL;
    uint16_t conf_DELAY_LASER_ON;
    uint16_t conf_PREHEATING_ENA;
    uint16_t conf_PREHEATING_TIME;
    uint16_t conf_PREHEATING_POWER;
    uint16_t conf_ROI_ROUND;
    uint16_t conf_ENA_DRIFT;
    uint16_t conf_DRIFT_INTENSITY;
} sys_data;

sys_data ReadSysFile(sys_data initialization_data);
sys_data InitializationDefaultValues(sys_data s_dat);
sys_data GetValuesOfString(FILE *fdCONFSYS, sys_data s_dat, char *auxString);
