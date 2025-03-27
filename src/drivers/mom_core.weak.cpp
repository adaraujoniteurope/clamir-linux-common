#include <nit/embedded/common/defs.h>
#include <nit/embedded/drivers/mb_core.h>
#include <nit/embedded/drivers/mom_core.h>
#include <nit/embedded/utils/config_file.h>

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

NIT_MOM_CORE_FIELD_TABLE(NIT_MOM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION)