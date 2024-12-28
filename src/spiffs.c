#include "spiffs.h"
#include "esp_spiffs.h"
#include "esp_err.h"
#include "esp_log.h"
#include <sys/stat.h>
#include <string.h>

spiffs_error_t spiffs_init(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs", .partition_label = NULL, .max_files = 5, .format_if_mount_failed = true};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL)
            return ERROR_MOUNT;
        else if (ret == ESP_ERR_NOT_FOUND)
            return ERROR_PARTITION;
        else
            return ERROR_INIT;
    }

    return ERROR_OK;
}

spiffs_error_t spiffs_get_file(char *buf, const char *__restrict__ path)
{
    memset((void *)buf, 0, sizeof(buf));
    struct stat st;
    if (stat(path, &st))
        return ERROR_FILE_PATH;

    FILE *fp = fopen(path, "r");
    printf("size=%ld\n", st.st_size);
    if (fread(buf, st.st_size, 1, fp) == 0)
        return ERROR_FREAD;

    fclose(fp);

    return ERROR_OK;
}

uint32_t spiffs_get_file_size(const char *__restrict__ path)
{
    struct stat st;
    if (stat(path, &st))
        return 0;
    else
        return st.st_size;
}
