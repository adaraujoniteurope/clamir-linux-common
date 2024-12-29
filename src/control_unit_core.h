#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_CONTROL_UNIT_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_CONTROL_UNIT_CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define NIT_CONTROL_UNIT_BASE_ADDRESS   0x40000000
#define NIT_CONTROL_UNIT_SIZE           0x00000020

#define INT_TIME 0			  // 0
#define BIAS_V 1			  // 4
#define OFFSET_EN 2			  // 8
#define OFFSET_UPDATE 3		  // 12
#define SHUTTER 4			  // 16
#define BPC_EN 5			  // 20
#define BPC_MEM_WRITE 6		  // 24
#define BPC_IDENTIFY 7		  // 28
#define TEMP1 8				  // 32
#define TEMP2 9				  // 36
#define TEMP3 10			  // 40
#define TEMP4 11			  // 44
#define SHUTTER_RESET 12	  // 48
#define SINCRONIZATION 13	  // 52
#define SAVE_EMBEDDED_CONF 14 // 56
#define ARM_SW_VERSION 15	  // 60
#define DRIFT_ENABLE 16		  // 64
#define DRIFT_POSITION 17	  // 68
#define DRIFT_LEVEL 18		  // 72
#define FPGA_VERSION 19		  // 76
#define RESERVED_9 20         // 80
#define RESERVED_10 21        // 84
#define RESERVED_11 22        // 88
#define RESERVED_12 23        // 92
#define RESERVED_13 24        // 96
#define RESERVED_14 25        // 100
#define RESERVED_15 26        // 104
#define RESERVED_16 27        // 108
#define RESERVED_17 28        // 112
#define RESERVED_18 29        // 116
#define RESERVED_19 30        // 120
#define RESERVED_20 31        // 124
#define TRIGGER_USEC 35		  // 140
#define BLACK_LEVEL 36		  // 144
#define RESERVED_21 37        // 152

#include "inttypes.h"

    typedef enum control_unit_core_register_offset_enum
    {
        NIT_CONTROL_UNIT_BIAS_V_OFFSET = BIAS_V,
        NIT_CONTROL_UNIT_BLACK_LEVEL_OFFSET = BLACK_LEVEL,
        NIT_CONTROL_UNIT_BPC_EN_OFFSET = BPC_EN,
        NIT_CONTROL_UNIT_DRIFT_ENABLE_OFFSET = DRIFT_ENABLE,
        NIT_CONTROL_UNIT_DRIFT_POSITION_OFFSET = DRIFT_POSITION,
        NIT_CONTROL_UNIT_INT_TIME_OFFSET = INT_TIME,
        NIT_CONTROL_UNIT_OFFSET_UPDATE_OFFSET = OFFSET_UPDATE,
        NIT_CONTROL_UNIT_SHUTTER_RESET_OFFSET = SHUTTER_RESET,
        NIT_CONTROL_UNIT_SHUTTER_OFFSET = SHUTTER,
        NIT_CONTROL_UNIT_TEMP1_OFFSET = TEMP1,
        NIT_CONTROL_UNIT_TEMP2_OFFSET = TEMP2
    } control_unit_core_register_offset;

    typedef struct control_unit_core_state_struct
    {
        int fd;
        volatile void *priv;
    } control_unit_core_state_t;

    int control_unit_open(control_unit_core_state_t* state);

    int control_unit_close(control_unit_core_state_t* state);

    void control_unit_bias_v_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_bias_v_get(control_unit_core_state_t *state);

    void control_unit_black_level_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_black_level_get(control_unit_core_state_t *state);

    void control_unit_bpc_en_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_bpcc_en_get(control_unit_core_state_t *state);

    void control_unit_drift_enable_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_drift_enable_get(control_unit_core_state_t *state);

    void control_unit_drift_position_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_drift_position_get(control_unit_core_state_t *state);

    void control_unit_int_time_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_int_time_get(control_unit_core_state_t *state);

    void control_unit_offset_update_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_offset_update_get(control_unit_core_state_t *state);

    uint16_t control_unit_shutter_reset_get(control_unit_core_state_t *state);

    void control_unit_shutter_reset_set(control_unit_core_state_t *state, uint16_t value);

    void control_unit_shutter_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_shutter_get(control_unit_core_state_t *state);

    uint16_t control_unit_temp1_get(control_unit_core_state_t *state);

    void control_unit_temp1_set(control_unit_core_state_t *state, uint16_t value);

    uint16_t control_unit_temp2_get(control_unit_core_state_t *state);

    double control_unit_temp_get_voltage(uint16_t value);
    double control_unit_temp_get_resistance(uint16_t value);
    double control_unit_temp_to_degc(uint16_t value);

    void control_unit_temp2_set(control_unit_core_state_t *state, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif