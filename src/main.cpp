
#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "light_sensor.h"
#include "relay_control.h"

static const char *TAG = "SMART_HOME";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Smart Home System Starting");

    light_sensor_init();
    relay_init();

    bool previous_dark = false;
    bool first_reading = true;

    while (true)
    {
        int light_value = light_sensor_read();
        bool is_dark = light_sensor_is_dark(light_value);

        relay_set(is_dark);

        ESP_LOGI(TAG, "LDR ADC Value: %d", light_value);
        ESP_LOGI(TAG, "Light: %s | Lamp: %s",
                 is_dark ? "DARK" : "BRIGHT",
                 is_dark ? "ON" : "OFF");

        if (first_reading || is_dark != previous_dark)
        {
            ESP_LOGI(TAG, "Light state changed: %s",
                     is_dark ? "DARK" : "BRIGHT");

            previous_dark = is_dark;
            first_reading = false;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
