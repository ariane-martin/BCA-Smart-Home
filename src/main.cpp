
#include <stdio.h>

#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "light_sensor.h"

static const char *TAG = "SMART_HOME";

#define RELAY_PIN GPIO_NUM_26

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Smart Home System Starting");

    // Initialize relay
    gpio_reset_pin(RELAY_PIN);
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_PIN, 0);

    // Initialize light sensor
    light_sensor_init();

    bool previous_dark = false;
    bool first_reading = true;

    while (true)
    {
        int light_value = light_sensor_read();

        bool is_dark = light_sensor_is_dark(light_value);

        // Control relay
        gpio_set_level(RELAY_PIN, is_dark ? 1 : 0);

        ESP_LOGI(TAG, "LDR ADC Value: %d", light_value);

        ESP_LOGI(TAG, "Light: %s | Lamp: %s",
                 is_dark ? "DARK" : "BRIGHT",
                 is_dark ? "ON" : "OFF");

        if (first_reading || is_dark != previous_dark)
        {
            ESP_LOGI(TAG, "Light state changed: %s",
                     is_dark ? "DARK" : "BRIGHT");

            // Future BLE Mesh publication
            previous_dark = is_dark;
            first_reading = false;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
