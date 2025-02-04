#ifndef _FRAMEBUFFER_CORE_H_
#define _FRAMEBUFFER_CORE_H_

#include <stdint.h>
#include <stdbool.h>

#if !DEBUGGING_HOST
#define NIT_FRAMEBUFFER_CORE_BASE_ADDRESS               0x42000000
#define NIT_FRAMEBUFFER_CORE_SIZE                       0x00010000
#else
#define NIT_FRAMEBUFFER_CORE_BASE_ADDRESS               0x00000000
#define NIT_FRAMEBUFFER_CORE_SIZE                       0x00010000
#endif

typedef struct nit_framebuffer_core_config_struct
{
} nit_framebuffer_core_config_t;

extern const nit_framebuffer_core_config_t framebuffer_core_config_default;

typedef struct nit_framebuffer_core_state_struct
{
    int fd;
    volatile void* priv;
    bool is_open;
}nit_framebuffer_core_state_t;

extern nit_framebuffer_core_state_t nit_framebuffer_core_driver;

int nit_framebuffer_core_open(nit_framebuffer_core_state_t* state);
int nit_framebuffer_core_close(nit_framebuffer_core_state_t* state);

int nit_framebuffer_core_config_save_to_file(nit_framebuffer_core_state_t* state, const char *path);
int nit_framebuffer_core_config_load_from_file(nit_framebuffer_core_state_t* state, const char *path);

int nit_framebuffer_core_state_assert(nit_framebuffer_core_state_t* state);
uint8_t* nit_framebuffer_core_metadata(nit_framebuffer_core_state_t* state);

volatile uint16_t * nit_framebuffer_core_get_memory_map(nit_framebuffer_core_state_t* state);

#endif