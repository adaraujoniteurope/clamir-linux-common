#include "common/defs.h"
#include "drivers/mb_core.h"
#include "drivers/mom_core.h"
#include "utils/config_file.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

const mom_core_config_t mom_core_config_default =
{
        .reference_track = 3,
        .end_of_track = 30,
        .start_track_mom_t = 40,
        .threshold = 1182,
        .time_track_high = 40,
        .time_track_low = 30,
        .mode = 1,
};

int mom_core_open(mb_core_state_t* state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_MOM_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_MOM_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

int mom_core_close(mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*)state->priv, NIT_MOM_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

int mom_core_config_save_to_file(mb_core_state_t* state, const char *path)
{
    return 0;
}

int mom_core_config_load_from_file(mb_core_state_t* state, const char *path)
{
    
    if (state == NULL) {
        return -1;
    }

    if (path == NULL) {
        return -2;
    }
    
    state->config.threshold = config_file_read_key_as_int(path, "threshold", mom_core_config_default.threshold);
    state->config.intensity_min = config_file_read_key_as_int(path, "intensity_min", mom_core_config_default.intensity_min);
    state->config.intensity_max = config_file_read_key_as_int(path, "intensity_max", mom_core_config_default.intensity_max);
    state->config.mom_00 = config_file_read_key_as_int(path, "mom_00", mom_core_config_default.mom_00);
    state->config.mom_01 = config_file_read_key_as_int(path, "mom_01", mom_core_config_default.mom_01);
    state->config.mom_10 = config_file_read_key_as_int(path, "mom_10", mom_core_config_default.mom_10);
    state->config.mom_11 = config_file_read_key_as_int(path, "mom_11", mom_core_config_default.mom_11);
    state->config.mom_02 = config_file_read_key_as_int(path, "mom_02", mom_core_config_default.mom_02);
    state->config.mom_20 = config_file_read_key_as_int(path, "mom_20", mom_core_config_default.mom_20);
    state->config.start_track_mom_t = config_file_read_key_as_int(path, "start_track_mom_t", mom_core_config_default.start_track_mom_t);
    state->config.track_nbr = config_file_read_key_as_int(path, "track_nbr", mom_core_config_default.track_nbr);
    state->config.mode = config_file_read_key_as_int(path, "mode", mom_core_config_default.mode);
    state->config.time_track_low = config_file_read_key_as_int(path, "time_track_low", mom_core_config_default.time_track_low);
    state->config.time_track_high = config_file_read_key_as_int(path, "time_track_high", mom_core_config_default.time_track_high);
    state->config.frame_max = config_file_read_key_as_int(path, "frame_max", mom_core_config_default.frame_max);
    state->config.reference_track = config_file_read_key_as_int(path, "reference_track", mom_core_config_default.reference_track);
    state->config.end_of_track = config_file_read_key_as_int(path, "end_of_track", mom_core_config_default.end_of_track);

    return 0;
}

void nit_mom_core_reference_track_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_mom_core_reference_track_offset (%d): %d\n", __func__, nit_mom_core_reference_track_offset, value);
    *((volatile uint32_t*)((state->priv)) + nit_mom_core_reference_track_offset) = value;
}

uint32_t nit_mom_core_reference_track_get(mb_core_state_t* state)
{
    uint32_t value = *((volatile uint32_t*)((state->priv)) + nit_mom_core_reference_track_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}

void nit_mom_core_end_of_track_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_mom_core_end_of_track_offset (%d): %d\n", __func__, nit_mom_core_end_of_track_offset, value);
    *((volatile uint32_t*)((state->priv)) + nit_mom_core_end_of_track_offset) = value;
}

uint32_t nit_mom_core_end_of_track_get(mb_core_state_t* state)
{
    uint32_t value = *((volatile uint32_t*)((state->priv)) + nit_mom_core_end_of_track_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}

void nit_mom_core_start_track_mom_t_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_mom_core_start_track_mom_t_offset (%d): %d\n", __func__, nit_mom_core_start_track_mom_t_offset, value);
    *((volatile uint32_t*)((state->priv)) + nit_mom_core_start_track_mom_t_offset) = value;
}

uint32_t nit_mom_core_start_track_mom_t_get(mb_core_state_t* state)
{
    uint32_t value = *((volatile uint32_t*)((state->priv)) + nit_mom_core_start_track_mom_t_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}

void nit_mom_core_threshold_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_mom_core_threshold_offset (%d): %d\n", __func__, nit_mom_core_threshold_offset, value);
    *((volatile uint32_t*)((state->priv)) + nit_mom_core_threshold_offset) = value;
}

uint32_t nit_mom_core_threshold_get(mb_core_state_t* state)
{
    uint32_t value = *((volatile uint32_t*)((state->priv)) + nit_mom_core_threshold_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}

void nit_mom_core_time_track_high_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_mom_core_time_track_high_offset (%d): %d\n", __func__, nit_mom_core_time_track_high_offset, value);
    *((volatile uint32_t*)((state->priv)) + nit_mom_core_time_track_high_offset) = value;
}

uint32_t nit_mom_core_time_track_high_get(mb_core_state_t* state)
{
    uint32_t value = *((volatile uint32_t*)((state->priv)) + nit_mom_core_time_track_high_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}

void nit_mom_core_time_track_low_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_mom_core_time_track_low_offset (%d): %d\n", __func__, nit_mom_core_time_track_low_offset, value);
    *((volatile uint32_t*)((state->priv)) + nit_mom_core_time_track_low_offset) = value;
}

uint32_t nit_mom_core_time_track_low_get(mb_core_state_t* state)
{
    uint32_t value = *((volatile uint32_t*)((state->priv)) + nit_mom_core_time_track_low_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}

void nit_mom_core_mode_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: write nit_mom_core_mode_offset (%d): %d\n", __func__, nit_mom_core_mode_offset, value);
    *((volatile uint32_t*)((state->priv)) + nit_mom_core_mode_offset) = value;
}

uint32_t nit_mom_core_mode_get(mb_core_state_t* state)
{
    uint32_t value = *((volatile uint32_t*)((state->priv)) + nit_mom_core_mode_offset);
    print_debug("%s: read nit_gen_core_change_op_mode_offset: %d\n", __func__, value);
    return value;
}
