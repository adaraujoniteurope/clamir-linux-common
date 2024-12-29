#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_MB_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_MB_CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define NIT_MB_CORE_BASE_ADDRESS    0x50000000
#define NIT_MB_CORE_SIZE            0x00500008

/*
 * Bank 0 "GEN" offsets 0x0000
 */
#define VERSION_ID 0x0000 + 0
#define ENABLE_MOM 0x0000 + 1
#define ENABLE_ROI 0x0000 + 2
#define ENABLE_PWM 0x0000 + 3
#define ENABLE_IRQ 0x0000 + 4
#define CHANGE_OP_MODE 0x0000 + 5
#define IRQ_MICROBLAZE 0x0000 + 6
#define DIGITAL_IN_0 0x0000 + 7
#define DIGITAL_IN_1 0x0000 + 8
#define DIGITAL_OUT_0 0x0000 + 9
#define DIGITAL_OUT_1 0x0000 + 10
#define DIGITAL_OUT_CONF 0x0000 + 11
#define DIGITAL_IN_2 0x0000 + 12
#define DIGITAL_IN_3 0x0000 + 13
#define DIGITAL_OUT_2 0x0000 + 14
#define DIGITAL_OUT_3 0x0000 + 15

/*
 * Bank 1 "MOM" offsets 0x4000
 */
#define THRESHOLD 0x4000 + 0
#define INTENSITY_MIN 0x4000 + 1
#define INTENSITY_MAX 0x4000 + 2
#define MOM_00 0x4000 + 3
#define MOM_01 0x4000 + 4
#define MOM_10 0x4000 + 5
#define MOM_11 0x4000 + 6
#define MOM_02 0x4000 + 7
#define MOM_20 0x4000 + 8
#define START_TRACK_MOM_T 0x4000 + 9
#define TRACK_NBR 0x4000 + 10
#define MODE 0x4000 + 11
#define TIME_TRACK_LOW 0x4000 + 12
#define TIME_TRACK_HIGH 0x4000 + 13
#define FRAME_MAX 0x4000 + 14
#define REFERENCE_TRACK 0x4000 + 15
#define END_OF_TRACK 0x4000 + 16

/*
 *  Bank 2 "ROI" offsets 0x8000
 */
#define ROI_X1 0x8000 + 0
#define ROI_Y1 0x8000 + 1
#define ROI_X2 0x8000 + 2
#define ROI_Y2 0x8000 + 3
#define ROI_ROUND 0x8000 + 4

/*
 *  Bank 3 " PWM" offsets 0xC000
 */

#define PWM_LIMIT_MAX 0xC000 + 0
#define PWM 0xC000 + 1
#define POWER 0xC000 + 2
#define PWM_LIMIT_MIN 0xC000 + 3

/*
 *  Bank 0 AIMEN_STARTUP "RST" offsets 0x0000
 *  Resets y modos de debug en memoria shmemReset
 */

#define IP_OP_MODE 0x0000 + 0
#define IRQ_ARM 0x0000 + 1
#define IRQ_DEBUG 0x0000 + 2
#define FRAME_NBR_RST 0x0000 + 3
#define TIMESTAMP_RST 0x0000 + 4
#define SOFT_RST 0x0000 + 5

