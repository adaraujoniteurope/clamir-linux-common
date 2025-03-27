#include <nit/embedded/drivers/control_unit_core.h>
#include <nit/embedded/utils/config_file.h>
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

NIT_CONTROL_UNIT_CORE_FIELD_TABLE(NIT_CONTROL_UNIT_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION)