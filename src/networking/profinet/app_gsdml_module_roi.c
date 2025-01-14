#include "app_data.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>
#include <endian.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "app_gsdml_utils.h"
#include "app_gsdml_module_roi.h"

#include <drivers/roi_core.h>

APP_GSDML_SUBMOD_ID_ROI_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

APP_GSDML_ROI_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(APP_GSDML_MOD_ID_ROI, module_roi, "ROI", APP_GSDML_ROI_SUBMODULES_TABLE)

int submodule_roi_set(void* submodule, void *data, uint16_t size)
{
    return 0;
}

int submodule_roi_get(void* submodule, void *data, uint16_t *size)
{
    return 0;
}

int app_gsdml_roi_submodule_parameter_x1_set(void* parameter, void *data, uint16_t size)
{
    return app_gsdml_default_param_set(parameter, data, size);
}

int app_gsdml_roi_submodule_parameter_x1_get(void* parameter, void **data, uint16_t *size)
{
    return app_gsdml_default_param_get(parameter, data, size);
}

int app_gsdml_roi_submodule_parameter_y1_set(void* parameter, void *data, uint16_t size)
{
    return app_gsdml_default_param_set(parameter, data, size);
}

int app_gsdml_roi_submodule_parameter_y1_get(void* parameter, void **data, uint16_t *size)
{
    return app_gsdml_default_param_get(parameter, data, size);
}

int app_gsdml_roi_submodule_parameter_x2_set(void* parameter, void *data, uint16_t size)
{
    return app_gsdml_default_param_set(parameter, data, size);
}

int app_gsdml_roi_submodule_parameter_x2_get(void* parameter, void **data, uint16_t *size)
{
    return app_gsdml_default_param_get(parameter, data, size);
}

int app_gsdml_roi_submodule_parameter_y2_set(void* parameter, void *data, uint16_t size)
{
    return app_gsdml_default_param_set(parameter, data, size);
}

int app_gsdml_roi_submodule_parameter_y2_get(void* parameter, void **data, uint16_t *size)
{
    return app_gsdml_default_param_get(parameter, data, size);
}