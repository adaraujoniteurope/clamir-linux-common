#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_MOM_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_MOM_CORE_H_

#include "common.h"
#include "inttypes.h"
#include "mb_core.h"
#include "mom_core_field_table.h"

#define NIT_MOM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(nit_mb_core_state_t, name, parameter, type, size, offset)
#define NIT_MOM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(nit_mb_core_state_t, name, parameter, type, size, offset)

#if !DEBUGGING_HOST
#define NIT_MOM_CORE_BASE_ADDRESS (0x50000000)
#define NIT_MOM_CORE_SIZE 0x00010000
#else
#define NIT_MOM_CORE_BASE_ADDRESS 0x00000000
#define NIT_MOM_CORE_SIZE 0x00010000
#endif

    DRIVER_DECLARE_OFFSET_TABLE_BEGIN(mom_core)
    NIT_MOM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
    DRIVER_DECLARE_OFFSET_TABLE_END(mom_core)

    DRIVER_DECLARE_CONFIG_BEGIN(mom_core)
    NIT_MOM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)

    DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(mom_core)
    NIT_MOM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_SERIALIZER_END(mom_core)
    
    DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(mom_core)
    NIT_MOM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_DESERIALIZER_END(mom_core)

    DRIVER_DECLARE_CONFIG_END(mom_core)

    // DRIVER_DECLARE_STATE(mom_core)
    typedef nit_mb_core_state_t nit_mom_core_state_t;

    int mom_core_open(nit_mb_core_state_t *state);
    int mom_core_close(nit_mb_core_state_t *state);

    int mom_core_config_save_to_file(nit_mb_core_state_t *state, const char *path);
    int mom_core_config_load_from_file(nit_mb_core_state_t *state, const char *path);

    NIT_MOM_CORE_FIELD_TABLE(NIT_MOM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#endif