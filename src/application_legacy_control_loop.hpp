#ifndef _application_legacy_control_loop_hpp_
#define _application_legacy_control_loop_hpp_

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

#endif