#include "actuator.h"
#include "../log/log.h"

// Timeout for each actuator (0 = no timeout / indefinite)
#define TIMEOUT_SPRINKLER_MS  5000  // 5 seconds
#define TIMEOUT_FAN_MS        3000  // 3 seconds
#define TIMEOUT_LIGHT_MS      5000  // 5 seconds

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

static unsigned long getActuatorTimeout(ActuatorType type) {
  switch (type) {
    case ACTUATOR_SPRINKLER: return TIMEOUT_SPRINKLER_MS;
    case ACTUATOR_FAN: return TIMEOUT_FAN_MS;
    case ACTUATOR_LIGHT: return TIMEOUT_LIGHT_MS;
    default: return 0;
  }
}

void checkActuatorTimeout() {
  if (activeActuator != ACTUATOR_NONE && activationTime > 0) {
    unsigned long timeout = getActuatorTimeout(activeActuator);
    if (timeout > 0 && millis() - activationTime >= timeout) {
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
