#include <iostream>
#include <csignal>
#include <atomic>
#include <mutex>
#include <cstring>
#include <chrono>

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
constexpr const char* TOPIC_SOIL = "vltim43/sensor/soil";

// Timeout: insert partial data if not all sensors respond within this time
constexpr int BUFFER_TIMEOUT_MS = 10000;  // 10 seconds

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
    int soil = 0;
    bool hasTemp = false, hasHum = false, hasLight = false, hasSoil = false;
    std::mutex bufferMutex;
    auto lastReceiveTime = std::chrono::steady_clock::now();

    // Helper to flush buffer and insert reading
    auto flushBuffer = [&](bool partial = false) {
        if (partial) {
            std::cout << "[PARTIAL] ";
        }
        std::cout << "Temp: " << (hasTemp ? std::to_string(temperature) + "C" : "--") << " | "
                  << "Humidity: " << (hasHum ? std::to_string((int)humidity) + "%" : "--") << " | "
                  << "Light: " << (hasLight ? (light ? "DAY" : "NIGHT") : "--") << " | "
                  << "Soil: " << (hasSoil ? std::to_string(soil) : "--") << std::endl;

        if (!db.insertReading(temperature, humidity, light, soil)) {
            std::cerr << "Failed to store reading" << std::endl;
        }

        hasTemp = hasHum = hasLight = hasSoil = false;
        temperature = 0; humidity = 0; light = false; soil = 0;
        lastReceiveTime = std::chrono::steady_clock::now();
    };

    // Set up message handler
    mqtt.setMessageCallback([&](const std::string& topic, const std::string& payload) {
        std::lock_guard<std::mutex> lock(bufferMutex);

        // Update timestamp on any message
        lastReceiveTime = std::chrono::steady_clock::now();

        if (topic == TOPIC_TEMPERATURE) {
            temperature = std::stof(payload);
            hasTemp = true;
        } else if (topic == TOPIC_HUMIDITY) {
            humidity = std::stof(payload);
            hasHum = true;
        } else if (topic == TOPIC_LIGHT) {
            light = (payload == "day");
            hasLight = true;
        } else if (topic == TOPIC_SOIL) {
            soil = std::stoi(payload);
            hasSoil = true;
        }

        // Once we have all 4 values, insert and reset
        if (hasTemp && hasHum && hasLight && hasSoil) {
            flushBuffer(false);
        }
    });

    // Subscribe to sensor topics
    mqtt.subscribe(TOPIC_TEMPERATURE);
    mqtt.subscribe(TOPIC_HUMIDITY);
    mqtt.subscribe(TOPIC_LIGHT);
    mqtt.subscribe(TOPIC_SOIL);

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

        // Check for timeout - flush partial data if waiting too long
        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            bool hasAny = hasTemp || hasHum || hasLight || hasSoil;
            if (hasAny) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastReceiveTime).count();
                if (elapsed >= BUFFER_TIMEOUT_MS) {
                    flushBuffer(true);
                }
            }
        }
    }

    std::cout << "Server stopped." << std::endl;
    return 0;
}
