#ifndef MQTT_MODULE_H
#define MQTT_MODULE_H

#include <PubSubClient.h>
#include <WiFi.h>
#include "../../config/pins.h"

// Configuration constants
#define TOPIC_SUBSCRIBE_LED "vltim43/led/control"
#define TOPIC_ACTUATOR_SPRINKLER "vltim43/actuator/sprinkler"
#define TOPIC_ACTUATOR_FAN "vltim43/actuator/fan"
#define TOPIC_ACTUATOR_LIGHT "vltim43/actuator/light"
#define TOPIC_PUBLISH_TEMPERATURE "vltim43/sensor/temperature"
#define TOPIC_PUBLISH_HUMIDITY "vltim43/sensor/humidity"
#define TOPIC_PUBLISH_LIGHT "vltim43/sensor/light"
#define TOPIC_PUBLISH_SOIL "vltim43/sensor/soil"
#define ID_MQTT "IoT_Garden_ESP32"

// WiFi credentials - set in platformio.ini [credentials] section
#ifndef WIFI_SSID
#define WIFI_SSID "YOUR_WIFI_SSID"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#endif

// MQTT Broker settings - set in platformio.ini [credentials] section
#ifndef MQTT_BROKER
#define MQTT_BROKER "test.mosquitto.org"
#endif
#ifndef MQTT_PORT
#define MQTT_PORT 1883
#endif

// Timeout settings (in milliseconds)
#define WIFI_TIMEOUT 20000
#define MQTT_TIMEOUT 10000
#define MQTT_RETRY_DELAY 3000
#define MAX_MQTT_RETRIES 5


// Function declarations
void initWiFi();
void initMQTT();
void checkWiFiAndMQTTConnections();
void reconnectWiFi();
void reconnectMQTT();
void mqttLoop();
bool isWiFiConnected();
bool isMQTTConnected();
bool publishMQTT(const char* topic, const char* payload);

#endif
