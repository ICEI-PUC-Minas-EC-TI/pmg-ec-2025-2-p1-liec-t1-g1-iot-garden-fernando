# IOT Garden

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-F5822A?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![MQTT](https://img.shields.io/badge/MQTT-660066?style=for-the-badge&logo=mqtt&logoColor=white)
![SQLite](https://img.shields.io/badge/SQLite-003B57?style=for-the-badge&logo=sqlite&logoColor=white)

## Testing

```bash
# Build server
cmake -B server/build -S server && cmake --build server/build

# Start data collector
./server/build/iot_garden_server

# Start simulator (in another terminal)
./server/build/esp32_simulator

# Watch database
watch -n 2 'sqlite3 server/build/garden.db "SELECT * FROM sensor_readings ORDER BY timestamp DESC LIMIT 10;"'

# Debug MQTT messages
mosquitto_sub -h test.mosquitto.org -t "vltim43/sensor/#" -v
```

## Architecture

```
                                  ┌─────────────┐
                                  │     App     │
                                  └──────┬──────┘
                                         │ commands
                                         ▼
┌─────────────┐       MQTT        ┌─────────────┐
│   ESP32     │ ◄───────────────► │  Mosquitto  │
│  (sensors)  │   sensor/*        │   Broker    │
└─────────────┘                   └──────┬──────┘
                                         │
                                         ▼
                                  ┌─────────────┐      ┌─────────────┐
                                  │   Server    │ ───► │   SQLite    │
                                  └─────────────┘      │  garden.db  │
                                                       └─────────────┘
```

# MQTT Topics

## Actuators

```bash
# GPIO 4
mosquitto_pub -h test.mosquitto.org -t "vltim43/actuator/fan" -m "on"
# GPIO 23
mosquitto_pub -h test.mosquitto.org -t "vltim43/actuator/sprinkler" -m "on"
# GPIO 19
mosquitto_pub -h test.mosquitto.org -t "vltim43/actuator/light" -m "on"


# GPIO 2
mosquitto_pub -h test.mosquitto.org -t "vltim43/led/control" -m "on"
```

## Sensors

```bash
mosquitto_sub -h test.mosquitto.org -t "vltim43/sensor/temperature"  # DHT11 GPIO 25
mosquitto_sub -h test.mosquitto.org -t "vltim43/sensor/humidity"     # DHT11 GPIO 25
mosquitto_sub -h test.mosquitto.org -t "vltim43/sensor/light"        # Photoresistor GPIO 26
mosquitto_sub -h test.mosquitto.org -t "vltim43/sensor/soil"         # Soil Moisture GPIO 34
```

## Wiring

| Component     | GPIO Pin |
| ------------- | -------- |
| Sprinkler LED | 23       |
| Fan LED       | 4        |
| Light LED     | 19       |
| DHT11         | 25       |
| Photoresistor | 26       |
| Soil Moisture | 34       |
| OLED SDA      | 21       |
| OLED SCL      | 22       |
