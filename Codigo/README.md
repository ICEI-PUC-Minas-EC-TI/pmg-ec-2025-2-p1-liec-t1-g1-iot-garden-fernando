# IOT Garden

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-F5822A?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![MQTT](https://img.shields.io/badge/MQTT-660066?style=for-the-badge&logo=mqtt&logoColor=white)
![SQLite](https://img.shields.io/badge/SQLite-003B57?style=for-the-badge&logo=sqlite&logoColor=white)

## Build

```bash
cmake -B server/build -S server && cmake --build server/build
```

## Testing

```bash
# Start mosquitto
sudo systemctl start mosquitto

# Terminal 1 - start data collector
cd server/build && ./iot_garden_server

# Terminal 2 - start simulator
cd server/build && ./esp32_simulator

# Terminal 3 - watch data
watch -n 2 'sqlite3 build/garden.db "SELECT * FROM sensor_readings ORDER BY timestamp DESC LIMIT 10;"'

# Debug MQTT messages
mosquitto_sub -t "sensor/#" -v
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
# GPIO 23
mosquitto_pub -h test.mosquitto.org -t "vltim43/actuator/fan" -m "on"
# GPIO 4
mosquitto_pub -h test.mosquitto.org -t "vltim43/actuator/sprinkler" -m "on"
# GPIO 19
mosquitto_pub -h test.mosquitto.org -t "vltim43/actuator/light" -m "on"


# GPIO 2
mosquitto_pub -h test.mosquitto.org -t "vltim43/led/control" -m "on"
```

## Sensors

```bash
mosquitto_sub -h test.mosquitto.org -t "vltim43/actuator/light"  # Photoresistor GPIO 22
mosquitto_sub -h test.mosquitto.org -t "vltim43/sensor/soil"     # Soil Moisture GPIO 34
```
