#include "drivers/scc_core.h"
#include "utils/config_file.h"
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

nit_scc_core_state_t nit_scc_core_driver;

typedef struct scc_core_private_state_struct
{

    volatile int* framebuffer_metadata_shm;
    volatile int16_t* framebuffer_shm;

    volatile int16_t* scale_shm;
    volatile int16_t* offset_shm;

    volatile int16_t* min_shm;
    volatile int16_t* max_shm;


} scc_core_private_state_t;

int nit_scc_core_open(nit_scc_core_state_t* state)
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

    state->fd = open("/dev/mem", O_RDWR | O_SYNC);

    if (state->fd < 0)
    {
        return -2;
    }

    state->priv = (volatile int*)mmap(NULL, NIT_SCC_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_SCC_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}

int nit_scc_core_close(nit_scc_core_state_t* state)
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
        munmap((void*)state->priv, NIT_SCC_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_scc_core_config_save_to_file(nit_scc_core_state_t* state, const char* path)
{
    nit_scc_core_opmode_get(state, &state->config.opmode);
    return config_file_save_to_file(state, path);
}

int nit_scc_core_config_load_from_file(nit_scc_core_state_t* state, const char* path)
{
    auto retval = config_file_load_from_file(state, path);
    nit_scc_core_opmode_set(state, state->config.opmode);
    return retval;
}

int nit_scc_core_assert(nit_scc_core_state_t* state)
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

int scc_core_config_save_to_file(nit_scc_core_state_t* state, const char* path)
{
    return config_file_save_to_file(state, path);
}

int scc_core_config_load_from_file(nit_scc_core_state_t* state, const char* path)
{
    return config_file_load_from_file(state, path);
}

int nit_scc_core_opmode_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_opmode_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_scc_core_opmode_offset, value);
    return retval;
}

int nit_scc_core_opmode_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_scc_core_opmode_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_opmode_offset, *value);
    return retval;
}

int nit_scc_core_acquire_max_enable_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_acquire_max_enable_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_scc_core_acquire_max_enable_offset, value);
    return retval;
}

int nit_scc_core_acquire_max_enable_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_scc_core_acquire_max_enable_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_acquire_max_enable_offset, *value);
    return retval;
}

int nit_scc_core_acquire_min_enable_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_acquire_min_enable_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    unsafe_set(state, nit_scc_core_acquire_min_enable_offset, value);
    return retval;
}

int nit_scc_core_acquire_min_enable_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = unsafe_get<typeof(*state), uint32_t>(state, nit_scc_core_acquire_min_enable_offset);
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_acquire_min_enable_offset, *value);
    return retval;
}

int nit_scc_core_stub_eval_calibrate_acquire_min(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

}

int nit_scc_core_stub_eval_calibrate_acquire_max(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

}

int nit_scc_core_stub_eval_calibrate_update(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

}

int nit_scc_core_stub_eval_calibrate(nit_scc_core_state_t* state) {
    
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;

    auto acquire_min = unsafe_get<typeof(*state), uint32_t>(state, nit_scc_core_acquire_min_enable_offset);

    if (acquire_min == 1) {
        if ((retval = nit_scc_core_stub_eval_calibrate_acquire_min(state)) < 0) {
            print_debug("%s: %s %d\n", __func__, "failed", retval);
            return retval;
        }
    }

    auto acquire_max = unsafe_get<typeof(*state), uint32_t>(state, nit_scc_core_acquire_max_enable_offset);

    if (acquire_max == 1) {
        if ((retval = nit_scc_core_stub_eval_calibrate_acquire_max(state)) < 0) {
            print_debug("%s: %s %d\n", __func__, "failed", retval);
            return retval;
        }
    }

    return nit_scc_core_stub_eval_calibrate_update(state);

}

int nit_scc_core_stub_eval_process(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;

    for (size_t i = 0; i < state->config.height; i++) {
        for (size_t j = 0; j < state->config.width; j++) {
            priv->framebuffer_shm[i * state->config.width + j] = priv->framebuffer_shm[i * state->config.width + j] * priv->scale_shm[i * state->config.width + j] + priv->offset_shm[i * state->config.width + j];
        }
    }

}

typedef enum nit_scc_core_opmode_enum
{
    NIT_SCC_CORE_OPMODE_PROCESS,
    NIT_SCC_CORE_OPMODE_CALIBRATE,
    NIT_SCC_CORE_OPMODE_MAX
} nit_scc_core_opmode_t;

/**
 * this model will be implemented on hardware
 * is convenient that it has this structure once
 * it will have a state machine to control it...
 */
int nit_scc_core_stub_eval(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    // scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;

    auto opmode = unsafe_get<typeof(*state), uint32_t>(state, nit_scc_core_opmode_offset);

    switch(opmode) {
        case NIT_SCC_CORE_OPMODE_PROCESS:
            nit_scc_core_stub_eval_process(state);
            break;
        case NIT_SCC_CORE_OPMODE_CALIBRATE:
            nit_scc_core_stub_eval_calibrate(state);
            break;
        default:
            unsafe_set(state, nit_scc_core_acquire_min_enable_offset, (uint32_t) NIT_SCC_CORE_OPMODE_PROCESS);
            break;
    }

    return 0;
}