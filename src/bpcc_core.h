#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_BPCC_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_BPCC_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NIT_BPCC_TABLE_BASE_ADDRESS 0x44000000
#define NIT_BPCC_TABLE_SIZE         0x00001000

#ifndef BPCC_TABLE_COEFFICIENTS_TABLE_FILE_PATH
#define BPCC_TABLE_COEFFICIENTS_TABLE_FILE_PATH "/etc/clamir/bpcc_table.conf"
#endif

typedef struct bpcc_table_core_state_struct
{
    int fd;
    volatile void * priv;
} bpcc_table_core_state_t;

int bpcc_table_core_open(bpcc_table_core_state_t* state);
int bpcc_table_core_close(bpcc_table_core_state_t* state);

int bpcc_table_core_load_coefficients_from_file(bpcc_table_core_state_t* state, const char *path);

#ifdef __cplusplus
}
#endif

#endif