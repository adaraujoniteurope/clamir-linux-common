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

int roi_core_open(nit_mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open == true)
    {
        return 0;
    }

    state->is_open = false;

    state->fd = -1;

    if (!DEBUGGING_HOST) {
        
        state->fd = open("/dev/mem", O_RDWR | O_SYNC);

        if (state->fd < 0)
        {
            return -2;
        }
    }

    if (!DEBUGGING_HOST) {
        state->priv = (volatile int*)mmap(NULL, NIT_ROI_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_ROI_CORE_BASE_ADDRESS);
    } else {
        state->priv = (volatile int*)malloc(NIT_ROI_CORE_SIZE);
    }

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}

int roi_core_close(nit_mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open == false)
    {
        return 0;
    }

    state->is_open = false;

    if (state->priv != NULL)
    {
        munmap((void*)state->priv, NIT_ROI_CORE_SIZE);
        state->priv = NULL;
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int roi_core_config_save_to_file(nit_roi_core_state_t* state, const char* path)
{
    return config_file_save_to_file(state, path);
}

int roi_core_config_load_from_file(nit_roi_core_state_t* state, const char* path)
{
    return config_file_load_from_file(state, path);
}

int nit_roi_core_round_set(nit_mb_core_state_t* state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_y1_offset, value);
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *((volatile uint32_t*)((state->priv)) + nit_roi_core_round_offset) = value;
    return retval;
}

int nit_roi_core_round_get(nit_mb_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = *((volatile uint32_t*)((state->priv)) + nit_roi_core_round_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_round_offset, *value);
    return retval;
}

int nit_roi_core_x1_set(nit_mb_core_state_t* state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_x1_offset, value);
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    *((volatile uint32_t*)((state->priv)) + nit_roi_core_x1_offset) = value;
    return retval;
}

int nit_roi_core_x1_get(nit_mb_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = *((volatile uint32_t*)((state->priv)) + nit_roi_core_x1_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_x1_offset, *value);
    return retval;
}

int nit_roi_core_x2_set(nit_mb_core_state_t* state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_x2_offset, value);
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    *((volatile uint32_t*)((state->priv)) + nit_roi_core_x2_offset) = value;
    return retval;
}

int nit_roi_core_x2_get(nit_mb_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = *((volatile uint32_t*)((state->priv)) + nit_roi_core_x2_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_x2_offset, *value);
    return retval;
}

int nit_roi_core_y1_set(nit_mb_core_state_t* state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_y1_offset, value);
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *((volatile uint32_t*)((state->priv)) + nit_roi_core_y1_offset) = value;
    return retval;
}

int nit_roi_core_y1_get(nit_mb_core_state_t* state, uint16_t* value)
{
    int retval = 0;
    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = *((volatile uint32_t*)((state->priv)) + nit_roi_core_y1_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_y1_offset, *value);
    return retval;
}

int nit_roi_core_y2_set(nit_mb_core_state_t* state, uint16_t value)
{
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_y2_offset, value);
    int retval = 0;
    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *((volatile uint32_t*)((state->priv)) + nit_roi_core_y2_offset) = value;
    return retval;
}

int nit_roi_core_y2_get(nit_mb_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_mb_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = *((volatile uint32_t*)((state->priv)) + nit_roi_core_y2_offset);
    print_debug("%s: (%04x): %d\n", __func__, nit_roi_core_y2_offset, *value);
    return retval;
}