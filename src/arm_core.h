#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NIT_ARM_CORE_BASE_ADDRESS   0x44A40000
#define NIT_ARM_CORE_SIZE           0x00020000

#define LED_R (0x4000 + 0)
#define LED_B (0x4000 + 1)
#define LED_G (0x4000 + 2)

typedef enum arm_core_register_offset_enum
{
    NIT_ARM_CORE_LED_R_OFFSET = LED_R,
    NIT_ARM_CORE_LED_G_OFFSET = LED_G,
    NIT_ARM_CORE_LED_B_OFFSET = LED_B,
    ARM_CORE_REGISTER_OFFSET_MAX
} arm_core_register_offset;

typedef struct arm_core_state_struct
{
    int fd;
    volatile void * priv;
} arm_core_state_t;

uint32_t arm_core_open(arm_core_state_t* state);
uint32_t arm_core_close(arm_core_state_t* state);

void arm_core_led_r_set(arm_core_state_t* state, uint32_t value);
uint32_t arm_core_led_r_get(arm_core_state_t* state, uint32_t value);

void arm_core_led_g_set(arm_core_state_t* state, uint32_t value);
uint32_t arm_core_led_g_get(arm_core_state_t* state, uint32_t value);

void arm_core_led_b_set(arm_core_state_t* state, uint32_t value);
uint32_t arm_core_led_b_get(arm_core_state_t* state, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif