#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_CONTROL_UNIT_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_CONTROL_UNIT_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum control_unit_core_register_offset_enum
{
    NIT_CONTROL_UNIT_BIAS_V_OFFSET,
    NIT_CONTROL_UNIT_BLACK_LEVEL_OFFSET,
    NIT_CONTROL_UNIT_BPC_EN_OFFSET,
    NIT_CONTROL_UNIT_DRIFT_ENABLE_OFFSET,
    NIT_CONTROL_UNIT_DRIFT_POSITION_OFFSET,
    NIT_CONTROL_UNIT_INT_TIME_OFFSET,
    NIT_CONTROL_UNIT_OFFSET_UPDATE_OFFSET,
    NIT_CONTROL_UNIT_SHUTTER_RESET_OFFSET,
    NIT_CONTROL_UNIT_SHUTTER_OFFSET,
    NIT_CONTROL_UNIT_TEMP1_OFFSET,
    NIT_CONTROL_UNIT_TEMP2_OFFSET
} control_unit_core_register_offset;

typedef struct control_unit_core_state_struct
{
    void * priv;
} control_unit_core_state;

int control_unit_core_open(control_unit_core_state* state);

int control_unit_core_close(control_unit_core_state* state);

int control_unit_bias_voltage_set(control_unit_core_state* state);

int control_unit_bias_voltage_get(control_unit_core_state* state);

int control_unit_bpcc_enable_set(control_unit_core_state* state);

int control_unit_bpcc_enable_get(control_unit_core_state* state);

int control_unit_drift_enable_set(control_unit_core_state* state);

int control_unit_drift_enable_get(control_unit_core_state* state);

int control_unit_integration_time_set(control_unit_core_state* state);

int control_unit_integration_time_get(control_unit_core_state* state);

int control_unit_offset_update_set(control_unit_core_state* state);

int control_unit_offset_update_get(control_unit_core_state* state);

int control_unit_offset_shutter_reset_get(control_unit_core_state* state);

int control_unit_offset_shutter_reset_set(control_unit_core_state* state);

int control_unit_offset_shutter_set(control_unit_core_state* state);

int control_unit_offset_shutter_get(control_unit_core_state* state);

int control_unit_offset_temperature_1_get(control_unit_core_state* state);

int control_unit_offset_temperature_1_set(control_unit_core_state* state);

int control_unit_offset_temperature_2_get(control_unit_core_state* state);

int control_unit_offset_temperature_2_set(control_unit_core_state* state);

#ifdef __cplusplus
}
#endif

#endif