#ifndef _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_
#define _NIT_EMBEDDED_CLAMIR_DRIVER_ARM_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum arm_core_register_offset_enum
{
} arm_core_register_offset;

typedef struct arm_core_state_struct
{
    void * priv;
} arm_core_state;

int arm_core_open(arm_core_state* state);
int arm_core_close(arm_core_state* state);

#ifdef __cplusplus
}
#endif

#endif