#include "light_sensor.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"

static adc_oneshot_unit_handle_t adc_handle = nullptr;

void light_sensor_init(void)
{
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;

    ESP_ERROR_CHECK(adc_oneshot_new_unit(
        &init_config, &adc_handle));

    adc_oneshot_chan_cfg_t channel_config = {};
    channel_config.atten = ADC_ATTEN_DB_12;
    channel_config.bitwidth = ADC_BITWIDTH_DEFAULT;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(
        adc_handle,
        ADC_CHANNEL_6,
        &channel_config));
}

int light_sensor_read(void)
{
    int adc_value = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(
        adc_handle,
        ADC_CHANNEL_6,
        &adc_value));

    return adc_value;
}

bool light_sensor_is_dark(int adc_value)
{
    return adc_value > LIGHT_THRESHOLD;
}
