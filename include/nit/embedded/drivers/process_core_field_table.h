#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_PROCESS_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_PROCESS_CORE_FIELD_TABLE_H_

#define NIT_PROCESS_CORE_FIELD_TABLE(_)                                    \
    _(process_core, ki, uint32_t, sizeof(uint32_t), 0)                     \
    _(process_core, kp, uint32_t, sizeof(uint32_t), 1)                     \
    _(process_core, kd, uint32_t, sizeof(uint32_t), 2)                     \
    _(process_core, max_power, uint32_t, sizeof(uint32_t), 3)              \
    _(process_core, min_power, uint32_t, sizeof(uint32_t), 4)              \
    _(process_core, power_man, uint32_t, sizeof(uint32_t), 5)              \
    _(process_core, power_limit_max, uint32_t, sizeof(uint32_t), 6)        \
    _(process_core, power_limit_min, uint32_t, sizeof(uint32_t), 7)        \
    _(process_core, set_ref_width, uint32_t, sizeof(uint32_t), 8)          \
    _(process_core, width_ref, uint32_t, sizeof(uint32_t), 9)              \
    _(process_core, pixel_mm_ratio, uint32_t, sizeof(uint32_t), 10)        \
    _(process_core, pid_error, uint32_t, sizeof(uint32_t), 11)             \
    _(process_core, end_of_process, uint32_t, sizeof(uint32_t), 12)        \
    _(process_core, limit_integral, uint32_t, sizeof(uint32_t), 13)        \
    _(process_core, limit_slew, uint32_t, sizeof(uint32_t), 14)            \
    _(process_core, buff_size, uint32_t, sizeof(uint32_t), 15)             \
    _(process_core, background_remove, uint32_t, sizeof(uint32_t), 16)          \
    _(process_core, enable_alarm, uint32_t, sizeof(uint32_t), 17)          \
    _(process_core, alarm_max, uint32_t, sizeof(uint32_t), 18)             \
    _(process_core, alarm_min, uint32_t, sizeof(uint32_t), 19)             \
    _(process_core, alarm_time, uint32_t, sizeof(uint32_t), 20)            \
    _(process_core, serial_number_low, uint32_t, sizeof(uint32_t), 21)     \
    _(process_core, serial_number_high, uint32_t, sizeof(uint32_t), 22)    \
    _(process_core, automeasure, uint32_t, sizeof(uint32_t), 23)           \
    _(process_core, autoshutter_config, uint32_t, sizeof(uint32_t), 24)    \
    _(process_core, autoshutter_temp, uint32_t, sizeof(uint32_t), 25)      \
    _(process_core, autoshutter_timer, uint32_t, sizeof(uint32_t), 26)     \
    _(process_core, track_ref_start, uint32_t, sizeof(uint32_t), 27)       \
    _(process_core, laser_external_control, uint32_t, sizeof(uint32_t), 28) \
    _(process_core, delay_laser_on, uint32_t, sizeof(uint32_t), 29)        \
    _(process_core, preheating_ena, uint32_t, sizeof(uint32_t), 30)        \
    _(process_core, preheating_time, uint32_t, sizeof(uint32_t), 31)       \
    _(process_core, preheating_power, uint32_t, sizeof(uint32_t), 32)

#endif