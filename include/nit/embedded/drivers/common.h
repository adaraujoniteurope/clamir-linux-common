#ifndef COMMON_H_
#define COMMON_H_

#include <memory>

#include <stdbool.h>

#include <boost/serialization/nvp.hpp>

#define DRIVER_DECLARE_STATE(name)    \
    struct nit_##name##_state_t       \
    {                                 \
        nit_##name##_config_t config; \
        int fd;                       \
        volatile void *priv;          \
        bool is_open;                 \
    };

#define DRIVER_DECLARE_CONFIG_BEGIN(name) \
    struct nit_##name##_config_t          \
    {

#define DRIVER_DECLARE_CONFIG_END(name) \
    int reserved;                       \
    }                                   \
    ;

#define DRIVER_DECLARE_CONFIG_SERIALIZER_BEGIN(name) \
    template <class archiver>                        \
    void serialize(archiver &ar, const unsigned int version)                    \
    {                                               
#define DRIVER_DECLARE_CONFIG_SERIALIZER_END(name) \
    }

#define DRIVER_DECLARE_CONFIG_DESERIALIZER_BEGIN(name) \
    template <class archiver>                          \
    void load(archiver &ar, const unsigned int version)                            \
    {

#define DRIVER_DECLARE_CONFIG_DESERIALIZER_END(name) \
    }

#define DRIVER_DECLARE_OFFSET_TABLE_BEGIN(name)    \
    typedef enum nit_##name##_register_offset_enum \
    {

#define DRIVER_DECLARE_OFFSET_TABLE_END(name) \
    }                                         \
    nit_##name##_register_offset_t;

#define DRIVER_DECLARE_NAMES_TABLE(name) \
    extern const char *nit_##name##_names[];

#define DRIVER_DEFINE_NAMES_TABLE_BEGIN(name) \
    const char *nit_##name##_names[] = {

#define DRIVER_DEFINE_NAMES_TABLE_END(name) \
    }                                       \
    ;

#define DRIVER_FIELD_AS_CONFIG_SERIALIZER_TABLE_ITEM(name, parameter, type, size, offset) ar & BOOST_SERIALIZATION_NVP(parameter);
#define DRIVER_FIELD_AS_OFFSET_TABLE_ITEM(name, parameter, type, size, offset) nit_##name##_##parameter##_offset = offset,
#define DRIVER_FIELD_AS_CONFIG_TABLE_ITEM(name, parameter, type, size, offset) type parameter;
#define DRIVER_FIELD_AS_NAMES_TABLE_ITEM(name, parameter, type, size, offset) #name,
#define DRIVER_FIELD_AS_WEAK_FUNCTION_DECLARATION(container_type, name, parameter, type, size, offset) \
    int nit_##name##_##parameter##_set(container_type *, type);                                        \
    int nit_##name##_##parameter##_get(container_type *, type *);

#define DRIVER_FIELD_AS_WEAK_FUNCTION_DEFINITION(container_type, name, parameter, type, size, offset) \
    __attribute__((weak)) int nit_##name##_##parameter##_set(container_type *, type) { return -1; }   \
    __attribute__((weak)) int nit_##name##_##parameter##_get(container_type *, type *) { return -1; }

template <typename state_type, typename type>
constexpr type unsafe_get(state_type *state, size_t offset)
{
    return (type) *(((volatile uint32_t *)state->priv) + offset);
}

template <typename state_type, typename type>
constexpr type unsafe_set(state_type *state, size_t offset, type value)
{
    return *(((uint32_t *)state->priv) + offset) = value;
}

#ifdef NIT_CLAMIR_DEBUGGING
#define print_debug(...) printf(__VA_ARGS__)
#else
#define print_debug(...)
#endif

#ifndef NIT_CLAMIR_HOST_MOCKUP
#define DEBIGGING_HOST 0
#endif

#endif