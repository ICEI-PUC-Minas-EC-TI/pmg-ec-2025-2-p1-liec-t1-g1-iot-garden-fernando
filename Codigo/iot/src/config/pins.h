#ifndef PINS_H
#define PINS_H

// Central Pin Configuration for ESP32

// --- Onboard ---
#define PIN_LED_BUILTIN 2

// --- Actuators ---
#define PIN_SPRINKLER 23
#define PIN_FAN 4
#define PIN_LIGHT 19

// --- Sensors ---
#define PIN_DHT 25
#define PIN_PHOTORESISTOR 26
#define PIN_SOIL_MOISTURE 34  // ADC input only

// --- I2C (OLED Display) ---
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

// Free pins available for future use:
// 5, 27, 32, 33

#endif
