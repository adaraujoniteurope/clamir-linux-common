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

#include "app_gsdml_utils.h"
#include "app_gsdml_module_echo.h"

APP_GSDML_SUBMOD_ID_ECHO_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

APP_GSDML_ECHO_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(APP_GSDML_MOD_ID_ECHO, module_echo, "Echo module", APP_GSDML_ECHO_SUBMODULES_TABLE)

uint64_t output_data;
uint64_t input_data;

int submod_echo_get(void* arg, void* data, uint16_t* size)
{
    if (data == NULL) {
        return -1;
    }

    if (size == NULL) {
        return -1;
    }

    *(uint64_t*)data = be64toh(input_data);
    *size = sizeof(input_data);
    return 0;
}

int submod_echo_set(void* arg, void* data, uint16_t size)
{

    if (size != sizeof(output_data))
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    output_data = htobe64(*(uint64_t*) data);
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