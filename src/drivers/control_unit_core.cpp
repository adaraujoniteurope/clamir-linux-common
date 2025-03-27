#include "drivers/control_unit_core.h"
#include "utils/config_file.h"
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

nit_control_unit_core_state_t nit_control_unit_core_driver;

int nit_control_unit_core_open(nit_control_unit_core_state_t *state)
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

    if (!NIT_CLAMIR_HOST_MOCKUP) {
        
        state->fd = open("/dev/mem", O_RDWR | O_SYNC);

        if (state->fd < 0)
        {
            return -2;
        }
    }

    state->priv = (volatile int *)mmap(NULL, NIT_CONTROL_UNIT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_CONTROL_UNIT_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}

int nit_control_unit_core_close(nit_control_unit_core_state_t *state)
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
        munmap((void *)state->priv, NIT_CONTROL_UNIT_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_control_unit_core_config_save_to_file(nit_control_unit_core_state_t *state, const char *path)
{
    nit_control_unit_core_bias_v_get(state, &state->config.bias_v);
    nit_control_unit_core_black_level_get(state, &state->config.black_level);
    nit_control_unit_core_bpc_en_get(state, &state->config.bpc_en);
    nit_control_unit_core_drift_enable_get(state, &state->config.drift_enable);
    nit_control_unit_core_drift_position_get(state, &state->config.drift_position);
    nit_control_unit_core_int_time_get(state, &state->config.int_time);
    nit_control_unit_core_offset_update_get(state, &state->config.offset_update);
    nit_control_unit_core_shutter_reset_get(state, &state->config.shutter_reset);
    nit_control_unit_core_shutter_get(state, &state->config.shutter);
    nit_control_unit_core_temp1_get(state, &state->config.temp1);
    nit_control_unit_core_temp2_get(state, &state->config.temp2);
    return config_file_save_to_file(state, path);
}

int nit_control_unit_core_config_load_from_file(nit_control_unit_core_state_t *state, const char *path)
{
    auto retval = config_file_load_from_file(state, path);

    nit_control_unit_core_bias_v_set(state, state->config.bias_v);
    nit_control_unit_core_black_level_set(state, state->config.black_level);
    nit_control_unit_core_bpc_en_set(state, state->config.bpc_en);
    nit_control_unit_core_drift_enable_set(state, state->config.drift_enable);
    nit_control_unit_core_drift_position_set(state, state->config.drift_position);
    nit_control_unit_core_int_time_set(state, state->config.int_time);
    nit_control_unit_core_offset_update_set(state, state->config.offset_update);
    nit_control_unit_core_shutter_reset_set(state, state->config.shutter_reset);
    nit_control_unit_core_shutter_set(state, state->config.shutter);
    nit_control_unit_core_temp1_set(state, state->config.temp1);
    nit_control_unit_core_temp2_set(state, state->config.temp2);

    return retval;
}

int nit_control_unit_core_assert(nit_control_unit_core_state_t *state)
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

int control_unit_core_config_save_to_file(nit_control_unit_core_state_t *state, const char *path)
{
    return config_file_save_to_file(state, path);
}

int control_unit_core_config_load_from_file(nit_control_unit_core_state_t *state, const char *path)
{
    return config_file_load_from_file(state, path);
}

int nit_control_unit_core_bias_v_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_bias_v_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_bias_v_offset, value);
    return retval;
}

int nit_control_unit_core_bias_v_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_bias_v_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_bias_v_offset, *value);
    return retval;
}

int nit_control_unit_core_black_level_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_black_level_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_black_level_offset, value);
    return retval;
}

int nit_control_unit_core_black_level_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_black_level_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_black_level_offset, *value);
    return retval;
}

int nit_control_unit_core_bpc_en_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_bpc_en_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_bpc_en_offset, value);
    return retval;
}

int nit_control_unit_core_drift_enable_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_drift_enable_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_drift_enable_offset, value);
    return retval;
}

int nit_control_unit_core_drift_enable_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_drift_enable_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_drift_enable_offset, *value);
    return retval;
}

int nit_control_unit_core_drift_position_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_drift_position_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_drift_position_offset, value);
    return retval;
}

int nit_control_unit_core_drift_position_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_drift_position_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_drift_position_offset, *value);
    return retval;
}


int nit_control_unit_core_drift_level_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_drift_level_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_drift_level_offset, value);
    return retval;
}

int nit_control_unit_core_drift_level_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_drift_level_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_drift_level_offset, *value);
    return retval;
}


int nit_control_unit_core_bpc_en_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_bpc_en_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_bpc_en_offset, *value);
    return retval;
}

int nit_control_unit_core_int_time_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_int_time_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_int_time_offset, value);
    return retval;
}

int nit_control_unit_core_int_time_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_int_time_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_int_time_offset, *value);
    return retval;
}

int nit_control_unit_core_offset_update_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_offset_update_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_offset_update_offset) = value;
    return retval;
}

int nit_control_unit_core_offset_update_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_offset_update_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_offset_update_offset, *value);
    return retval;
}

int nit_control_unit_core_shutter_reset_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_shutter_reset_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_shutter_reset_offset) = value;
    return retval;
}

int nit_control_unit_core_shutter_reset_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_shutter_reset_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_shutter_reset_offset, *value);
    return retval;
}

int nit_control_unit_core_shutter_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_shutter_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_shutter_offset, value);
    return retval;
}

int nit_control_unit_core_shutter_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_shutter_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_shutter_offset, *value);
    return retval;
}

int nit_control_unit_core_temp1_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_temp1_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_temp1_offset, *value);
    return retval;
}

int nit_control_unit_core_temp1_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_temp1_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_temp1_offset, value);
    return retval;
}

int nit_control_unit_core_temp2_get(nit_control_unit_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_control_unit_core_temp2_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_temp2_offset, *value);
    return retval;
}

int nit_control_unit_core_temp2_set(nit_control_unit_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_control_unit_core_temp2_offset, value);

    int retval = 0;

    if ((retval = nit_control_unit_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_control_unit_core_temp2_offset, value);
    return retval;
}

double nit_control_unit_core_temp_get_voltage(uint16_t value)
{
    return 3.0f / 4096.0f * ((double)value);
}

double nit_control_unit_core_temp_get_resistance(uint16_t value)
{
    return 10000.f * ((3.3f / nit_control_unit_core_temp_get_voltage(value)) - 1.f);
}

double nit_control_unit_core_temp_to_degc(uint16_t value)
{
    return (int)(10 * ((3380.0f / (log(nit_control_unit_core_temp_get_resistance(value)) + 2.126235177f)) - 273.15f));
}