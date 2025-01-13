#ifndef _APP_GSDML_UTILS_H_
#define _APP_GSDML_UTILS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define GSDML_SUBMODULE_TABLE_ID(OBJECT, NAME, API, DATA_DIR, ID, INSIZE, OUTSIZE, PARAMETERS_TABLE) ID,

#define GSDML_SUBMODULE_PARAMETER_ID(OBJECT, INDEX, NAME, LENGTH) INDEX,

#define GSDML_PARAMETER_DEFINE(OBJECT, INDEX, NAME, LENGTH) \
    uint8_t OBJECT##_container[LENGTH];                     \
    app_gsdml_param_t OBJECT = {                            \
        .index = INDEX,                                     \
        .name = NAME,                                       \
        .length = LENGTH,                                   \
        .container = OBJECT##_container,                    \
        .set = (app_gsdml_param_value_setter)OBJECT##_set,  \
        .get = (app_gsdml_param_value_getter)OBJECT##_get};

#define GSDML_PARAMETER_DECLARE(OBJECT, INDEX, NAME, LENGTH)           \
    extern app_gsdml_param_t OBJECT;                                   \
    int OBJECT##_set(void* arg, void *value, uint16_t size); \
    int OBJECT##_get(void* arg, void **value, uint16_t *size);

#define GSDML_SUBMODULE_DEFINE(OBJECT, NAME, API, DATA_DIR, ID, INSIZE, OUTSIZE, PARAMETERS_TABLE) \
    app_gsdml_submodule_t OBJECT = {                                                               \
        .id = ID,                                                                                  \
        .name = NAME,                                                                              \
        .api = API,                                                                                \
        .data_dir = DATA_DIR,                                                                      \
        .insize = INSIZE,                                                                          \
        .outsize = OUTSIZE,                                                                        \
        .set = (app_gsdml_submodule_value_setter)OBJECT##_set,                                     \
        .get = (app_gsdml_submodule_value_getter)OBJECT##_get,                                     \
        .parameters =                                                                              \
            {                                                                                      \
                PARAMETERS_TABLE(GSDML_SUBMODULE_PARAMETER_ID) ID,                                 \
                0},                                                                                \
    };                                                                                             \
                                                                                                   \
    // __attribute__((weak)) int OBJECT##_get(void* arg, void *value, uint16_t *size) { return -1; } \
    // __attribute__((weak)) int OBJECT##_set(void* arg, void *value, uint16_t size) { return -1; } \
    // \

#define GSDML_SUBMODULE_DECLARE(OBJECT, NAME, API, DATA_DIR, ID, INSIZE, OUTSIZE, PARAMETERS_TABLE) \
    extern app_gsdml_submodule_t OBJECT;                                                            \
    int OBJECT##_set(void* arg, void *value, uint16_t size);                              \
    int OBJECT##_get(void* arg, void *value, uint16_t *size);

#define GSDML_MODULE(ID, NAME, DESCRIPTION, SUBMODULES_TABLE)  \
    app_gsdml_module_t NAME = {                                \
        .id = ID,                                              \
        .name = DESCRIPTION,                                   \
        .submodules =                                          \
            {                                                  \
                SUBMODULES_TABLE(GSDML_SUBMODULE_TABLE_ID) 0}, \
    };

#define GSDML_MODULE_DECLARE(ID, NAME, DESCRIPTION, SUBMODULES_TABLE) \
    extern app_gsdml_module_t NAME;

#ifdef __cplusplus
}
#endif

#endif