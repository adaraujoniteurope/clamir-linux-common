#include "mb_core.h"

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

int mb_core_open(mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }
    
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

    return 0;
}

int mb_core_close(mb_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    if (state->priv != NULL) {
        munmap((void*)state->priv, NIT_MB_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }
    
    return 0;
}

void nit_mb_core_change_op_mode_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_CHANGE_OP_MODE_OFFSET) = value;
}

uint32_t nit_mb_core_change_op_mode_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_CHANGE_OP_MODE_OFFSET);
}


void nit_mb_core_digital_in_0_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_IN_0_OFFSET) = value;
}

uint32_t nit_mb_core_digital_in_0_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_IN_0_OFFSET);
}


void nit_mb_core_digital_in_1_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_IN_1_OFFSET) = value;
}

uint32_t nit_mb_core_digital_in_1_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_IN_1_OFFSET);
}


void nit_mb_core_digital_out_0_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_0_OFFSET) = value;
}

uint32_t nit_mb_core_digital_out_0_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_0_OFFSET);
}


void nit_mb_core_digital_out_1_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_1_OFFSET) = value;
}

uint32_t nit_mb_core_digital_out_1_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_1_OFFSET);
}


void nit_mb_core_digital_out_2_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_2_OFFSET) = value;
}

uint32_t nit_mb_core_digital_out_2_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_2_OFFSET);
}


void nit_mb_core_digital_out_3_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_3_OFFSET) = value;
}

uint32_t nit_mb_core_digital_out_3_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_3_OFFSET);
}


void nit_mb_core_digital_out_conf_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_CONF_OFFSET) = value;
}

uint32_t nit_mb_core_digital_out_conf_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_DIGITAL_OUT_CONF_OFFSET);
}


void nit_mb_core_enable_roi_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_ENABLE_ROI_OFFSET) = value;
}

uint32_t nit_mb_core_enable_roi_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_ENABLE_ROI_OFFSET);
}


void nit_mb_core_end_of_track_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_END_OF_TRACK_OFFSET) = value;
}

uint32_t nit_mb_core_end_of_track_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_END_OF_TRACK_OFFSET);
}


void nit_mb_core_mode_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) +NIT_MB_CORE_MODE_OFFSET) = value;
}

uint32_t nit_mb_core_mode_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) +NIT_MB_CORE_MODE_OFFSET);
}


void nit_mb_core_pwm_limit_max_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_PWM_LIMIT_MAX_OFFSET) = value;
}

uint32_t nit_mb_core_pwm_limit_max_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_PWM_LIMIT_MAX_OFFSET);
}


void nit_mb_core_pwm_limit_min_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_PWM_LIMIT_MIN_OFFSET) = value;
}

uint32_t nit_mb_core_pwm_limit_min_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_PWM_LIMIT_MIN_OFFSET);
}


void nit_mb_core_pwm_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) +NIT_MB_CORE_PWM_OFFSET) = value;
}

uint32_t nit_mb_core_pwm_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) +NIT_MB_CORE_PWM_OFFSET);
}


void nit_mb_core_reference_track_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_REFERENCE_TRACK_OFFSET) = value;
}

uint32_t nit_mb_core_reference_track_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_REFERENCE_TRACK_OFFSET);
}


void nit_mb_core_roi_round_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_ROI_ROUND_OFFSET) = value;
}

uint32_t nit_mb_core_roi_round_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_ROI_ROUND_OFFSET);
}


void nit_mb_core_roi_x1_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_X1_OFFSET) = value;
}

uint32_t nit_mb_core_roi_x1_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_X1_OFFSET);
}


void nit_mb_core_roi_x2_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_X2_OFFSET ) = value;
}

uint32_t nit_mb_core_roi_x2_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_X2_OFFSET );
}


void nit_mb_core_roi_y1_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_Y1_OFFSET ) = value;
}

uint32_t nit_mb_core_roi_y1_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_Y1_OFFSET );
}


void nit_mb_core_roi_y2_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_Y2_OFFSET ) = value;
}

uint32_t nit_mb_core_roi_y2_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv)+ NIT_MB_CORE_ROI_Y2_OFFSET );
}


void nit_mb_core_start_track_mom_t_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_START_TRACK_MOM_T_OFFSET) = value;
}

uint32_t nit_mb_core_start_track_mom_t_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_START_TRACK_MOM_T_OFFSET);
}


void nit_mb_core_threshold_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_THRESHOLD_OFFSET) = value;
}

uint32_t nit_mb_core_threshold_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_THRESHOLD_OFFSET);
}


void nit_mb_core_time_track_high_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_TIME_TRACK_HIGH_OFFSET) = value;
}

uint32_t nit_mb_core_time_track_high_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_TIME_TRACK_HIGH_OFFSET);
}


void nit_mb_core_time_track_low_set(mb_core_state_t* state, uint32_t value)
{
    *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_TIME_TRACK_LOW_OFFSET) = value;
}

uint32_t nit_mb_core_time_track_low_get(mb_core_state_t* state)
{
    return *(uint32_t*)(((uint32_t*)state->priv) + NIT_MB_CORE_TIME_TRACK_LOW_OFFSET);
}
