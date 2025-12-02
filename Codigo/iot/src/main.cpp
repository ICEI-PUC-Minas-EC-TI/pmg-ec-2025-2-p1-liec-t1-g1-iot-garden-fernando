#include <Arduino.h>

#include "modules/mqtt/mqtt.h"
#include "modules/sensor_light/sensor_light.h"
#include "modules/sensor_soil/sensor_soil.h"
#include "modules/sensor_dht/sensor_dht.h"
#include "modules/actuator/actuator.h"

unsigned long lastCheckTime = 0;
const unsigned long CHECK_INTERVAL = 2000;

void setup() {
  Serial.begin(115200);
  Serial.println("IoT Garden Starting...");

  initActuators();
  initLightSensor();
  initSoilSensor();
  initDHTSensor();
  initWiFi();
  reconnectWiFi();
  initMQTT();
  reconnectMQTT();
}

void loop() {
  checkWiFiAndMQTTConnections();
  mqttLoop();
  checkActuatorTimeout();

  if (millis() - lastCheckTime >= CHECK_INTERVAL) {
    lastCheckTime = millis();

    // DHT sensor (temperature & humidity)
    DHTReading dht = readDHT();
    Serial.print("Temp: ");
    if (dht.valid) {
      Serial.print(dht.temperature);
      Serial.print("C");
    } else {
      Serial.print("--");
    }

    Serial.print(" | Humidity: ");
    if (dht.valid) {
      Serial.print(dht.humidity);
      Serial.print("%");
    } else {
      Serial.print("--");
    }

    // Light sensor
    LightReading light = readLight();
    Serial.print(" | Light: ");
    if (light.valid) {
      Serial.print(light.night ? "NIGHT" : "DAY");
    } else {
      Serial.print("--");
    }

    // Soil moisture sensor
    SoilReading soil = readSoilMoisture();
    Serial.print(" | Soil: ");
    if (soil.valid) {
      Serial.print(soil.percent);
      Serial.print("%");
    } else {
      Serial.print("--");
    }

    Serial.print(" | Active: ");
    Serial.println(getActuatorName(getActiveActuator()));

    // Publish sensor data to MQTT
    if (dht.valid) {
      char tempStr[10];
      char humStr[10];
      dtostrf(dht.temperature, 4, 1, tempStr);
      dtostrf(dht.humidity, 4, 1, humStr);
      publishMQTT(TOPIC_PUBLISH_TEMPERATURE, tempStr);
      publishMQTT(TOPIC_PUBLISH_HUMIDITY, humStr);
    }

    if (light.valid) {
      publishMQTT(TOPIC_PUBLISH_LIGHT, light.night ? "night" : "day");
    }

    if (soil.valid) {
      char soilStr[10];
      itoa(soil.percent, soilStr, 10);
      publishMQTT(TOPIC_PUBLISH_SOIL, soilStr);
    }

    // Sensor triggers (only if valid readings)
    if (light.valid) {
      if (light.night) {
        requestActuator(ACTUATOR_LIGHT, true, SOURCE_TRIGGER);
      } else {
        requestActuator(ACTUATOR_LIGHT, false, SOURCE_TRIGGER);
      }
    }

    if (soil.valid && soil.percent < 30) {
      requestActuator(ACTUATOR_SPRINKLER, true, SOURCE_TRIGGER);
    } else if (soil.valid) {
      requestActuator(ACTUATOR_SPRINKLER, false, SOURCE_TRIGGER);
    }

    if (dht.valid && dht.temperature > 30) {
      requestActuator(ACTUATOR_FAN, true, SOURCE_TRIGGER);
    } else if (dht.valid) {
      requestActuator(ACTUATOR_FAN, false, SOURCE_TRIGGER);
    }
  }
}
