#ifndef TRIGGERS_H
#define TRIGGERS_H

#include <Arduino.h>
#include "../actuator/actuator.h"
#include "../sensor_dht/sensor_dht.h"
#include "../sensor_light/sensor_light.h"
#include "../sensor_soil/sensor_soil.h"

// ============================================
// TRIGGER CONFIGURATION - Edit thresholds here
// ============================================

// Enable/disable each trigger individually
#define TRIGGER_FAN_ENABLED       true
#define TRIGGER_SPRINKLER_ENABLED false  // disabled for now
#define TRIGGER_LIGHT_ENABLED     true

// Fan: turns on when temperature exceeds this (Celsius)
#define TRIGGER_FAN_TEMP_ON      30.0
#define TRIGGER_FAN_TEMP_OFF     28.0   // Hysteresis to prevent rapid switching

// Sprinkler: turns on when soil moisture is below this (raw ADC 0-4095)
// Higher value = drier soil
#define TRIGGER_SPRINKLER_DRY    3000   // Turn on when above (dry)
#define TRIGGER_SPRINKLER_WET    2500   // Turn off when below (wet enough)

// Light: turns on at night, off during day
// (no threshold, just night/day)

// ============================================

struct SensorData {
  DHTReading dht;
  LightReading light;
  SoilReading soil;
};

// Process all triggers based on sensor data
// Respects state machine: SOURCE_TRIGGER only activates if nothing else is active
// SOURCE_MANUAL (MQTT) always has priority
void processTriggers(const SensorData& sensors);

#endif
