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

    volatile int32_t* scale_shm;
    volatile int32_t* offset_shm;

    int frame_scale_default_fd;
    volatile int32_t* frame_scale_default;

    int frame_offset_default_fd;
    volatile int32_t* frame_offset_default;

    volatile int16_t* frame_min;
    volatile int16_t* frame_max;


} scc_core_private_state_t;

int nit_scc_core_assert(nit_scc_core_state_t* state);

void scc_core_cleanup_stub(nit_scc_core_state_t* state)
{
    scc_core_private_state_t* priv = NULL;
    priv = (scc_core_private_state_t*)state->priv;

    if (priv->ctrl_shm != NULL) {
        free((void*)priv->ctrl_shm);
    }

    if (priv->ctrl_stub_shm != NULL) {
        free((void*)priv->ctrl_stub_shm);
    }

    if (priv->offset_shm != NULL) {
        free((void*)priv->offset_shm);
    }

    if (priv->scale_shm != NULL) {
        free((void*)priv->scale_shm);
    }

    if (priv->frame_max != NULL) {
        free((void*)priv->frame_max);
    }

    if (priv->frame_min != NULL) {
        free((void*)priv->frame_min);
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
        memory_map_close((void*)priv->ctrl_shm, NIT_SCC_CORE_CTRL_BASE_SIZE);
    }

    if (priv->ctrl_stub_shm != NULL) {
        memory_map_close((void*)priv->ctrl_stub_shm, NIT_SCC_CORE_CTRL_BASE_SIZE);
    }

    if (priv->offset_shm != NULL) {
        memory_map_close((void*)priv->offset_shm, NIT_SCC_CORE_OFFSET_BASE_SIZE);
    }

    if (priv->scale_shm != NULL) {
        memory_map_close((void*)priv->scale_shm, NIT_SCC_CORE_SCALE_BASE_SIZE);
    }

    if (priv->frame_max != NULL) {
        memory_map_close((void*)priv->frame_max, NIT_SCC_CORE_MAX_BASE_SIZE);
    }

    if (priv->frame_min != NULL) {
        memory_map_close((void*)priv->frame_min, NIT_SCC_CORE_MIN_BASE_SIZE);
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

    state->priv = (scc_core_private_state_t*)malloc(sizeof(scc_core_private_state_t));
    memset((void*)state->priv, 0, sizeof(scc_core_private_state_t));

    if (state->priv == nullptr) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;

    priv->framebuffer_shm = (int16_t*)nit_framebuffer_core_get_memory_map(nit_framebuffer_core_state);

    priv->dev_fd = -1;

    state->config.width = 64;
    state->config.height = 64;

    priv->dev_fd = open("/dev/mem", O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    if (NIT_SCC_CORE_CTRL_BASE_SIZE < sizeof(nit_scc_core_config_t)) {
        return -1;
    }

    priv->ctrl_shm = (volatile uint32_t*)memory_map_open(NULL, NIT_SCC_CORE_CTRL_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_CTRL_BASE_ADDRESS);

    if (priv->ctrl_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    /**
     * startup core
     */
    priv->ctrl_stub_shm = (volatile uint32_t*)memory_map_open(NULL, NIT_SCC_CORE_CTRL_STUB_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_CTRL_STUB_BASE_ADDRESS);

    if (priv->ctrl_stub_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    priv->offset_shm = (volatile int32_t*)memory_map_open(NULL, NIT_SCC_CORE_OFFSET_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_OFFSET_BASE_ADDRESS);

    if (priv->offset_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    priv->scale_shm = (volatile int32_t*)memory_map_open(NULL, NIT_SCC_CORE_SCALE_BASE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, priv->dev_fd, NIT_SCC_CORE_SCALE_BASE_ADDRESS);

    if (priv->scale_shm == NULL) {
        nit_scc_core_cleanup(state);
        return -1;
    }

    size_t frame_size = sizeof(int16_t) * state->config.width * state->config.height;

    priv->frame_max = (decltype(priv->frame_max))malloc(frame_size);
    priv->frame_min = (decltype(priv->frame_min))malloc(frame_size);

    priv->frame_scale_default = (decltype(priv->frame_scale_default))malloc(frame_size);
    priv->frame_offset_default = (decltype(priv->frame_offset_default))malloc(frame_size);

    if (state->config.frame_scale_default_file_path.empty())
    {
        state->config.frame_scale_default_file_path = "scc_core_scale_default.dat";
    }



    size_t scale_size = sizeof(int32_t) * state->config.width * state->config.height;

    if (std::filesystem::exists(state->config.frame_scale_default_file_path))
    {
        if (std::filesystem::file_size(state->config.frame_scale_default_file_path) != scale_size)
        {
            std::filesystem::remove(state->config.frame_scale_default_file_path);
        } else {
            return 0;
        }
    }

    // scc_core_scale_default_initialize(state);
    {
        auto fd = ::open(state->config.frame_scale_default_file_path.c_str(), O_CREAT | O_RDWR);

        if (fd < 0) {
            print_debug("Failed to open default scale path at %s", state->config.frame_scale_default_file_path.c_str());
            return -1;
        }
    
        for (size_t rows = 0; rows < state->config.height; rows++) {
            for (size_t cols = 0; cols < state->config.width; cols++) {
                ::write(fd, &state->config.frame_pixel_scale_default_value, sizeof(int32_t));
            }
        }
    
        ::close(fd);
    }

    // scc_core_scale_default_load(state);
    {
        auto fd = ::open(state->config.frame_scale_default_file_path.c_str(), O_CREAT | O_RDWR);

        if (fd < 0) {
            print_debug("Failed to open default scale path at %s", state->config.frame_scale_default_file_path.c_str());
            return -1;
        }

        for (size_t rows = 0; rows < state->config.height; rows++) {
            for (size_t cols = 0; cols < state->config.width; cols++) {
                ::read(fd, &priv->frame_scale_default, sizeof(int32_t));
            }
        }
    
        ::close(fd);
    }

    size_t offset_size = sizeof(int32_t) * state->config.width * state->config.height;

    if (std::filesystem::exists(state->config.frame_offset_default_file_path))
    {
        if (std::filesystem::file_size(state->config.frame_offset_default_file_path) != offset_size)
        {
            std::filesystem::remove(state->config.frame_offset_default_file_path);
        } else {
            return 0;
        }
    }

    // scc_core_offset_default_initialize(state);
    {
        auto fd = ::open(state->config.frame_offset_default_file_path.c_str(), O_CREAT | O_RDWR);

        if (fd < 0) {
            print_debug("Failed to open default offset path at %s", state->config.frame_offset_default_file_path.c_str());
            return -1;
        }
    
        for (size_t rows = 0; rows < state->config.height; rows++) {
            for (size_t cols = 0; cols < state->config.width; cols++) {
                ::write(fd, &state->config.frame_pixel_offset_default_value, sizeof(int32_t));
            }
        }
    
        ::close(fd);
    }

    // scc_core_offset_default_load(state);
    {
        auto fd = ::open(state->config.frame_offset_default_file_path.c_str(), O_CREAT | O_RDWR);

        if (fd < 0) {
            print_debug("Failed to open default offset path at %s", state->config.frame_offset_default_file_path.c_str());
            return -1;
        }

        for (size_t rows = 0; rows < state->config.height; rows++) {
            for (size_t cols = 0; cols < state->config.width; cols++) {
                ::read(fd, &priv->frame_offset_default, sizeof(int32_t));
            }
        }
    
        ::close(fd);
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

    auto priv = (scc_core_private_state_t*)state->priv;

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

    auto priv = (scc_core_private_state_t*)state->priv;

    for (size_t row = 0; row < state->config.height; row++)
    {
        for (size_t col = 0; col < state->config.height; col++)
        {
            size_t index = row * state->config.width + col;
            priv->frame_max[index] = 0;
            priv->frame_min[index] = 0;
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

int nit_scc_core_testing_disable_set(nit_scc_core_state_t* state, uint32_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_disable_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_disable_offset) = value;

    return retval;
}

int nit_scc_core_testing_disable_get(nit_scc_core_state_t* state, uint32_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_disable_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_disable_offset, *value);
    return retval;
}

int nit_scc_core_testing_addr_set(nit_scc_core_state_t* state, uint32_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_addr_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_addr_offset) = value;

    return retval;
}

int nit_scc_core_testing_addr_get(nit_scc_core_state_t* state, uint32_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_addr_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_addr_offset, *value);
    return retval;
}

int nit_scc_core_testing_wren_set(nit_scc_core_state_t* state, uint32_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_wren_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_wren_offset) = value;

    return retval;
}

int nit_scc_core_testing_wren_get(nit_scc_core_state_t* state, uint32_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
    *value = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_wren_offset);

    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_wren_offset, *value);
    return retval;
}

int nit_scc_core_testing_data_set(nit_scc_core_state_t* state, uint32_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_scc_core_testing_data_offset, value);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
    *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_testing_data_offset) = value;

    return retval;
}

