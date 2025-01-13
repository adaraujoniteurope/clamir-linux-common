#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_GEN_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_GEN_CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "common.h"
#include "inttypes.h"
#include "mb_core.h"
#include "gen_core_field_table.h"

#define NIT_GEN_CORE_BASE_ADDRESS 0x50000000
#define NIT_GEN_CORE_SIZE 0x00010000

    DRIVER_DECLARE_OFFSET_TABLE_BEGIN(gen_core)
    NIT_GEN_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
    DRIVER_DECLARE_OFFSET_TABLE_END(gen_core)

    DRIVER_DECLARE_CONFIG_BEGIN(gen_core)
    NIT_GEN_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_END(gen_core)

    DRIVER_DECLARE_STATE(gen_core)

    int gen_core_open(mb_core_state_t *state);

    int gen_core_close(mb_core_state_t *state);

    #define NIT_GEN_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(mb_core_state_t, name, parameter, type, size, offset)
    #define NIT_GEN_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(mb_core_state_t, name, parameter, type, size, offset)

    NIT_GEN_CORE_FIELD_TABLE(NIT_GEN_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#ifdef __cplusplus
}
#endif

#endif