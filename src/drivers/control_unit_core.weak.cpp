#include "math.h"
#include <nit/embedded/drivers/control_unit_core.h>
#include <nit/embedded/drivers/config_file.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

NIT_CONTROL_UNIT_CORE_FIELD_TABLE(
    NIT_CONTROL_UNIT_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION)