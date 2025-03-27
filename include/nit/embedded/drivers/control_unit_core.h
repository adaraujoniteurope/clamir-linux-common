#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_CONTROL_UNIT_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_CONTROL_UNIT_CORE_H_

#include "common.h"

#define NIT_CONTROL_UNIT_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(nit_control_unit_core_state_t, name, parameter, type, size, offset)
#define NIT_CONTROL_UNIT_DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(name, parameter, type, size, offset) DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(nit_control_unit_core_state_t, name, parameter, type, size, offset)

#define NIT_CONTROL_UNIT_BASE_ADDRESS 0x40000000
#define NIT_CONTROL_UNIT_SIZE 0x00010000

#define INT_TIME 0            // 0
#define BIAS_V 1              // 4
#define OFFSET_EN 2           // 8
#define OFFSET_UPDATE 3       // 12
#define SHUTTER 4             // 16
#define BPC_EN 5              // 20
#define BPC_MEM_WRITE 6       // 24
#define BPC_IDENTIFY 7        // 28
#define TEMP1 8               // 32
#define TEMP2 9               // 36
#define TEMP3 10              // 40
#define TEMP4 11              // 44
#define SHUTTER_RESET 12      // 48
#define SINCRONIZATION 13     // 52
#define SAVE_EMBEDDED_CONF 14 // 56
#define ARM_SW_VERSION 15     // 60
#define DRIFT_ENABLE 16       // 64
#define DRIFT_POSITION 17     // 68
#define DRIFT_LEVEL 18        // 72
#define FPGA_VERSION 19       // 76
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
#define TRIGGER_USEC 35       // 140
#define BLACK_LEVEL 36        // 144
#define RESERVED_21 37        // 152

#include "inttypes.h"

#define NIT_CONTROL_UNIT_CORE_FIELD_TABLE(_)                                          \
    _(control_unit_core, int_time, uint16_t, sizeof(uint16_t), 0x0000 + 0)            \
    _(control_unit_core, bias_v, uint16_t, sizeof(uint16_t), 0x0000 + 1)              \
    _(control_unit_core, offset_en, uint16_t, sizeof(uint16_t), 0x0000 + 2)           \
    _(control_unit_core, offset_update, uint16_t, sizeof(uint16_t), 0x0000 + 3)       \
    _(control_unit_core, shutter, uint16_t, sizeof(uint16_t), 0x0000 + 4)             \
    _(control_unit_core, bpc_en, uint16_t, sizeof(uint16_t), 0x0000 + 5)              \
    _(control_unit_core, bpc_mem_write, uint16_t, sizeof(uint16_t), 0x0000 + 6)       \
    _(control_unit_core, bpc_identify, uint16_t, sizeof(uint16_t), 0x0000 + 7)        \
    _(control_unit_core, temp1, uint16_t, sizeof(uint16_t), 0x0000 + 8)               \
    _(control_unit_core, temp2, uint16_t, sizeof(uint16_t), 0x0000 + 9)               \
    _(control_unit_core, temp3, uint16_t, sizeof(uint16_t), 0x0000 + 10)              \
    _(control_unit_core, temp4, uint16_t, sizeof(uint16_t), 0x0000 + 11)              \
    _(control_unit_core, shutter_reset, uint16_t, sizeof(uint16_t), 0x0000 + 12)      \
    _(control_unit_core, sincronization, uint16_t, sizeof(uint16_t), 0x0000 + 13)     \
    _(control_unit_core, save_embedded_conf, uint16_t, sizeof(uint16_t), 0x0000 + 14) \
    _(control_unit_core, arm_sw_version, uint16_t, sizeof(uint16_t), 0x0000 + 15)     \
    _(control_unit_core, drift_enable, uint16_t, sizeof(uint16_t), 0x0000 + 16)       \
    _(control_unit_core, drift_position, uint16_t, sizeof(uint16_t), 0x0000 + 17)     \
    _(control_unit_core, drift_level, uint16_t, sizeof(uint16_t), 0x0000 + 18)        \
    _(control_unit_core, fpga_version, uint16_t, sizeof(uint16_t), 0x0000 + 19)       \
    _(control_unit_core, reserved_9, uint16_t, sizeof(uint16_t), 0x0000 + 20)         \
    _(control_unit_core, reserved_10, uint16_t, sizeof(uint16_t), 0x0000 + 21)        \
    _(control_unit_core, reserved_11, uint16_t, sizeof(uint16_t), 0x0000 + 22)        \
    _(control_unit_core, reserved_12, uint16_t, sizeof(uint16_t), 0x0000 + 23)        \
    _(control_unit_core, reserved_13, uint16_t, sizeof(uint16_t), 0x0000 + 24)        \
    _(control_unit_core, reserved_14, uint16_t, sizeof(uint16_t), 0x0000 + 25)        \
    _(control_unit_core, reserved_15, uint16_t, sizeof(uint16_t), 0x0000 + 26)        \
    _(control_unit_core, reserved_16, uint16_t, sizeof(uint16_t), 0x0000 + 27)        \
    _(control_unit_core, reserved_17, uint16_t, sizeof(uint16_t), 0x0000 + 28)        \
    _(control_unit_core, reserved_18, uint16_t, sizeof(uint16_t), 0x0000 + 29)        \
    _(control_unit_core, reserved_19, uint16_t, sizeof(uint16_t), 0x0000 + 30)        \
    _(control_unit_core, reserved_20, uint16_t, sizeof(uint16_t), 0x0000 + 31)        \
    _(control_unit_core, trigger_usec, uint16_t, sizeof(uint16_t), 0x0000 + 35)       \
    _(control_unit_core, black_level, uint16_t, sizeof(uint16_t), 0x0000 + 36)        \
    _(control_unit_core, reserved_21, uint16_t, sizeof(uint16_t), 0x0000 + 37)

DRIVER_DECLARE_OFFSET_TABLE_BEGIN(control_unit_core)
NIT_CONTROL_UNIT_CORE_FIELD_TABLE(DRIVER_FIELD_AS_OFFSET_TABLE_ITEM)
DRIVER_DECLARE_OFFSET_TABLE_END(control_unit_core)

DRIVER_DECLARE_CONFIG_BEGIN(control_unit_core)
NIT_CONTROL_UNIT_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_TABLE_ITEM)

DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(control_unit_core)
NIT_CONTROL_UNIT_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
DRIVER_DECLARE_CONFIG_SERIALIZER_END(control_unit_core)

DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(control_unit_core)
NIT_CONTROL_UNIT_CORE_FIELD_TABLE(DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM)
DRIVER_DECLARE_CONFIG_DESERIALIZER_END(control_unit_core)

DRIVER_DECLARE_CONFIG_END(control_unit_core)

extern const nit_control_unit_core_config_t control_unit_core_config_default;

DRIVER_DECLARE_STATE(control_unit_core)

int nit_control_unit_core_open(nit_control_unit_core_state_t *state);
int nit_control_unit_core_close(nit_control_unit_core_state_t *state);
int nit_control_unit_core_config_save_to_file(nit_control_unit_core_state_t *state, const char *path);
int nit_control_unit_core_config_load_from_file(nit_control_unit_core_state_t *state, const char *path);

int nit_control_unit_core_assert(nit_control_unit_core_state_t *state);

double nit_control_unit_core_temp_to_degc(uint16_t value);

extern nit_control_unit_core_state_t nit_control_unit_core_driver;

NIT_CONTROL_UNIT_CORE_FIELD_TABLE(NIT_CONTROL_UNIT_DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION)

#endif