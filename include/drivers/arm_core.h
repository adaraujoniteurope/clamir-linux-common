#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_

#include <stdint.h>
#include "common.h"

#define NIT_ARM_CORE_BASE_ADDRESS   (0x44A40000)
#define NIT_ARM_CORE_SIZE           0x00010000

#define NIT_ARM_CORE_FIELD_TABLE(_) \
_(arm_core,led_r,uint16_t,sizeof(uint16_t),0x0000+0) \
_(arm_core,led_g,uint16_t,sizeof(uint16_t),0x0000+1) \
_(arm_core,led_b,uint16_t,sizeof(uint16_t),0x0000+2) \
_(arm_core,soft_reset,uint16_t,sizeof(uint16_t),0x0000+5) \

DRIVER_DECLARE_OFFSET_TABLE_BEGIN(arm_core)
NIT_ARM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
DRIVER_DECLARE_OFFSET_TABLE_END(arm_core)

DRIVER_DECLARE_CONFIG_BEGIN(arm_core)
NIT_ARM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)


// template <class archiver> void save(archiver &ar) const { ar(led_r, led_g, led_b, soft_reset,reserved ); }
// template <class archiver> void load(archiver &ar) { ar(led_r, led_g, led_b, soft_reset,reserved ); }

DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(arm_core)
NIT_ARM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
DRIVER_DECLARE_CONFIG_SERIALIZER_END(arm_core)

DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(arm_core)
NIT_ARM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
DRIVER_DECLARE_CONFIG_DESERIALIZER_END(arm_core)

DRIVER_DECLARE_CONFIG_END(arm_core)

DRIVER_DECLARE_STATE(arm_core)

int nit_arm_core_assert(nit_arm_core_state_t* state);
int nit_arm_core_open(nit_arm_core_state_t* state);
int nit_arm_core_close(nit_arm_core_state_t* state);

int nit_arm_core_config_save_to_file(nit_arm_core_state_t* state, const char *path);
int nit_arm_core_config_load_from_file(nit_arm_core_state_t* state, const char *path);

int nit_arm_core_led_r_set(nit_arm_core_state_t* state, uint16_t value);
int nit_arm_core_led_r_get(nit_arm_core_state_t* state, uint16_t *value);

int nit_arm_core_led_g_set(nit_arm_core_state_t* state, uint16_t value);
int nit_arm_core_led_g_get(nit_arm_core_state_t* state, uint16_t *value);

int nit_arm_core_led_b_set(nit_arm_core_state_t* state, uint16_t value);
int nit_arm_core_led_b_get(nit_arm_core_state_t* state, uint16_t *value);

int nit_arm_core_soft_reset_set(nit_arm_core_state_t* state, uint16_t value);
int nit_arm_core_soft_reset_get(nit_arm_core_state_t* state, uint16_t *value);

extern nit_arm_core_state_t nit_arm_core_driver;

#endif