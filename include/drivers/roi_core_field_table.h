#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_ROI_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_ROI_CORE_FIELD_TABLE_H_

#define NIT_ROI_CORE_FIELD_TABLE(_)                         \
    _(roi_core, x1, uint16_t, sizeof(uint16_t), 0x8000 + 0) \
    _(roi_core, y1, uint16_t, sizeof(uint16_t), 0x8000 + 1) \
    _(roi_core, x2, uint16_t, sizeof(uint16_t), 0x8000 + 2) \
    _(roi_core, y2, uint16_t, sizeof(uint16_t), 0x8000 + 3) \
    _(roi_core, round, uint16_t, sizeof(uint16_t), 0x8000 + 4)

#endif