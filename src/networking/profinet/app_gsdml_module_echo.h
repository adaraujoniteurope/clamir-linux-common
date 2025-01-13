#ifndef _APP_GSDML_MODULE_ECHO_H_
#define _APP_GSDML_MODULE_ECHO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pnet_api.h>

#include "app_data.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "osal.h"
#include "pnal.h"

#include "app_gsdml_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_GSDML_MOD_ID_ECHO 0x00000040

#define APP_GSDML_SUBMOD_ID_ECHO 0x00000140

#define APP_GSDML_PARAMETER_ECHO_IDX 125

#define APP_GSDML_INPUT_DATA_ECHO_SIZE 8
#define APP_GSDML_OUTPUT_DATA_ECHO_SIZE APP_GSDML_INPUT_DATA_ECHO_SIZE

#define APP_GSDML_SUBMOD_ID_ECHO_PARAMETERS_TABLE(_) \
    _(app_gsdml_echo_submodule_parameter_1, APP_GSDML_PARAMETER_ECHO_IDX, "Echo gain setting", APP_GSDML_PARAMETER_LENGTH)

APP_GSDML_SUBMOD_ID_ECHO_PARAMETERS_TABLE(GSDML_PARAMETER_DECLARE)

#define APP_GSDML_ECHO_SUBMODULES_TABLE(_) \
    _(submod_echo, "Test Submodule", APP_GSDML_API, PNET_DIR_IO, APP_GSDML_SUBMOD_ID_ECHO, APP_GSDML_INPUT_DATA_ECHO_SIZE, APP_GSDML_OUTPUT_DATA_ECHO_SIZE, APP_GSDML_SUBMOD_ID_ECHO_PARAMETERS_TABLE)

APP_GSDML_ECHO_SUBMODULES_TABLE(GSDML_SUBMODULE_DECLARE)

GSDML_MODULE_DECLARE(APP_GSDML_MOD_ID_ECHO, module_echo, "Echo module", APP_GSDML_ECHO_SUBMODULES_TABLE)

#ifdef __cplusplus
}
#endif

#endif