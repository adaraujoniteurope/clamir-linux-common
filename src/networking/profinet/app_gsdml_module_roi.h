#ifndef _APP_GSDML_MODULE_ROI_H_
#define _APP_GSDML_MODULE_ROI_H_

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

#define APP_GSDML_MOD_ID_ROI 0x00000040

#define APP_GSDML_SUBMOD_ID_ROI 0x00000140

#define APP_GSDML_PARAMETER_ROI_IDX 125

#define APP_GSDML_INPUT_DATA_ROI_SIZE 2
#define APP_GSDML_OUTPUT_DATA_ROI_SIZE APP_GSDML_INPUT_DATA_ROI_SIZE

#define APP_GSDML_SUBMOD_ID_ROI_PARAMETERS_TABLE(_) \
    _(app_gsdml_roi_submodule_parameter_x1, APP_GSDML_PARAMETER_ROI_IDX, "ROI X1", sizeof(uint16_t)) \
    _(app_gsdml_roi_submodule_parameter_y1, APP_GSDML_PARAMETER_ROI_IDX, "ROI Y1", sizeof(uint16_t)) \
    _(app_gsdml_roi_submodule_parameter_x2, APP_GSDML_PARAMETER_ROI_IDX, "ROI X2", sizeof(uint16_t)) \
    _(app_gsdml_roi_submodule_parameter_y2, APP_GSDML_PARAMETER_ROI_IDX, "ROI Y2", sizeof(uint16_t)) \

APP_GSDML_SUBMOD_ID_ROI_PARAMETERS_TABLE(GSDML_PARAMETER_DECLARE)

#define APP_GSDML_ROI_SUBMODULES_TABLE(_) \
    _(submodule_roi, "ROI", APP_GSDML_API, PNET_DIR_IO, APP_GSDML_SUBMOD_ID_ROI, APP_GSDML_INPUT_DATA_ROI_SIZE, APP_GSDML_OUTPUT_DATA_ROI_SIZE, APP_GSDML_SUBMOD_ID_ROI_PARAMETERS_TABLE) \

APP_GSDML_ROI_SUBMODULES_TABLE(GSDML_SUBMODULE_DECLARE)

GSDML_MODULE_DECLARE(APP_GSDML_MOD_ID_ROI, module_roi, "ROI", APP_GSDML_ROI_SUBMODULES_TABLE)

#ifdef __cplusplus
}
#endif

#endif