
#include <stdio.h>

#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "light_sensor.h"
#include "relay_control.h"

static const char *TAG = "SMART_HOME";

#define BUTTON_PIN GPIO_NUM_23

enum ControlMode {
    AUTO_MODE,
    MANUAL_ON,
    MANUAL_OFF
};

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Smart Home System Starting");

    light_sensor_init();
    relay_init();

    // Configure pushbutton
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    ControlMode mode = AUTO_MODE;
    bool last_button_pressed = false;
    bool last_lamp_state = false;
    bool first_reading = true;

    while (true)
    {
        // Read pushbutton (active LOW)
        bool button_pressed =
            gpio_get_level(BUTTON_PIN) == 0;

        // Detect a new button press
        if (button_pressed && !last_button_pressed)
        {
            if (mode == AUTO_MODE) {
                mode = MANUAL_ON;
            }
            else if (mode == MANUAL_ON) {
                mode = MANUAL_OFF;
            }
            else {
                mode = AUTO_MODE;
            }

            ESP_LOGI(TAG, "Button pressed! Mode: %s",
                mode == AUTO_MODE ? "AUTO" :
                mode == MANUAL_ON ? "MANUAL ON" :
                "MANUAL OFF");
        }

        last_button_pressed = button_pressed;

        // Read light sensor
        int light_value = light_sensor_read();
        bool is_dark = light_sensor_is_dark(light_value);

        // Determine lamp state
        bool lamp_on;

if (mode == AUTO_MODE) {
    lamp_on = is_dark;
}
else if (mode == MANUAL_ON) {
    lamp_on = true;
}
else {
    lamp_on = false;
}

        // Control relay
        relay_set(lamp_on);

        // Print status when lamp state changes
        if (first_reading || lamp_on != last_lamp_state)
        {
            ESP_LOGI(TAG,
                "LDR: %d | Light: %s | Lamp: %s",
                light_value,
                is_dark ? "DARK" : "BRIGHT",
                lamp_on ? "ON" : "OFF");

            last_lamp_state = lamp_on;
            first_reading = false;
        }

        // 50ms sampling interval
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
