#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#include "drivers/common.h"
#include "drivers/mb_core.h"

mb_core_state_t nit_mb_core_driver;

int mb_core_open(mb_core_state_t *state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_MB_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_MB_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}
int mb_core_close(mb_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL)
    {
        munmap((void *)state->priv, NIT_MB_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}
