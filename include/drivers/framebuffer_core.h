#ifndef _FRAMEBUFFER_CORE_H_
#define _FRAMEBUFFER_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NIT_FRAMEBUFFER_CORE_BASE_ADDRESS               0x42000000
#define NIT_FRAMEBUFFER_CORE_SIZE                       0x00010000

typedef struct framebuffer_core_config_struct
{
} framebuffer_core_config_t;

extern const framebuffer_core_config_t framebuffer_core_config_default;

typedef struct framebuffer_core_state_struct
{
    int fd;
    volatile void* priv;
}framebuffer_core_state_t;

extern framebuffer_core_state_t nit_framebuffer_core_driver;

int framebuffer_core_open(framebuffer_core_state_t* state);
int framebuffer_core_close(framebuffer_core_state_t* state);

int framebuffer_core_config_save_to_file(framebuffer_core_state_t* state, const char *path);
int framebuffer_core_config_load_from_file(framebuffer_core_state_t* state, const char *path);

volatile uint16_t * framebuffer_core_get_memory_map(framebuffer_core_state_t* state);

#ifdef __cplusplus
}
#endif

#endif