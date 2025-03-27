#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_PWM_CORE_FIELD_TABLE_H_

#define NIT_PWM_CORE_FIELD_TABLE(_)                                    \
    _(pwm_core, pwm_limit_max, uint16_t, sizeof(uint16_t), 0xC000 + 0) \
    _(pwm_core, pwm, uint16_t, sizeof(uint16_t), 0xC000 + 1)           \
    _(pwm_core, power, uint16_t, sizeof(uint16_t), 0xC000 + 2)         \
    _(pwm_core, pwm_limit_min, uint16_t, sizeof(uint16_t), 0xC000 + 3)


#endif