#include "bpcc_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

int bpcc_table_dev_index_set(bpcc_table_core_state_t* state, uint16_t index, uint16_t value);

int bpcc_table_core_open(bpcc_table_core_state_t* state)
{

	state->fd = open("/dev/mem", O_RDWR | O_SYNC);

	if (state->fd < 0)
	{
		return -1;
	}

	state->priv = (volatile int *)mmap(NULL, NIT_BPCC_TABLE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_BPCC_TABLE_BASE_ADDRESS);

	if (state->priv == NULL)
	{
		close(state->fd);
		return 0;
	}
}

int bpcc_table_core_close(bpcc_table_core_state_t* state)
{
	close(state->fd);
	munmap((int *)state->priv, NIT_BPCC_TABLE_SIZE);
	state->priv = NULL;
}

int bpcc_table_core_load_coefficients_from_file(bpcc_table_core_state_t* state, const char *path)
{

	FILE *fd = fopen(path, "r");
	if (fd == NULL)
	{
		return -1;
	}

	int bpcc_y, bpcc_x;

	int index = 0;
	
	while (fscanf(fd, "%d %d", &bpcc_y, &bpcc_x) == 2)
	{
		bpcc_table_dev_index_set(state, index, (bpcc_y << 8) | (bpcc_x));
		index++;
	}

	fclose(fd);

	return 1;
}

int bpcc_table_dev_index_set(bpcc_table_core_state_t* state, uint16_t index, uint16_t value)
{

	if (index > NIT_BPCC_TABLE_SIZE / sizeof(value)) 
	{
		return -1;
	}

	((volatile uint16_t*) state->priv)[index] = value;
	
	return 0;
}