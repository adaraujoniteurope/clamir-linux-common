#include <nit/embedded/drivers/scc_core.h>
#include <nit/embedded/utils/config_file.h>
#include <nit/embedded/utils/memory_map.hpp>
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <thread>

nit_scc_core_state_t nit_scc_core_driver;

using namespace utils;

typedef struct scc_core_private_state_struct
{
    int dev_fd;

    volatile int* framebuffer_metadata_shm;
    volatile int16_t* framebuffer_shm;

    volatile uint32_t* ctrl_shm;
    volatile uint32_t* ctrl_stub_shm;

    volatile int16_t* scale_shm;
    volatile int16_t* offset_shm;

    int scale_default_fd;
    volatile int16_t* scale_default;

    int offset_default_fd;
    volatile int16_t* offset_default;

    volatile int16_t* min_shm;
    volatile int16_t* max_shm;


} scc_core_private_state_t;

int nit_scc_core_assert(nit_scc_core_state_t* state);

void scc_core_cleanup_stub(nit_scc_core_state_t* state)
{
    scc_core_private_state_t* priv = NULL;
    priv = (scc_core_private_state_t*) state->priv;

    if (priv->ctrl_shm != NULL) {
        free((void*) priv->ctrl_shm);
    }

    if (priv->ctrl_stub_shm != NULL) {
        free((void*) priv->ctrl_stub_shm);
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

    if (priv->ctrl_shm != NULL) {
        memory_map_close((void*) priv->ctrl_shm, NIT_SCC_CORE_CTRL_BASE_SIZE);
    }

    if (priv->ctrl_stub_shm != NULL) {
        memory_map_close((void*) priv->ctrl_stub_shm, NIT_SCC_CORE_CTRL_BASE_SIZE);
    }

    if (priv->offset_shm != NULL) {
        memory_map_close((void*) priv->offset_shm, NIT_SCC_CORE_OFFSET_BASE_SIZE);
    }

    if (priv->scale_shm != NULL) {
        memory_map_close((void*) priv->scale_shm, NIT_SCC_CORE_SCALE_BASE_SIZE);
    }

    if (priv->max_shm != NULL) {
        memory_map_close((void*) priv->max_shm, NIT_SCC_CORE_MAX_BASE_SIZE);
    }

    if (priv->min_shm != NULL) {
        memory_map_close((void*) priv->min_shm, NIT_SCC_CORE_MIN_BASE_SIZE);
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

    state->priv = (scc_core_private_state_t*) malloc(sizeof(scc_core_private_state_t));
    memset((void*)state->priv, 0, sizeof(scc_core_private_state_t));

    if (state->priv == nullptr) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*) state->priv;

    priv->framebuffer_shm = (int16_t*) nit_framebuffer_core_get_memory_map(nit_framebuffer_core_state);

    priv->dev_fd = -1;

    state->config.width = 64;
    state->config.height = 64;

    priv->dev_fd = open("/dev/mem", O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    if (NIT_SCC_CORE_CTRL_BASE_SIZE < sizeof(nit_scc_core_config_t)) {
        return -1;
    }

    priv->ctrl_shm = (volatile uint32_t*) memory_map_open(NULL, NIT_SCC_CORE_CTRL_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_CTRL_BASE_ADDRESS);

    if (priv->ctrl_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    priv->ctrl_stub_shm = (volatile uint32_t*) memory_map_open(NULL, NIT_SCC_CORE_CTRL_STUB_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_CTRL_STUB_BASE_ADDRESS);

    if (priv->ctrl_stub_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

     priv->offset_shm = (volatile int16_t*)memory_map_open(NULL, NIT_SCC_CORE_OFFSET_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_OFFSET_BASE_ADDRESS);

    if (priv->offset_shm == NULL) {
         nit_scc_core_cleanup(state);
         return -1;
    }

    priv->scale_shm = (volatile int16_t*)memory_map_open(NULL, NIT_SCC_CORE_SCALE_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_SCALE_BASE_ADDRESS);

    if (priv->scale_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    if (state->config.scale_default_file_path.empty()) {
        state->config.scale_default_file_path = "scc_core_scale_default.dat";
    }

    priv->scale_default_fd = open(state->config.scale_default_file_path.c_str(), O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    if (priv->scale_default_fd < 0) {
        print_debug("%s: couldn't open scale data file\n", __func__);
        return -1;
    }

    priv->scale_default = (volatile int16_t*)mmap(NULL, NIT_SCC_CORE_SCALE_DEFAULT_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->scale_default_fd, NIT_SCC_CORE_SCALE_DEFAULT_BASE_ADDRESS);

    if (priv->scale_default == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }


    if (std::filesystem::is_empty(state->config.scale_default_file_path)) {
        
        int16_t* buffer = nullptr;
        buffer = (int16_t*) malloc(sizeof(int16_t) * state->config.width * state->config.height);

        if (buffer == nullptr) {
            print_debug("%s: failed to allocate memory for the default scale matrix.", __func__);
            exit(1);
        }

        memset(buffer, 0, state->config.width * state->config.height);

        for (int row = 0; row < state->config.height; row++)
            for (int col = 0; col < state->config.width; col++)
            {
                int index = row * state->config.width + col;
                buffer[index] = INT16_MAX >> 1;
            }
        
        if (write(priv->scale_default_fd, buffer, sizeof(int16_t) * state->config.width * state->config.height) < 0)
        {
            print_debug("%s: failed to write default values to scale matrix.", __func__);
        }

        free(buffer);

        /**
         * check if file war written successfully
         */
        for (int row = 0; row < state->config.height; row++)
            for (int col = 0; col < state->config.width; col++)
            {
                int index = row * state->config.width + col;
                if (priv->scale_default[index] != (INT16_MAX >> 1)) {
                    print_debug("%s: failed to write on scale file default coefficient at row: %d col: %d", __func__, row, col);
                }
            }
    }

    memcpy((void*)priv->scale_shm, (void*) priv->scale_default, state->config.width * state->config.height * sizeof(int16_t));

    if (state->config.offset_default_file_path.empty()) {
        state->config.offset_default_file_path = "scc_core_offset_default.dat";
    }

    priv->offset_default_fd = open(state->config.offset_default_file_path.c_str(), O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    if (priv->offset_default_fd < 0) {
        print_debug("%s: couldn't open offset data file\n", __func__);
        return -1;
    }

    priv->offset_default = (volatile int16_t*)mmap(NULL, NIT_SCC_CORE_OFFSET_DEFAULT_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->offset_default_fd, NIT_SCC_CORE_OFFSET_DEFAULT_BASE_ADDRESS);

    if (priv->offset_default == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    if (std::filesystem::is_empty(state->config.offset_default_file_path)) {

        int16_t* buffer = nullptr;

        buffer = (int16_t*) malloc(sizeof(int16_t) * state->config.width * state->config.height);

        if (buffer == nullptr) {
            print_debug("%s: failed to allocate memory for the default offset matrix.", __func__);
            exit(1);
        }

        memset(buffer, 0, state->config.width * state->config.height);
        
        if (write(priv->offset_default_fd, buffer, sizeof(int16_t) * state->config.width * state->config.height) < 0)
        {
            print_debug("%s: failed to write default values to offset matrix.", __func__);
        }

        free(buffer);

        /**
         * check if file war written successfully
         */
        for (int row = 0; row < state->config.height; row++)
            for (int col = 0; col < state->config.width; col++)
            {
                int index = row * state->config.width + col;
                if (priv->scale_default[index] != 0) {
                    print_debug("%s: failed to write on offset file default coefficient at row: %d col: %d", __func__, row, col);
                }
            }
    }

    memcpy((void*)priv->offset_shm, (void*) priv->offset_default, state->config.width * state->config.height * sizeof(int16_t));

    priv->max_shm = (volatile int16_t*)memory_map_open(NULL, NIT_SCC_CORE_MAX_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, -1, NIT_SCC_CORE_MAX_BASE_ADDRESS);    

    if (priv->max_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    priv->min_shm = (volatile int16_t*)memory_map_open(NULL, NIT_SCC_CORE_MIN_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, -1, NIT_SCC_CORE_MIN_BASE_ADDRESS);

    if (priv->min_shm == NULL) {
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

    if (priv->dev_fd >= 0)
    {
        close(priv->dev_fd);
    }

    state->is_open = false;
    return 0;
}

int nit_scc_core_reset(nit_scc_core_state_t* state)
{

    print_debug("%s\n", __func__);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) < 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;

    for (size_t row = 0; row < state->config.height; row ++)
    {
        for (size_t col = 0; col < state->config.height; col ++)
        {
            size_t index = row * state->config.width + col;
            priv->max_shm[index] = 0;
            priv->min_shm[index] = 0;
            priv->scale_shm[index] = (INT16_MAX >> 1);
            priv->offset_shm[index] = 0;
        }
    }
    return 0;
}

int nit_scc_core_config_save_to_file(nit_scc_core_state_t* state, const char* path)
{

    nit_scc_core_calibration_bypass_get(state, &state->config.calibration_bypass);
    nit_scc_core_width_get(state, &state->config.width);
    nit_scc_core_height_get(state, &state->config.width);
    return config_file_save_to_file(state, path);
}

int nit_scc_core_config_load_from_file(nit_scc_core_state_t* state, const char* path)
{
    auto retval = config_file_load_from_file(state, path);
    nit_scc_core_calibration_bypass_set(state, state->config.calibration_bypass);
    nit_scc_core_width_set(state, state->config.width);
    nit_scc_core_height_set(state, state->config.width);
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

int nit_scc_core_testing_disable_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_disable_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_disable_offset) = value;

    return retval;
}

int nit_scc_core_testing_disable_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_disable_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_disable_offset, *value);
    return retval;
}

int nit_scc_core_testing_addr_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_addr_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_addr_offset) = value;

    return retval;
}

int nit_scc_core_testing_addr_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_addr_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_addr_offset, *value);
    return retval;
}

int nit_scc_core_testing_wren_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_wren_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_wren_offset) = value;

    return retval;
}

int nit_scc_core_testing_wren_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_wren_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_wren_offset, *value);
    return retval;
}

int nit_scc_core_testing_data_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_data_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_data_offset) = value;

    return retval;
}

int nit_scc_core_testing_data_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_data_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_data_offset, *value);
    return retval;
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

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_opmode_offset) = value;

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

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_opmode_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_opmode_offset, *value);
    return retval;
}

int nit_scc_core_process_bypass_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_process_bypass_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_process_bypass_offset, *value);
    return retval;
}

int nit_scc_core_process_bypass_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_process_bypass_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_process_bypass_offset) = value;

    return retval;
}

int nit_scc_core_calibration_bypass_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_calibration_bypass_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_calibration_bypass_offset, *value);
    return retval;
}

int nit_scc_core_calibration_bypass_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_calibration_bypass_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_calibration_bypass_offset) = value;

    return retval;
}

int nit_scc_core_width_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_width_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_width_offset, *value);
    return retval;
}

int nit_scc_core_width_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_width_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_width_offset) = value;

    return retval;
}

int nit_scc_core_height_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_height_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_height_offset, *value);
    return retval;
}

int nit_scc_core_height_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_height_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_height_offset) = value;

    return retval;
}

int nit_scc_core_calibration_mode_set(nit_scc_core_state_t* state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_calibration_mode_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_calibration_mode_offset) = value;

    return retval;
}

int nit_scc_core_calibration_mode_get(nit_scc_core_state_t* state, uint16_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_stub_shm) + nit_scc_core_calibration_mode_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_calibration_mode_offset, *value);
    return retval;
}

int nit_scc_core_stub_eval_calibrate_acquire_min(nit_scc_core_state_t* state) {

    int retval = 0;

    print_debug("nit_scc_core_stub_eval_calibrate_acquire_min\n");

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    auto frame = nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);

    memcpy((void*)priv->min_shm, (void*) frame, sizeof(int16_t) * 4096);

    return retval;
}

int nit_scc_core_stub_eval_calibrate_acquire_max(nit_scc_core_state_t* state) {

    int retval = 0;

    print_debug("nit_scc_core_stub_eval_calibrate_acquire_max\n");

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;
    auto frame = nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);

    memcpy((void*)priv->max_shm, (void*) frame, sizeof(int16_t) * 4096);

    return retval;
}

int nit_scc_core_stub_eval_calibrate_update(nit_scc_core_state_t* state) {

    int retval = 0;

    print_debug("nit_scc_core_stub_eval_calibrate_update\n");

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*) state->priv;

    double mean_min = 0;

    /**
     * get average of minimum
     */
    for (size_t row = 0; row < state->config.height; row++) {
        for (size_t col = 0; col < state->config.width; col++) {
            size_t index = row * state->config.width + col;
            mean_min = mean_min + (double)priv->min_shm[index];
        }
    }

    mean_min /= state->config.height * state->config.width;

    double mean_max = 0;

    /**
     * get average of maximum
     */
    for (size_t row = 0; row < state->config.height; row++) {
        for (size_t col = 0; col < state->config.width; col++) {
            size_t index = row * state->config.width + col;
            mean_max += priv->max_shm[index];
        }
    }
    
    mean_max /= state->config.height * state->config.width;

    if (mean_min == mean_max) {
        print_debug("failed to sample two sparse samples, check shutter!\n");
        nit_scc_core_reset(state);
        return -1;
    }

    /**
     * compute scale and offset
     */
    for (size_t row = 0; row < state->config.height; row++) {
        for (size_t col = 0; col < state->config.width; col++) {
            size_t index = row * state->config.width + col;
            double x0 = priv->min_shm[index];
            double y0 = mean_min;

            double x1 = priv->max_shm[index];
            double y1 = mean_max;

            if (x0 == x1) {
                print_debug("Dead pixel detected at row: %lu col: %lu\n", row, col);
                continue;
            }

            double a = (y1 - y0) / (x1 - x0);
            double b = y1-a*x1;

            priv->scale_shm[index] = a * (INT16_MAX >> 1);
            priv->offset_shm[index] = b * (INT16_MAX >> 1);
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

    auto& calibration_mode = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_calibration_mode_offset);

    print_debug("nit_scc_core_stub_eval_calibrate\n");

    switch(calibration_mode)
    {
        case NIT_SCC_CORE_CALIBRATION_STATUS_IDLE:

        break;

        case NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MIN:

            if ((retval = nit_scc_core_stub_eval_calibrate_acquire_min(state)) < 0) {
                print_debug("%s: %s %d\n", __func__, "failed", retval);
                return retval;
            }

        break;

        case NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MAX:

            if ((retval = nit_scc_core_stub_eval_calibrate_acquire_max(state)) < 0) {
                print_debug("%s: %s %d\n", __func__, "failed", retval);
                return retval;
            }

        break;

        case NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_UPDATING:

            if ((retval = nit_scc_core_stub_eval_calibrate_update(state)) < 0) {
                print_debug("%s: %s %d\n", __func__, "failed", retval);
                return retval;
            }

            calibration_mode = NIT_SCC_CORE_CALIBRATION_STATUS_IDLE;

        break;

        default:
        break;
    }

    return retval;

}

