#ifndef SENSOR_SOIL_H
#define SENSOR_SOIL_H

#include <Arduino.h>
#include "../../config/pins.h"

struct SoilReading {
  int raw;
  int percent;
  bool valid;
};

void initSoilSensor();
SoilReading readSoilMoisture();

#endif
