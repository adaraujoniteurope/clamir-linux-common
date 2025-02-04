#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_H_

#include <stdint.h>
#include "common.h"
#include "mb_core.h"
#include "pwm_core_field_table.h"

#define NIT_PWM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(nit_mb_core_state_t, name, parameter, type, size, offset)
#define NIT_PWM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(nit_mb_core_state_t, name, parameter, type, size, offset)

#if !DEBUGGING_HOST
#define NIT_PWM_CORE_BASE_ADDRESS (0x50000000)
#define NIT_PWM_CORE_SIZE 0x00010000
#else
#define NIT_PWM_CORE_BASE_ADDRESS 0x00000000
#define NIT_PWM_CORE_SIZE 0x00010000
#endif

    DRIVER_DECLARE_OFFSET_TABLE_BEGIN(pwm_core)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
    DRIVER_DECLARE_OFFSET_TABLE_END(pwm_core)

    DRIVER_DECLARE_CONFIG_BEGIN(pwm_core)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)

    DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(pwm_core)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_SERIALIZER_END(pwm_core)
    
    DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(pwm_core)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_DESERIALIZER_END(pwm_core)

    DRIVER_DECLARE_CONFIG_END(pwm_core)

    DRIVER_DECLARE_STATE(pwm_core)

    int pwm_core_open(nit_mb_core_state_t *state);
    int pwm_core_close(nit_mb_core_state_t *state);

    int pwm_core_config_save_to_file(nit_mb_core_state_t *state, const char *path);
    int pwm_core_config_load_from_file(nit_mb_core_state_t *state, const char *path);

        NIT_PWM_CORE_FIELD_TABLE(NIT_PWM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#endif