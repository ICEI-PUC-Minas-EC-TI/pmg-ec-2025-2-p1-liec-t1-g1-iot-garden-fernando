#ifndef LOG_MODULE_H
#define LOG_MODULE_H

#include <Arduino.h>

// Module identifiers for logging
#define LOG_MAIN   "MAIN"
#define LOG_WIFI   "WIFI"
#define LOG_MQTT   "MQTT"
#define LOG_SENSOR "SENSOR"

// Log functions
void logInit();
void logInfo(const char* module, const char* message);
void logError(const char* module, const char* message);
void logSuccess(const char* module, const char* message);
void logSection(const char* title);
void logKeyValue(const char* module, const char* key, const char* value);
void logKeyValue(const char* module, const char* key, int value);
void logKeyValue(const char* module, const char* key, IPAddress value);
void logBoot();

#endif
