#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_ROI_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_ROI_CORE_H_

#include <stdint.h>
#include "common.h"
#include "mb_core.h"
#include "roi_core_field_table.h"

#define NIT_ROI_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(nit_mb_core_state_t, name, parameter, type, size, offset)
#define NIT_ROI_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(nit_mb_core_state_t, name, parameter, type, size, offset)

#if !NIT_CLAMIR_HOST_MOCKUP
#define NIT_ROI_CORE_BASE_ADDRESS (0x50000000)
#define NIT_ROI_CORE_SIZE 0x00010000
#else
#define NIT_ROI_CORE_BASE_ADDRESS 0x00000000
#define NIT_ROI_CORE_SIZE 0x00010000
#endif

    DRIVER_DECLARE_OFFSET_TABLE_BEGIN(roi_core)
    NIT_ROI_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
    DRIVER_DECLARE_OFFSET_TABLE_END(roi_core)

    DRIVER_DECLARE_CONFIG_BEGIN(roi_core)
    NIT_ROI_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)

    DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(roi_core)
    NIT_ROI_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_SERIALIZER_END(roi_core)
    
    DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(roi_core)
    NIT_ROI_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_DESERIALIZER_END(roi_core)

    DRIVER_DECLARE_CONFIG_END(roi_core)

    DRIVER_DECLARE_STATE(roi_core)

    int roi_core_open(nit_mb_core_state_t *state);
    int roi_core_close(nit_mb_core_state_t *state);

    int roi_core_config_save_to_file(nit_mb_core_state_t *state, const char *path);
    int roi_core_config_load_from_file(nit_mb_core_state_t *state, const char *path);

    NIT_ROI_CORE_FIELD_TABLE(NIT_ROI_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#endif