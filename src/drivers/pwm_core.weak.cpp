#include <nit/embedded/common/defs.h>
#include <nit/embedded/drivers/mb_core.h>
#include <nit/embedded/drivers/pwm_core.h>
#include <nit/embedded/utils/config_file.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

NIT_PWM_CORE_FIELD_TABLE(NIT_PWM_CORE_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION)