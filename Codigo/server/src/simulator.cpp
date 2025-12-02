#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>

#include "mqtt_client.h"

constexpr const char* MQTT_BROKER = "test.mosquitto.org";
constexpr int MQTT_PORT = 1883;
constexpr const char* CLIENT_ID = "esp32_simulator";

constexpr const char* TOPIC_TEMPERATURE = "vltim43/sensor/temperature";
constexpr const char* TOPIC_HUMIDITY = "vltim43/sensor/humidity";
constexpr const char* TOPIC_LIGHT = "vltim43/sensor/light";
constexpr const char* TOPIC_SOIL = "vltim43/sensor/soil";

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << "\nStopping simulator..." << std::endl;
    running = false;
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::cout << "=== ESP32 Simulator ===" << std::endl;
    std::cout << "Simulating IoT Garden sensor data" << std::endl;
    std::cout << std::endl;

    MQTTClient mqtt(MQTT_BROKER, MQTT_PORT, CLIENT_ID);

    if (!mqtt.connect()) {
        std::cerr << "Failed to connect to MQTT broker" << std::endl;
        std::cerr << "Make sure mosquitto is running: systemctl start mosquitto" << std::endl;
        return 1;
    }

    // Wait for connection
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    mqtt.loop();

    std::random_device rd;
    std::mt19937 gen(rd());

    // Simulate realistic sensor values
    float baseTemp = 22.0f;
    float baseHumidity = 55.0f;
    int baseSoil = 2000;  // Raw ADC value (0-4095, higher = drier)
    bool isDay = true;
    int cycleCount = 0;

    std::cout << "Publishing sensor data every second..." << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    std::cout << std::endl;

    while (running) {
        // Add some variation to temperature (20-28°C range)
        std::normal_distribution<float> tempDist(0, 0.5f);
        float temp = baseTemp + tempDist(gen) + std::sin(cycleCount * 0.1f) * 2.0f;
        temp = std::clamp(temp, 18.0f, 32.0f);

        // Add some variation to humidity (40-70% range)
        std::normal_distribution<float> humDist(0, 1.0f);
        float humidity = baseHumidity + humDist(gen) + std::cos(cycleCount * 0.1f) * 5.0f;
        humidity = std::clamp(humidity, 35.0f, 75.0f);

        // Toggle day/night every ~30 cycles
        if (cycleCount % 30 == 0) {
            isDay = !isDay;
        }

        // Soil moisture variation (slowly drifts, occasional "watering")
        std::uniform_int_distribution<int> soilDist(-50, 50);
        int soil = baseSoil + soilDist(gen) + static_cast<int>(std::sin(cycleCount * 0.05f) * 300);
        soil = std::clamp(soil, 500, 3500);

        // Format values like ESP32 does
        char tempStr[10];
        char humStr[10];
        char soilStr[10];
        snprintf(tempStr, sizeof(tempStr), "%.1f", temp);
        snprintf(humStr, sizeof(humStr), "%.1f", humidity);
        snprintf(soilStr, sizeof(soilStr), "%d", soil);
        const char* lightStr = isDay ? "day" : "night";  // Match ESP32 format

        // Publish
        mqtt.publish(TOPIC_TEMPERATURE, tempStr);
        mqtt.publish(TOPIC_HUMIDITY, humStr);
        mqtt.publish(TOPIC_LIGHT, lightStr);
        mqtt.publish(TOPIC_SOIL, soilStr);

        std::cout << "Temp: " << tempStr << "C | "
                  << "Humidity: " << humStr << "% | "
                  << "Light: " << (isDay ? "DAY" : "NIGHT") << " | "
                  << "Soil: " << soilStr << std::endl;

        cycleCount++;

        // 500ms interval
        for (int i = 0; i < 5 && running; i++) {
            mqtt.loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    std::cout << "Simulator stopped." << std::endl;
    return 0;
}
