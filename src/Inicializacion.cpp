/*
 * Inicializacion.c
 *
 *  Created on: 7 ago. 2018
 *      Author: sfuentel
 */

#include "Inicializacion.h"

sys_data ReadSysFile(sys_data initialization_data)
{
	char *auxstr;
	auxstr = (char *)malloc(30);
	initialization_data = InitializationDefaultValues(initialization_data);
	FILE *fdCONFSYS = fopen("/mnt/mmc/sys/CONFIG.sys", "r");
	
	if (fdCONFSYS == NULL)
	{
		printf("\nUnable to read CONFIG.sys file. Loading default values.\n");
	}
	else
	{
		while (fscanf(fdCONFSYS, "%s", auxstr) == 1)
		{
			initialization_data = GetValuesOfString(fdCONFSYS, initialization_data, auxstr);
		}
		if (fclose(fdCONFSYS) == 0)
		{
			printf("\nInit file closed successfully\n");
		}
	}
	// close(fdCONFSYS);

	return initialization_data;
}

// No he encontrado otra
sys_data GetValuesOfString(FILE *fdCONFSYS, sys_data s_dat, char *auxString)
{

	int auxInt;
	float auxDbl;

	if (strcmp("ALARM_ENABLE", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				s_dat.conf_ALARM_ENABLE = 1;
			else
				s_dat.conf_ALARM_ENABLE = 0;
		}
	}
	else if (strcmp("ALARM_MAX", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_ALARM_MAX)
			{
				s_dat.conf_ALARM_MAX = (uint16_t)((float)LMIN_ALARM_MAX * 100);
			}
			else if (auxDbl > LMAX_ALARM_MAX)
			{
				s_dat.conf_ALARM_MAX = (uint16_t)((float)LMAX_ALARM_MAX * 100);
			}
			else
			{
				s_dat.conf_ALARM_MAX = (uint16_t)(auxDbl * 100);
			}
		}
	}
	else if (strcmp("ALARM_MIN", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_ALARM_MIN)
			{
				s_dat.conf_ALARM_MIN = (uint16_t)((float)LMIN_ALARM_MIN * 100);
			}
			else if (auxDbl > LMAX_ALARM_MIN)
			{
				s_dat.conf_ALARM_MIN = (uint16_t)((float)LMAX_ALARM_MIN * 100);
			}
			else
			{
				s_dat.conf_ALARM_MIN = (uint16_t)(auxDbl * 100);
			}
		}
	}
	else if (strcmp("ALARM_TIME", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_ALARM_TIME)
			{
				s_dat.conf_ALARM_TIME = LMIN_ALARM_TIME;
			}
			else if (auxInt > LMAX_ALARM_TIME)
			{
				s_dat.conf_ALARM_TIME = LMAX_ALARM_TIME;
			}
			else
			{
				s_dat.conf_ALARM_TIME = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("AUTOMEASURE", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				s_dat.conf_AUTOMEASURE = 1;
			else
				s_dat.conf_AUTOMEASURE = 0;
		}
	}
	else if (strcmp("BIAS_VOLTAGE", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_BIAS_VOLTAGE)
			{
				s_dat.conf_BIAS_VOLTAGE = (uint16_t)((float)LMIN_BIAS_VOLTAGE / 0.00018311);
			}
			else if (auxDbl > LMAX_BIAS_VOLTAGE)
			{
				s_dat.conf_BIAS_VOLTAGE = (uint16_t)((float)LMAX_BIAS_VOLTAGE / 0.00018311);
			}
			else
			{
				s_dat.conf_BIAS_VOLTAGE = (uint16_t)(auxDbl / 0.00018311);
			}
		}
	}
	else if (strcmp("BLACK_LEVEL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_BLACK_LEVEL)
			{
				s_dat.conf_BLACK_LEVEL = LMIN_BLACK_LEVEL;
			}
			else if (auxInt > LMAX_BLACK_LEVEL)
			{
				s_dat.conf_BLACK_LEVEL = LMAX_BLACK_LEVEL;
			}
			else
			{
				s_dat.conf_BLACK_LEVEL = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("CIRCULAR_BUFFER_SIZE", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_CIRCULAR_BUFFER_SIZE)
			{
				s_dat.conf_CIRCULAR_BUFFER_SIZE = LMIN_CIRCULAR_BUFFER_SIZE;
			}
			else if (auxInt > LMAX_CIRCULAR_BUFFER_SIZE)
			{
				s_dat.conf_CIRCULAR_BUFFER_SIZE = LMAX_CIRCULAR_BUFFER_SIZE;
			}
			else
			{
				s_dat.conf_CIRCULAR_BUFFER_SIZE = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("CONF_AUTOSHUTTER", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			s_dat.conf_CONF_AUTOSHUTTER = (uint16_t)auxInt;
		}
	}
	else if (strcmp("CONF_DIGITALIO", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_CONF_DIGITALIO)
			{
				s_dat.conf_CONF_DIGITALIO = LMIN_CONF_DIGITALIO;
			}
			else if (auxInt > LMAX_CONF_DIGITALIO)
			{
				s_dat.conf_CONF_DIGITALIO = LMAX_CONF_DIGITALIO;
			}
			else
			{
				s_dat.conf_CONF_DIGITALIO = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("DRIFT_TEMP_AUTOSHUTTER", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_DRIFT_TEMP_AUTOSHUTTER)
			{
				s_dat.conf_DRIFT_TEMP_AUTOSHUTTER = (uint16_t)((float)LMIN_DRIFT_TEMP_AUTOSHUTTER * 10);
			}
			else if (auxDbl > LMAX_DRIFT_TEMP_AUTOSHUTTER)
			{
				s_dat.conf_DRIFT_TEMP_AUTOSHUTTER = (uint16_t)((float)LMAX_DRIFT_TEMP_AUTOSHUTTER * 10);
			}
			else
			{
				s_dat.conf_DRIFT_TEMP_AUTOSHUTTER = (uint16_t)(auxDbl * 10);
			}
		}
	}
	else if (strcmp("END_OF_PROCESS", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_END_OF_PROCESS)
			{
				s_dat.conf_END_OF_PROCESS = LMIN_END_OF_PROCESS;
			}
			else if (auxInt > LMAX_END_OF_PROCESS)
			{
				s_dat.conf_END_OF_PROCESS = LMAX_END_OF_PROCESS;
			}
			else
			{
				s_dat.conf_END_OF_PROCESS = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("INTEGRATION_TIME", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_INTEGRATION_TIME)
			{
				s_dat.conf_INTEGRATION_TIME = LMIN_INTEGRATION_TIME;
			}
			else if (auxInt > LMAX_INTEGRATION_TIME)
			{
				s_dat.conf_INTEGRATION_TIME = LMAX_INTEGRATION_TIME;
			}
			else
			{
				s_dat.conf_INTEGRATION_TIME = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("KD", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_KD)
			{
				s_dat.conf_KD = LMIN_KD;
			}
			else if (auxInt > LMAX_KD)
			{
				s_dat.conf_KD = LMAX_KD;
			}
			else
			{
				s_dat.conf_KD = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("KI", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_KI)
			{
				s_dat.conf_KI = LMIN_KI;
			}
			else if (auxInt > LMAX_KI)
			{
				s_dat.conf_KI = LMAX_KI;
			}
			else
			{
				s_dat.conf_KI = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("KP", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_KP)
			{
				s_dat.conf_KP = LMIN_KP;
			}
			else if (auxInt > LMAX_KP)
			{
				s_dat.conf_KP = LMAX_KP;
			}
			else
			{
				s_dat.conf_KP = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("LIMIT_INTEGRAL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_LIMIT_INTEGRAL)
			{
				s_dat.conf_LIMIT_INTEGRAL = LMIN_LIMIT_INTEGRAL;
			}
			else if (auxInt > LMAX_LIMIT_INTEGRAL)
			{
				s_dat.conf_LIMIT_INTEGRAL = LMAX_LIMIT_INTEGRAL;
			}
			else
			{
				s_dat.conf_LIMIT_INTEGRAL = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("LIMIT_SLEW", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_LIMIT_SLEW)
			{
				s_dat.conf_LIMIT_SLEW = (uint16_t)((float)LMIN_LIMIT_SLEW * 100);
			}
			else if (auxDbl > LMAX_LIMIT_SLEW)
			{
				s_dat.conf_LIMIT_SLEW = (uint16_t)((float)LMAX_LIMIT_SLEW * 100);
			}
			else
			{
				s_dat.conf_LIMIT_SLEW = (uint16_t)auxDbl * 100;
			}
		}
	}
	else if (strcmp("MAX_POWER", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MAX_POWER)
			{
				s_dat.conf_MAX_POWER = LMIN_MAX_POWER;
			}
			else if (auxInt > LMAX_MAX_POWER)
			{
				s_dat.conf_MAX_POWER = LMAX_MAX_POWER;
			}
			else
			{
				s_dat.conf_MAX_POWER = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("MAX_POWER_LIMIT", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MAX_POWER_LIMIT)
			{
				s_dat.conf_MAX_POWER_LIMIT = LMIN_MAX_POWER_LIMIT;
			}
			else if (auxInt > LMAX_MAX_POWER_LIMIT)
			{
				s_dat.conf_MAX_POWER_LIMIT = LMAX_MAX_POWER_LIMIT;
			}
			else
			{
				s_dat.conf_MAX_POWER_LIMIT = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("MIN_POWER", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MIN_POWER)
			{
				s_dat.conf_MIN_POWER = LMIN_MIN_POWER;
			}
			else if (auxInt > LMAX_MIN_POWER)
			{
				s_dat.conf_MIN_POWER = LMAX_MIN_POWER;
			}
			else
			{
				s_dat.conf_MIN_POWER = (int16_t)auxInt;
			}
		}
	}
	else if (strcmp("MIN_POWER_LIMIT", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MIN_POWER_LIMIT)
			{
				s_dat.conf_MIN_POWER_LIMIT = LMIN_MIN_POWER_LIMIT;
			}
			else if (auxInt > LMAX_MIN_POWER_LIMIT)
			{
				s_dat.conf_MIN_POWER_LIMIT = LMAX_MIN_POWER_LIMIT;
			}
			else
			{
				s_dat.conf_MIN_POWER_LIMIT = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("MODE", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_MODE)
			{
				s_dat.conf_MODE = LMIN_MODE;
			}
			else if (auxInt > LMAX_MODE)
			{
				s_dat.conf_MODE = LMAX_MODE;
			}
			else
			{
				s_dat.conf_MODE = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("PIXEL_MM_RATIO", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_PIXEL_MM_RATIO)
			{
				s_dat.conf_PIXEL_MM_RATIO = (uint16_t)((float)LMIN_PIXEL_MM_RATIO * 1000);
			}
			else if (auxDbl > LMAX_PIXEL_MM_RATIO)
			{
				s_dat.conf_PIXEL_MM_RATIO = (uint16_t)((float)LMAX_PIXEL_MM_RATIO * 1000);
			}
			else
			{
				s_dat.conf_PIXEL_MM_RATIO = (uint16_t)(auxDbl * 1000);
			}
		}
	}
	else if (strcmp("POWER", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_POWER)
			{
				s_dat.conf_POWER = LMIN_POWER;
			}
			else if (auxInt > LMAX_POWER)
			{
				s_dat.conf_POWER = LMAX_POWER;
			}
			else
			{
				s_dat.conf_POWER = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("ROI_ENABLE", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				s_dat.conf_ROI_ENABLE = 1;
			else
				s_dat.conf_ROI_ENABLE = 0;
		}
	}
	else if (strcmp("START_TRACK_MOM", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_START_TRACK_MOM)
			{
				s_dat.conf_START_TRACK_MOM = LMIN_START_TRACK_MOM;
			}
			else if (auxInt > LMAX_START_TRACK_MOM)
			{
				s_dat.conf_START_TRACK_MOM = LMAX_START_TRACK_MOM;
			}
			else
			{
				s_dat.conf_START_TRACK_MOM = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("STOP_TRACK_MOM", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_STOP_TRACK_MOM)
			{
				s_dat.conf_STOP_TRACK_MOM = LMIN_STOP_TRACK_MOM;
			}
			else if (auxInt > LMAX_STOP_TRACK_MOM)
			{
				s_dat.conf_STOP_TRACK_MOM = LMAX_STOP_TRACK_MOM;
			}
			else
			{
				s_dat.conf_STOP_TRACK_MOM = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("THRESHOLD", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_THRESHOLD)
			{
				s_dat.conf_THRESHOLD = LMIN_THRESHOLD;
			}
			else if (auxInt > LMAX_THRESHOLD)
			{
				s_dat.conf_THRESHOLD = LMAX_THRESHOLD;
			}
			else
			{
				s_dat.conf_THRESHOLD = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("TIMER_AUTOSHUTTER", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_TIMER_AUTOSHUTTER)
			{
				s_dat.conf_TIMER_AUTOSHUTTER = (uint16_t)(LMIN_TIMER_AUTOSHUTTER / 10);
			}
			else if (auxInt > LMAX_TIMER_AUTOSHUTTER)
			{
				s_dat.conf_TIMER_AUTOSHUTTER = (uint16_t)(LMAX_TIMER_AUTOSHUTTER / 10);
			}
			else
			{
				s_dat.conf_TIMER_AUTOSHUTTER = (uint16_t)(auxInt / 10);
			}
		}
	}
	else if (strcmp("TRACK_LENGTH", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_TRACK_LENGTH)
			{
				s_dat.conf_TRACK_LENGTH = (uint16_t)(LMIN_TRACK_LENGTH * 10);
			}
			else if (auxDbl > LMAX_TRACK_LENGTH)
			{
				s_dat.conf_TRACK_LENGTH = (uint16_t)(LMAX_TRACK_LENGTH * 10);
			}
			else
			{
				s_dat.conf_TRACK_LENGTH = (uint16_t)(auxDbl * 10);
			}
		}
	}
	else if (strcmp("TRACK_REFERENCE", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_TRACK_REFERENCE)
			{
				s_dat.conf_TRACK_REFERENCE = LMIN_TRACK_REFERENCE;
			}
			else if (auxInt > LMAX_TRACK_REFERENCE)
			{
				s_dat.conf_TRACK_REFERENCE = LMAX_TRACK_REFERENCE;
			}
			else
			{
				s_dat.conf_TRACK_REFERENCE = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("TRACK_REF_START", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_TRACK_REF_START)
			{
				s_dat.conf_TRACK_REF_START = LMIN_TRACK_REF_START;
			}
			else if (auxInt > LMAX_TRACK_REF_START)
			{
				s_dat.conf_TRACK_REF_START = LMAX_TRACK_REF_START;
			}
			else
			{
				s_dat.conf_TRACK_REF_START = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("WIDTH_MANUAL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%f", &auxDbl) == 1)
		{
			if (auxDbl < LMIN_WIDTH_MANUAL)
			{
				s_dat.conf_WIDTH_MANUAL = (uint16_t)(LMIN_WIDTH_MANUAL * 100);
			}
			else if (auxDbl > LMAX_WIDTH_MANUAL)
			{
				s_dat.conf_WIDTH_MANUAL = (uint16_t)(LMAX_WIDTH_MANUAL * 100);
			}
			else
			{
				s_dat.conf_WIDTH_MANUAL = (uint16_t)(auxDbl * 100);
			}
		}
	}
	else if (strcmp("X1_PIXEL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_X1_PIXEL)
			{
				s_dat.conf_X1_PIXEL = LMIN_X1_PIXEL;
			}
			else if (auxInt > LMAX_X1_PIXEL)
			{
				s_dat.conf_X1_PIXEL = LMAX_X1_PIXEL;
			}
			else
			{
				s_dat.conf_X1_PIXEL = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("X2_PIXEL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_X2_PIXEL)
			{
				s_dat.conf_X2_PIXEL = LMIN_X2_PIXEL;
			}
			else if (auxInt > LMAX_X2_PIXEL)
			{
				s_dat.conf_X2_PIXEL = LMAX_X2_PIXEL;
			}
			else
			{
				s_dat.conf_X2_PIXEL = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("Y1_PIXEL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_Y1_PIXEL)
			{
				s_dat.conf_Y1_PIXEL = LMIN_Y1_PIXEL;
			}
			else if (auxInt > LMAX_Y1_PIXEL)
			{
				s_dat.conf_Y1_PIXEL = LMAX_Y1_PIXEL;
			}
			else
			{
				s_dat.conf_Y1_PIXEL = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("Y2_PIXEL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_Y2_PIXEL)
			{
				s_dat.conf_Y2_PIXEL = LMIN_Y2_PIXEL;
			}
			else if (auxInt > LMAX_Y2_PIXEL)
			{
				s_dat.conf_Y2_PIXEL = LMAX_Y2_PIXEL;
			}
			else
			{
				s_dat.conf_Y2_PIXEL = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("LASER_EXTERNAL_CONTROL", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				s_dat.conf_LASER_EXTERNAL_CONTROL = 1;
			else
				s_dat.conf_LASER_EXTERNAL_CONTROL = 0;
		}
	}
	else if (strcmp("DELAY_LASER_ON", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_DELAY_LASER_ON)
			{
				s_dat.conf_DELAY_LASER_ON = LMIN_DELAY_LASER_ON;
			}
			else if (auxInt > LMAX_DELAY_LASER_ON)
			{
				s_dat.conf_DELAY_LASER_ON = LMAX_DELAY_LASER_ON;
			}
			else
			{
				s_dat.conf_DELAY_LASER_ON = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("PREHEATING_ENA", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt > 0)
				s_dat.conf_PREHEATING_ENA = 1;
			else
				s_dat.conf_PREHEATING_ENA = 0;
		}
	}
	else if (strcmp("PREHEATING_TIME", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_PREHEATING_TIME)
			{
				s_dat.conf_PREHEATING_TIME = LMIN_PREHEATING_TIME;
			}
			else if (auxInt > LMAX_PREHEATING_TIME)
			{
				s_dat.conf_PREHEATING_TIME = LMAX_PREHEATING_TIME;
			}
			else
			{
				s_dat.conf_PREHEATING_TIME = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("PREHEATING_POWER", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_PREHEATING_POWER)
			{
				s_dat.conf_PREHEATING_POWER = LMIN_PREHEATING_POWER;
			}
			else if (auxInt > LMAX_PREHEATING_POWER)
			{
				s_dat.conf_PREHEATING_POWER = LMAX_PREHEATING_POWER;
			}
			else
			{
				s_dat.conf_PREHEATING_POWER = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("ROI_ROUND", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_ROI_ROUND)
			{
				s_dat.conf_ROI_ROUND = LMIN_ROI_ROUND;
			}
			else if (auxInt > LMAX_ROI_ROUND)
			{
				s_dat.conf_ROI_ROUND = LMAX_ROI_ROUND;
			}
			else
			{
				s_dat.conf_ROI_ROUND = (uint16_t)auxInt;
			}
		}
	}
	else if (strcmp("ENA_DRIFT", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
			{
				if (auxInt > 0)
					s_dat.conf_ENA_DRIFT = 1;
				else
					s_dat.conf_ENA_DRIFT = 0;
			}
		}
	}
	else if (strcmp("DRIFT_INTENSITY", auxString) == 0)
	{
		if (fscanf(fdCONFSYS, "%d", &auxInt) == 1)
		{
			if (auxInt < LMIN_DRIFT_INTENSITY)
			{
				s_dat.conf_DRIFT_INTENSITY = LMIN_DRIFT_INTENSITY;
			}
			else if (auxInt > LMAX_DRIFT_INTENSITY)
			{
				s_dat.conf_DRIFT_INTENSITY = LMAX_DRIFT_INTENSITY;
			}
			else
			{
				s_dat.conf_DRIFT_INTENSITY = (uint16_t)auxInt;
			}
		}
	}
	else
	{
		printf("Bad line in the CONFIG.sys file\n");
	}

	return s_dat;
}

sys_data InitializationDefaultValues(sys_data s_dat)
{

	s_dat.conf_ALARM_ENABLE = 0;
	s_dat.conf_ALARM_MAX = 500; // 5,0
	s_dat.conf_ALARM_MIN = 100; // 1,0
	s_dat.conf_ALARM_TIME = 2000;
	s_dat.conf_AUTOMEASURE = 1;
	s_dat.conf_BIAS_VOLTAGE = (uint16_t)((float)2 / 0.00018311);
	s_dat.conf_BLACK_LEVEL = 1000;
	s_dat.conf_CIRCULAR_BUFFER_SIZE = 8;
	s_dat.conf_CONF_AUTOSHUTTER = 8;
	s_dat.conf_CONF_DIGITALIO = 0;
	s_dat.conf_DRIFT_TEMP_AUTOSHUTTER = 30; // equivalente a 3,0 en el fichero
	s_dat.conf_END_OF_PROCESS = 5000;
	s_dat.conf_INTEGRATION_TIME = 200;
	s_dat.conf_KD = 100;
	s_dat.conf_KI = 500;
	s_dat.conf_KP = 200;
	s_dat.conf_LIMIT_INTEGRAL = 5000;
	s_dat.conf_LIMIT_SLEW = 100; // equivale a 1,00
	s_dat.conf_MAX_POWER = 5000;
	s_dat.conf_MAX_POWER_LIMIT = 1500;
	s_dat.conf_MIN_POWER = 0;
	s_dat.conf_MIN_POWER_LIMIT = 500;
	s_dat.conf_MODE = 2;
	s_dat.conf_PIXEL_MM_RATIO = 150; // Equivalente a 0,15 en el fichero
	s_dat.conf_POWER = 1000;
	s_dat.conf_ROI_ENABLE = 0;
	s_dat.conf_START_TRACK_MOM = 40;
	s_dat.conf_STOP_TRACK_MOM = 30;
	s_dat.conf_THRESHOLD = 1200;
	s_dat.conf_TIMER_AUTOSHUTTER = 18; // equivalente a 180 en fichero, se evia en decenas de segundos como unidad desde el PC
	s_dat.conf_TRACK_LENGTH = 20;	   // equivalente a 2,0 en el fichero
	s_dat.conf_TRACK_REFERENCE = 3;
	s_dat.conf_TRACK_REF_START = 0;
	s_dat.conf_WIDTH_MANUAL = 100; // equivalente a 1,0 en el fichero
	s_dat.conf_ROI_ROUND = 0;
	s_dat.conf_X1_PIXEL = 2;
	s_dat.conf_X2_PIXEL = 61;
	s_dat.conf_Y1_PIXEL = 2;
	s_dat.conf_Y2_PIXEL = 61;
	s_dat.conf_LASER_EXTERNAL_CONTROL = 0;
	s_dat.conf_DELAY_LASER_ON = 0;
	s_dat.conf_PREHEATING_ENA = 0;
	s_dat.conf_PREHEATING_TIME = 0;
	s_dat.conf_PREHEATING_POWER = 0;
	s_dat.conf_ENA_DRIFT = 1;
	s_dat.conf_DRIFT_INTENSITY = 14;
	return s_dat;
}
