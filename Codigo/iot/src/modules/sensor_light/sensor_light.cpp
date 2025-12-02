#include "sensor_light.h"
#include "../log/log.h"

void initLightSensor() {
  pinMode(PIN_PHOTORESISTOR, INPUT);
  logSuccess(LOG_SENSOR, "Light sensor ready");
}

LightReading readLight() {
  LightReading reading;
  reading.valid = true;
  reading.night = (digitalRead(PIN_PHOTORESISTOR) == HIGH);  // HIGH = dark/night
  return reading;
}
