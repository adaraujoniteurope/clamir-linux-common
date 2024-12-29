#ifndef _FRAMEBUFFER_METADATA_CORE_H_
#define _FRAMEBUFFER_METADATA_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NIT_FRAMEBUFFER_METADATA_CORE_BASE_ADDRESS  0x42002000
#define NIT_FRAMEBUFFER_METADATA_CORE_SIZE          0x00000040

typedef struct framebuffer_metadata_core_state_struct
{
    int fd;
    volatile void* priv;
}framebuffer_metadata_core_state_t;


typedef struct __attribute__((packed)) metadata_struct
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
} metadata_t;

int framebuffer_metadata_core_open(framebuffer_metadata_core_state_t* state);
int framebuffer_metadata_core_close(framebuffer_metadata_core_state_t* state);
volatile int* framebuffer_metadata_core_memory_map_get(framebuffer_metadata_core_state_t* state);

#ifdef __cplusplus
}
#endif

#endif