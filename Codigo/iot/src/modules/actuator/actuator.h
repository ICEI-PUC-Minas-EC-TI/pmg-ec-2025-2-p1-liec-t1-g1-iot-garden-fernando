#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>
#include "../../config/pins.h"

enum ActuatorType {
  ACTUATOR_NONE = 0,
  ACTUATOR_SPRINKLER,
  ACTUATOR_FAN,
  ACTUATOR_LIGHT
};

enum ActuatorSource {
  SOURCE_NONE = 0,
  SOURCE_MANUAL,   // MQTT manual command (highest priority)
  SOURCE_TRIGGER   // Sensor trigger (lower priority)
};

// Initialize all actuator pins
void initActuators();

// Request to activate an actuator
// Returns true if activated, false if blocked
bool requestActuator(ActuatorType type, bool on, ActuatorSource source);

// Force release (turn off and unlock)
void releaseActuator();

// Check and auto-release if timeout exceeded (call in main loop)
void checkActuatorTimeout();

// Check current state
ActuatorType getActiveActuator();
ActuatorSource getActiveSource();
bool isActuatorLocked();

// Get actuator name for logging
const char* getActuatorName(ActuatorType type);

#endif
