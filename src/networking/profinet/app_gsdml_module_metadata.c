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
#include "app_gsdml_module_metadata.h"

APP_GSDML_SUBMOD_ID_METADATA_PARAMETERS_TABLE(GSDML_PARAMETER_DEFINE)

APP_GSDML_METADATA_SUBMODULES_TABLE(GSDML_SUBMODULE_DEFINE)

GSDML_MODULE(APP_GSDML_MOD_ID_METADATA, module_metadata, "METADATA", APP_GSDML_METADATA_SUBMODULES_TABLE)

int submodule_metadata_set(void* submodule, void *data, uint16_t size)
{
    return 0;
}

int submodule_metadata_get(void* submodule, void *data, uint16_t *size)
{
    return 0;
}