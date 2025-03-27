#include <nit/embedded/drivers/arm_core.h>
#include <nit/embedded/utils/config_file.h>

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

nit_arm_core_state_t nit_arm_core_driver;

int nit_arm_core_assert(nit_arm_core_state_t *state)
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

int nit_arm_core_open(nit_arm_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open)
    {
        return 0;
    }

    state->fd = -1;

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

    state->is_open = true;

    return 0;
}

int nit_arm_core_close(nit_arm_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL)
    {
        munmap((void *)state->priv, NIT_ARM_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_arm_core_config_save_to_file(nit_arm_core_state_t *state, const char *path)
{

    nit_arm_core_led_r_get(state, &state->config.led_r);
    nit_arm_core_led_g_get(state, &state->config.led_g);
    nit_arm_core_led_b_get(state, &state->config.led_b);
    nit_arm_core_soft_reset_get(state, &state->config.soft_reset);

    return config_file_save_to_file(state, path);
}

int nit_arm_core_config_load_from_file(nit_arm_core_state_t *state, const char *path)
{
    auto retval = config_file_load_from_file(state, path);

    nit_arm_core_led_r_set(state, state->config.led_r);
    nit_arm_core_led_g_set(state, state->config.led_g);
    nit_arm_core_led_b_set(state, state->config.led_b);
    nit_arm_core_soft_reset_set(state, state->config.soft_reset);

    return retval;
}

int nit_arm_core_led_r_set(nit_arm_core_state_t *state, uint16_t value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    print_debug("%s: (%d): %d\n", __func__, nit_arm_core_led_r_offset, value);
    unsafe_set<typeof(*state), uint32_t>(state, nit_arm_core_led_r_offset, value);
    return retval;
}

int nit_arm_core_led_r_get(nit_arm_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_arm_core_led_r_offset);
    print_debug("%s: (%d): %d\n", __func__, nit_arm_core_led_r_offset, *value);
    return retval;
}

int nit_arm_core_led_g_set(nit_arm_core_state_t *state, uint16_t value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    print_debug("%s: (%d): %d\n", __func__, nit_arm_core_led_g_offset, value);
    unsafe_set<typeof(*state), uint32_t>(state, nit_arm_core_led_g_offset, value);
    return retval;
}

int nit_arm_core_led_g_get(nit_arm_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_arm_core_led_g_offset);
    print_debug("%s: (%d): %d\n", __func__, nit_arm_core_led_g_offset, *value);
    return retval;
}

int nit_arm_core_led_b_set(nit_arm_core_state_t *state, uint16_t value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    print_debug("%s: (%d): %d\n", __func__, nit_arm_core_led_b_offset, value);
    unsafe_set<typeof(*state), uint32_t>(state, nit_arm_core_led_b_offset, value);
    return retval;
}

int nit_arm_core_led_b_get(nit_arm_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_arm_core_led_b_offset);
    print_debug("%s: (%d): %d\n", __func__, nit_arm_core_led_b_offset, *value);
    return retval;
}

int nit_arm_core_soft_reset_set(nit_arm_core_state_t *state, uint16_t value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    print_debug("%s: write nit_arm_core_soft_reset_offset (%d): %d\n", __func__, nit_arm_core_soft_reset_offset, value);
    unsafe_set<typeof(*state), uint32_t>(state, nit_arm_core_soft_reset_offset, value);

    return retval;
}

int nit_arm_core_soft_reset_get(nit_arm_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_arm_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_arm_core_soft_reset_offset);
    print_debug("%s: write nit_arm_core_soft_reset_offset (%d): %d\n", __func__, nit_arm_core_soft_reset_offset, *value);
    return retval;
}