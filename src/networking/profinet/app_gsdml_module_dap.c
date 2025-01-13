#include "app_data.h"
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
#include "app_gsdml_module_dap.h"

/******************* Supported modules ***************************/

PNET_SUBMOD_DAP_IDENT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

PNET_SUBMOD_DAP_IDENT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

PNET_SUBMOD_DAP_INTERFACE_1_PORT_1_IDENT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

PNET_SUBMOD_DAP_INTERFACE_1_PORT_2_IDENT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

PNET_SUBMOD_DAP_INTERFACE_1_PORT_3_IDENT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

PNET_SUBMOD_DAP_INTERFACE_1_PORT_4_IDENT_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

GSDML_DAP_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(PNET_MOD_DAP_IDENT, dap_1, "DAP 1", GSDML_DAP_SUBMODULES_TABLE)

static uint32_t dap_identity_1_value = 0;
static uint16_t dap_identity_1_value_size = sizeof(dap_identity_1_value);

int dap_identity_1_get(void* arg, void* data, uint16_t* size)
{
    app_gsdml_submodule_t* context = (app_gsdml_submodule_t*) arg;
    data = &dap_identity_1_value;
    size = &dap_identity_1_value_size;
    return 0;
}

int dap_identity_1_set(void* arg, void* data, uint16_t size)
{

    if (size != dap_identity_1_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    dap_identity_1_value = *(uint8_t*) data;
    return 0;
}

static uint32_t dap_interface_1_value = 0;
static uint16_t dap_interface_1_value_size = sizeof(dap_interface_1_value);

int dap_interface_1_get(void* arg, void* data, uint16_t* size)
{
    data = &dap_interface_1_value;
    size = &dap_interface_1_value_size;
    return 0;
}

int dap_interface_1_set(void* arg, void* data, uint16_t size)
{

    if (size != dap_interface_1_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    dap_interface_1_value = *(uint8_t*) data;
    return 0;
}


static uint32_t dap_port_1_value = 0;
static uint16_t dap_port_1_value_size = sizeof(dap_port_1_value);

int dap_port_1_get(void* arg, void* data, uint16_t* size)
{
    data = &dap_port_1_value;
    size = &dap_port_1_value_size;
    return 0;
}

int dap_port_1_set(void* arg, void* data, uint16_t size)
{

    if (size != dap_port_1_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    dap_port_1_value = *(uint8_t*) data;
    return 0;
}

static uint32_t dap_port_2_value = 0;
static uint16_t dap_port_2_value_size = sizeof(dap_port_2_value);

int dap_port_2_get(void* arg, void* data, uint16_t* size)
{
    data = &dap_port_2_value;
    size = &dap_port_2_value_size;
    return 0;
}

int dap_port_2_set(void* arg, void* data, uint16_t size)
{

    if (size != dap_port_2_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    dap_port_2_value = *(uint8_t*) data;
    return 0;
}

static uint32_t dap_port_3_value = 0;
static uint16_t dap_port_3_value_size = sizeof(dap_port_3_value);

int dap_port_3_get(void* arg, void* data, uint16_t* size)
{
    data = &dap_port_3_value;
    size = &dap_port_3_value_size;
    return 0;
}

int dap_port_3_set(void* arg, void* data, uint16_t size)
{

    if (size != dap_port_3_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    dap_port_3_value = *(uint8_t*) data;
    return 0;
}

static uint32_t dap_port_4_value = 0;
static uint16_t dap_port_4_value_size = sizeof(dap_port_4_value);

int dap_port_4_get(void* arg, void* data, uint16_t* size)
{
    data = &dap_port_4_value;
    size = &dap_port_4_value_size;
    return 0;
}

int dap_port_4_set(void* arg, void* data, uint16_t size)
{

    if (size != dap_port_4_value_size)
    {
        return -1;
    }

    if (data == NULL)
    {
        return -1;
    }

    dap_port_4_value = *(uint8_t*) data;
    return 0;
}