#ifndef SENSOR_DHT_H
#define SENSOR_DHT_H

#include <Arduino.h>
#include <DHTesp.h>
#include "../../config/pins.h"

struct DHTReading {
  float temperature;
  float humidity;
  bool valid;
};

void initDHTSensor();
DHTReading readDHT();

#endif
