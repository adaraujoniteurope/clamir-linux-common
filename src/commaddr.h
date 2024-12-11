/*
 * commaddr.h
 *
 *  Created on: 7 ago. 2018
 *      Author: sfuentel
 */

#ifndef SRC_COMMADDR_H_
#define SRC_COMMADDR_H_

#define BASEADDRESS_NIT_COMMAND 0x40000000
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
#define BRAM_IMG_ADDRESS 0x42000000
// #define BRAM_IMG_ADDRESS 0x43000000
#define BRAM_IMG_SIZE 8192
#define BRAM_IMG_METADATOS 0x42002000

#define BASEADDRESS_BPC_TABLE 0x44000000

#define BASEADDRESS_AIMEN_COMMAND 0x50000000
#define BASEADDRESS_AIMEN_STARTUP 0x44A40000

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
#define LED_R 0x4000 + 0
#define LED_B 0x4000 + 1
#define LED_G 0x4000 + 2

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

#endif /* SRC_COMMADDR_H_ */

typedef struct
{
	unsigned int lectura;
	int baseaddress;
	int offset;
	unsigned int fpga_write;
} gestion_comandos;

gestion_comandos CommandManager(uint16_t buffercomm)
{
	gestion_comandos gestor;
	// Lectura o escritura
	gestor.lectura = (buffercomm & 0x00F0); // 1XXX XXXX lectura, 0XXX XXXX escritura
	// Inicializacion de gestor en modo escritura en FPGA
	gestor.fpga_write = 1; // 1 escritura en FPGA, 0 no escritura en FPGA
	// Tipo de comando
	if ((buffercomm & 0xFF00) == 0x0500)
	{
		gestor.baseaddress = BASEADDRESS_NIT_COMMAND;
		switch (buffercomm)
		{
		case 0x0501:
		case 0x0581:
			gestor.offset = NIT_INT_TIME;
			break;

		case 0x0502:
		case 0x0582:
			gestor.offset = NIT_BIAS_V;
			break;
		case 0x0503:
		case 0x0583:
			gestor.offset = NIT_OFFSET_EN;
			break;

		case 0x0504:
			gestor.offset = NIT_OFFSET_UPDATE;
			break;

		case 0x0505:
		case 0x0585:
			gestor.offset = NIT_SHUTTER;
			break;

		case 0x0506:
		case 0x0586:
			gestor.offset = NIT_BPC_EN;
			break;
		case 0x0507:
		case 0x0587:
			gestor.offset = NIT_BPC_MEM_WRITE;
			break;

		case 0x0508:
		case 0x0588:
			gestor.offset = NIT_BPC_IDENTIFY;
			break;

		case 0x0589:
			gestor.offset = NIT_TEMP1;
			break;

		case 0x058A:
			gestor.offset = NIT_TEMP2;
			break;

		case 0x058B:
			gestor.offset = NIT_TEMP3;
			break;

		case 0x058C:
			gestor.offset = NIT_TEMP4;
			break;

		case 0x0520:
		case 0x05A0:
			gestor.offset = NIT_TRIGGER_USEC;
			break;

		case 0x0521:
		case 0x05A1:
			gestor.offset = NIT_BLACK_LEVEL;
			break;

		case 0x05A2:
			gestor.offset = NIT_SINCRONIZATION; // solo lectura, no se escribe en memoria
			break;

		case 0x0523:
			gestor.offset = NIT_SAVE_EMBEDDED_CONF; // solo lectura, no se escribe en memoria
			break;

		case 0x05A4:
			gestor.offset = NIT_ARM_SW_VERSION; // solo lectura, no se escribe en memoria
			break;

		case 0x050D:
		case 0x058D:
			gestor.offset = NIT_DRIFT_ENABLE;
			break;

		case 0x050E:
		case 0x058E:
			gestor.offset = NIT_DRIFT_POSITION;
			break;

		case 0x058F:
			gestor.offset = NIT_DRIFT_LEVEL;
			break;

		case 0x05A5:
		case 0x05A6:
			gestor.offset = NIT_FPGA_VERSION;
			break;

		default:
			break;
		}
	}
	else if (((buffercomm & 0xFF00) == 0x0400) || ((buffercomm & 0xFF00) == 0x0300))
	{
		gestor.baseaddress = BASEADDRESS_AIMEN_COMMAND;
		switch (buffercomm)
		{

		// No escriben en FPGA
		case 0x0401:
		case 0x0481:
			gestor.offset = KI;
			gestor.fpga_write = 0;
			break;
		case 0x0402:
		case 0x0482:
			gestor.offset = KP;
			gestor.fpga_write = 0;
			break;
		case 0x0403:
		case 0x0483:
			gestor.offset = KD;
			gestor.fpga_write = 0;
			break;
		case 0x0404:
		case 0x0484:
			gestor.offset = MAX_POWER;
			gestor.fpga_write = 0;
			break;
		case 0x0405:
		case 0x0485:
			gestor.offset = MIN_POWER;
			gestor.fpga_write = 0;
			break;
		case 0x0407:
		case 0x0487:
			gestor.offset = POWER_MAN;
			gestor.fpga_write = 0;
			break;
		case 0x0409:
			gestor.offset = AUTO_SHUTTER;
			gestor.fpga_write = 0;
			break;
		case 0x040D:
		case 0x048D:
			gestor.offset = SET_REF_WIDTH;
			gestor.fpga_write = 0;
			break;
		case 0x0418:
		case 0x0498:
			gestor.offset = POWER_LIMIT_MAX;
			gestor.fpga_write = 0;
			break;
		case 0x0419:
		case 0x0499:
			gestor.offset = POWER_LIMIT_MIN;
			gestor.fpga_write = 0;
			break;
		case 0x041F:
		case 0x049F:
			gestor.offset = WIDTH_REF;
			gestor.fpga_write = 0;
			break;
		case 0x0420:
		case 0x04A0:
			gestor.offset = PIXEL_MM_RATIO;
			gestor.fpga_write = 0;
			break;

		case 0x04A1:
			gestor.offset = PID_ERROR;
			gestor.fpga_write = 0;
			break;

		case 0x0422:
		case 0x04A2:
			gestor.offset = END_OF_PROCESS;
			gestor.fpga_write = 0;
			break;

		case 0x0423:
		case 0x04A3:
			gestor.offset = LIMIT_INTEGRAL;
			gestor.fpga_write = 0;
			break;

		case 0x0424:
		case 0x04A4:
			gestor.offset = LIMIT_SLEW;
			gestor.fpga_write = 0;
			break;

		case 0x04A5:
		case 0x0425:
			gestor.offset = BUFF_SIZE;
			gestor.fpga_write = 0;
			break;

		case 0x0426:
		case 0x04A6:
			gestor.offset = ENABLE_ALARM;
			gestor.fpga_write = 0;
			break;

		case 0x0427:
		case 0x04A7:
			gestor.offset = ALARM_MAX;
			gestor.fpga_write = 0;
			break;

		case 0x0428:
		case 0x04A8:
			gestor.offset = ALARM_MIN;
			gestor.fpga_write = 0;
			break;

		case 0x0429:
		case 0x04A9:
			gestor.offset = ALARM_TIME;
			gestor.fpga_write = 0;
			break;

		case 0x04AA:
			gestor.offset = SERIAL_NUMBER_LOW;
			gestor.fpga_write = 0;
			break;

		case 0x042E:
		case 0x04AE:
			gestor.offset = AUTOMEASURE;
			gestor.fpga_write = 0;
			break;

		case 0x042F:
		case 0x04AF:
			gestor.offset = AUTOSHUTTER_CONFIG; // flags, booleanos
			gestor.fpga_write = 0;
			break;

		case 0x0431:
		case 0x04B1:
			gestor.offset = AUTOSHUTTER_TEMP;
			gestor.fpga_write = 0;
			break;

		case 0x0432:
		case 0x04B2:
			gestor.offset = AUTOSHUTTER_TIMER;
			gestor.fpga_write = 0;
			break;

		case 0x0433:
		case 0x04B3:
			gestor.offset = TRACK_REF_START;
			gestor.fpga_write = 0;
			break;

		case 0x0434:
		case 0x04B4:
			gestor.offset = LASER_EXTERAL_CONTROL;
			gestor.fpga_write = 0;
			break;

		case 0x0435:
		case 0x04B5:
			gestor.offset = DELAY_LASER_ON;
			gestor.fpga_write = 0;
			break;

		case 0x0436:
		case 0x04B6:
			gestor.offset = PREHEATING_ENA;
			gestor.fpga_write = 0;
			break;

		case 0x0437:
		case 0x04B7:
			gestor.offset = PREHEATING_TIME;
			gestor.fpga_write = 0;
			break;

		case 0x0438:
		case 0x04B8:
			gestor.offset = PREHEATING_POWER;
			gestor.fpga_write = 0;
			break;

		// Escriben en FPGA
		case 0x0406:
		case 0x0486:
			gestor.offset = START_TRACK_MOM_T;
			break;

		case 0x0408:
		case 0x0488:
			gestor.offset = END_OF_TRACK;
			break;

		case 0x048A:
		case 0x040A:
			gestor.offset = MODE;
			break;

		case 0x040B:
		case 0x048B:
			gestor.offset = REFERENCE_TRACK;
			break;

		case 0x040C:
		case 0x048C:
			gestor.offset = TIME_TRACK_LOW;
			break;

		case 0x040E: // STARTUP
			gestor.baseaddress = BASEADDRESS_AIMEN_STARTUP;
			gestor.offset = SOFT_RST;
			break;

		case 0x040F:
		case 0x048F:
			gestor.offset = THRESHOLD;
			break;

		case 0x0410:
		case 0x0490:
			gestor.offset = ROI_ROUND;
			break;

		case 0x0411:
		case 0x0491:
			gestor.offset = ENABLE_ROI;
			break;

		case 0x0492:
		case 0x0412:
			gestor.offset = ROI_X1;
			break;

		case 0x0413:
		case 0x0493:
			gestor.offset = ROI_Y1;
			break;

		case 0x0414:
		case 0x0494:
			gestor.offset = ROI_X2;
			break;

		case 0x0415:
		case 0x0495:
			gestor.offset = ROI_Y2;
			break;

		case 0x0416: // TOOGLE DEBUG
			gestor.baseaddress = BASEADDRESS_AIMEN_STARTUP;
			gestor.offset = IP_OP_MODE;
			break;

		case 0x0417:
		case 0x0497:
			gestor.offset = PWM;
			break;

		case 0x041A:
		case 0x049A:
			gestor.baseaddress = BASEADDRESS_AIMEN_STARTUP;
			gestor.offset = LED_R;
			break;

		case 0x041B:
		case 0x049B:
			gestor.baseaddress = BASEADDRESS_AIMEN_STARTUP;
			gestor.offset = LED_G;
			break;

		case 0x041C:
		case 0x049C:
			gestor.baseaddress = BASEADDRESS_AIMEN_STARTUP;
			gestor.offset = LED_B;
			break;

		case 0x041D:
		case 0x049D:
			gestor.offset = PWM_LIMIT_MAX;
			break;
		case 0x041E:
		case 0x049E:
			gestor.offset = PWM_LIMIT_MIN;
			break;

		case 0x0430:
		case 0x04B0:
			gestor.offset = DIGITAL_OUT_CONF;
			break;

		case 0x0439:
		case 0x04B9:
			gestor.offset = DIGITAL_OUT_0;
			break;

		case 0x043A:
		case 0x04BA:
			gestor.offset = DIGITAL_OUT_1;
			break;

		case 0x043B:
		case 0x04BB:
			gestor.offset = DIGITAL_OUT_2;
			break;

		case 0x043C:
		case 0x04BC:
			gestor.offset = DIGITAL_OUT_3;
			break;

		default:
			break;
		}
	}
	else
	{
		gestor.lectura = 0;
		gestor.baseaddress = 0;
		gestor.offset = 0;
		gestor.fpga_write = 0;
		return gestor;
	}

	return gestor;
}