int nit_scc_core_stub_eval_process_eval(nit_scc_core_state_t* state, volatile int16_t* target, volatile int16_t* source, volatile int16_t* scale, volatile int16_t* offset)
{
    for (size_t row = 0; row < state->config.height; row++) {
        for (size_t col = 0; col < state->config.width; col++) {
            size_t index = row * state->config.width + col;
            target[index] = ((double)(source[index] * scale[index] + offset[index]))/((double)(INT16_MAX >> 1));
        }
    }

    return 0;
}

int nit_scc_core_stub_eval_process(nit_scc_core_state_t* state)
{

    int retval = 0;
    static int processing_override = false;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;

    if (priv == nullptr)
    {
        return -1;
    }

    if (processing_override == true) {
        return 0;
    }

    if (state->config.calibration_bypass)
    {
        nit_scc_core_stub_eval_process_eval(state, priv->framebuffer_shm, priv->framebuffer_shm, priv->scale_default, priv->offset_default);
    } else {
        nit_scc_core_stub_eval_process_eval(state, priv->framebuffer_shm, priv->framebuffer_shm, priv->scale_shm, priv->offset_shm);
    }

    return retval;
}

int nit_scc_core_stub_eval(nit_scc_core_state_t* state) {

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;
    auto opmode = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_opmode_offset);

    switch(opmode) {
        case NIT_SCC_CORE_OPMODE_PROCESS:

            if (state->config.process_bypass) {
                break;
            }

            nit_scc_core_stub_eval_process(state);

            break;
        case NIT_SCC_CORE_OPMODE_CALIBRATE:

            
            nit_scc_core_stub_eval_calibrate(state);
            break;
        default:
            break;
    }

    return 0;
}

int nit_scc_core_calibration_mode_wait_idle(nit_scc_core_state_t* state)
{
    uint16_t result = NIT_SCC_CORE_CALIBRATION_STATUS_MAX;
    auto start = std::chrono::high_resolution_clock::now();
    int retval = -1;

    scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;

    print_debug("calibration core started waiting\n");

    nit_scc_core_calibration_mode_get(state, &result);

    while(result != NIT_SCC_CORE_CALIBRATION_STATUS_IDLE && (std::chrono::high_resolution_clock::now() - start) < std::chrono::milliseconds(100)) {

        int retval = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_calibration_mode_offset);

        if (retval < 0) {
            return retval;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (result == NIT_SCC_CORE_CALIBRATION_STATUS_IDLE) {
        retval = 0;
    }

    print_debug("calibration code is idle\n");

    return retval;
}