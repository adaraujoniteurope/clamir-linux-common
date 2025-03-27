#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_MOM_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_MOM_CORE_FIELD_TABLE_H_

#define NIT_MOM_CORE_FIELD_TABLE(_)                               \
    _(mom_core, threshold, uint16_t, sizeof(uint16_t), 0x4000 + 0)         \
    _(mom_core, intensity_min, uint16_t, sizeof(uint16_t), 0x4000 + 1)     \
    _(mom_core, intensity_max, uint16_t, sizeof(uint16_t), 0x4000 + 2)     \
    _(mom_core, mom_00, uint16_t, sizeof(uint16_t), 0x4000 + 3)            \
    _(mom_core, mom_01, uint16_t, sizeof(uint16_t), 0x4000 + 4)            \
    _(mom_core, mom_10, uint16_t, sizeof(uint16_t), 0x4000 + 5)            \
    _(mom_core, mom_11, uint16_t, sizeof(uint16_t), 0x4000 + 6)            \
    _(mom_core, mom_02, uint16_t, sizeof(uint16_t), 0x4000 + 7)            \
    _(mom_core, mom_20, uint16_t, sizeof(uint16_t), 0x4000 + 8)            \
    _(mom_core, start_track_mom_t, uint16_t, sizeof(uint16_t), 0x4000 + 9) \
    _(mom_core, track_nbr, uint16_t, sizeof(uint16_t), 0x4000 + 10)        \
    _(mom_core, mode, uint16_t, sizeof(uint16_t), 0x4000 + 11)             \
    _(mom_core, time_track_low, uint32_t, sizeof(uint32_t), 0x4000 + 12)   \
    _(mom_core, time_track_high, uint32_t, sizeof(uint32_t), 0x4000 + 13)  \
    _(mom_core, frame_max, uint16_t, sizeof(uint16_t), 0x4000 + 14)        \
    _(mom_core, reference_track, uint16_t, sizeof(uint16_t), 0x4000 + 15)  \
    _(mom_core, end_of_track, uint16_t, sizeof(uint16_t), 0x4000 + 16)


#endif