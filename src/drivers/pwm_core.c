#include "common/defs.h"
#include "drivers/mb_core.h"
#include "drivers/pwm_core.h"
#include "utils/config_file.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

const pwm_core_config_t pwm_core_config_default = {

};

int pwm_core_open(mb_core_state_t* state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_PWM_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_PWM_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

int pwm_core_close(mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*)state->priv, NIT_PWM_CORE_BASE_ADDRESS);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

int pwm_core_config_save_to_file(mb_core_state_t* state, const char *path)
{
    return 0;
}

int pwm_core_config_load_from_file(mb_core_state_t* state, const char *path)
{

    if (state == NULL) {
        return -1;
    }

    if (path == NULL) {
        return -2;
    }
    
    state->config.pwm_limit_max = config_file_read_key_as_int(path, "pwm_limit_max", pwm_core_config_default.pwm_limit_max);
    state->config.pwm = config_file_read_key_as_int(path, "pwm", pwm_core_config_default.pwm);
    state->config.power = config_file_read_key_as_int(path, "power", pwm_core_config_default.power);
    state->config.pwm_limit_min = config_file_read_key_as_int(path, "pwm_limit_min", pwm_core_config_default.pwm_limit_min);

    return 0;

}

void nit_pwm_core_pwm_limit_max_set(mb_core_state_t *state, uint32_t value)
{
    print_debug("%s: write nit_pwm_core_pwm_limit_max_offset (%d): %d\n", __func__, nit_pwm_core_pwm_limit_max_offset, value);
    *(((volatile uint32_t*) state->priv) + nit_pwm_core_pwm_limit_max_offset) = value;
}

uint32_t nit_pwm_core_pwm_limit_max_get(mb_core_state_t *state)
{
    uint32_t value = *(((volatile uint32_t*) state->priv) + nit_pwm_core_pwm_limit_max_offset);
    print_debug("%s: read: nit_pwm_core_pwm_limit_max_offset: %d\n", __func__, value);
    return value;
}

void nit_pwm_core_pwm_limit_min_set(mb_core_state_t *state, uint32_t value)
{
    print_debug("%s: write nit_pwm_core_pwm_limit_min_offset (%d): %d\n", __func__, nit_pwm_core_pwm_limit_min_offset, value);
    *(((volatile uint32_t*) state->priv) + nit_pwm_core_pwm_limit_min_offset) = value;
}

uint32_t nit_pwm_core_pwm_limit_min_get(mb_core_state_t *state)
{
    uint32_t value = *(((volatile uint32_t*) state->priv) + nit_pwm_core_pwm_limit_min_offset);
    print_debug("%s: read: nit_pwm_core_pwm_limit_min_offset: %d\n", __func__, value);
    return value;
}

void nit_pwm_core_pwm_set(mb_core_state_t *state, uint32_t value)
{
    print_debug("%s: write nit_pwm_core_pwm_offset (%d): %d\n", __func__, nit_pwm_core_pwm_offset, value);
    *(((volatile uint32_t*) state->priv) + nit_pwm_core_pwm_offset) = value;
}

uint32_t nit_pwm_core_pwm_get(mb_core_state_t *state)
{
    uint32_t value = *(((volatile uint32_t*) state->priv) + nit_pwm_core_pwm_offset);
    print_debug("%s: read: nit_pwm_core_pwm_offset: %d\n", __func__, value);
    return value;
}