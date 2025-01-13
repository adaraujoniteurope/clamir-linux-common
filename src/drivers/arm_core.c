#include "drivers/arm_core.h"
#include "utils/config_file.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

DRIVER_DEFINE_NAMES_TABLE_BEGIN(arm_core)
DRIVER_DEFINE_NAMES_TABLE_END(arm_core)

arm_core_state_t nit_arm_core_driver;

const arm_core_config_t arm_core_confg_default = {
    .led_b = 0,
    .led_g = 0,
    .led_r = 0,
};

uint32_t nit_arm_core_open(arm_core_state_t* state)
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

uint32_t nit_arm_core_close(arm_core_state_t* state)
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

int arm_core_config_save_to_file(arm_core_state_t* state, const char *path)
{
    return 0;
}

int arm_core_config_load_from_file(arm_core_state_t* state, const char *path)
{
    if (state == NULL) {
        return -1;
    }

    if (path == NULL) {
        return -2;
    }

    state->config.led_b = config_file_read_key_as_int(path, "led_b", arm_core_confg_default.led_b);
    state->config.led_g = config_file_read_key_as_int(path, "led_g", arm_core_confg_default.led_g);
    state->config.led_g = config_file_read_key_as_int(path, "led_r", arm_core_confg_default.led_r);

}

void nit_arm_core_led_r_set(arm_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_arm_core_led_r_offset (%d): %d\n", __func__, nit_arm_core_led_r_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_arm_core_led_r_offset) = value;
}

uint32_t nit_arm_core_led_r_get(arm_core_state_t* state)
{
    return *(((volatile uint32_t*)state->priv) + nit_arm_core_led_r_offset);
}

void nit_arm_core_led_g_set(arm_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_arm_core_led_r_offset (%d): %d\n", __func__, nit_arm_core_led_r_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_arm_core_led_r_offset) = value;
}

uint32_t nit_arm_core_led_g_get(arm_core_state_t* state)
{
    return *(((volatile uint32_t*)state->priv) + nit_arm_core_led_r_offset);
}

void nit_arm_core_led_b_set(arm_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_arm_core_led_r_offset (%d): %d\n", __func__, nit_arm_core_led_r_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_arm_core_led_r_offset) = value;
}

uint32_t nit_arm_core_led_b_get(arm_core_state_t* state)
{
    return *(((volatile uint32_t*)state->priv) + nit_arm_core_led_r_offset);
}