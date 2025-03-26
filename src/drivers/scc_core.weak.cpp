#include "drivers/scc_core.h"
#include "utils/config_file.h"
#include "math.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

NIT_SCC_CORE_FIELD_TABLE(NIT_SCC_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION)