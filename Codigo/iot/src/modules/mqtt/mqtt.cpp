#include "mqtt.h"
#include "../log/log.h"
#include "../actuator/actuator.h"

WiFiClient espClient;
PubSubClient MQTT(espClient);

void mqtt_callback(char* topic, byte* payload, unsigned int length);

void initWiFi() {
  pinMode(PIN_LED_BUILTIN, OUTPUT);
}

void initMQTT() {
  MQTT.setServer(MQTT_BROKER, MQTT_PORT);
  MQTT.setCallback(mqtt_callback);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  String topicStr = String(topic);
  logKeyValue(LOG_MQTT, "Topic", topicStr.c_str());
  logKeyValue(LOG_MQTT, "Received", msg.c_str());

  if (topicStr == TOPIC_SUBSCRIBE_LED) {
    // LED is separate, not part of actuator state machine
    if (msg == "on") {
      digitalWrite(PIN_LED_BUILTIN, HIGH);
      logInfo(LOG_MQTT, "LED ON");
    } else if (msg == "off") {
      digitalWrite(PIN_LED_BUILTIN, LOW);
      logInfo(LOG_MQTT, "LED OFF");
    }
  } else if (topicStr == TOPIC_ACTUATOR_SPRINKLER) {
    requestActuator(ACTUATOR_SPRINKLER, msg == "on", SOURCE_MANUAL);
  } else if (topicStr == TOPIC_ACTUATOR_FAN) {
    requestActuator(ACTUATOR_FAN, msg == "on", SOURCE_MANUAL);
  } else if (topicStr == TOPIC_ACTUATOR_LIGHT) {
    requestActuator(ACTUATOR_LIGHT, msg == "on", SOURCE_MANUAL);
  } else {
    logError(LOG_MQTT, "Unknown topic");
  }
}

bool connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return true;

  logSection("WiFi");
  logKeyValue(LOG_WIFI, "SSID", WIFI_SSID);
  logInfo(LOG_WIFI, "Connecting...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    logSuccess(LOG_WIFI, "Connected");
    logKeyValue(LOG_WIFI, "IP", WiFi.localIP());
    logKeyValue(LOG_WIFI, "RSSI", WiFi.RSSI());
    return true;
  } else {
    logError(LOG_WIFI, "Connection timeout");
    return false;
  }
}

bool connectMQTT(bool showHeader = true) {
  if (MQTT.connected()) return true;

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  if (showHeader) {
    logSection("MQTT");
    logKeyValue(LOG_MQTT, "Broker", MQTT_BROKER);
    logKeyValue(LOG_MQTT, "Port", MQTT_PORT);
  }

  int retries = 0;
  while (!MQTT.connected() && retries < MAX_MQTT_RETRIES) {
    retries++;
    logInfo(LOG_MQTT, "Connecting...");

    if (MQTT.connect(ID_MQTT)) {
      logSuccess(LOG_MQTT, "Connected");

      MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
      MQTT.subscribe(TOPIC_ACTUATOR_SPRINKLER);
      MQTT.subscribe(TOPIC_ACTUATOR_FAN);
      MQTT.subscribe(TOPIC_ACTUATOR_LIGHT);
      logInfo(LOG_MQTT, "Subscribed to all topics");
      return true;
    } else {
      logKeyValue(LOG_MQTT, "Failed, code", MQTT.state());
      if (retries < MAX_MQTT_RETRIES) {
        delay(MQTT_RETRY_DELAY);
      }
    }
  }

  logError(LOG_MQTT, "Max retries reached");
  return false;
}

void reconnectWiFi() {
  connectWiFi();
}

void reconnectMQTT() {
  connectMQTT();
}

void checkWiFiAndMQTTConnections() {
  static bool wifiWasConnected = true;
  static bool mqttWasConnected = true;

  // Only log on state change
  if (WiFi.status() != WL_CONNECTED) {
    if (wifiWasConnected) {
      logInfo(LOG_WIFI, "Connection lost, reconnecting...");
      wifiWasConnected = false;
    }
    connectWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      wifiWasConnected = true;
    }
  }

  if (!MQTT.connected() && WiFi.status() == WL_CONNECTED) {
    if (mqttWasConnected) {
      logInfo(LOG_MQTT, "Connection lost, reconnecting...");
      mqttWasConnected = false;
    }
    connectMQTT(false);  // No header on reconnect
    if (MQTT.connected()) {
      mqttWasConnected = true;
    }
  }
}

void mqttLoop() {
  if (MQTT.connected()) {
    MQTT.loop();
  }
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool isMQTTConnected() {
  return MQTT.connected();
}

bool publishMQTT(const char* topic, const char* payload) {
  if (MQTT.connected()) {
    return MQTT.publish(topic, payload);
  }
  return false;
}
