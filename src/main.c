#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "local_page.h"

void vTask_blink(void *pvParameters);
static void led_init(void);

#define LED_PIN 2

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    led_init();
    wifi_init(WIFI_MODE_LOCAL);
    local_page_init();
    xTaskCreate(vTask_blink, "Task_blink", 1000, NULL, 1, NULL);

}

static void led_init(void)
{
    gpio_config_t led_config;
    led_config.intr_type    = GPIO_INTR_DISABLE;
    led_config.mode         = GPIO_MODE_OUTPUT;
    led_config.pin_bit_mask = (1ULL << LED_PIN);
    led_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    led_config.pull_up_en   = GPIO_PULLUP_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&led_config));
}

void vTask_blink(void *pvParameters)
{
    static bool state = true;
    for (;;) {
        // printf("Blink is running on core %d\n", xPortGetCoreID());
        ESP_ERROR_CHECK(gpio_set_level(LED_PIN, state));
        state = !state;
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}