#include "drivers/control_unit_core.h"
#include "utils/config_file.h"
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

control_unit_core_state_t nit_control_unit_core_driver;

const control_unit_core_config_t control_unit_config_default =
    {
        .int_time = 600,
        .bias_v = 13652,
        .offset_en = 0,
        .offset_update = 0,
        .shutter = 0,
        .bpc_en = 0,
        .bpc_mem_write = 0,
        .bpc_identify = 0,
        .temp1 = 0,
        .temp2 = 0,
        .temp3 = 0,
        .temp4 = 0,
        .shutter_reset = 0,
        .sincronization = 0,
        .save_embedded_conf = 0,
        .arm_sw_version = 0,
        .drift_enable = 1,
        .drift_position = 14,
        .drift_level = 0,
        .fpga_version = 0,
        .reserved_9 = 0,
        .reserved_10 = 0,
        .reserved_11 = 0,
        .reserved_12 = 0,
        .reserved_13 = 0,
        .reserved_14 = 0,
        .reserved_15 = 0,
        .reserved_16 = 0,
        .reserved_17 = 0,
        .reserved_18 = 0,
        .reserved_19 = 0,
        .reserved_20 = 0,
        .trigger_usec = 0,
        .black_level = 1000,
        .reserved_21 = 0,
};

int nit_control_unit_core_open(control_unit_core_state_t *state)
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

int nit_control_unit_core_close(control_unit_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

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

int nit_control_unit_core_config_save_to_file(control_unit_core_state_t *state, const char *path)
{
    return 0;
}

int nit_control_unit_core_config_load_from_file(control_unit_core_state_t *state, const char *path)
{

    if (state == NULL)
    {
        return -1;
    }

    if (path == NULL)
    {
        return -2;
    }

    state->config.int_time = config_file_read_key_as_int(path, "int_time", control_unit_config_default.int_time);
    state->config.bias_v = config_file_read_key_as_int(path, "bias_v", control_unit_config_default.bias_v);
    state->config.offset_en = config_file_read_key_as_int(path, "offset_en", control_unit_config_default.offset_en);
    state->config.offset_update = config_file_read_key_as_int(path, "offset_update", control_unit_config_default.offset_update);
    state->config.shutter = config_file_read_key_as_int(path, "shutter", control_unit_config_default.shutter);
    state->config.bpc_en = config_file_read_key_as_int(path, "bpc_en", control_unit_config_default.bpc_en);
    state->config.bpc_mem_write = config_file_read_key_as_int(path, "bpc_mem_write", control_unit_config_default.bpc_mem_write);
    state->config.bpc_identify = config_file_read_key_as_int(path, "bpc_identify", control_unit_config_default.bpc_identify);
    state->config.temp1 = config_file_read_key_as_int(path, "temp1", control_unit_config_default.temp1);
    state->config.temp2 = config_file_read_key_as_int(path, "temp2", control_unit_config_default.temp2);
    state->config.temp3 = config_file_read_key_as_int(path, "temp3", control_unit_config_default.temp3);
    state->config.temp4 = config_file_read_key_as_int(path, "temp4", control_unit_config_default.temp4);
    state->config.shutter_reset = config_file_read_key_as_int(path, "shutter_reset", control_unit_config_default.shutter_reset);
    state->config.sincronization = config_file_read_key_as_int(path, "sincronization", control_unit_config_default.sincronization);
    state->config.save_embedded_conf = config_file_read_key_as_int(path, "save_embedded_conf", control_unit_config_default.save_embedded_conf);
    state->config.arm_sw_version = config_file_read_key_as_int(path, "arm_sw_version", control_unit_config_default.arm_sw_version);
    state->config.drift_enable = config_file_read_key_as_int(path, "drift_enable", control_unit_config_default.drift_enable);
    state->config.drift_position = config_file_read_key_as_int(path, "drift_position", control_unit_config_default.drift_position);
    state->config.drift_level = config_file_read_key_as_int(path, "drift_level", control_unit_config_default.drift_level);
    state->config.fpga_version = config_file_read_key_as_int(path, "fpga_version", control_unit_config_default.fpga_version);
    state->config.reserved_9 = config_file_read_key_as_int(path, "reserved_9", control_unit_config_default.reserved_9);
    state->config.reserved_10 = config_file_read_key_as_int(path, "reserved_10", control_unit_config_default.reserved_10);
    state->config.reserved_11 = config_file_read_key_as_int(path, "reserved_11", control_unit_config_default.reserved_11);
    state->config.reserved_12 = config_file_read_key_as_int(path, "reserved_12", control_unit_config_default.reserved_12);
    state->config.reserved_13 = config_file_read_key_as_int(path, "reserved_13", control_unit_config_default.reserved_13);
    state->config.reserved_14 = config_file_read_key_as_int(path, "reserved_14", control_unit_config_default.reserved_14);
    state->config.reserved_15 = config_file_read_key_as_int(path, "reserved_15", control_unit_config_default.reserved_15);
    state->config.reserved_16 = config_file_read_key_as_int(path, "reserved_16", control_unit_config_default.reserved_16);
    state->config.reserved_17 = config_file_read_key_as_int(path, "reserved_17", control_unit_config_default.reserved_17);
    state->config.reserved_18 = config_file_read_key_as_int(path, "reserved_18", control_unit_config_default.reserved_18);
    state->config.reserved_19 = config_file_read_key_as_int(path, "reserved_19", control_unit_config_default.reserved_19);
    state->config.reserved_20 = config_file_read_key_as_int(path, "reserved_20", control_unit_config_default.reserved_20);
    state->config.trigger_usec = config_file_read_key_as_int(path, "trigger_usec", control_unit_config_default.trigger_usec);
    state->config.black_level = config_file_read_key_as_int(path, "black_level", control_unit_config_default.black_level);
    state->config.reserved_21 = config_file_read_key_as_int(path, "reserved_21", control_unit_config_default.reserved_21);
    
    return 0;
}

void nit_control_unit_core_bias_v_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_bias_v_offset (%d): %d\n", __func__, nit_control_unit_core_bias_v_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_bias_v_offset) = value;
}

