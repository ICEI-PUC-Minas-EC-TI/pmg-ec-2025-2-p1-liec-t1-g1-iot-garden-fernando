#include "log.h"

void logInit() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
}

void logBoot() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("       ESP32 IoT Garden System");
  Serial.println("========================================");
  Serial.println();
}

void logSection(const char* title) {
  Serial.println();
  Serial.print("--- ");
  Serial.print(title);
  Serial.println(" ---");
}

void logInfo(const char* module, const char* message) {
  Serial.print("[");
  Serial.print(module);
  Serial.print("] ");
  Serial.println(message);
}

void logError(const char* module, const char* message) {
  Serial.print("[");
  Serial.print(module);
  Serial.print("] ERROR: ");
  Serial.println(message);
}

void logSuccess(const char* module, const char* message) {
  Serial.print("[");
  Serial.print(module);
  Serial.print("] OK: ");
  Serial.println(message);
}

void logKeyValue(const char* module, const char* key, const char* value) {
  Serial.print("[");
  Serial.print(module);
  Serial.print("] ");
  Serial.print(key);
  Serial.print(": ");
  Serial.println(value);
}

void logKeyValue(const char* module, const char* key, int value) {
  Serial.print("[");
  Serial.print(module);
  Serial.print("] ");
  Serial.print(key);
  Serial.print(": ");
  Serial.println(value);
}

void logKeyValue(const char* module, const char* key, IPAddress value) {
  Serial.print("[");
  Serial.print(module);
  Serial.print("] ");
  Serial.print(key);
  Serial.print(": ");
  Serial.println(value);
}
