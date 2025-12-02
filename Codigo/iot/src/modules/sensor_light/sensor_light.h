#ifndef SENSOR_LIGHT_H
#define SENSOR_LIGHT_H

#include <Arduino.h>
#include "../../config/pins.h"

struct LightReading {
  bool night;
  bool valid;
};

// Function declarations
void initLightSensor();
LightReading readLight();

#endif
