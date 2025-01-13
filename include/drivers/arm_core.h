#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "common.h"

#define NIT_ARM_CORE_BASE_ADDRESS   (0x44A40000)
#define NIT_ARM_CORE_SIZE           0x00010000

#define NIT_ARM_CORE_FIELD_TABLE(_) \
_(arm_core,led_r,uint32_t,sizeof(uint32_t),0x0000+0) \
_(arm_core,led_g,uint32_t,sizeof(uint32_t),0x0000+1) \
_(arm_core,led_b,uint32_t,sizeof(uint32_t),0x0000+2) \

DRIVER_DECLARE_OFFSET_TABLE_BEGIN(arm_core)
NIT_ARM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
DRIVER_DECLARE_OFFSET_TABLE_END(arm_core)

DRIVER_DECLARE_CONFIG_BEGIN(arm_core)
NIT_ARM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)
DRIVER_DECLARE_CONFIG_END(arm_core)

DRIVER_DECLARE_STATE(arm_core)

uint32_t nit_arm_core_open(arm_core_state_t* state);
uint32_t nit_arm_core_close(arm_core_state_t* state);

int nit_arm_core_config_save_to_file(arm_core_state_t* state, const char *path);
int nit_arm_core_config_load_from_file(arm_core_state_t* state, const char *path);

void nit_arm_core_led_r_set(arm_core_state_t* state, uint32_t value);
uint32_t nit_arm_core_led_r_get(arm_core_state_t* state);

void nit_arm_core_led_g_set(arm_core_state_t* state, uint32_t value);
uint32_t nit_arm_core_led_g_get(arm_core_state_t* state);

void nit_arm_core_led_b_set(arm_core_state_t* state, uint32_t value);
uint32_t nit_arm_core_led_b_get(arm_core_state_t* state);

extern arm_core_state_t nit_arm_core_driver;

#ifdef __cplusplus
}
#endif

#endif