#include "sensor_dht.h"

//   ┌─────────────┐
//   │             │
//   │    DHT11    │
//   │  S          │
//   └──┬───┬───┬──┘
//      │   │   │
//      S  VCC GND

DHTesp dhtSensor;
static bool lastReadFailed = false;

void initDHTSensor() {
  dhtSensor.setup(PIN_DHT, DHTesp::DHT11);
  Serial.println("DHT sensor initialized");
}

DHTReading readDHT() {
  DHTReading reading;

  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  if (dhtSensor.getStatus() != 0) {
    // Only print error once, not every time
    if (!lastReadFailed) {
      Serial.println("DHT: disconnected");
      lastReadFailed = true;
    }
    reading.valid = false;
    reading.temperature = 0;
    reading.humidity = 0;
  } else {
    if (lastReadFailed) {
      Serial.println("DHT: reconnected");
      lastReadFailed = false;
    }
    reading.valid = true;
    reading.temperature = data.temperature;
    reading.humidity = data.humidity;
  }

  return reading;
}
