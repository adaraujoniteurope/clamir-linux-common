#include "sampleapp_common.h"
#include "app_utils.h"

#include "app_log.h"
#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_gsdml.h"
#include "app_gsdml_utils.h"
#include "app_gsdml_module_digital_in.h"

APP_GSDML_SUBMOD_ID_DIGITAL_IN_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

#define GSDML_DIGITAL_IN_SUBMODULES_TABLE(_) \
    _(submod_digital_in, "Digital Input", APP_GSDML_API, APP_GSDML_SUBMOD_ID_DIGITAL_IN, PNET_DIR_INPUT, APP_GSDML_INPUT_DATA_DIGITAL_SIZE, 0, APP_GSDML_SUBMOD_ID_DIGITAL_IN_PARAMETERS_TABLE)

GSDML_DIGITAL_IN_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(APP_GSDML_MOD_ID_8_0_DIGITAL_IN, module_digital_in, "DI 8xLogicLevel", GSDML_DIGITAL_IN_SUBMODULES_TABLE)

static uint8_t submod_digital_in_value = 0;
static uint16_t submod_digital_in_value_size = sizeof(submod_digital_in_value);

int submod_digital_in_get(void* arg, void* data, uint16_t* size)
{
    app_gsdml_submodule_t* context = (app_gsdml_submodule_t*) arg;
    data = &submod_digital_in_value;
    *size = submod_digital_in_value_size;
    return 0;
}

int submod_digital_in_set(void* arg, void* data, uint16_t size)
{
    if (size != submod_digital_in_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    submod_digital_in_value = *(uint8_t*) data;
    return 0;
}