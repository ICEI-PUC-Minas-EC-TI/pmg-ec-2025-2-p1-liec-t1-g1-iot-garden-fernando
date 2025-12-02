#include "actuator.h"
#include "../log/log.h"

#define ACTUATOR_TIMEOUT_MS 5000  // 5 seconds max

static ActuatorType activeActuator = ACTUATOR_NONE;
static ActuatorSource activeSource = SOURCE_NONE;
static unsigned long activationTime = 0;

void initActuators() {
  pinMode(PIN_SPRINKLER, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_LIGHT, OUTPUT);

  digitalWrite(PIN_SPRINKLER, LOW);
  digitalWrite(PIN_FAN, LOW);
  digitalWrite(PIN_LIGHT, LOW);

  logSuccess(LOG_SENSOR, "Actuators initialized");
}

static int getActuatorPin(ActuatorType type) {
  switch (type) {
    case ACTUATOR_SPRINKLER: return PIN_SPRINKLER;
    case ACTUATOR_FAN: return PIN_FAN;
    case ACTUATOR_LIGHT: return PIN_LIGHT;
    default: return -1;
  }
}

const char* getActuatorName(ActuatorType type) {
  switch (type) {
    case ACTUATOR_SPRINKLER: return "Sprinkler";
    case ACTUATOR_FAN: return "Fan";
    case ACTUATOR_LIGHT: return "Light";
    default: return "None";
  }
}

static void turnOffAll() {
  digitalWrite(PIN_SPRINKLER, LOW);
  digitalWrite(PIN_FAN, LOW);
  digitalWrite(PIN_LIGHT, LOW);
}

bool requestActuator(ActuatorType type, bool on, ActuatorSource source) {
  // Turning off request
  if (!on) {
    // Can only turn off if same source or manual override
    if (activeActuator == type && (activeSource == source || source == SOURCE_MANUAL)) {
      int pin = getActuatorPin(type);
      if (pin >= 0) {
        digitalWrite(pin, LOW);
        logKeyValue(LOG_MQTT, getActuatorName(type), "OFF");
      }
      activeActuator = ACTUATOR_NONE;
      activeSource = SOURCE_NONE;
      return true;
    }
    return false;
  }

  // Turning on request
  // Manual source can always take over
  if (source == SOURCE_MANUAL) {
    turnOffAll();
    activeActuator = type;
    activeSource = source;
    activationTime = millis();

    int pin = getActuatorPin(type);
    if (pin >= 0) {
      digitalWrite(pin, HIGH);
      logKeyValue(LOG_MQTT, getActuatorName(type), "ON (manual)");
    }
    return true;
  }

  // Trigger source: only if nothing is active
  if (source == SOURCE_TRIGGER) {
    if (activeActuator != ACTUATOR_NONE) {
      return false;  // Silently blocked
    }

    activeActuator = type;
    activeSource = source;
    activationTime = millis();

    int pin = getActuatorPin(type);
    if (pin >= 0) {
      digitalWrite(pin, HIGH);
      logKeyValue(LOG_MQTT, getActuatorName(type), "ON (trigger)");
    }
    return true;
  }

  return false;
}

void releaseActuator() {
  turnOffAll();
  activeActuator = ACTUATOR_NONE;
  activeSource = SOURCE_NONE;
  activationTime = 0;
  logInfo(LOG_MQTT, "All actuators released");
}

void checkActuatorTimeout() {
  if (activeActuator != ACTUATOR_NONE && activationTime > 0) {
    if (millis() - activationTime >= ACTUATOR_TIMEOUT_MS) {
      logKeyValue(LOG_MQTT, getActuatorName(activeActuator), "TIMEOUT");
      turnOffAll();
      activeActuator = ACTUATOR_NONE;
      activeSource = SOURCE_NONE;
      activationTime = 0;
    }
  }
}

ActuatorType getActiveActuator() {
  return activeActuator;
}

ActuatorSource getActiveSource() {
  return activeSource;
}

bool isActuatorLocked() {
  return activeActuator != ACTUATOR_NONE;
}
