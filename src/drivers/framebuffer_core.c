#include "drivers/framebuffer_core.h"
#include "utils/config_file.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

const framebuffer_core_config_t framebuffer_core_config_default = {
    // TBD
};

framebuffer_core_state_t nit_framebuffer_core_driver;

int framebuffer_core_open(framebuffer_core_state_t* state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_FRAMEBUFFER_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_FRAMEBUFFER_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

int framebuffer_core_close(framebuffer_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*) state->priv, NIT_FRAMEBUFFER_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

int framebuffer_core_config_save_to_file(framebuffer_core_state_t* state, const char *path)
{
    return 0;
}

int framebuffer_core_config_load_from_file(framebuffer_core_state_t* state, const char *path)
{
    return 0;
}

volatile uint16_t * framebuffer_core_get_memory_map(framebuffer_core_state_t* state)
{
    return (volatile uint16_t*) state->priv;
}