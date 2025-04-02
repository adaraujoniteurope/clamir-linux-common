#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_SCC_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_SCC_CORE_H_

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

#include "scc_core_field_table.h"

#include "nit/embedded/utils/waitable.hpp"

#define NIT_SCC_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(nit_scc_core_state_t, name, parameter, type, size, offset)
#define NIT_SCC_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(nit_scc_core_state_t, name, parameter, type, size, offset)

#define NIT_SCC_CORE_SCALE_DEFAULT_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_SCALE_DEFAULT_BASE_SIZE 0x00002000

#define NIT_SCC_CORE_OFFSET_DEFAULT_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_OFFSET_DEFAULT_BASE_SIZE 0x00002000

#if !NIT_CLAMIR_HOST_MOCKUP
#define NIT_SCC_CORE_CTRL_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_CTRL_BASE_SIZE 0x00002000

#define NIT_SCC_CORE_SCALE_BASE_ADDRESS 0x46000000
#define NIT_SCC_CORE_SCALE_BASE_SIZE 0x00008000

#define NIT_SCC_CORE_OFFSET_BASE_ADDRESS 0x48000000
#define NIT_SCC_CORE_OFFSET_BASE_SIZE 0x00008000

#define NIT_SCC_CORE_MIN_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_MIN_BASE_SIZE 0x00002000

#define NIT_SCC_CORE_MAX_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_MAX_BASE_SIZE 0x00002000
#else
#define NIT_SCC_CORE_CTRL_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_CTRL_BASE_SIZE 0x00002000

#define NIT_SCC_CORE_SCALE_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_SCALE_BASE_SIZE 0x00002000

#define NIT_SCC_CORE_OFFSET_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_OFFSET_BASE_SIZE 0x00002000

#define NIT_SCC_CORE_MIN_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_MIN_BASE_SIZE 0x00002000

#define NIT_SCC_CORE_MAX_BASE_ADDRESS 0x00000000
#define NIT_SCC_CORE_MAX_BASE_SIZE 0x00002000
#endif

    typedef enum nit_scc_core_opmode_enum
    {
        NIT_SCC_CORE_OPMODE_PROCESS,
        NIT_SCC_CORE_OPMODE_CALIBRATE,
        NIT_SCC_CORE_OPMODE_MAX
    } nit_scc_core_opmode_t;

    typedef enum nit_scc_core_calibration_mode_enum
    {
        NIT_SCC_CORE_CALIBRATION_STATUS_IDLE,
        NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MIN,
        NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_MAX,
        NIT_SCC_CORE_CALIBRATION_STATUS_ACQUIRING_UPDATING,
        NIT_SCC_CORE_CALIBRATION_STATUS_MAX
    } nit_scc_core_calibration_mode_t;

    DRIVER_DECLARE_OFFSET_TABLE_BEGIN(scc_core)
    NIT_SCC_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
    DRIVER_DECLARE_OFFSET_TABLE_END(scc_core)

    DRIVER_DECLARE_CONFIG_BEGIN(scc_core)
    std::string scale_default_file_path;
    std::string offset_default_file_path;
    NIT_SCC_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)

    DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(scc_core)
    cereal::make_nvp("scale_default_file_path", scale_default_file_path),
    cereal::make_nvp("offset_default_file_path", offset_default_file_path),
    NIT_SCC_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_SERIALIZER_END(scc_core)
    
    DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(scc_core)
    cereal::make_nvp("scale_default_file_path", scale_default_file_path),
    cereal::make_nvp("offset_default_file_path", offset_default_file_path),
    NIT_SCC_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
    DRIVER_DECLARE_CONFIG_DESERIALIZER_END(scc_core)

    DRIVER_DECLARE_CONFIG_END(scc_core)

    DRIVER_DECLARE_STATE(scc_core)

    extern nit_scc_core_state_t nit_scc_core_driver;

    int nit_scc_core_open(nit_scc_core_state_t *state, nit_framebuffer_core_state_t *nit_framebuffer_core_state);
    int nit_scc_core_close(nit_scc_core_state_t *state);

    int nit_scc_core_config_save_to_file(nit_scc_core_state_t *state, const char *path);
    int nit_scc_core_config_load_from_file(nit_scc_core_state_t *state, const char *path);

    int nit_scc_core_calibration_mode_wait_idle(nit_scc_core_state_t* state);

    /**
     * this function is a stub to implement the core
     * functionality by evaluating in the image capture
     * poll.
     */
    int nit_scc_core_stub_eval(nit_scc_core_state_t *state);
    

    NIT_SCC_CORE_FIELD_TABLE(NIT_SCC_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#endif