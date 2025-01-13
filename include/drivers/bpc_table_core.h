#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_BPCC_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_BPCC_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NIT_BPCC_TABLE_BASE_ADDRESS 0x44000000
#define NIT_BPCC_TABLE_SIZE         0x00001000

#ifndef BPCC_TABLE_COEFFICIENTS_TABLE_FILE_PATH
#define BPCC_TABLE_COEFFICIENTS_TABLE_FILE_PATH "/etc/clamir/bpc_table.conf"
#endif

#define BPCC_TABLE_WIDTH 128
#define BPCC_TABLE_HEIGHT 128

typedef struct bpc_table_core_config_struct
{
    int size;
    int table[BPCC_TABLE_WIDTH][BPCC_TABLE_HEIGHT];
} bpc_table_core_config_t;

extern const bpc_table_core_config_t bpc_table_core_config_default;

typedef struct bpc_table_core_state_struct
{
    int fd;
    volatile void * priv;
} bpc_table_core_state_t;

int bpc_table_core_open(bpc_table_core_state_t* state);
int bpc_table_core_close(bpc_table_core_state_t* state);

int bpc_table_core_config_save_to_file(bpc_table_core_state_t* state, const char *path);
int bpc_table_core_config_load_from_file(bpc_table_core_state_t* state, const char *path);

extern bpc_table_core_state_t nit_bpc_table_core_driver;

#ifdef __cplusplus
}
#endif

#endif