int nit_scc_core_testing_data_get(nit_scc_core_state_t* state, uint32_t* value)
{
    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
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

    auto priv = (scc_core_private_state_t*)state->priv;
    auto frame = nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);

    memcpy((void*)priv->frame_min, (void*)frame, sizeof(int16_t) * 4096);

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

    auto priv = (scc_core_private_state_t*)state->priv;
    auto frame = nit_framebuffer_core_get_memory_map(&nit_framebuffer_core_driver);

    memcpy((void*)priv->frame_max, (void*)frame, sizeof(int16_t) * 4096);

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

    auto priv = (scc_core_private_state_t*)state->priv;

    double mean_min = 0;

    /**
     * get average of minimum
     */
    for (size_t row = 0; row < state->config.height; row++) {
        for (size_t col = 0; col < state->config.width; col++) {
            size_t index = row * state->config.width + col;
            mean_min = mean_min + (double)priv->frame_min[index];
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
            mean_max += priv->frame_max[index];
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
            double x0 = priv->frame_min[index];
            double y0 = mean_min;

            double x1 = priv->frame_max[index];
            double y1 = mean_max;

            if (x0 == x1) {
                print_debug("Dead pixel detected at row: %lu col: %lu\n", row, col);
                continue;
            }

            double a = (y1 - y0) / (x1 - x0);
            double b = y1 - a * x1;

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

    switch (calibration_mode)
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

int nit_scc_core_stub_eval_process_eval(nit_scc_core_state_t* state, volatile int16_t* target, volatile int16_t* source, volatile int32_t* scale, volatile int32_t* offset)
{
    for (size_t row = 0; row < state->config.height; row++) {
        for (size_t col = 0; col < state->config.width; col++) {
            size_t index = row * state->config.width + col;
            target[index] = ((double)(source[index] * scale[index] + offset[index])) / ((double)(INT16_MAX >> 1));
        }
    }

    return 0;
}

int nit_scc_core_stub_eval_process(nit_scc_core_state_t* state)
{

    int retval = 0;

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

    if (state->config.calibration_bypass)
    {
        nit_scc_core_stub_eval_process_eval(state, priv->framebuffer_shm, priv->framebuffer_shm, priv->frame_scale_default, priv->frame_offset_default);
    }
    else {
        nit_scc_core_stub_eval_process_eval(state, priv->framebuffer_shm, priv->framebuffer_shm, priv->scale_shm, priv->offset_shm);
    }

    return retval;
}

#include <mutex>

std::mutex eval_mutex;

int nit_scc_core_stub_eval(nit_scc_core_state_t* state) {
    std::unique_lock lk(eval_mutex);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    scc_core_private_state_t* priv = (scc_core_private_state_t*)state->priv;
    auto opmode = *((uint32_t*)((uint8_t*)priv->ctrl_shm) + nit_scc_core_opmode_offset);

    switch (opmode) {
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

    while (result != NIT_SCC_CORE_CALIBRATION_STATUS_IDLE && (std::chrono::high_resolution_clock::now() - start) < std::chrono::milliseconds(100)) {

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

int* nit_scc_core_get_scale_memory_map(nit_scc_core_state_t* state)
{
    print_debug("%s\n", __func__);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) < 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return nullptr;
    }

    auto priv = (scc_core_private_state_t*)state->priv;

    return (int*)priv->scale_shm;
}

int* nit_scc_core_get_offset_memory_map(nit_scc_core_state_t* state)
{

    print_debug("%s\n", __func__);

    int retval = 0;

    if ((retval = nit_scc_core_assert(state)) < 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return nullptr;
    }

    auto priv = (scc_core_private_state_t*)state->priv;

    return (int*)priv->offset_shm;

}

int nit_scc_core_calibrate(nit_scc_core_state_t* state)
{

    nit_control_unit_core_offset_en_set(&nit_control_unit_core_driver, 1);
    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 1);

    /**
     * 1. enter calibration
     */
    nit_scc_core_opmode_set(&nit_scc_core_driver, NIT_SCC_CORE_OPMODE_CALIBRATE);

    /**
     * 1. set start acquiring min
     * 2. close shutter
     * 3. wait for 250 ms
     */
    nit_scc_core_calibration_mode_set(&nit_scc_core_driver, NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MIN);

    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 1);

    std::cout << "Acquiring minimum...." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    /**
     * 1. set start acquiring max
     * 2. open shutter
     * 3. wait for 250 ms
     */
    nit_scc_core_calibration_mode_set(&nit_scc_core_driver, NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MAX);

    nit_control_unit_core_shutter_set(&nit_control_unit_core_driver, 0);
    std::cout << "Acquiring maximum...." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    /**
     * 1. disable acquiring max
     * 2. wait for calibration to complete
     * 3. enable scc_core processing
     */
    nit_scc_core_calibration_mode_set(&nit_scc_core_driver, NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_UPDATING);
    nit_scc_core_calibration_mode_wait_idle(&nit_scc_core_driver);

    nit_scc_core_opmode_set(&nit_scc_core_driver, NIT_SCC_CORE_OPMODE_PROCESS);

    /**
     * older offset core processing disable offset update
     */
    nit_control_unit_core_offset_update_set(&nit_control_unit_core_driver, 0);

    return 0;
}