#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_SCC_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_SCC_CORE_FIELD_TABLE_H_

#define NIT_SCC_CORE_FIELD_TABLE(_) \
    _(scc_core, testing_disable, uint32_t, sizeof(uint32_t), 0) \
    _(scc_core, testing_addr, uint32_t, sizeof(uint32_t), 0) \
    _(scc_core, testing_wren, uint32_t, sizeof(uint32_t), 0) \
    _(scc_core, testing_data, uint32_t, sizeof(uint32_t), 0) \

#define NIT_SCC_CORE_STUB_FIELD_TABLE(_)                               \
    _(scc_core, opmode, uint16_t, sizeof(uint16_t), 0)                 \
    _(scc_core, calibration_mode, uint16_t, sizeof(uint16_t), 1)       \
    _(scc_core, process_bypass, uint16_t, sizeof(uint16_t), 2)         \
    _(scc_core, calibration_bypass, uint16_t, sizeof(uint16_t), 3)     \
    _(scc_core, width, uint16_t, sizeof(uint16_t), 4)                  \
    _(scc_core, height, uint16_t, sizeof(uint16_t), 5)                 \

#endif