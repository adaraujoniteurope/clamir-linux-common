/*
 * commandaddr.h
 *
 *  Created on: 27 dec. 2024
 *      Author: arthur
 */

#include "commandaddr.h"
#include <cstdint>

gestion_comandos command_manager(uint16_t command)
{
    gestion_comandos gestor;
    // Lectura o escritura
    gestor.lectura = (command & 0x00F0); // 1XXX XXXX lectura, 0XXX XXXX escritura
    // Inicializacion de gestor en modo escritura en FPGA
    gestor.fpga_write = 1; // 1 escritura en FPGA, 0 no escritura en FPGA
    // Tipo de comando
    if ((command & 0xFF00) == 0x0500)
    {
        gestor.baseaddress = NIT_CONTROL_UNIT_BASE_ADDRESS;
        switch (command)
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
    else if (((command & 0xFF00) == 0x0400) || ((command & 0xFF00) == 0x0300))
    {
        gestor.baseaddress = NIT_MB_CORE_BASE_ADDRESS;
        switch (command)
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
            gestor.baseaddress = NIT_ARM_CORE_BASE_ADDRESS;
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
            gestor.baseaddress = NIT_ARM_CORE_BASE_ADDRESS;
            gestor.offset = IP_OP_MODE;
            break;

        case 0x0417:
        case 0x0497:
            gestor.offset = PWM;
            break;

        case 0x041A:
        case 0x049A:
            gestor.baseaddress = NIT_ARM_CORE_BASE_ADDRESS;
            gestor.offset = LED_R;
            break;

        case 0x041B:
        case 0x049B:
            gestor.baseaddress = NIT_ARM_CORE_BASE_ADDRESS;
            gestor.offset = LED_G;
            break;

        case 0x041C:
        case 0x049C:
            gestor.baseaddress = NIT_ARM_CORE_BASE_ADDRESS;
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