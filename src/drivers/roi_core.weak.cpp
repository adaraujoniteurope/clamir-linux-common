
#include <nit/embedded/drivers/mb_core.h>
#include <nit/embedded/drivers/roi_core.h>
#include <nit/embedded/drivers/config_file.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

NIT_ROI_CORE_FIELD_TABLE(NIT_ROI_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION)