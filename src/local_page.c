#include "local_page.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "spiffs.h"
#include <stdlib.h>

#define BOOTSTRAP_PATH    "/spiffs/bootstrap.min.css"
#define DATA_PATH         "/spiffs/data.html"
#define FUNCTION_PATH     "/spiffs/function.js"
#define INDEX_PATH        "/spiffs/index.html"
#define STYLE_PATH        "/spiffs/style.css"

#define MAX_RESPONSE_SIZE 1024
char response_data[MAX_RESPONSE_SIZE];

esp_err_t start_page_handler(httpd_req_t *req);
esp_err_t get_file_handler(httpd_req_t *req);

httpd_uri_t uri_get       = {.uri = "/", .method = HTTP_GET, .handler = get_file_handler, .user_ctx = NULL};
httpd_uri_t uri_index     = {.uri = "/index.html", .method = HTTP_GET, .handler = get_file_handler, .user_ctx = NULL};
httpd_uri_t uri_bootstrap = {.uri      = "/bootstrap.min.css",
                             .method   = HTTP_GET,
                             .handler  = get_file_handler,
                             .user_ctx = NULL};
httpd_uri_t uri_function  = {.uri = "/function.js", .method = HTTP_GET, .handler = get_file_handler, .user_ctx = NULL};
httpd_uri_t uri_favicon   = {.uri = "/favicon.ico", .method = HTTP_GET, .handler = get_file_handler, .user_ctx = NULL};
httpd_uri_t uri_data      = {.uri = "/data.html", .method = HTTP_GET, .handler = get_file_handler, .user_ctx = NULL};
httpd_uri_t uri_style     = {.uri = "/style.css", .method = HTTP_GET, .handler = get_file_handler, .user_ctx = NULL};

const char *uri_start          = "/";
const char *uri_index_const    = "/index.html";
const char *uri_bootstrap_data = "/bootstrap.min.css";
const char *uri_function_data  = "/function.js";
const char *uri_favion_data    = "/favicon.ico";
const char *uri_data_data      = "/data.html";
const char *uri_style_data     = "/style.css";

esp_err_t get_file_handler(httpd_req_t *req)
{
    const char *uri_name = req->uri;
    char *path           = NULL;

    if (strcmp(uri_name, uri_start) == 0 || strcmp(uri_name, uri_index_const) == 0) {
        printf("INDEX_PATH\n");
        httpd_resp_set_type(req, "text/html");
        path = INDEX_PATH;
    } else if (strcmp(uri_name, uri_function_data) == 0) {
        path = FUNCTION_PATH;
        httpd_resp_set_type(req, "application/javascript");
        printf("FUNCTION_PATH\n");
    } else if (strcmp(uri_name, uri_data_data) == 0) {
        path = DATA_PATH;
        httpd_resp_set_type(req, "text/html");
        printf("DATA_PATH\n");
    } else if (strcmp(uri_name, uri_bootstrap_data) == 0) {
        path = BOOTSTRAP_PATH;
        httpd_resp_set_type(req, "text/css");
        printf("BOOTSTRAP_PATH\n");
    } else if (strcmp(uri_name, uri_style_data) == 0) {
        path = STYLE_PATH;
        httpd_resp_set_type(req, "text/css");
        printf("STYLE_PATH\n");
    }

    if (path == NULL) {
        printf("ERROR NULL\n");
        printf("PATH=%s\n", uri_name);
        return ESP_FAIL;
    }

    size_t file_size = spiffs_get_file_size(path);
    if (file_size == 0) {
        printf("ERROR GETTING FILE SIZE\n");
        return ESP_FAIL;
    }

    char *file_data = malloc(file_size + 1);
    if (file_data == NULL) {
        printf("ERROR ALLOCATING MEMORY\n");
        return ESP_FAIL;
    }

    if (spiffs_get_file(file_data, path) != ERROR_OK) {
        printf("ERROR READING FILE\n");
        free(file_data);
        return ESP_FAIL;
    }

    file_data[file_size] = '\0';
    printf("File successfully loaded\n");

    esp_err_t result = httpd_resp_send(req, file_data, HTTPD_RESP_USE_STRLEN);
    free(file_data);
    return result;
}

void local_page_init(void)
{
    if (spiffs_init() != ERROR_OK)
        return;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_index);
        httpd_register_uri_handler(server, &uri_bootstrap);
        httpd_register_uri_handler(server, &uri_function);
        httpd_register_uri_handler(server, &uri_favicon);
        httpd_register_uri_handler(server, &uri_data);
        httpd_register_uri_handler(server, &uri_style);
    }
}

void local_page_disable(httpd_handle_t server)
{
    httpd_stop(server);
}
