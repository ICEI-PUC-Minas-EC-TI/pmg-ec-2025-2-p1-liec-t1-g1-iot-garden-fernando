#ifndef PINS_H
#define PINS_H

// Central Pin Configuration for ESP32

// --- Onboard ---
#define PIN_LED_BUILTIN 2

// --- Actuators ---
#define PIN_SPRINKLER 4
#define PIN_FAN 23
#define PIN_LIGHT 19

// --- Sensors ---
#define PIN_DHT 21
#define PIN_PHOTORESISTOR 22
#define PIN_SOIL_MOISTURE 34  // ADC input only

// Free pins available for future use:
// 23, 25, 26, 27, 32, 33

#endif
