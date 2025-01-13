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
#include "app_gsdml_module_dap.h"
#include "app_gsdml_module_digital_in.h"
#include "app_gsdml_module_digital_out.h"
#include "app_gsdml_module_digital_in_out.h"
#include "app_gsdml_module_echo.h"

#define APP_GSDML_MODULES_TABLE(_) \
    _(dap_1)                       \
    _(module_digital_in)           \
    _(module_digital_out)          \
    _(module_digital_in_out)       \
    _(module_echo)

#define APP_GSDML_MODULES_TABABLE_AS_LIST_ITEM(object) &object,

/** List of supported modules */
app_gsdml_module_t *app_gsdml_modules[] =
    {
        APP_GSDML_MODULES_TABLE(APP_GSDML_MODULES_TABABLE_AS_LIST_ITEM)};

#define APP_GSDML_SUBMODULES_TABLE_AS_LIST_ITEM(OBJECT, NAME, API, ID, DATA_DIR, INSIZE, OUTSIZE, PARAMETERS_TABLE) &OBJECT,

app_gsdml_submodule_t *app_gsdml_submodules[] =
    {
        GSDML_DAP_SUBMODULES_TABLE(APP_GSDML_SUBMODULES_TABLE_AS_LIST_ITEM)
        GSDML_DIGITAL_IN_SUBMODULES_TABLE(APP_GSDML_SUBMODULES_TABLE_AS_LIST_ITEM)
        GSDML_DIGITAL_OUT_SUBMODULES_TABLE(APP_GSDML_SUBMODULES_TABLE_AS_LIST_ITEM)
        GSDML_DIGITAL_IN_OUT_SUBMODULES_TABLE(APP_GSDML_SUBMODULES_TABLE_AS_LIST_ITEM)
        APP_GSDML_ECHO_SUBMODULES_TABLE(APP_GSDML_SUBMODULES_TABLE_AS_LIST_ITEM)
};

#define GSDML_SUBMODULE_PARAMETERS_TABLE_AS_LIST_ITEM(OBJECT, INDEX, NAME, LENGTH) &OBJECT,

app_gsdml_param_t* app_gsdml_parameters[] =
{
    GSDML_DIGITAL_IN_OUT_SUBMODULE_PARAMETERS_TABLE(GSDML_SUBMODULE_PARAMETERS_TABLE_AS_LIST_ITEM)
    APP_GSDML_SUBMOD_ID_DIGITAL_IN_PARAMETERS_TABLE(GSDML_SUBMODULE_PARAMETERS_TABLE_AS_LIST_ITEM)
    APP_GSDML_SUBMOD_ID_DIGITAL_OUT_PARAMETERS_TABLE(GSDML_SUBMODULE_PARAMETERS_TABLE_AS_LIST_ITEM)
    APP_GSDML_SUBMOD_ID_ECHO_PARAMETERS_TABLE(GSDML_SUBMODULE_PARAMETERS_TABLE_AS_LIST_ITEM)
};

const app_gsdml_module_t *app_gsdml_get_module_cfg(uint32_t id)
{
    uint32_t i;
    APP_LOG_DEBUG("%s\n", __func__);

    /**
     * TODO:
     * 
     * Solve the module by it's index with complexity O(1)
     * 
     */

    for (i = 0; i < NELEMENTS(app_gsdml_modules); i++)
    {
        if (app_gsdml_modules[i]->id == id)
        {
            return app_gsdml_modules[i];
        }
    }
    return NULL;
}

const app_gsdml_submodule_t *app_gsdml_get_submodule_cfg(uint32_t id)
{
    uint32_t i;
    APP_LOG_DEBUG("%s\n", __func__);

    /**
     * TODO:
     * 
     * Solve the submodule by it's index with complexity O(1)
     * 
     */

    for (i = 0; i < NELEMENTS(app_gsdml_submodules); i++)
    {
        if (app_gsdml_submodules[i]->id == id)
        {
            return app_gsdml_submodules[i];
        }
    }
    return NULL;
}

const app_gsdml_param_t *app_gsdml_get_parameter_cfg(
    uint32_t submodule_id,
    uint32_t index)
{
    uint16_t i;
    uint16_t j;

    APP_LOG_DEBUG("%s\n", __func__);

    const app_gsdml_submodule_t *submodule_cfg =
        app_gsdml_get_submodule_cfg(submodule_id);

    if (submodule_cfg == NULL)
    {
        /* Unsupported submodule id */
        return NULL;
    }

    /**
     * TODO:
     * 
     * Solve the parameter by it's index with complexity O(1)
     * 
     */

    /* Search for parameter index in submodule configuration */
    for (i = 0; submodule_cfg->parameters[i] != 0; i++)
    {
        if (submodule_cfg->parameters[i] == index)
        {
            /* Find parameter configuration */
            for (j = 0; j < NELEMENTS(app_gsdml_parameters); j++)
            {
                if (app_gsdml_parameters[j]->index == index)
                {
                    return app_gsdml_parameters[j];
                }
            }
        }
    }

    return NULL;
}

int app_gsdml_default_param_set(void* arg, void *value, uint16_t size)
{

    app_gsdml_param_t* param = (app_gsdml_param_t*) arg;

    if (arg == NULL)
    {
        return -1;
    }

    if (value == NULL)
    {
        return -1;
    }

    if (size < param->length)
    {
        return -1;
    }

    memcpy(param->container, value, param->length);

    return 0;
}

int app_gsdml_default_param_get(void* arg, void **value, uint16_t *size)
{
    app_gsdml_param_t* param = (app_gsdml_param_t*) arg;

    if (value == NULL)
    {
        return -1;
    }

    if (*size < param->length)
    {
        return -1;
    }
    *size = param->length;
    // it get's the pointer to the storage locations instead of giving a buffer
    // memcpy(*value, param->container, param->length);
    *value = param->container;

    return 0;
}