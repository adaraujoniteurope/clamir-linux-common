/*
 * config.h
 *
 *  Created on: 27 dec. 2024
 *      Author: adaraujo
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
    uint16_t ki;
    uint16_t kp;
    uint16_t kd;
    uint16_t max_power;
    int16_t min_power;
    uint16_t start_track_mom;
    uint16_t power;
    uint16_t stop_track_mom;
    uint16_t mode;
    uint16_t track_reference;
    uint16_t track_length;
    uint16_t width_manual;
    uint16_t threshold;
    uint16_t roi_enable;
    uint16_t x2_pixel;
    uint16_t x1_pixel;
    uint16_t y2_pixel;
    uint16_t y1_pixel;
    uint16_t max_power_limit;
    uint16_t min_power_limit;
    uint16_t end_of_process;
    uint16_t pixel_mm_ratio;
    uint16_t limit_slew;
    uint16_t black_level;
    uint16_t alarm_enable;
    uint16_t alarm_max;
    uint16_t alarm_min;
    uint16_t alarm_time;
    uint16_t automeasure;
    uint16_t limit_integral;
    uint16_t circular_buffer_size;
    uint16_t integration_time;
    uint16_t digitalio;
    uint16_t track_ref_start;
    uint16_t autoshutter;
    uint16_t drift_temp_autoshutter;
    uint16_t timer_autoshutter;
    uint16_t bias_voltage;
    uint16_t laser_external_control;
    uint16_t delay_laser_on;
    uint16_t preheating_ena;
    uint16_t preheating_time;
    uint16_t preheating_power;
    uint16_t roi_round;
    uint16_t ena_drift;
    uint16_t drift_intensity;
} config_data_t;

config_data_t config_file_read(config_data_t initialization_data);
config_data_t config_initialize(config_data_t s_dat);
int config_save(const char* dest, volatile int *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_nit_command);
