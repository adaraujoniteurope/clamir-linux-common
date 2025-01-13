#ifndef UTILS_CONF_FILE_H_
#define UTILS_CONF_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

char* config_file_read_key_as_string(const char* path, const char* key);
int config_file_read_key_as_int(const char* path, const char* key, int __default);
double config_file_read_key_as_double(const char* path, const char* key, double __default);

#ifdef __cplusplus
}
#endif

#endif