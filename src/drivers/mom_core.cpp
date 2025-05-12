#include <nit/embedded/common/defs.h>
#include <nit/embedded/drivers/mb_core.h>
#include <nit/embedded/drivers/mom_core.h>
#include <nit/embedded/utils/config_file.h>
#include <nit/embedded/utils/memory_map.hpp>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace utils;

int mom_core_open(nit_mb_core_state_t *state) {
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
      NULL, NIT_MOM_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd,
      NIT_MOM_CORE_BASE_ADDRESS);

  if (state->priv == NULL) {
    close(state->fd);
    return -3;
  }

  state->is_open = true;
  return 0;
}

int mom_core_close(nit_mb_core_state_t *state) {
  if (state == NULL) {
    return -1;
  }

  if (state->is_open == false) {
    return 0;
  }

  state->is_open = false;

  if (state->priv != NULL) {
    memory_map_close((void *)state->priv, NIT_MOM_CORE_SIZE);
  }

  state->is_open = false;

  if (state->fd >= 0) {
    close(state->fd);
  }

  return 0;
}

int mom_core_config_save_to_file(nit_mom_core_state_t *state,
                                 const char *path) {
  return config_file_save_to_file(state, path);
}

int mom_core_config_load_from_file(nit_mom_core_state_t *state,
                                   const char *path) {
  return config_file_load_from_file(state, path);
}

int nit_mom_core_reference_track_set(nit_mb_core_state_t *state,
                                     uint16_t value) {
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_reference_track_offset, value);

  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) +
    nit_mom_core_reference_track_offset) = value;
  return retval;
}

int nit_mom_core_reference_track_get(nit_mb_core_state_t *state,
                                     uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) +
             nit_mom_core_reference_track_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_reference_track_offset, *value);
  return retval;
}

int nit_mom_core_end_of_track_set(nit_mb_core_state_t *state, uint16_t value) {
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_end_of_track_offset, value);

  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) + nit_mom_core_end_of_track_offset) =
      value;
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_end_of_track_offset, value);
  return retval;
}

int nit_mom_core_end_of_track_get(nit_mb_core_state_t *state, uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) +
             nit_mom_core_end_of_track_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_end_of_track_offset, *value);
  return retval;
}

int nit_mom_core_start_track_mom_t_set(nit_mb_core_state_t *state,
                                       uint16_t value) {
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_start_track_mom_t_offset, value);

  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) +
    nit_mom_core_start_track_mom_t_offset) = value;
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_start_track_mom_t_offset, value);
  return retval;
}

int nit_mom_core_start_track_mom_t_get(nit_mb_core_state_t *state,
                                       uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) +
             nit_mom_core_start_track_mom_t_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_start_track_mom_t_offset, *value);
  return retval;
}

int nit_mom_core_threshold_set(nit_mb_core_state_t *state, uint16_t value) {
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__, nit_mom_core_threshold_offset,
         value);

  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) + nit_mom_core_threshold_offset) =
      value;
  return retval;
}

int nit_mom_core_threshold_get(nit_mb_core_state_t *state, uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value =
      *((volatile uint32_t *)((state->priv)) + nit_mom_core_threshold_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__, nit_mom_core_threshold_offset,
         *value);
  return retval;
}

int nit_mom_core_time_track_high_set(nit_mb_core_state_t *state,
                                     uint32_t value) {
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_time_track_high_offset, value);

  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) +
    nit_mom_core_time_track_high_offset) = value;
  return retval;
}

int nit_mom_core_time_track_high_get(nit_mb_core_state_t *state,
                                     uint32_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) +
             nit_mom_core_time_track_high_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_time_track_high_offset, *value);
  return retval;
}

int nit_mom_core_time_track_low_set(nit_mb_core_state_t *state,
                                    uint32_t value) {
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_time_track_low_offset, value);

  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) + nit_mom_core_time_track_low_offset) =
      value;
  return retval;
}

int nit_mom_core_time_track_low_get(nit_mb_core_state_t *state,
                                    uint32_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) +
             nit_mom_core_time_track_low_offset);
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__,
         nit_mom_core_time_track_low_offset, *value);
  return retval;
}

int nit_mom_core_mode_set(nit_mb_core_state_t *state, uint16_t value) {
  syslog(LOG_INFO, "%s: (0x%02x) %d\n", __func__, nit_mom_core_mode_offset,
         value);

  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *((volatile uint32_t *)((state->priv)) + nit_mom_core_mode_offset) = value;
  return retval;
}

int nit_mom_core_mode_get(nit_mb_core_state_t *state, uint16_t *value) {
  int retval = 0;

  if ((retval = nit_mb_core_state_assert(state)) != 0) {
    syslog(LOG_INFO, "%s: %s %d\n", __func__, "failed", retval);
    return retval;
  }

  *value = *((volatile uint32_t *)((state->priv)) + nit_mom_core_mode_offset);
  return retval;
}
