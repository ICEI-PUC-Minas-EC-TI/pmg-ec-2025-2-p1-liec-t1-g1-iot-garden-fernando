#include <iostream>
#include <csignal>
#include <atomic>
#include <mutex>
#include <cstring>

#include "mqtt_client.h"
#include "database.h"

// Configuration - match these with your ESP32 settings
constexpr const char* MQTT_BROKER = "test.mosquitto.org";
constexpr int MQTT_PORT = 1883;
constexpr const char* CLIENT_ID = "iot_garden_server";
constexpr const char* DB_PATH = "garden.db";

// Topics from ESP32
constexpr const char* TOPIC_TEMPERATURE = "vltim43/sensor/temperature";
constexpr const char* TOPIC_HUMIDITY = "vltim43/sensor/humidity";
constexpr const char* TOPIC_LIGHT = "vltim43/sensor/light";

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << "\nShutting down..." << std::endl;
    running = false;
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::cout << "=== IoT Garden Server ===" << std::endl;
    std::cout << "Broker: " << MQTT_BROKER << ":" << MQTT_PORT << std::endl;
    std::cout << "Database: " << DB_PATH << std::endl;
    std::cout << std::endl;

    // Initialize database
    Database db(DB_PATH);
    if (!db.open()) {
        std::cerr << "Failed to open database" << std::endl;
        return 1;
    }

    // Initialize MQTT client
    MQTTClient mqtt(MQTT_BROKER, MQTT_PORT, CLIENT_ID);

    // Buffer for sensor values
    float temperature = 0;
    float humidity = 0;
    bool light = false;
    bool hasTemp = false, hasHum = false, hasLight = false;
    std::mutex bufferMutex;

    // Set up message handler
    mqtt.setMessageCallback([&](const std::string& topic, const std::string& payload) {
        std::lock_guard<std::mutex> lock(bufferMutex);

        if (topic == TOPIC_TEMPERATURE) {
            temperature = std::stof(payload);
            hasTemp = true;
        } else if (topic == TOPIC_HUMIDITY) {
            humidity = std::stof(payload);
            hasHum = true;
        } else if (topic == TOPIC_LIGHT) {
            light = (payload == "true" || payload == "1");
            hasLight = true;
        }

        // Once we have all 3 values, insert and reset
        if (hasTemp && hasHum && hasLight) {
            std::cout << "Temp: " << temperature << " °C | "
                      << "Humidity: " << humidity << " % | "
                      << "Light: " << (light ? "Day" : "Night") << std::endl;

            if (!db.insertReading(temperature, humidity, light)) {
                std::cerr << "Failed to store reading" << std::endl;
            }

            hasTemp = hasHum = hasLight = false;
        }
    });

    // Subscribe to sensor topics
    mqtt.subscribe(TOPIC_TEMPERATURE);
    mqtt.subscribe(TOPIC_HUMIDITY);
    mqtt.subscribe(TOPIC_LIGHT);

    // Connect to broker
    if (!mqtt.connect()) {
        std::cerr << "Failed to connect to MQTT broker" << std::endl;
        return 1;
    }

    std::cout << "Server running. Press Ctrl+C to stop." << std::endl;
    std::cout << "Waiting for sensor data..." << std::endl;
    std::cout << std::endl;

    // Main loop
    while (running) {
        mqtt.loop();
    }

    std::cout << "Server stopped." << std::endl;
    return 0;
}
