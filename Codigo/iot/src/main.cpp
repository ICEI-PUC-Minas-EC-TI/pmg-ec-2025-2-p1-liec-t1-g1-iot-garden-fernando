#include <Arduino.h>

#include "modules/mqtt/mqtt.h"
#include "modules/sensor_light/sensor_light.h"
#include "modules/sensor_soil/sensor_soil.h"
#include "modules/sensor_dht/sensor_dht.h"
#include "modules/actuator/actuator.h"
#include "modules/display/display.h"
#include "modules/triggers/triggers.h"

unsigned long lastCheckTime = 0;
const unsigned long CHECK_INTERVAL = 2000;

void setup() {
  Serial.begin(115200);
  Serial.println("IoT Garden Starting...");

  initDisplay();
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
      dtostrf(dht.humidity, 4, 1, humStr);  // DHT already returns raw % (0-100)
      publishMQTT(TOPIC_PUBLISH_TEMPERATURE, tempStr);
      publishMQTT(TOPIC_PUBLISH_HUMIDITY, humStr);
    }

    if (light.valid) {
      publishMQTT(TOPIC_PUBLISH_LIGHT, light.night ? "night" : "day");
    }

    if (soil.valid) {
      char soilStr[10];
      itoa(soil.raw, soilStr, 10);  // Send raw ADC value
      publishMQTT(TOPIC_PUBLISH_SOIL, soilStr);
    }

    // Update OLED display
    DisplayData displayData;
    displayData.temperature = dht.temperature;
    displayData.humidity = dht.humidity;
    displayData.light = !light.night;
    displayData.soil = soil.raw;
    displayData.tempValid = dht.valid;
    displayData.humValid = dht.valid;
    displayData.lightValid = light.valid;
    displayData.soilValid = soil.valid;
    displayData.activeActuator = getActiveActuator();
    updateDisplay(displayData);

    // Process automatic triggers
    SensorData sensors;
    sensors.dht = dht;
    sensors.light = light;
    sensors.soil = soil;
    processTriggers(sensors);
  }
}
