#include "sensor_light.h"
#include "../log/log.h"

#define LIGHT_SAMPLES 5
#define LIGHT_SAMPLE_DELAY 10

static int lastStableState = -1;
static int stableCount = 0;

void initLightSensor() {
  pinMode(PIN_PHOTORESISTOR, INPUT);
  logSuccess(LOG_SENSOR, "Light sensor ready");
}

LightReading readLight() {
  LightReading reading;

  // Take multiple samples to detect floating pin
  int highCount = 0;
  for (int i = 0; i < LIGHT_SAMPLES; i++) {
    if (digitalRead(PIN_PHOTORESISTOR) == HIGH) {
      highCount++;
    }
    delay(LIGHT_SAMPLE_DELAY);
  }

  int currentState = (highCount > LIGHT_SAMPLES / 2) ? 1 : 0;

  // Check for stability (floating pins fluctuate)
  if (currentState == lastStableState) {
    stableCount++;
  } else {
    stableCount = 0;
    lastStableState = currentState;
  }

  // Need 3 consistent readings to be valid
  if (stableCount < 3) {
    reading.valid = false;
    reading.night = false;
    return reading;
  }

  reading.valid = true;
  reading.night = (currentState == 1);
  return reading;
}
