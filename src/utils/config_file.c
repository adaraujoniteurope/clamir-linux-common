#include "utils/config_file.h"

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <inttypes.h>

char* config_file_read_key_as_string(const char* path, const char* key)
{
    FILE* file = NULL;
    int file_size = 0;
    file = fopen(path, "r");

    if (file == NULL) {
        return NULL;
    }

    struct stat fstats;
    if(stat(path, &fstats) < 0) {
        return NULL;
    }

    if (fstats.st_size == 0) {
        return NULL;
    }

    char* conf = malloc(fstats.st_size + 1);
    char* ptr = conf;

    // real file contents
    while((*ptr = fgetc(file)) != EOF) ptr++;

    char* result = NULL;

    char _key[256];
    char _value[256];

    ptr = conf;
    char* token = strtok(ptr, "\n");

    while(token != NULL) {

        sscanf(token, "%s %s", _key, _value);

        if (strcmp(key, _key) == 0) {
            result = strdup(_value);
            break;
        }

        token = strtok(NULL, "\n");
    }

    free(conf);
    fclose(file);
    return result;
}

int config_file_read_key_as_int(const char* path, const char* key, int __default)
{
    char* str = config_file_read_key_as_string(path, key);
    
    if (str == NULL) {
        return __default;
    }

    uint32_t value = strtol(str, NULL, 10);
    free(str);
    return value;
}

double config_file_read_key_as_double(const char* path, const char* key, double __default)
{
    char* str = config_file_read_key_as_string(path, key);

    if (str == NULL) {
        return __default;
    }

    double value = strtod(str, NULL);
    free(str);
    return value;
}