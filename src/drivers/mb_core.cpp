#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#include <nit/embedded/drivers/common.h>
#include <nit/embedded/drivers/mb_core.h>

#include <nit/embedded/utils/config_file.h>

nit_mb_core_state_t nit_mb_core_driver = {0};

int nit_mb_core_open(nit_mb_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }
    state->is_open = false;
    
    state->fd = -1;

    state->fd = open("/dev/mem", O_RDWR | O_SYNC);

    if (state->fd < 0)
    {
        return -2;
    }

    state->priv = (volatile int *)mmap(NULL, NIT_MB_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_MB_CORE_BASE_ADDRESS);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;

    return 0;
}
int nit_mb_core_close(nit_mb_core_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open == true)
    {
        return 0;
    }

    state->is_open = false;

    if (state->priv != NULL)
    {
        munmap((void *)state->priv, NIT_MB_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_mb_core_state_assert(nit_mb_core_state_t *state)
{

    if (state == NULL)
    {
        return -1;
    }

    if (state->priv == NULL)
    {
        return -2;
    }

    if (state->is_open == false)
    {
        return -3;
    }

    return 0;
}

#include <nit/embedded/drivers/gen_core.h>
#include <nit/embedded/drivers/mom_core.h>
#include <nit/embedded/drivers/pwm_core.h>
#include <nit/embedded/drivers/roi_core.h>

int nit_mb_core_config_save_to_file(nit_mb_core_state_t* state, const char *path)
{

    nit_gen_core_version_id_get(state, &state->config.version_id);
    nit_gen_core_enable_mom_get(state, &state->config.enable_mom);
    nit_gen_core_enable_roi_get(state, &state->config.enable_roi);
    nit_gen_core_enable_pwm_get(state, &state->config.enable_pwm);
    nit_gen_core_enable_irq_get(state, &state->config.enable_irq);
    nit_gen_core_change_op_mode_get(state, &state->config.change_op_mode);
    nit_gen_core_irq_microblaze_get(state, &state->config.irq_microblaze);
    nit_gen_core_digital_in_0_get(state, &state->config.digital_in_0);
    nit_gen_core_digital_in_1_get(state, &state->config.digital_in_1);
    nit_gen_core_digital_out_0_get(state, &state->config.digital_out_0);
    nit_gen_core_digital_out_1_get(state, &state->config.digital_out_1);
    nit_gen_core_digital_out_conf_get(state, &state->config.digital_out_conf);
    nit_gen_core_digital_in_2_get(state, &state->config.digital_in_2);
    nit_gen_core_digital_in_3_get(state, &state->config.digital_in_3);
    nit_gen_core_digital_out_2_get(state, &state->config.digital_out_2);
    nit_gen_core_digital_out_3_get(state, &state->config.digital_out_3);

    nit_mom_core_reference_track_get(state, &state->config.reference_track);
    nit_mom_core_end_of_track_get(state, &state->config.end_of_track);
    nit_mom_core_start_track_mom_t_get(state, &state->config.start_track_mom_t);
    nit_mom_core_threshold_get(state, &state->config.threshold);
    nit_mom_core_time_track_high_get(state, &state->config.time_track_high);
    nit_mom_core_time_track_low_get(state, &state->config.time_track_low);
    nit_mom_core_mode_get(state, &state->config.mode);

    nit_pwm_core_pwm_limit_max_get(state, &state->config.pwm_limit_max);
    nit_pwm_core_pwm_limit_min_get(state, &state->config.pwm_limit_min);
    nit_pwm_core_pwm_get(state, &state->config.pwm);

    nit_roi_core_round_get(state, &state->config.round);
    nit_roi_core_x1_get(state, &state->config.x1);
    nit_roi_core_x2_get(state, &state->config.x2);
    nit_roi_core_y1_get(state, &state->config.y1);
    nit_roi_core_y2_get(state, &state->config.y2);

    return config_file_save_to_file(state, path);
}

int nit_mb_core_config_load_from_file(nit_mb_core_state_t* state, const char *path)
{

    auto retval = config_file_load_from_file(state, path);

    if (retval < 0) {
        return retval;
    }

    nit_gen_core_version_id_set(state, state->config.version_id);
    nit_gen_core_enable_mom_set(state, state->config.enable_mom);
    nit_gen_core_enable_roi_set(state, state->config.enable_roi);
    nit_gen_core_enable_pwm_set(state, state->config.enable_pwm);
    nit_gen_core_enable_irq_set(state, state->config.enable_irq);
    nit_gen_core_change_op_mode_set(state, state->config.change_op_mode);
    nit_gen_core_irq_microblaze_set(state, state->config.irq_microblaze);
    nit_gen_core_digital_in_0_set(state, state->config.digital_in_0);
    nit_gen_core_digital_in_1_set(state, state->config.digital_in_1);
    nit_gen_core_digital_out_0_set(state, state->config.digital_out_0);
    nit_gen_core_digital_out_1_set(state, state->config.digital_out_1);
    nit_gen_core_digital_out_conf_set(state, state->config.digital_out_conf);
    nit_gen_core_digital_in_2_set(state, state->config.digital_in_2);
    nit_gen_core_digital_in_3_set(state, state->config.digital_in_3);
    nit_gen_core_digital_out_2_set(state, state->config.digital_out_2);
    nit_gen_core_digital_out_3_set(state, state->config.digital_out_3);

    nit_mom_core_reference_track_set(state, state->config.reference_track);
    nit_mom_core_end_of_track_set(state, state->config.end_of_track);
    nit_mom_core_start_track_mom_t_set(state, state->config.start_track_mom_t);
    nit_mom_core_threshold_set(state, state->config.threshold);
    nit_mom_core_time_track_high_set(state, state->config.time_track_high);
    nit_mom_core_time_track_low_set(state, state->config.time_track_low);
    nit_mom_core_mode_set(state, state->config.mode);

    nit_pwm_core_pwm_limit_max_set(state, state->config.pwm_limit_max);
    nit_pwm_core_pwm_limit_min_set(state, state->config.pwm_limit_min);

    nit_roi_core_round_set(state, state->config.round);
    nit_roi_core_x1_set(state, state->config.x1);
    nit_roi_core_x2_set(state, state->config.x2);
    nit_roi_core_y1_set(state, state->config.y1);
    nit_roi_core_y2_set(state, state->config.y2);

    return retval;
}