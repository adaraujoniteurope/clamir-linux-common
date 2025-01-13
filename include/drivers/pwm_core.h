#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "common.h"
#include "mb_core.h"
#include "pwm_core_field_table.h"

#define NIT_PWM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(mb_core_state_t, name, parameter, type, size, offset)
#define NIT_PWM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(mb_core_state_t, name, parameter, type, size, offset)

#define NIT_PWM_CORE_BASE_ADDRESS (0x50000000)
#define NIT_PWM_CORE_SIZE 0x00010000

    DRIVER_DECLARE_OFFSET_TABLE_BEGIN(pwm_core)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
    DRIVER_DECLARE_OFFSET_TABLE_END(pwm_core)

    DRIVER_DECLARE_CONFIG_BEGIN(pwm_core)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_END(pwm_core)

    DRIVER_DECLARE_STATE(pwm_core)

    int pwm_core_open(mb_core_state_t *state);
    int pwm_core_close(mb_core_state_t *state);

    int pwm_core_config_save_to_file(mb_core_state_t *state, const char *path);
    int pwm_core_config_load_from_file(mb_core_state_t *state, const char *path);

    NIT_PWM_CORE_FIELD_TABLE(NIT_PWM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#ifdef __cplusplus
}
#endif

#endif