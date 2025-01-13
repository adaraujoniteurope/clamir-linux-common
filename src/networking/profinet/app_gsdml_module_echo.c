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
#include "app_gsdml_module_echo.h"

APP_GSDML_SUBMOD_ID_ECHO_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

APP_GSDML_ECHO_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(APP_GSDML_MOD_ID_ECHO, module_echo, "Echo module", APP_GSDML_ECHO_SUBMODULES_TABLE)

static unsigned char value[8] = {0};
static uint16_t value_size = sizeof(value);

int submod_echo_get(void* arg, void* data, uint16_t* size)
{
    data = &value;
    *size = value_size;
    return 0;
}

int submod_echo_set(void* arg, void* data, uint16_t size)
{

    if (size != value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    memcpy(value, data, sizeof(value));
    return 0;
}

int app_gsdml_echo_submodule_parameter_1_get(void* arg, void** data, uint16_t* size)
{
    return app_gsdml_default_param_get(arg, data, size);
}

int app_gsdml_echo_submodule_parameter_1_set(void* arg, void* data, uint16_t size)
{
    return app_gsdml_default_param_set(arg, data, size);
}