uint32_t nit_control_unit_core_bias_v_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_bias_v_offset);
}

void nit_control_unit_core_black_level_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_black_level_offset (%d): %d\n", __func__, nit_control_unit_core_black_level_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_black_level_offset) = value;
}

uint32_t nit_control_unit_core_black_level_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_black_level_offset);
}

void nit_control_unit_core_bpc_en_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_bpc_en_offset (%d): %d\n", __func__, nit_control_unit_core_bpc_en_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_bpc_en_offset) = value;
}

uint32_t nit_control_unit_core_bpc_en_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_bpc_en_offset);
}

void nit_control_unit_core_drift_enable_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_drift_enable_offset (%d): %d\n", __func__, nit_control_unit_core_drift_enable_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_drift_enable_offset) = value;
}

uint32_t nit_control_unit_core_drift_enable_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_drift_enable_offset);
}

void nit_control_unit_core_drift_position_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_drift_position_offset (%d): %d\n", __func__, nit_control_unit_core_drift_position_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_drift_position_offset) = value;
}

uint32_t nit_control_unit_core_drift_position_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_drift_position_offset);
}

void nit_control_unit_core_int_time_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_int_time_offset (%d): %d\n", __func__, nit_control_unit_core_int_time_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_int_time_offset) = value;
}

uint32_t nit_control_unit_core_int_time_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_int_time_offset);
}

void nit_control_unit_core_offset_update_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_offset_update_offset (%d): %d\n", __func__, nit_control_unit_core_offset_update_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_offset_update_offset) = value;
}

uint32_t nit_control_unit_core_offset_update_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_offset_update_offset);
}

void nit_control_unit_core_shutter_reset_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_shutter_reset_offset (%d): %d\n", __func__, nit_control_unit_core_shutter_reset_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_shutter_reset_offset) = value;
}

uint32_t nit_control_unit_core_shutter_reset_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_shutter_reset_offset);
}

void nit_control_unit_core_shutter_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_shutter_offset (%d): %d\n", __func__, nit_control_unit_core_shutter_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_shutter_offset) = value;
}

uint32_t nit_control_unit_core_shutter_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_shutter_offset);
}

uint32_t nit_control_unit_core_temp1_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_temp1_offset);
}

void nit_control_unit_core_temp1_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_temp1_offset (%d): %d\n", __func__, nit_control_unit_core_temp1_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_temp1_offset) = value;
}

uint32_t nit_control_unit_core_temp2_get(control_unit_core_state_t *state)
{
    return *(((volatile uint32_t *)state->priv) + nit_control_unit_core_temp2_offset);
}

void nit_control_unit_core_temp2_set(control_unit_core_state_t *state, uint32_t value)
{
    printf("%s: nit_control_unit_core_temp2_offset (%d): %d\n", __func__, nit_control_unit_core_temp2_offset, value);
    *(((volatile uint32_t *)state->priv) + nit_control_unit_core_temp2_offset) = value;
}

double nit_control_unit_core_temp_get_voltage(uint32_t value)
{
    return 3.0f / 4096.0f * ((double)value);
}

double nit_control_unit_core_temp_get_resistance(uint32_t value)
{
    return 10000.f * ((3.3f / nit_control_unit_core_temp_get_voltage(value)) - 1.f);
}

double nit_control_unit_core_temp_to_degc(uint32_t value)
{
    return (int)(10 * ((3380.0f / (log(nit_control_unit_core_temp_get_resistance(value)) + 2.126235177f)) - 273.15f));
}