#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "../../config/pins.h"
#include "../actuator/actuator.h"

struct DisplayData {
  float temperature;
  float humidity;
  bool light;  // true = day
  int soil;
  bool tempValid;
  bool humValid;
  bool lightValid;
  bool soilValid;
  ActuatorType activeActuator;
};

void initDisplay();
void updateDisplay(const DisplayData& data);

#endif
