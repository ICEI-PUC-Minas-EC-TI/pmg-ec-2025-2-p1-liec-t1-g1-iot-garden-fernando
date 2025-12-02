#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>

struct SensorReading {
    int id;
    float temperature;
    float humidity;
    bool light;
    int soil;
    std::string timestamp;
};

class Database {
public:
    Database(const std::string& db_path);
    ~Database();

    bool open();
    void close();
    bool isOpen() const;

    bool insertReading(float temperature, float humidity, bool light, int soil);
    std::vector<SensorReading> getReadings(int limit = 100);
    std::optional<SensorReading> getLatestReading();

private:
    bool createTables();
    bool execute(const std::string& sql);

    sqlite3* m_db;
    std::string m_dbPath;
};

#endif
