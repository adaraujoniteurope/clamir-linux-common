/*
 * config.cpp
 *
 *  Created on: 27 dec. 2024
 *      Author: adaraujo
 */

#include "commandaddr.h"
#include "config.h"

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
				config_data.limit_slew = (uint16_t)((float)LMIN_LIMIT_SLEW * 100);
			}
			else if (auxDbl > LMAX_LIMIT_SLEW)
			{
				config_data.limit_slew = (uint16_t)((float)LMAX_LIMIT_SLEW * 100);
			}
			else
			{
				config_data.limit_slew = (uint16_t)auxDbl * 100;
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
				config_data.pixel_mm_ratio = (uint16_t)((float)LMIN_PIXEL_MM_RATIO * 1000);
			}
			else if (auxDbl > LMAX_PIXEL_MM_RATIO)
			{
				config_data.pixel_mm_ratio = (uint16_t)((float)LMAX_PIXEL_MM_RATIO * 1000);
			}
			else
			{
				config_data.pixel_mm_ratio = (uint16_t)(auxDbl * 1000);
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

/*
 * Salva la configuracion inicial en el fichero CONFIG.sys.
 * Permite que la configuracion actual sea la de inicio del sistema embebido.
 */
int config_save(const char* dest, volatile int *shm_proc_var, volatile int *shm_ai_men_com, volatile int *shm_nit_command)
{
	int result = 0;
	uint64_t time_track_aux = 0;
	FILE *fd;
	fd = fopen(dest, "w");
	if (fd == NULL)
	{
		printf("\n*** UNABLE TO SAVE CONFIGURATION ***\n");
		return result;
	}
	else
	{
		fprintf(fd, "KI %d\n", shm_proc_var[KI]);
		fprintf(fd, "KP %d\n", shm_proc_var[KP]);
		fprintf(fd, "KD %d\n", shm_proc_var[KD]);
		fprintf(fd, "MAX_POWER %d\n", shm_proc_var[MAX_POWER]);
		fprintf(fd, "MIN_POWER %d\n", shm_proc_var[MIN_POWER]);
		fprintf(fd, "POWER %d\n", shm_proc_var[POWER_MAN]);
		fprintf(fd, "MAX_POWER_LIMIT %d\n", shm_proc_var[POWER_LIMIT_MAX]);
		fprintf(fd, "MIN_POWER_LIMIT %d\n", shm_proc_var[POWER_LIMIT_MIN]);
		fprintf(fd, "WIDTH_MANUAL %.2f\n", ((float)shm_proc_var[WIDTH_REF] / 100));
		fprintf(fd, "PIXEL_MM_RATIO %.3f\n", ((float)shm_proc_var[PIXEL_MM_RATIO] / 1000));
		fprintf(fd, "END_OF_PROCESS %d\n", shm_proc_var[END_OF_PROCESS]);
		fprintf(fd, "LIMIT_INTEGRAL %d\n", shm_proc_var[LIMIT_INTEGRAL]);
		fprintf(fd, "LIMIT_SLEW %.2f\n", ((float)shm_proc_var[LIMIT_SLEW] / 100));
		fprintf(fd, "CIRCULAR_BUFFER_SIZE %d\n", shm_proc_var[BUFF_SIZE]);
		fprintf(fd, "ALARM_ENABLE %d\n", shm_proc_var[ENABLE_ALARM]);
		fprintf(fd, "ALARM_MAX %.2f\n", ((float)shm_proc_var[ALARM_MAX] / 100));
		fprintf(fd, "ALARM_MIN %.2f\n", ((float)shm_proc_var[ALARM_MIN] / 100));
		fprintf(fd, "ALARM_TIME %d\n", shm_proc_var[ALARM_TIME]);
		fprintf(fd, "AUTOMEASURE %d\n", shm_proc_var[AUTOMEASURE]);
		fprintf(fd, "CONF_AUTOSHUTTER %d\n", shm_proc_var[AUTOSHUTTER_CONFIG]);
		fprintf(fd, "DRIFT_TEMP_AUTOSHUTTER %.1f\n", ((float)shm_proc_var[AUTOSHUTTER_TEMP] / 10));
		fprintf(fd, "TIMER_AUTOSHUTTER %d\n", (shm_proc_var[AUTOSHUTTER_TIMER] * 10));
		fprintf(fd, "TRACK_REF_START %d\n", shm_proc_var[TRACK_REF_START]);
		fprintf(fd, "LASER_EXTERNAL_CONTROL %d\n", shm_proc_var[LASER_EXTERAL_CONTROL]);
		fprintf(fd, "DELAY_LASER_ON %d\n", shm_proc_var[DELAY_LASER_ON]);
		fprintf(fd, "PREHEATING_ENA %d\n", shm_proc_var[PREHEATING_ENA]);
		fprintf(fd, "PREHEATING_TIME %d\n", shm_proc_var[PREHEATING_TIME]);
		fprintf(fd, "PREHEATING_POWER %d\n", shm_proc_var[PREHEATING_POWER]);
		fprintf(fd, "START_TRACK_MOM %d\n", shm_ai_men_com[START_TRACK_MOM_T]);
		fprintf(fd, "STOP_TRACK_MOM %d\n", shm_ai_men_com[END_OF_TRACK]);
		fprintf(fd, "MODE %d\n", shm_ai_men_com[MODE]);
		fprintf(fd, "TRACK_REFERENCE %d\n", shm_ai_men_com[REFERENCE_TRACK]);
		time_track_aux = ((unsigned int)shm_ai_men_com[TIME_TRACK_HIGH]);
		time_track_aux = ((time_track_aux << 32) | ((unsigned int)shm_ai_men_com[TIME_TRACK_LOW]));
		fprintf(fd, "TRACK_LENGTH %.1f\n", (((float)time_track_aux / CLK_100MHZ)));
		fprintf(fd, "THRESHOLD %d\n", shm_ai_men_com[THRESHOLD]);
		fprintf(fd, "ROI_ROUND %d\n", shm_ai_men_com[ROI_ROUND]);
		fprintf(fd, "ROI_ENABLE %d\n", shm_ai_men_com[ENABLE_ROI]);
		fprintf(fd, "X1_PIXEL %d\n", shm_ai_men_com[ROI_X1]);
		fprintf(fd, "X2_PIXEL %d\n", shm_ai_men_com[ROI_X2]);
		fprintf(fd, "Y1_PIXEL %d\n", shm_ai_men_com[ROI_Y1]);
		fprintf(fd, "Y2_PIXEL %d\n", shm_ai_men_com[ROI_Y2]);
		fprintf(fd, "CONF_DIGITALIO %d\n", shm_ai_men_com[DIGITAL_OUT_CONF]);
		fprintf(fd, "BLACK_LEVEL %d\n", shm_nit_command[NIT_BLACK_LEVEL]);
		fprintf(fd, "BIAS_VOLTAGE %f\n", ((float)shm_nit_command[NIT_BIAS_V] * 0.00018311));
		fprintf(fd, "INTEGRATION_TIME %d\n", shm_nit_command[NIT_INT_TIME]);
		fprintf(fd, "ENA_DRIFT %d\n", shm_nit_command[NIT_DRIFT_ENABLE]);
		fprintf(fd, "DRIFT_INTENSITY %d\n", shm_nit_command[NIT_DRIFT_POSITION]);
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
	config_data.track_length = 20;	   // equivalente a 2,0 en el fichero
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
