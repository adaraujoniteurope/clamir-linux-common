#include <stddef.h>

#include "mb_core.h"

// NIT_MB_CORE_CHANGE_OP_MODE_OFFSET = 0x0000 + 5,
// NIT_MB_CORE_DIGITAL_IN_0_OFFSET = 0x0000 + 7,
// NIT_MB_CORE_DIGITAL_IN_1_OFFSET = 0x0000 + 8,
// NIT_MB_CORE_DIGITAL_OUT_0_OFFSET = 0x0000 + 9,
// NIT_MB_CORE_DIGITAL_OUT_1_OFFSET = 0x0000 + 10,
// NIT_MB_CORE_DIGITAL_OUT_2_OFFSET = 0x0000 + 14,
// NIT_MB_CORE_DIGITAL_OUT_3_OFFSET = 0x0000 + 15,
// NIT_MB_CORE_DIGITAL_OUT_CONF_OFFSET = 0x0000 + 11,
// NIT_MB_CORE_ENABLE_ROI_OFFSET = 0x0000 + 2,

int mb_core_open(mb_core_state* state)
{
    return 0;
}

int mb_core_close(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_change_op_mode_set(mb_core_state* state)
{
    if (state == NULL) {
        return -1;
    }


    if (state->priv == NULL)
    {
        return -2;
    }
    
    ((volatile unsigned int*)(state->priv))[NIT_MB_CORE_CHANGE_OP_MODE_OFFSET] = 1;

    return 0;
}

int nit_mb_core_change_op_mode_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_digital_in_0_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_digital_in_0_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_digital_in_1_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_digital_in_1_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_digital_out_0_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_digital_out_0_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_digital_out_1_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_digital_out_1_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_digital_out_2_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_digital_out_2_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_digital_out_3_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_digital_out_3_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_digital_out_conf_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_digital_out_conf_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_enable_roi_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_enable_roi_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_end_of_track_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_end_of_track_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_mode_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_mode_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_pwm_limit_max_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_pwm_limit_max_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_pwm_limit_min_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_pwm_limit_min_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_pwm_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_pwm_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_reference_track_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_reference_track_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_roi_round_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_roi_round_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_roi_x1_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_roi_x1_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_roi_x2_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_roi_x2_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_roi_y1_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_roi_y1_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_roi_y2_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_roi_y2_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_start_track_mom_t_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_start_track_mom_t_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_threshold_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_threshold_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_time_track_high_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_time_track_high_get(mb_core_state* state)
{
    return 0;
}


int nit_mb_core_time_track_low_set(mb_core_state* state)
{
    return 0;
}

int nit_mb_core_time_track_low_get(mb_core_state* state)
{
    return 0;
}
