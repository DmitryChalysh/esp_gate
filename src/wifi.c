#include "wifi.h"
#include "esp_err.h"
#include "esp_log.h"

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

const char *ssid = "MTS_61";
const char *pass = "Mvdj200pqc";

const char *ssid_ap = "EPS_TEST";
const char *pass_ap = "Mvdj200pqc";

uint8_t retry_num               = 0;
wifi_state_t wifi_current_state = WIFI_NOT_INIT;

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,
                               void *event_data);
static void wifi_ip_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,
                                  void *event_data);

void wifi_init(uint8_t wifi_mode)
{
    // wifi_config_t wifi_configuration;

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    if (wifi_mode == WIFI_MODE_LOCAL)
        esp_netif_create_default_wifi_ap();
    else if (wifi_mode == WIFI_MODE_GLOBAL)
        esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, wifi_ip_event_handler, NULL);

    if (wifi_mode == WIFI_MODE_LOCAL) {
        wifi_config_t wifi_configuration = {
            .ap = {.ssid = "", .password = "", .max_connection = 2, .authmode = WIFI_AUTH_WPA2_PSK}

        };
        strcpy((char *)wifi_configuration.ap.ssid, ssid_ap);
        strcpy((char *)wifi_configuration.ap.password, pass_ap);
        esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_configuration);
        esp_wifi_set_mode(WIFI_MODE_AP);
    } else if (wifi_mode == WIFI_MODE_GLOBAL) {
        wifi_config_t wifi_configuration = {.sta =
                                                {
                                                    .ssid     = "",
                                                    .password = "",
                                                    
                                                }

        };
        strcpy((char *)wifi_configuration.sta.ssid, ssid);
        strcpy((char *)wifi_configuration.sta.password, pass);
        esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
        esp_wifi_set_mode(WIFI_MODE_STA);
    }
    esp_wifi_start();
    esp_wifi_connect();
    wifi_current_state = WIFI_INIT;
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id) {
    case WIFI_EVENT_STA_START:
        printf("WIFI CONNECTING....\n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi CONNECTED\n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection\n");
        if (retry_num < 5) {
            esp_wifi_connect();
            retry_num++;
            printf("Retrying to Connect...\n");
        }
        break;
    case WIFI_EVENT_AP_START:
        printf("WIFI AP START\n");
        wifi_current_state = WIFI_STARTED;
        break;
    case WIFI_EVENT_AP_STACONNECTED:
        printf("Пользователь подключён\n");
        wifi_current_state = WIFI_CLIENT_CONNECTED;
        break;
    case WIFI_EVENT_AP_STADISCONNECTED:
        printf("Пользователь отключён\n");
        wifi_current_state = WIFI_CLIENT_DISCONNECTED;
        break;
    default:
        printf("Unknown event...\n\n");
        break;
    }
}

static void wifi_ip_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,
                                  void *event_data)
{
    switch (event_id) {
    case IP_EVENT_STA_GOT_IP:
        printf("Wifi got IP\n\n");
        break;
    case IP_EVENT_AP_STAIPASSIGNED:
        printf("Wifi AP STAIPASSIGNED\n\n");
        break;
    default:
        printf("Unknown ip event\n\n");
        break;
    }
}

wifi_state_t wifi_get_currrent_state(void)
{
    return wifi_current_state;
}
