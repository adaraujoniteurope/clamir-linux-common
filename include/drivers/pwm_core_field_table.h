#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_FIELD_TABLE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define NIT_PWM_CORE_FIELD_TABLE(_)                                    \
    _(pwm_core, pwm_limit_max, uint32_t, sizeof(uint32_t), 0xC000 + 0) \
    _(pwm_core, pwm, uint32_t, sizeof(uint32_t), 0xC000 + 1)           \
    _(pwm_core, power, uint32_t, sizeof(uint32_t), 0xC000 + 2)         \
    _(pwm_core, pwm_limit_min, uint32_t, sizeof(uint32_t), 0xC000 + 3)

#ifdef __cplusplus
}
#endif

#endif