
#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_PIN GPIO_NUM_23

static const char *TAG = "SWITCH_NODE";

extern "C" void app_main(void)
{
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    ESP_LOGI(TAG, "Switch Node Started");

    bool last_pressed = false;
    bool lamp_on = false;

    while (true)
    {
        bool pressed = gpio_get_level(BUTTON_PIN) == 0;

        if (pressed && !last_pressed)
        {
            lamp_on = !lamp_on;

            ESP_LOGI(TAG,
                     "Switch Command: %s",
                     lamp_on ? "ON" : "OFF");

            // TODO: Publish command through BLE Mesh
        }

        last_pressed = pressed;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
