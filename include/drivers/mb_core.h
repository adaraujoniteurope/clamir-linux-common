#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_MB_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_MB_CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "common.h"
#include "inttypes.h"

#include "gen_core_field_table.h"
#include "mom_core_field_table.h"
#include "pwm_core_field_table.h"
#include "roi_core_field_table.h"

#define NIT_MB_CORE_BASE_ADDRESS 0x50000000
#define NIT_MB_CORE_SIZE 0x00050008

    DRIVER_DECLARE_CONFIG_BEGIN(mb_core)
    NIT_GEN_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)
    NIT_MOM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)
    NIT_PWM_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)
    NIT_ROI_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_END(mb_core)

    DRIVER_DECLARE_STATE(mb_core)

    extern mb_core_state_t nit_mb_core_driver;

    int mb_core_open(mb_core_state_t *state);
    int mb_core_close(mb_core_state_t *state);

#ifdef __cplusplus
}
#endif

#endif