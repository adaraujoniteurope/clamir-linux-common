/*
 * commandaddr.h
 *
 *  Created on: 27 dec. 2024
 *      Author: arthur
 */

#ifndef SRC_COMMADDR_H_
#define SRC_COMMADDR_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#define NIT_CONTROL_UNIT_BASE_ADDRESS 0x40000000
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

#define BRAM_NIT_SIZE 32
#define NIT_IMAGE_BRAM_BASE_ADDRESS 0x42000000
#define NIT_IMAGE_BRAM_SIZE 8192
#define NIT_IMAGE_METADATA_BRAM_BASE_ADDRESS 0x42002000

#define NIT_BPCC_TABLE_BASE_ADDRESS 0x44000000
#define NIT_MB_CORE_BASE_ADDRESS 0x50000000
#define NIT_ARM_CORE_BASE_ADDRESS 0x44A40000

/*
 * Bank 0 "GEN" offsets 0x0000
 */
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

/*
 * Bank 1 "MOM" offsets 0x4000
 */
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

	/*
	 *  Bank 2 "ROI" offsets 0x8000
	 */

#define ROI_X1 0x8000 + 0
#define ROI_Y1 0x8000 + 1
#define ROI_X2 0x8000 + 2
#define ROI_Y2 0x8000 + 3
#define ROI_ROUND 0x8000 + 4

	/*
	 *  Bank 3 " PWM" offsets 0xC000
	 */

#define PWM_LIMIT_MAX 0xC000 + 0
#define PWM 0xC000 + 1
#define POWER 0xC000 + 2
#define PWM_LIMIT_MIN 0xC000 + 3

	/*
	 *  Bank 0 AIMEN_STARTUP "RST" offsets 0x0000
	 *  Resets y modos de debug en memoria shmemReset
	 */

#define IP_OP_MODE 0x0000 + 0
#define IRQ_ARM 0x0000 + 1
#define IRQ_DEBUG 0x0000 + 2
#define FRAME_NBR_RST 0x0000 + 3
#define TIMESTAMP_RST 0x0000 + 4
#define SOFT_RST 0x0000 + 5

/*
 *  Bank 1 AIMEN_STARTUP "LED" offsets 0x4000 en memoria shmemReset
 */
#define LED_R (0x4000 + 0)
#define LED_B (0x4000 + 1)
#define LED_G (0x4000 + 2)

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

// Definiciones para imagen debug
// #define MEM_IMG_DEBUG_ADDRESS 0x46000000  //Prueba NIT
#define MEM_IMG_DEBUG_ADDRESS 0x41000000
#define MEM_IMG_DEBUG_SIZE 8192

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

typedef struct
{
	unsigned int lectura;
	int baseaddress;
	int offset;
	unsigned int fpga_write;
} gestion_comandos;

gestion_comandos command_manager(uint16_t command);

#ifdef __cplusplus
}
#endif

#endif
