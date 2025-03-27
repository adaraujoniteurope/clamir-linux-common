#include <nit/embedded/drivers/bpc_table_core.h>
#include <nit/embedded/utils/config_file.h>

#include <nit/embedded/drivers/control_unit_core.h>
#include <nit/embedded/utils/config_file.h>
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include <string.h>
#include <stdbool.h>

nit_bpc_table_core_state_t nit_bpc_table_core_driver = {
    .fd = -1,
    .priv = NULL,
    .is_open = false
};

const nit_bpc_table_core_config_t bpc_table_core_config_default = {

};

int nit_bpc_table_core_open(nit_bpc_table_core_state_t* state)
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

    state->is_open = false;

    if (state->fd < 0)
    {
        return -2;
    }

    state->priv = (volatile int *)mmap(NULL, NIT_BPCC_TABLE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_BPCC_TABLE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}

int nit_bpc_table_core_close(nit_bpc_table_core_state_t* state)
{

    if (state == NULL)
    {
        return 0;
    }

    if (state->priv == NULL)
    {
        return 0;
    }

    if (state->is_open == false)
    {
        return 0;
    }

	close(state->fd);

	munmap((int *)state->priv, NIT_BPCC_TABLE_SIZE);

	state->priv = NULL;

    return 0;
}

int nit_bpc_table_core_assert(nit_bpc_table_core_state_t *state)
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

int nit_bpc_table_core_config_save_to_file(nit_bpc_table_core_state_t* state, const char *path)
{
	return 0;
}

int nit_bpc_table_core_config_load_from_file(nit_bpc_table_core_state_t* state, const char *path)
{
	return 0;
}

int nit_bpc_table_core_bpc_table_write(nit_bpc_table_core_state_t* state, const unsigned char* buffer, size_t size)
{
    print_debug("%s\n", __func__);

    if (nit_bpc_table_core_assert(state) < 0) {
        return -1;
    }

    memcpy((void*)state->priv, (void*) buffer, size);

    return 0;
}

int nit_bpc_table_core_bpc_table_read(nit_bpc_table_core_state_t* state, const unsigned char* buffer, size_t size)
{
    return 0;
}