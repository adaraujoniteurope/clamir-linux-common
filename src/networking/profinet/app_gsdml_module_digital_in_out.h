#ifndef _APP_GSDML_MODULE_DIGITAL_IN_OUT_H_
#define _APP_GSDML_MODULE_DIGITAL_IN_OUT_H_

#include "app_data.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "rtlabs/osal/osal.h"
#include "pnal.h"
#include <rtlabs/p-net/pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_gsdml_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_GSDML_MOD_ID_8_8_DIGITAL_IN_OUT 0x00000032
#define APP_GSDML_SUBMOD_ID_DIGITAL_IN_OUT 0x00000132

#define APP_GSDML_PARAMETER_1_IDX 123
#define APP_GSDML_PARAMETER_2_IDX 124

#define APP_GSDML_OUTPUT_DATA_DIGITAL_SIZE 1 /* bytes */
#define APP_GSDML_INPUT_DATA_DIGITAL_SIZE 1  /* bytes */

#define GSDML_DIGITAL_IN_OUT_SUBMODULE_PARAMETERS_TABLE(_) \
    _(app_gsdml_digital_in_out_submodule_parameter_1, APP_GSDML_PARAMETER_1_IDX, "Demo 1", APP_GSDML_PARAMETER_LENGTH) \
    _(app_gsdml_digital_in_out_submodule_parameter_2, APP_GSDML_PARAMETER_2_IDX, "Demo 2", APP_GSDML_PARAMETER_LENGTH)

GSDML_DIGITAL_IN_OUT_SUBMODULE_PARAMETERS_TABLE(GSDML_PARAMETER_DECLARE)

#define GSDML_DIGITAL_IN_OUT_SUBMODULES_TABLE(_) \
    _(submod_digital_inout, "Digital Input/Output", APP_GSDML_API, PNET_DIR_IO, APP_GSDML_SUBMOD_ID_DIGITAL_IN_OUT, APP_GSDML_INPUT_DATA_DIGITAL_SIZE, APP_GSDML_OUTPUT_DATA_DIGITAL_SIZE, GSDML_DIGITAL_IN_OUT_SUBMODULE_PARAMETERS_TABLE)

GSDML_DIGITAL_IN_OUT_SUBMODULES_TABLE(GSDML_SUBMODULE_DECLARE)

GSDML_MODULE_DECLARE(APP_GSDML_MOD_ID_8_8_DIGITAL_IN_OUT, module_digital_in_out, "DIO 8xLogicLevel", GSDML_DIGITAL_IN_OUT_SUBMODULES_TABLE)

#ifdef __cplusplus
}
#endif

#endif