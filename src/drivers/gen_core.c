#include "common/defs.h"
#include "drivers/mb_core.h"
#include "drivers/gen_core.h"
#include "utils/config_file.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

const gen_core_config_t gen_core_config_default = {
    .change_op_mode = 0,
    .digital_in_0 = 0,
    .digital_in_1 = 0,
    .digital_out_0 = 0,
    .digital_out_1 = 0,
    .digital_out_2 = 0,
    .digital_out_3 = 0,
    .digital_out_conf = 0,
    .enable_roi = 1,
};

int gen_core_open(mb_core_state_t* state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_GEN_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_GEN_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

int gen_core_close(mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*)state->priv, NIT_GEN_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

int gen_core_config_save_to_file(mb_core_state_t* state, const char *path)
{
    return 0;
}

int gen_core_config_load_from_file(mb_core_state_t* state, const char *path)
{
    if (state == NULL) {
        return -1;
    }

    if (path == NULL) {
        return -2;
    }
    
    state->config.version_id = config_file_read_key_as_int(path, "version_id", gen_core_config_default.version_id);
    state->config.enable_mom = config_file_read_key_as_int(path, "enable_mom", gen_core_config_default.enable_mom);
    state->config.enable_roi = config_file_read_key_as_int(path, "enable_roi", gen_core_config_default.enable_roi);
    state->config.enable_pwm = config_file_read_key_as_int(path, "enable_pwm", gen_core_config_default.enable_pwm);
    state->config.enable_irq = config_file_read_key_as_int(path, "enable_irq", gen_core_config_default.enable_irq);
    state->config.change_op_mode = config_file_read_key_as_int(path, "change_op_mode", gen_core_config_default.change_op_mode);
    state->config.irq_microblaze = config_file_read_key_as_int(path, "irq_microblaze", gen_core_config_default.irq_microblaze);
    state->config.digital_in_0 = config_file_read_key_as_int(path, "digital_in_0", gen_core_config_default.digital_in_0);
    state->config.digital_in_1 = config_file_read_key_as_int(path, "digital_in_1", gen_core_config_default.digital_in_1);
    state->config.digital_out_0 = config_file_read_key_as_int(path, "digital_out_0", gen_core_config_default.digital_out_0);
    state->config.digital_out_1 = config_file_read_key_as_int(path, "digital_out_1", gen_core_config_default.digital_out_1);
    state->config.digital_out_conf = config_file_read_key_as_int(path, "digital_out_conf", gen_core_config_default.digital_out_conf);
    state->config.digital_in_2 = config_file_read_key_as_int(path, "digital_in_2", gen_core_config_default.digital_in_2);
    state->config.digital_in_3 = config_file_read_key_as_int(path, "digital_in_3", gen_core_config_default.digital_in_3);
    state->config.digital_out_2 = config_file_read_key_as_int(path, "digital_out_2", gen_core_config_default.digital_out_2);
    state->config.digital_out_3 = config_file_read_key_as_int(path, "digital_out_3", gen_core_config_default.digital_out_3);

    return 0;
}

void nit_gen_core_change_op_mode_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_change_op_mode_offset (%d): %d\n", __func__, nit_gen_core_change_op_mode_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_change_op_mode_offset) = value;
}

uint32_t nit_gen_core_change_op_mode_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_change_op_mode_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_digital_in_0_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_digital_in_0_offset (%d): %d\n", __func__, nit_gen_core_digital_in_0_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_in_0_offset) = value;
}

uint32_t nit_gen_core_digital_in_0_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_in_0_offset);
    print_debug("%s: read nit_gen_core_digital_in_0_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_digital_in_1_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_digital_in_1_offset (%d): %d\n", __func__, nit_gen_core_digital_in_1_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_in_1_offset) = value;
}

uint32_t nit_gen_core_digital_in_1_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_in_1_offset);
    print_debug("%s: read nit_gen_core_digital_in_1_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_digital_out_0_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_digital_out_0_offset (%d): %d\n", __func__, nit_gen_core_digital_out_0_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_0_offset) = value;
}

uint32_t nit_gen_core_digital_out_0_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_0_offset);
    print_debug("%s: read nit_gen_core_digital_out_0_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_digital_out_1_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_digital_out_1_offset (%d): %d\n", __func__, nit_gen_core_digital_out_1_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_1_offset) = value;
}

uint32_t nit_gen_core_digital_out_1_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_1_offset);
    print_debug("%s: read nit_gen_core_digital_out_1_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_digital_out_2_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_digital_out_2_offset (%d): %d\n", __func__, nit_gen_core_digital_out_2_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_2_offset) = value;
}

uint32_t nit_gen_core_digital_out_2_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_2_offset);
    print_debug("%s: read nit_gen_core_digital_out_2_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_digital_out_3_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_digital_out_3_offset (%d): %d\n", __func__, nit_gen_core_digital_out_3_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_3_offset) = value;
}

uint32_t nit_gen_core_digital_out_3_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_3_offset);
    print_debug("%s: read nit_gen_core_digital_out_3_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_digital_out_conf_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_digital_out_conf_offset (%d): %d\n", __func__, nit_gen_core_digital_out_conf_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_conf_offset) = value;
}

uint32_t nit_gen_core_digital_out_conf_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_digital_out_conf_offset);
    print_debug("%s: read nit_gen_core_digital_out_conf_offset: %d\n", __func__, value);
    return value;
}


void nit_gen_core_enable_roi_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_gen_core_enable_roi_offset (%d): %d\n", __func__, nit_gen_core_enable_roi_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_gen_core_enable_roi_offset) = value;
}

uint32_t nit_gen_core_enable_roi_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_gen_core_enable_roi_offset);
    print_debug("%s: read nit_gen_core_enable_roi_offset: %d\n", __func__, value);
    return value;
}