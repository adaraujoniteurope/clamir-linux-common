#include <nit/embedded/drivers/framebuffer_core.h>
#include <nit/embedded/utils/config_file.h>
#include <nit/embedded/utils/memory_map.hpp>

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

const nit_framebuffer_core_config_t framebuffer_core_config_default = {
    // TBD
};

nit_framebuffer_core_state_t nit_framebuffer_core_driver;

using namespace utils;

int nit_framebuffer_core_open(nit_framebuffer_core_state_t* state)
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

    state->fd = open("/dev/mem", O_RDWR | O_SYNC);


    state->priv = (volatile int*)memory_map_open(NULL, NIT_FRAMEBUFFER_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_FRAMEBUFFER_CORE_BASE_ADDRESS);
    
    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}

int nit_framebuffer_core_close(nit_framebuffer_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    state->is_open = false;
    if (state->priv != NULL) {
        memory_map_close((void*)state->priv, NIT_FRAMEBUFFER_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_framebuffer_core_config_save_to_file(nit_framebuffer_core_state_t* state, const char* path)
{
    return 0;
}

int nit_framebuffer_core_config_load_from_file(nit_framebuffer_core_state_t* state, const char* path)
{
    return 0;
}

volatile uint16_t* nit_framebuffer_core_get_memory_map(nit_framebuffer_core_state_t* state)
{
    return (volatile uint16_t*)state->priv;
}

int nit_framebuffer_core_state_assert(nit_framebuffer_core_state_t* state)
{

    if (state == NULL) {
        return -1;
    }

    if (state->priv == NULL) {
        return -2;
    }

    if (state->is_open == false) {
        return -3;
    }

    return 0;
}

uint8_t* nit_framebuffer_core_metadata(nit_framebuffer_core_state_t* state)
{

    if (nit_framebuffer_core_state_assert(state) < 0) {
        return nullptr;
    }

    return ((uint8_t*)state->priv) + 8192;
}