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
#include "app_gsdml_module_digital_in_out.h"

GSDML_DIGITAL_IN_OUT_SUBMODULE_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

GSDML_DIGITAL_IN_OUT_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(APP_GSDML_MOD_ID_8_8_DIGITAL_IN_OUT, module_digital_in_out, "DIO 8xLogicLevel", GSDML_DIGITAL_IN_OUT_SUBMODULES_TABLE)

static uint8_t submod_digital_inout_value = 0;
static uint16_t submod_digital_inout_value_size = sizeof(submod_digital_inout_value);

int submod_digital_inout_get(void* arg, void *data, uint16_t *size)
{
    app_gsdml_submodule_t* context = (app_gsdml_submodule_t*) arg;
    data = &submod_digital_inout_value;
    *size = submod_digital_inout_value_size;
    return 0;
}

int submod_digital_inout_set(void* arg, void *data, uint16_t size)
{

    if (size != submod_digital_inout_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    submod_digital_inout_value = *(uint8_t*)data;
    return 0;
}

int app_gsdml_digital_in_out_submodule_parameter_1_get(void* arg, void **data, uint16_t *size)
{
    return app_gsdml_default_param_get(arg, data, size);
}

int app_gsdml_digital_in_out_submodule_parameter_1_set(void* arg, void *data, uint16_t size)
{
    return app_gsdml_default_param_set(arg, data, size);
}

int app_gsdml_digital_in_out_submodule_parameter_2_get(void* arg, void **data, uint16_t *size)
{
    return app_gsdml_default_param_get(arg, data, size);
}

int app_gsdml_digital_in_out_submodule_parameter_2_set(void* arg, void *data, uint16_t size)
{
    return app_gsdml_default_param_set(arg, data, size);
}