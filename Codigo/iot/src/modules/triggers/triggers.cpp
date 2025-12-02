#include "triggers.h"
#include "../log/log.h"

// Track what triggers have requested (to know when to turn off)
static bool fanTriggered = false;
static bool sprinklerTriggered = false;
static bool lightTriggered = false;

void processTriggers(const SensorData& sensors) {
  // --- Fan trigger (temperature) ---
  if (TRIGGER_FAN_ENABLED && sensors.dht.valid) {
    if (!fanTriggered && sensors.dht.temperature >= TRIGGER_FAN_TEMP_ON) {
      if (requestActuator(ACTUATOR_FAN, true, SOURCE_TRIGGER)) {
        fanTriggered = true;
      }
    } else if (fanTriggered && sensors.dht.temperature <= TRIGGER_FAN_TEMP_OFF) {
      requestActuator(ACTUATOR_FAN, false, SOURCE_TRIGGER);
      fanTriggered = false;
    }
  }

  // --- Sprinkler trigger (soil moisture) ---
  if (TRIGGER_SPRINKLER_ENABLED && sensors.soil.valid) {
    if (!sprinklerTriggered && sensors.soil.raw >= TRIGGER_SPRINKLER_DRY) {
      if (requestActuator(ACTUATOR_SPRINKLER, true, SOURCE_TRIGGER)) {
        sprinklerTriggered = true;
      }
    } else if (sprinklerTriggered && sensors.soil.raw <= TRIGGER_SPRINKLER_WET) {
      requestActuator(ACTUATOR_SPRINKLER, false, SOURCE_TRIGGER);
      sprinklerTriggered = false;
    }
  }

  // --- Light trigger (night/day) ---
  if (TRIGGER_LIGHT_ENABLED && sensors.light.valid) {
    if (!lightTriggered && sensors.light.night) {
      if (requestActuator(ACTUATOR_LIGHT, true, SOURCE_TRIGGER)) {
        lightTriggered = true;
      }
    } else if (lightTriggered && !sensors.light.night) {
      requestActuator(ACTUATOR_LIGHT, false, SOURCE_TRIGGER);
      lightTriggered = false;
    }
  }
}
