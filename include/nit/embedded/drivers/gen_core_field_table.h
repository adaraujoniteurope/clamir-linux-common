#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_GEN_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_GEN_CORE_FIELD_TABLE_H_

#define NIT_GEN_CORE_FIELD_TABLE(_)                               \
    _(gen_core, version_id, uint16_t, sizeof(uint16_t), 0x0000 + 0)        \
    _(gen_core, enable_mom, uint16_t, sizeof(uint16_t), 0x0000 + 1)        \
    _(gen_core, enable_roi, uint16_t, sizeof(uint16_t), 0x0000 + 2)        \
    _(gen_core, enable_pwm, uint16_t, sizeof(uint16_t), 0x0000 + 3)        \
    _(gen_core, enable_irq, uint16_t, sizeof(uint16_t), 0x0000 + 4)        \
    _(gen_core, change_op_mode, uint16_t, sizeof(uint16_t), 0x0000 + 5)    \
    _(gen_core, irq_microblaze, uint16_t, sizeof(uint16_t), 0x0000 + 6)    \
    _(gen_core, digital_in_0, uint16_t, sizeof(uint16_t), 0x0000 + 7)      \
    _(gen_core, digital_in_1, uint16_t, sizeof(uint16_t), 0x0000 + 8)      \
    _(gen_core, digital_out_0, uint16_t, sizeof(uint16_t), 0x0000 + 9)     \
    _(gen_core, digital_out_1, uint16_t, sizeof(uint16_t), 0x0000 + 10)    \
    _(gen_core, digital_out_conf, uint16_t, sizeof(uint16_t), 0x0000 + 11) \
    _(gen_core, digital_in_2, uint16_t, sizeof(uint16_t), 0x0000 + 12)     \
    _(gen_core, digital_in_3, uint16_t, sizeof(uint16_t), 0x0000 + 13)     \
    _(gen_core, digital_out_2, uint16_t, sizeof(uint16_t), 0x0000 + 14)    \
    _(gen_core, digital_out_3, uint16_t, sizeof(uint16_t), 0x0000 + 15)

#endif