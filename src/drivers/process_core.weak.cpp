#include "common/defs.h"
#include "drivers/common.h"
#include "drivers/process_core_field_table.h"
#include "drivers/process_core.h"
#include "utils/config_file.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

NIT_PROCESS_CORE_FIELD_TABLE(NIT_PROCESS_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION)