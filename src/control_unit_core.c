#include "control_unit_core.h"
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

int control_unit_open(control_unit_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }
    
	state->fd = open("/dev/mem", O_RDWR | O_SYNC);

	if (state->fd < 0)
	{
		return -2;
	}

	state->priv = (volatile int *)mmap(NULL, NIT_CONTROL_UNIT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_CONTROL_UNIT_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

int control_unit_close(control_unit_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*)state->priv, NIT_CONTROL_UNIT_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

void control_unit_bias_v_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_BIAS_V_OFFSET) = value;
}

uint32_t control_unit_bias_v_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_BIAS_V_OFFSET);
}

void control_unit_black_level_set(control_unit_core_state_t *state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_BLACK_LEVEL_OFFSET) = value;
}

uint32_t control_unit_black_level_get(control_unit_core_state_t *state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_BLACK_LEVEL_OFFSET);
}

void control_unit_bpc_en_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_BPC_EN_OFFSET) = value;
}

uint32_t control_unit_bpc_en_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_BPC_EN_OFFSET);
}

void control_unit_drift_enable_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_DRIFT_ENABLE_OFFSET) = value;
}

uint32_t control_unit_drift_enable_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_DRIFT_ENABLE_OFFSET);
}

void control_unit_drift_position_set(control_unit_core_state_t *state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_DRIFT_POSITION_OFFSET) = value;
}

uint32_t control_unit_drift_position_get(control_unit_core_state_t *state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_DRIFT_POSITION_OFFSET);
}

void control_unit_int_time_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_INT_TIME_OFFSET) = value;
}

uint32_t control_unit_int_time_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_INT_TIME_OFFSET);
}

void control_unit_offset_update_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_OFFSET_UPDATE_OFFSET) = value;
}

uint32_t control_unit_offset_update_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_OFFSET_UPDATE_OFFSET);
}

void control_unit_shutter_reset_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_SHUTTER_RESET_OFFSET) = value;
}

uint32_t control_unit_shutter_reset_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_SHUTTER_RESET_OFFSET);
}

void control_unit_shutter_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_SHUTTER_OFFSET) = value;
}

uint32_t control_unit_shutter_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_SHUTTER_OFFSET);
}

uint32_t control_unit_temp1_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_TEMP1_OFFSET);
}

void control_unit_temp1_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_TEMP1_OFFSET) = value;
}

uint32_t control_unit_temp2_get(control_unit_core_state_t* state)
{
    return *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_TEMP2_OFFSET);
}

void control_unit_temp2_set(control_unit_core_state_t* state, uint32_t value)
{
    *((uint32_t*)(((uint32_t*)state->priv)) + NIT_CONTROL_UNIT_TEMP2_OFFSET) = value;
}

double control_unit_temp_get_voltage(uint32_t value)
{
    return 3.0f / 4096.0f * ((double) value);
}

double control_unit_temp_get_resistance(uint32_t value)
{
    return 10000.f * ((3.3f / control_unit_temp_get_voltage(value)) - 1.f);
}

double control_unit_temp_to_degc(uint32_t value)
{
    return (int)(10 * ((3380.0f / (log(control_unit_temp_get_resistance(value)) + 2.126235177f)) - 273.15f));
}