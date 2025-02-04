#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_BPCC_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_BPCC_CORE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define NIT_BPCC_TABLE_BASE_ADDRESS 0x44000000
#define NIT_BPCC_TABLE_SIZE         0x00002000

#ifndef BPCC_TABLE_COEFFICIENTS_TABLE_FILE_PATH
#define BPCC_TABLE_COEFFICIENTS_TABLE_FILE_PATH "/etc/clamir/bpc_table.conf"
#endif

#define BPCC_TABLE_WIDTH 128
#define BPCC_TABLE_HEIGHT 128

typedef struct nit_bpc_table_core_config_struct
{
    int size;
    int table[BPCC_TABLE_WIDTH][BPCC_TABLE_HEIGHT];
} nit_bpc_table_core_config_t;

extern const nit_bpc_table_core_config_t bpc_table_core_config_default;

typedef struct nit_bpc_table_core_state_struct
{
    int fd;
    volatile void * priv;
    bool is_open;
} nit_bpc_table_core_state_t;

int nit_bpc_table_core_open(nit_bpc_table_core_state_t* state);
int nit_bpc_table_core_close(nit_bpc_table_core_state_t* state);

int nit_bpc_table_core_config_save_to_file(nit_bpc_table_core_state_t* state, const char *path);
int nit_bpc_table_core_config_load_from_file(nit_bpc_table_core_state_t* state, const char *path);

int nit_bpc_table_core_bpc_table_write(nit_bpc_table_core_state_t* state, const unsigned char* buffer, size_t size);
int nit_bpc_table_core_bpc_table_read(nit_bpc_table_core_state_t* state, const unsigned char* buffer, size_t size);

extern nit_bpc_table_core_state_t nit_bpc_table_core_driver;

#endif