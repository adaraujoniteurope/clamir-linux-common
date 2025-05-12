#include <nit/embedded/common/defs.h>
#include <nit/embedded/drivers/mb_core.h>
#include <nit/embedded/drivers/pwm_core.h>
#include <nit/embedded/utils/config_file.h>
#include <nit/embedded/utils/memory_map.hpp>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

const nit_pwm_core_config_t pwm_core_config_default = {

};

using namespace utils;

int pwm_core_open(nit_mb_core_state_t *state) {
  if (state == NULL) {
    return -1;
  }

  if (state->is_open == true) {
    return 0;
  }

  state->is_open = false;

  state->fd = -1;

  state->fd = open("/dev/mem", O_RDWR | O_SYNC);

  state->priv = (volatile int *)memory_map_open(
      NULL, NIT_PWM_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd,
      NIT_PWM_CORE_BASE_ADDRESS);

  if (state->priv == NULL) {
    close(state->fd);
    return -3;
  }

  state->is_open = true;

  return 0;
}

int pwm_core_close(nit_mb_core_state_t *state) {
  if (state == NULL) {
    return -1;
  }

  if (state->is_open == false) {
    return 0;
  }

  state->is_open = false;

  if (state->priv != NULL) {
    memory_map_close((void *)state->priv, NIT_PWM_CORE_BASE_ADDRESS);
  }

  if (state->fd >= 0) {
    close(state->fd);
  }

  return 0;
}

int pwm_core_config_save_to_file(nit_pwm_core_state_t *state,
                                 const char *path) {
  return config_file_save_to_file(state, path);
}

int pwm_core_config_load_from_file(nit_pwm_core_state_t *state,
                                   const char *path) {
  return config_file_load_from_file(state, path);
}

int nit_pwm_core_pwm_limit_max_set(nit_mb_core_state_t *state, uint16_t value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) + nit_pwm_core_pwm_limit_max_offset) =
      value;
  return retval;
}

int nit_pwm_core_pwm_limit_max_get(nit_mb_core_state_t *state,
                                   uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) +
             nit_pwm_core_pwm_limit_max_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_pwm_core_pwm_limit_max_offset, *value);
  return retval;
}

int nit_pwm_core_pwm_limit_min_set(nit_mb_core_state_t *state, uint16_t value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) + nit_pwm_core_pwm_limit_min_offset) =
      value;
  return retval;
}

int nit_pwm_core_pwm_limit_min_get(nit_mb_core_state_t *state,
                                   uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) +
             nit_pwm_core_pwm_limit_min_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_pwm_core_pwm_limit_min_offset, *value);
  return retval;
}

int nit_pwm_core_pwm_set(nit_mb_core_state_t *state, uint16_t value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) + nit_pwm_core_pwm_offset) = value;
  return retval;
}

int nit_pwm_core_pwm_get(nit_mb_core_state_t *state, uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) + nit_pwm_core_pwm_offset);
  syslog(LOG_INFO, "%s: (0x%04x): %d\n", __func__, nit_pwm_core_pwm_offset,
         *value);
  return retval;
}