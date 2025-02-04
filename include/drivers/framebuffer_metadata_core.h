#ifndef _FRAMEBUFFER_METADATA_CORE_H_
#define _FRAMEBUFFER_METADATA_CORE_H_

#include <stdint.h>
#include <stdbool.h>

#if !DEBUGGING_HOST
#define NIT_FRAMEBUFFER_METADATA_CORE_BASE_ADDRESS  (0x42000000 + 0x2000)
#define NIT_FRAMEBUFFER_METADATA_CORE_SIZE          0x00010000
#else
#define NIT_FRAMEBUFFER_METADATA_CORE_BASE_ADDRESS  0x00000000
#define NIT_FRAMEBUFFER_METADATA_CORE_SIZE          0x00010000
#endif

typedef struct framebuffer_metadata_config_struct
{
} framebuffer_metadata_config_t;

extern const framebuffer_metadata_config_t framebuffer_metadata_config_default;

typedef struct framebuffer_metadata_core_state_struct
{
    int fd;
    volatile void* priv;
	bool is_open;
}framebuffer_metadata_core_state_t;

extern framebuffer_metadata_core_state_t nit_framebuffer_metadata_core_driver;

typedef struct __attribute__((packed)) framebuffer_metadata_struct
{
	int power;
	int M00;
	int M01;
	int M10;
	int M11;
	int M02;
	int M20;
	int width;
	int track_number;
	int frame_max;
	int frame_number;
	int timestamp;
	int io_status;
	int t1;
	int t2;
} framebuffer_metadata_t;

int framebuffer_metadata_core_open(framebuffer_metadata_core_state_t* state);
int framebuffer_metadata_core_close(framebuffer_metadata_core_state_t* state);

int framebuffer_metadata_core_config_save_to_file(framebuffer_metadata_core_state_t* state, const char *path);
int framebuffer_metadata_core_config_load_from_file(framebuffer_metadata_core_state_t* state, const char *path);

volatile int* framebuffer_metadata_core_memory_map_get(framebuffer_metadata_core_state_t* state);

#endif