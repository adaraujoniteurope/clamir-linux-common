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

int gen_core_open(nit_mb_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

    state->is_open = false;

    state->fd = -1;

    if (!NIT_CLAMIR_HOST_MOCKUP) {
        
        state->fd = open("/dev/mem", O_RDWR | O_SYNC);

        if (state->fd < 0)
        {
            return -2;
        }
    }

    state->priv = (volatile int *)mmap(NULL, NIT_GEN_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_GEN_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}

int gen_core_close(nit_mb_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv == NULL)
    {
        return -2;
    }

    state->is_open = false;

    if (state->priv != NULL)
    {
        munmap((void *)state->priv, NIT_GEN_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int gen_core_config_save_to_file(nit_mb_core_state_t *state, const char *path)
{
    return 0;
}

int gen_core_config_load_from_file(nit_mb_core_state_t *state, const char *path)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv == NULL)
    {
        return -2;
    }

    if (state->is_open == false)
    {
        return -3;
    }

    return 0;
}

int nit_gen_core_state_assert(nit_mb_core_state_t *state)
{

    if (state == NULL)
    {
        return -1;
    }

    if (state->priv == NULL)
    {
        return -2;
    }

    if (state->is_open == false)
    {
        return -3;
    }

    return 0;
}

int gen_core_config_save_to_file(nit_gen_core_state_t *state, const char *path)
{
    return config_file_save_to_file(state, path);
}

int gen_core_config_load_from_file(nit_gen_core_state_t *state, const char *path)
{
    return config_file_load_from_file(state, path);
}

int nit_gen_core_change_op_mode_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_change_op_mode_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_change_op_mode_offset, value);

    return retval;
}

int nit_gen_core_change_op_mode_get(nit_mb_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_change_op_mode_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_change_op_mode_offset, *value);

    return retval;
}

int nit_gen_core_digital_in_0_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_in_0_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_digital_in_0_offset, value);

    return retval;
}

int nit_gen_core_digital_in_0_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_digital_in_0_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_in_0_offset, *value);

    return retval;
}

int nit_gen_core_digital_in_1_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_in_1_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_digital_in_1_offset, value);

    return retval;
}

int nit_gen_core_digital_in_1_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_digital_in_1_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_in_1_offset, *value);

    return retval;
}

int nit_gen_core_digital_out_0_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_0_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_digital_out_0_offset, value);

    return retval;
}

int nit_gen_core_digital_out_0_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_digital_out_0_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_0_offset, *value);

    return retval;
}

int nit_gen_core_digital_out_1_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_1_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_digital_out_1_offset, value);

    return retval;
}

int nit_gen_core_digital_out_1_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_digital_out_1_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_1_offset, *value);

    return retval;
}

int nit_gen_core_digital_out_2_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_2_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_digital_out_2_offset, value);

    return retval;
}

int nit_gen_core_digital_out_2_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_digital_out_2_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_2_offset, *value);

    return retval;
}

int nit_gen_core_digital_out_3_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_3_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_digital_out_3_offset, value);

    return retval;
}

int nit_gen_core_digital_out_3_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_digital_out_3_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_3_offset, *value);

    return retval;
}

int nit_gen_core_digital_out_conf_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_conf_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_digital_out_conf_offset, value);

    return retval;
}

int nit_gen_core_digital_out_conf_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_digital_out_conf_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_digital_out_conf_offset, *value);

    return retval;
}

int nit_gen_core_enable_roi_set(nit_mb_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_enable_roi_offset, value);

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_gen_core_enable_roi_offset, value);

    return retval;
}

int nit_gen_core_enable_roi_get(nit_mb_core_state_t *state, uint16_t *value)
{

    int retval = 0;

    if ((retval = nit_gen_core_state_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_gen_core_enable_roi_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_gen_core_enable_roi_offset, *value);

    return retval;
}