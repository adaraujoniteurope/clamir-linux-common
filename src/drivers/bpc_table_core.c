#include "drivers/bpc_table_core.h"
#include "utils/config_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

bpc_table_core_state_t nit_bpc_table_core_driver;

const bpc_table_core_config_t bpc_table_core_config_default = {

};

int bpc_table_core_open(bpc_table_core_state_t* state)
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

	state->priv = (volatile int *)mmap(NULL, NIT_BPCC_TABLE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_BPCC_TABLE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    return 0;
}

int bpc_table_core_close(bpc_table_core_state_t* state)
{
	close(state->fd);
	munmap((int *)state->priv, NIT_BPCC_TABLE_SIZE);
	state->priv = NULL;
}

int bpc_table_core_config_save_to_file(bpc_table_core_state_t* state, const char *path)
{
	return 0;
}

int bpc_table_core_config_load_from_file(bpc_table_core_state_t* state, const char *path)
{
	return 0;
}