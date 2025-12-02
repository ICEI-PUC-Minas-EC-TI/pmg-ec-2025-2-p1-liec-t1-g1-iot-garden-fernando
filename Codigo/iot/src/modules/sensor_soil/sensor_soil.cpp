#include "sensor_soil.h"
#include "../log/log.h"

// Thresholds to detect disconnected sensor
#define SOIL_MIN_VALID 50    // Below this = likely shorted or wet
#define SOIL_MAX_VALID 4000  // Above this = likely disconnected

void initSoilSensor() {
  pinMode(PIN_SOIL_MOISTURE, INPUT);
  logSuccess(LOG_SENSOR, "Soil moisture sensor ready");
}

SoilReading readSoilMoisture() {
  SoilReading reading;

  reading.raw = analogRead(PIN_SOIL_MOISTURE);

  // Check if sensor is connected (not floating)
  if (reading.raw < SOIL_MIN_VALID || reading.raw > SOIL_MAX_VALID) {
    reading.valid = false;
    reading.percent = -1;
    return reading;
  }

  // ESP32 ADC is 12-bit (0-4095)
  // Higher value = drier soil, Lower value = wetter soil
  reading.percent = map(reading.raw, 4095, 0, 0, 100);
  reading.percent = constrain(reading.percent, 0, 100);
  reading.valid = true;

  return reading;
}
