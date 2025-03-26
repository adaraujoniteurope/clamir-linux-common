#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_SCC_CORE_FIELD_TABLE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_SCC_CORE_FIELD_TABLE_H_

#define NIT_SCC_CORE_FIELD_TABLE(_)                                    \
    _(scc_core, opmode, uint32_t, sizeof(uint32_t), 0)                     \
    _(scc_core, acquire_min_enable, uint32_t, sizeof(uint32_t), 1)                     \
    _(scc_core, acquire_max_enable, uint32_t, sizeof(uint32_t), 1)                     \
    _(scc_core, width, uint32_t, sizeof(uint32_t), 2)                     \
    _(scc_core, height, uint32_t, sizeof(uint32_t), 3)                     \

#endif