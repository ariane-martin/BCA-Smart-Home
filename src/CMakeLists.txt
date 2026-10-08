
idf_component_register(
    SRCS
        "main.cpp"
        "light_sensor.cpp"
        "relay_control.cpp"
    INCLUDE_DIRS
        "."
        "../include"
    REQUIRES
        esp_adc
        esp_driver_gpio
)
