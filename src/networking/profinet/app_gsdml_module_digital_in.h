#ifndef _APP_GSDML_MODULE_DIGITAL_IN_H_
#define _APP_GSDML_MODULE_DIGITAL_IN_H_

#include "sampleapp_common.h"
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

#define APP_GSDML_SUBMOD_ID_DIGITAL_IN 0x00000130
#define APP_GSDML_MOD_ID_8_0_DIGITAL_IN 0x00000030
#define APP_GSDML_OUTPUT_DATA_DIGITAL_SIZE 1 /* bytes */
#define APP_GSDML_INPUT_DATA_DIGITAL_SIZE 1  /* bytes */

#define APP_GSDML_SUBMOD_ID_DIGITAL_IN_PARAMETERS_TABLE(_)

#define GSDML_DIGITAL_IN_SUBMODULES_TABLE(_) \
    _(submod_digital_in, "Digital Input", APP_GSDML_API, APP_GSDML_SUBMOD_ID_DIGITAL_IN, PNET_DIR_INPUT, APP_GSDML_INPUT_DATA_DIGITAL_SIZE, 0, APP_GSDML_SUBMOD_ID_DIGITAL_IN_PARAMETERS_TABLE)

GSDML_DIGITAL_IN_SUBMODULES_TABLE(GSDML_SUBMODULE_DECLARE)

GSDML_MODULE_DECLARE(APP_GSDML_MOD_ID_8_0_DIGITAL_IN, module_digital_in, "DI 8xLogicLevel", GSDML_DIGITAL_IN_SUBMODULES_TABLE)

#ifdef __cplusplus
}
#endif

#endif