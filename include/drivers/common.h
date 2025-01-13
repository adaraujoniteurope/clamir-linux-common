#ifndef COMMON_H_
#define COMMON_H_

#define DRIVER_DECLARE_STATE(name)     \
    typedef struct name##_state_struct \
    {                                  \
        name##_config_t config;        \
        int fd;                        \
        volatile void *priv;           \
    } name##_state_t;

#define DRIVER_DECLARE_CONFIG_BEGIN(name) \
    typedef struct name##_config_struct   \
    {

#define DRIVER_DECLARE_CONFIG_END(name) \
    }                                   \
    name##_config_t;

#define DRIVER_DECLARE_OFFSET_TABLE_BEGIN(name) \
    typedef enum name##_register_offset_enum    \
    {

#define DRIVER_DECLARE_OFFSET_TABLE_END(name) \
    }                                         \
    name##_register_offset_t;

#define DRIVER_DECLARE_NAMES_TABLE(name) \
    extern const char *name##_names[];

#define DRIVER_DEFINE_NAMES_TABLE_BEGIN(name) \
    const char *name##_names[] = {

#define DRIVER_DEFINE_NAMES_TABLE_END(name) \
    }                                       \
    ;

#define DRIVER_FIELD_AS_OFFSET_TABLE_ITEM(name, parameter, type, size, offset) nit_##name##_##parameter##_offset = offset,
#define DRIVER_FIELD_AS_CONFIG_TABLE_ITEM(name, parameter, type, size, offset) type parameter;
#define DRIVER_FIELD_AS_NAMES_TABLE_ITEM(name, parameter, type, size, offset) #name,
#define DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(container_type, name, parameter, type, size, offset) \
    void nit_##name##_##parameter##_set(container_type *, type);                                 \
    type nit_##name##_##parameter##_get(container_type *);

#define DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(container_type, name, parameter, type, size, offset) \
    __attribute__((weak)) void nit_##name##_##parameter##_set(container_type *, type) {}              \
    __attribute__((weak)) type nit_##name##_##parameter##_get(container_type *) { return 0; }

#ifdef NIT_CLAMIR_DEBUGGING
#define print_debug(...) printf(__VA_ARGS__)
#else
#define print_debug(...)
#endif

#endif