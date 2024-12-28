#ifndef SPIFFS_H_
#define SPIFFS_H_

#include "stdint.h"

typedef enum{
    ERROR_OK,
    ERROR_FILE_PATH,
    ERROR_FREAD,
    ERROR_MOUNT,
    ERROR_PARTITION,
    ERROR_INIT
}spiffs_error_t;

spiffs_error_t spiffs_init(void);
spiffs_error_t spiffs_get_file(char* buf, const char *__restrict__ path);
uint32_t spiffs_get_file_size(const char *__restrict__ path);

#endif