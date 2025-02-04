#include "app_data.h"
#include "app_utils.h"

#include "app_log.h"
#include "rtlabs/osal/osal.h"
#include "pnal.h"
#include <rtlabs/p-net/pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_gsdml.h"
#include "app_gsdml_utils.h"
#include "app_gsdml_module_digital_out.h"

APP_GSDML_SUBMOD_ID_DIGITAL_OUT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

#define GSDML_DIGITAL_OUT_SUBMODULES_TABLE(_) \
    _(submod_digital_out, "Digital Output", APP_GSDML_API, PNET_DIR_OUTPUT, APP_GSDML_SUBMOD_ID_DIGITAL_OUT, 0, APP_GSDML_OUTPUT_DATA_DIGITAL_SIZE, APP_GSDML_SUBMOD_ID_DIGITAL_OUT_PARAMETERS_TABLE)

GSDML_DIGITAL_OUT_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(APP_GSDML_MOD_ID_0_8_DIGITAL_OUT, module_digital_out, "DO 8xLogicLevel", GSDML_DIGITAL_OUT_SUBMODULES_TABLE)

static uint8_t submod_digital_out_value = 0;
static uint16_t submod_digital_out_value_size = sizeof(submod_digital_out_value);

int submod_digital_out_get(void* arg, void* data, uint16_t* size)
{
    app_gsdml_submodule_t* context = (app_gsdml_submodule_t*) arg;
    data = &submod_digital_out_value;
    *size = submod_digital_out_value_size;
    return 0;
}

int submod_digital_out_set(void* arg, void* data, uint16_t size)
{
    app_gsdml_submodule_t* context = (app_gsdml_submodule_t*) arg;
    if (size != submod_digital_out_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    submod_digital_out_value = *(uint8_t*) data;
    return 0;
}