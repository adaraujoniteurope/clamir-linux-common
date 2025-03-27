#include <nit/embedded/drivers/framebuffer_metadata_core.h>
#include <nit/embedded/utils/config_file.h>
#include <nit/embedded/utils/memory_map.hpp>

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

framebuffer_metadata_core_state_t nit_framebuffer_metadata_core_driver;

const framebuffer_metadata_config_t framebuffer_metadata_config_default = {
    
};

using namespace utils;

int framebuffer_metadata_core_open(framebuffer_metadata_core_state_t* state)
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



    state->priv = (volatile int *)memory_map_open(NULL, NIT_FRAMEBUFFER_METADATA_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_FRAMEBUFFER_METADATA_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}

int framebuffer_metadata_core_close(framebuffer_metadata_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    state->is_open = false;

    if (state->priv != NULL) {
        memory_map_close((void*)state->priv, NIT_FRAMEBUFFER_METADATA_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

int framebuffer_metadata_core_config_save_to_file(framebuffer_metadata_core_state_t* state, const char *path)
{
    return 0;
}

int framebuffer_metadata_core_config_load_from_file(framebuffer_metadata_core_state_t* state, const char *path)
{
    return 0;
}

volatile int* framebuffer_metadata_core_memory_map_get(framebuffer_metadata_core_state_t* state)
{
    
    if (state == NULL) {
        return NULL;
    }

    return (volatile int*) state->priv;
}