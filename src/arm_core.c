#include "arm_core.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

uint16_t arm_core_open(arm_core_state_t* state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_ARM_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_ARM_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

uint16_t arm_core_close(arm_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*)state->priv, NIT_ARM_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

void arm_core_led_r_set(arm_core_state_t* state, uint16_t value)
{
    *((uint16_t*)(((uint8_t*)state->priv)) + NIT_ARM_CORE_LED_R_OFFSET) = value;
}

uint16_t arm_core_led_r_get(arm_core_state_t* state, uint16_t value)
{
    *((uint16_t*)(((uint8_t*)state->priv)) + NIT_ARM_CORE_LED_R_OFFSET);
}

void arm_core_led_g_set(arm_core_state_t* state, uint16_t value)
{
    *((uint16_t*)(((uint8_t*)state->priv)) + NIT_ARM_CORE_LED_R_OFFSET) = value;
}

uint16_t arm_core_led_g_get(arm_core_state_t* state, uint16_t value)
{
    *((uint16_t*)(((uint8_t*)state->priv)) + NIT_ARM_CORE_LED_R_OFFSET);
}

void arm_core_led_b_set(arm_core_state_t* state, uint16_t value)
{
    *((uint16_t*)(((uint8_t*)state->priv)) + NIT_ARM_CORE_LED_R_OFFSET) = value;
}

uint16_t arm_core_led_b_get(arm_core_state_t* state, uint16_t value)
{
    *((uint16_t*)(((uint8_t*)state->priv)) + NIT_ARM_CORE_LED_R_OFFSET);
}