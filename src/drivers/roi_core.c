#include "common/defs.h"
#include "drivers/mb_core.h"
#include "drivers/roi_core.h"
#include "utils/config_file.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

const roi_core_config_t roi_core_config_default = {
    .round = 0,
    .x1 = 2,
    .x2 = 61,
    .y1 = 2,
    .y2 = 61,
};

int roi_core_open(mb_core_state_t* state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_ROI_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_ROI_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

int roi_core_close(mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*)state->priv, NIT_ROI_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

int roi_core_config_save_to_file(mb_core_state_t* state, const char *path)
{
    return 0;
}

int roi_core_config_load_from_file(mb_core_state_t* state, const char *path)
{

    if (state == NULL) {
        return -1;
    }

    if (path == NULL) {
        return -2;
    }
    
    state->config.x1 = config_file_read_key_as_int(path, "x1", roi_core_config_default.x1);
    state->config.y1 = config_file_read_key_as_int(path, "y1", roi_core_config_default.y1);
    state->config.x2 = config_file_read_key_as_int(path, "x2", roi_core_config_default.x2);
    state->config.y2 = config_file_read_key_as_int(path, "y2", roi_core_config_default.y2);
    state->config.round = config_file_read_key_as_int(path, "round", roi_core_config_default.round);

    return 0;
}

void nit_roi_core_round_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_roi_core_round_offset (%d): %d\n", __func__, nit_roi_core_round_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_roi_core_round_offset) = value;
}

uint32_t nit_roi_core_round_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_roi_core_round_offset);
    print_debug("%s: read nit_roi_core_round_offset: %d\n", value);
    return value;
}


void nit_roi_core_x1_set(mb_core_state_t* state, uint32_t value)
{
    print_debug("%s: nit_roi_core_x1_offset (%d): %d\n", __func__, nit_roi_core_x1_offset, value);
    *(((volatile uint32_t*)state->priv) + nit_roi_core_x1_offset) = value;
}

uint32_t nit_roi_core_x1_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_roi_core_x1_offset);
    print_debug("%s: read nit_roi_core_x1_offset: %d\n", value);
    return value;
}


void nit_roi_core_x2_set(mb_core_state_t* state, uint32_t value)
{print_debug("%s: nit_roi_core_x2_offset  (%d): %d\n", __func__, nit_roi_core_x2_offset , value);
    *(((volatile uint32_t*)state->priv) + nit_roi_core_x2_offset ) = value;
}

uint32_t nit_roi_core_x2_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_roi_core_x2_offset );
    print_debug("%s: read nit_roi_core_x2_offsett: %d\n", value);
    return value;
}


void nit_roi_core_y1_set(mb_core_state_t* state, uint32_t value)
{print_debug("%s: nit_roi_core_y1_offset  (%d): %d\n", __func__, nit_roi_core_y1_offset , value);
    *(((volatile uint32_t*)state->priv) + nit_roi_core_y1_offset ) = value;
}

uint32_t nit_roi_core_y1_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_roi_core_y1_offset );
    print_debug("%s: read nit_roi_core_y1_offsett: %d\n", value);
    return value;
}


void nit_roi_core_y2_set(mb_core_state_t* state, uint32_t value)
{print_debug("%s: nit_roi_core_y2_offset  (%d): %d\n", __func__, nit_roi_core_y2_offset , value);
    *(((volatile uint32_t*)state->priv) + nit_roi_core_y2_offset ) = value;
}

uint32_t nit_roi_core_y2_get(mb_core_state_t* state)
{
    uint32_t value = *(((volatile uint32_t*)state->priv) + nit_roi_core_y2_offset );
    print_debug("%s: read nit_roi_core_y2_offsett: %d\n", value);
    return value;
}