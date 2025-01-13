#ifndef _APP_GSDML_MODULE_DIGITAL_OUT_H_
#define _APP_GSDML_MODULE_DIGITAL_OUT_H_

#include "app_data.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_gsdml_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_GSDML_OUTPUT_DATA_DIGITAL_SIZE 1 /* bytes */
#define APP_GSDML_INPUT_DATA_DIGITAL_SIZE 1  /* bytes */
#define APP_GSDML_MOD_ID_0_8_DIGITAL_OUT 0x00000031
#define APP_GSDML_SUBMOD_ID_DIGITAL_OUT 0x00000131

#define APP_GSDML_SUBMOD_ID_DIGITAL_OUT_PARAMETERS_TABLE(_) \

APP_GSDML_SUBMOD_ID_DIGITAL_OUT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

#define GSDML_DIGITAL_OUT_SUBMODULES_TABLE(_) \
    _(submod_digital_out, "Digital Output", APP_GSDML_API, PNET_DIR_OUTPUT, APP_GSDML_SUBMOD_ID_DIGITAL_OUT, 0, APP_GSDML_OUTPUT_DATA_DIGITAL_SIZE, APP_GSDML_SUBMOD_ID_DIGITAL_OUT_PARAMETERS_TABLE)

GSDML_DIGITAL_OUT_SUBMODULES_TABLE(GSDML_SUBMODULE_DECLARE)

GSDML_MODULE_DECLARE(APP_GSDML_MOD_ID_0_8_DIGITAL_OUT, module_digital_out, "DO 8xLogicLevel", GSDML_DIGITAL_OUT_SUBMODULES_TABLE)

#ifdef __cplusplus
}
#endif

#endif