#ifndef _FRAMEBUFFER_CORE_H_
#define _FRAMEBUFFER_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NIT_FRAMEBUFFER_CORE_BASE_ADDRESS               0x42000000
#define NIT_FRAMEBUFFER_CORE_SIZE                       0x00002000

#define NIT_FRAMEBUFFER_METADATA_CORE_BRAM_BASE_ADDRESS 0x42002000

typedef struct framebuffer_core_state_struct
{
    int fd;
    volatile void* priv;
}framebuffer_core_state_t;

int framebuffer_core_open(framebuffer_core_state_t* state);
int framebuffer_core_close(framebuffer_core_state_t* state);
volatile uint16_t * framebuffer_get_memory_map(framebuffer_core_state_t* state);

#ifdef __cplusplus
}
#endif

#endif