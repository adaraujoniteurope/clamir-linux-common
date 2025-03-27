#include "drivers/scc_core.h"
#include "utils/config_file.h"
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#include <thread>

nit_scc_core_state_t nit_scc_core_driver;

typedef struct scc_core_private_state_struct
{

    volatile int* framebuffer_metadata_shm;
    volatile int16_t* framebuffer_shm;

    volatile uint32_t* ctrl_shm;
    volatile int16_t* scale_shm;
    volatile int16_t* offset_shm;

    volatile int16_t* min_shm;
    volatile int16_t* max_shm;


} scc_core_private_state_t;

void scc_core_cleanup_stub(nit_scc_core_state_t* state)
{
    scc_core_private_state_t* priv = NULL;
    priv = (scc_core_private_state_t*) state->priv;

    if (priv->ctrl_shm != NULL) {
        free((void*) priv->ctrl_shm);
    }

    if (priv->offset_shm != NULL) {
        free((void*) priv->offset_shm);
    }

    if (priv->scale_shm != NULL) {
        free((void*) priv->scale_shm);
    }

    if (priv->max_shm != NULL) {
        free((void*) priv->max_shm);
    }

    if (priv->min_shm != NULL) {
        free((void*) priv->min_shm);
    }

}

void nit_scc_core_cleanup(nit_scc_core_state_t* state)
{
    scc_core_private_state_t* priv = NULL;

    if (state == NULL) {
        return;
    }

    if (state->priv == NULL)
    {
        return;
    }

    if (NIT_CLAMIR_HOST_MOCKUP) {
        scc_core_cleanup_stub(state);
    }

    if (priv->ctrl_shm != NULL) {
        munmap((void*) priv->ctrl_shm, NIT_SCC_CORE_CTRL_BASE_SIZE);
    }

    if (priv->offset_shm != NULL) {
        munmap((void*) priv->offset_shm, NIT_SCC_CORE_OFFSET_BASE_SIZE);
    }

    if (priv->scale_shm != NULL) {
        munmap((void*) priv->scale_shm, NIT_SCC_CORE_SCALE_BASE_SIZE);
    }

    if (priv->max_shm != NULL) {
        munmap((void*) priv->max_shm, NIT_SCC_CORE_MAX_BASE_SIZE);
    }

    if (priv->min_shm != NULL) {
        munmap((void*) priv->min_shm, NIT_SCC_CORE_MIN_BASE_SIZE);
    }

    return;
}

int nit_scc_core_open(nit_scc_core_state_t* state, nit_framebuffer_core_state_t* nit_framebuffer_core_state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open == true)
    {
        return 0;
    }

    if (nit_framebuffer_core_state_assert(nit_framebuffer_core_state) < 0) {
        return -1;
    }

    state->is_open = false;

    state->priv = (scc_core_private_state_t*) malloc(sizeof(scc_core_private_state_struct));

    if (state->priv == nullptr) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*) state->priv;

    priv->framebuffer_shm = (int16_t*) nit_framebuffer_core_get_memory_map(nit_framebuffer_core_state);

    if (!NIT_CLAMIR_HOST_MOCKUP) {
        
        state->fd = open("/dev/mem", O_RDWR | O_SYNC);

        if (state->fd < 0)
        {
            nit_scc_core_cleanup(state);
            return -1;
        }
    }

    state->fd = -1;

    priv->ctrl_shm = (volatile uint32_t*) mmap(NULL, NIT_SCC_CORE_CTRL_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_SCC_CORE_CTRL_BASE_ADDRESS);

    if (priv->ctrl_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    priv->offset_shm = (volatile int16_t*)mmap(NULL, NIT_SCC_CORE_OFFSET_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_SCC_CORE_OFFSET_BASE_ADDRESS);

    if (priv->offset_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    priv->scale_shm = (volatile int16_t*)mmap(NULL, NIT_SCC_CORE_SCALE_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_SCC_CORE_SCALE_BASE_ADDRESS);

    if (priv->scale_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    priv->max_shm = (volatile int16_t*)mmap(NULL, NIT_SCC_CORE_MAX_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_SCC_CORE_MAX_BASE_ADDRESS);    

    if (priv->max_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }
    
    priv->min_shm = (volatile int16_t*)mmap(NULL, NIT_SCC_CORE_MIN_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_SCC_CORE_MIN_BASE_ADDRESS);

    if (priv->min_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    if (nit_framebuffer_core_state_assert(nit_framebuffer_core_state) < 0)
    {
        nit_scc_core_cleanup(state);
        return -1;
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

    auto priv = (scc_core_private_state_t*) state->priv;

    if (priv->framebuffer_shm != NULL) {
        free((void*)priv->framebuffer_shm);
    }

    if (state->priv != NULL)
    {
        free((void*)state->priv);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    state->is_open = false;
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

    return retval;
}

int nit_scc_core_stub_eval_calibrate_acquire_max(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }
    return retval;
}

int nit_scc_core_stub_eval_calibrate_update(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;

    int mean_min = 0;

    /**
     * get average of minimum
     */
    for (size_t i = 0; i < state->config.height; i++) {
        for (size_t j = 0; j < state->config.width; j++) {
            auto idx = i * state->config.width + j;
            mean_min += priv->min_shm[idx];
        }
    }

    mean_min /= state->config.height * state->config.width;

    int mean_max = 0;

    /**
     * get average of maximum
     */
    for (size_t i = 0; i < state->config.height; i++) {
        for (size_t j = 0; j < state->config.width; j++) {
            auto idx = i * state->config.width + j;
            mean_max += priv->min_shm[idx];
        }
    }
    
    mean_max /= state->config.height * state->config.width;

    /**
     * compute scale and offset
     */
    for (size_t i = 0; i < state->config.height; i++) {
        for (size_t j = 0; j < state->config.width; j++) {
            auto idx = i * state->config.width + j;

            auto x0 = priv->min_shm[idx];
            auto y0 = mean_min;
            auto x1 = priv->max_shm[idx];
            auto y1 = mean_max;

            auto a = (y0 - y1) / (x0 - x1);
            auto b = (x0*y1 - x1*y0) / x0 - x1;

            priv->scale_shm[idx] = a;
            priv->offset_shm[idx] = b;
        }
    }

    return retval;
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

    unsafe_set<typeof(*state), uint32_t>(state, nit_scc_core_calibration_status_offset, NIT_SCC_CORE_CALIBRATION_STATUS_BUSY);

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

    retval = nit_scc_core_stub_eval_calibrate_update(state);

    if (retval == 0) {
        unsafe_set<typeof(*state), uint32_t>(state, nit_scc_core_calibration_status_offset, NIT_SCC_CORE_CALIBRATION_STATUS_IDLE);
    }

    return retval;

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
    return retval;
}

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

int nit_scc_core_calibration_status_wait_idle(nit_scc_core_state_t* state)
{
    auto result = NIT_SCC_CORE_CALIBRATION_STATUS_BUSY;
    auto start = std::chrono::high_resolution_clock::now();
    int retval = -1;

    while(result == NIT_SCC_CORE_CALIBRATION_STATUS_BUSY && (std::chrono::high_resolution_clock::now() - start) < std::chrono::milliseconds(100)) {

        int retval = unsafe_get<nit_scc_core_state_t, uint32_t>(&nit_scc_core_driver, nit_scc_core_calibration_status_offset);

        if (retval < 0) {
            return retval;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (result == NIT_SCC_CORE_CALIBRATION_STATUS_IDLE) {
        retval = 0;
    }

    return retval;
}