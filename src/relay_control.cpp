#include "relay_control.h"
#include "driver/gpio.h"
#include "esp_check.h"

#define RELAY_PIN GPIO_NUM_26

void relay_init(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(RELAY_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(RELAY_PIN, 0));
}

void relay_set(bool turn_on)
{
    ESP_ERROR_CHECK(gpio_set_level(RELAY_PIN, turn_on ? 1 : 0));
}
