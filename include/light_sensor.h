
#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <stdbool.h>

// Approximate 30-lux threshold from Wokwi
#define LIGHT_THRESHOLD 2857

// Initialize the LDR sensor
void light_sensor_init(void);

// Read the LDR ADC value
int light_sensor_read(void);

// Return true if the environment is dark
bool light_sensor_is_dark(int adc_value);

#endif
