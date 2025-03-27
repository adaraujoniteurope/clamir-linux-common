#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_PROCESS_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_PROCESS_CORE_H_

#include <atomic>
#include <cstdint>

#include "common.h"

#include "arm_core.h"
#include "mb_core.h"
#include "control_unit_core.h"
#include "framebuffer_core.h"
#include "gen_core.h"
#include "pwm_core.h"
#include "roi_core.h"

#include "process_core_field_table.h"

#include <nit/embedded/utils/waitable.hpp>

#define NIT_PROCESS_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(nit_process_core_state_t, name, parameter, type, size, offset)
#define NIT_PROCESS_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(nit_process_core_state_t, name, parameter, type, size, offset)

#if !NIT_CLAMIR_HOST_MOCKUP
#define NIT_PROCESS_CORE_BASE_ADDRESS (0x50000000)
#define NIT_PROCESS_CORE_SIZE 0x00010000
#else
#define NIT_PROCESS_CORE_BASE_ADDRESS 0x00000000
#define NIT_PROCESS_CORE_SIZE 0x00010000
#endif

    DRIVER_DECLARE_OFFSET_TABLE_BEGIN(process_core)
    NIT_PROCESS_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
    DRIVER_DECLARE_OFFSET_TABLE_END(process_core)

    DRIVER_DECLARE_CONFIG_BEGIN(process_core)
    NIT_PROCESS_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)

    DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(process_core)
    NIT_PROCESS_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_SERIALIZER_END(process_core)
    
    DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(process_core)
    NIT_PROCESS_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_DESERIALIZER_END(process_core)

    DRIVER_DECLARE_CONFIG_END(process_core)

    DRIVER_DECLARE_STATE(process_core)

    extern nit_process_core_state_t nit_process_core_driver;

    int nit_process_core_open(nit_process_core_state_t *state, nit_arm_core_state_t *nit_arm_core_state, nit_control_unit_core_state_t *nit_control_unit_core_state, nit_mb_core_state_t *nit_mb_core_state, nit_framebuffer_core_state_t *nit_framebuffer_core_state);
    int nit_process_core_close(nit_process_core_state_t *state);

    int nit_process_core_config_save_to_file(nit_process_core_state_t *state, const char *path);
    int nit_process_core_config_load_from_file(nit_process_core_state_t *state, const char *path);

    int nit_process_core_run(nit_process_core_state_t *state, std::shared_ptr<utils::waitable> timer, std::atomic_bool& shutdown);

    volatile int* nit_process_core_get_virtual_metadata_shm_ptr(nit_process_core_state_t *state);
    

    NIT_PROCESS_CORE_FIELD_TABLE(NIT_PROCESS_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#endif