#include "inttypes.h"

    typedef enum mb_core_register_offset_enum
    {
        /**
         * GEN Peripheral
         */
        NIT_MB_CORE_CHANGE_OP_MODE_OFFSET = CHANGE_OP_MODE,
        NIT_MB_CORE_DIGITAL_IN_0_OFFSET = DIGITAL_IN_0,
        NIT_MB_CORE_DIGITAL_IN_1_OFFSET = DIGITAL_IN_1,
        NIT_MB_CORE_DIGITAL_OUT_0_OFFSET = DIGITAL_OUT_0,
        NIT_MB_CORE_DIGITAL_OUT_1_OFFSET = DIGITAL_OUT_1,
        NIT_MB_CORE_DIGITAL_OUT_2_OFFSET = DIGITAL_OUT_2,
        NIT_MB_CORE_DIGITAL_OUT_3_OFFSET = DIGITAL_OUT_3,
        NIT_MB_CORE_DIGITAL_OUT_CONF_OFFSET = DIGITAL_OUT_CONF,
        NIT_MB_CORE_ENABLE_ROI_OFFSET = ENABLE_ROI,

        /**
         * MOM Peripheral
         */
        NIT_MB_CORE_REFERENCE_TRACK_OFFSET = REFERENCE_TRACK,
        NIT_MB_CORE_END_OF_TRACK_OFFSET = END_OF_TRACK,
        NIT_MB_CORE_START_TRACK_MOM_T_OFFSET = START_TRACK_MOM_T,
        NIT_MB_CORE_THRESHOLD_OFFSET = THRESHOLD,
        NIT_MB_CORE_TIME_TRACK_HIGH_OFFSET = TIME_TRACK_HIGH,
        NIT_MB_CORE_TIME_TRACK_LOW_OFFSET = TIME_TRACK_LOW,
        NIT_MB_CORE_MODE_OFFSET = MODE,

        /**
         * ROI Peripheral
         */
        NIT_MB_CORE_ROI_ROUND_OFFSET = ROI_ROUND,
        NIT_MB_CORE_ROI_X1_OFFSET = ROI_X1,
        NIT_MB_CORE_ROI_X2_OFFSET = ROI_X2,
        NIT_MB_CORE_ROI_Y1_OFFSET = ROI_Y1,
        NIT_MB_CORE_ROI_Y2_OFFSET = ROI_Y2,

        /**
         * PWM Peripheral
         */
        NIT_MB_CORE_PWM_LIMIT_MAX_OFFSET = PWM_LIMIT_MAX,
        NIT_MB_CORE_PWM_LIMIT_MIN_OFFSET = PWM_LIMIT_MIN,
        NIT_MB_CORE_PWM_OFFSET = PWM,

    } mb_core_register_offset;

    typedef struct mb_core_state_struct
    {
        int fd;
        volatile void *priv;
    } mb_core_state_t;

    uint16_t mb_core_open(mb_core_state_t *state);

    uint16_t mb_core_close(mb_core_state_t *state);

    void nit_mb_core_change_op_mode_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_change_op_mode_get(mb_core_state_t *state);

    void nit_mb_core_digital_in_0_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_digital_in_0_get(mb_core_state_t *state);

    void nit_mb_core_digital_in_1_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_digital_in_1_get(mb_core_state_t *state);

    void nit_mb_core_digital_out_0_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_digital_out_0_get(mb_core_state_t *state);

    void nit_mb_core_digital_out_1_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_digital_out_1_get(mb_core_state_t *state);

    void nit_mb_core_digital_out_2_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_digital_out_2_get(mb_core_state_t *state);

    void nit_mb_core_digital_out_3_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_digital_out_3_get(mb_core_state_t *state);

    void nit_mb_core_digital_out_conf_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_digital_out_conf_get(mb_core_state_t *state);

    void nit_mb_core_enable_roi_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_enable_roi_get(mb_core_state_t *state);

    void nit_mb_core_end_of_track_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_end_of_track_get(mb_core_state_t *state);

    void nit_mb_core_mode_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_mode_get(mb_core_state_t *state);

    void nit_mb_core_pwm_limit_max_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_pwm_limit_max_get(mb_core_state_t *state);

    void nit_mb_core_pwm_limit_min_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_pwm_limit_min_get(mb_core_state_t *state);

    void nit_mb_core_pwm_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_pwm_get(mb_core_state_t *state);

    void nit_mb_core_reference_track_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_reference_track_get(mb_core_state_t *state);

    void nit_mb_core_roi_round_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_roi_round_get(mb_core_state_t *state);

    void nit_mb_core_roi_x1_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_roi_x1_get(mb_core_state_t *state);

    void nit_mb_core_roi_x2_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_roi_x2_get(mb_core_state_t *state);

    void nit_mb_core_roi_y1_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_roi_y1_get(mb_core_state_t *state);

    void nit_mb_core_roi_y2_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_roi_y2_get(mb_core_state_t *state);

    void nit_mb_core_start_track_mom_t_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_start_track_mom_t_get(mb_core_state_t *state);

    void nit_mb_core_threshold_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_threshold_get(mb_core_state_t *state);

    void nit_mb_core_time_track_high_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_time_track_high_get(mb_core_state_t *state);

    void nit_mb_core_time_track_low_set(mb_core_state_t *state, uint16_t value);

    uint16_t nit_mb_core_time_track_low_get(mb_core_state_t *state);

#ifdef __cplusplus
}
#endif

#endif