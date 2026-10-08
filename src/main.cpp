
#include <stdio.h>
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "SMART_HOME";

#define LIGHT_THRESHOLD 2857
#define LAMP_PIN GPIO_NUM_18

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Smart Home System Starting");

    // Configure LED output
    gpio_reset_pin(LAMP_PIN);
    gpio_set_direction(LAMP_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LAMP_PIN, 0);

    // Configure LDR ADC
    adc_oneshot_unit_handle_t adc_handle;

    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;

    ESP_ERROR_CHECK(adc_oneshot_new_unit(
        &init_config, &adc_handle));

    adc_oneshot_chan_cfg_t channel_config = {};
    channel_config.atten = ADC_ATTEN_DB_12;
    channel_config.bitwidth = ADC_BITWIDTH_DEFAULT;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(
        adc_handle, ADC_CHANNEL_6, &channel_config));

    bool previous_dark = false;
    bool first_reading = true;

    while (true)
    {
        int light_value = 0;

        ESP_ERROR_CHECK(adc_oneshot_read(
            adc_handle, ADC_CHANNEL_6, &light_value));

        bool is_dark = light_value > LIGHT_THRESHOLD;

        // Control the actual LED
        gpio_set_level(LAMP_PIN, is_dark ? 1 : 0);

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
