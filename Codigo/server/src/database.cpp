#include "database.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Database::Database(const std::string& db_path)
    : m_dbPath(db_path), m_db(nullptr) {}

Database::~Database() {
    close();
}

bool Database::open() {
    int rc = sqlite3_open(m_dbPath.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        std::cerr << "[DB] Failed to open database: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    std::cout << "[DB] Opened database: " << m_dbPath << std::endl;
    return createTables();
}

void Database::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool Database::isOpen() const {
    return m_db != nullptr;
}

bool Database::createTables() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS sensor_readings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            temperature REAL NOT NULL,
            humidity REAL NOT NULL,
            light INTEGER NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        );
        CREATE INDEX IF NOT EXISTS idx_timestamp ON sensor_readings(timestamp);
    )";

    return execute(sql);
}

bool Database::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "[DB] SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::insertReading(float temperature, float humidity, bool light) {
    if (!m_db) return false;

    const char* sql = "INSERT INTO sensor_readings (temperature, humidity, light) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "[DB] Prepare failed: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    sqlite3_bind_double(stmt, 1, temperature);
    sqlite3_bind_double(stmt, 2, humidity);
    sqlite3_bind_int(stmt, 3, light ? 1 : 0);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "[DB] Insert failed: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    return true;
}

std::vector<SensorReading> Database::getReadings(int limit) {
    std::vector<SensorReading> readings;
    if (!m_db) return readings;

    const char* sql = "SELECT id, temperature, humidity, light, timestamp FROM sensor_readings "
                      "ORDER BY timestamp DESC LIMIT ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return readings;
    }

    sqlite3_bind_int(stmt, 1, limit);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        SensorReading reading;
        reading.id = sqlite3_column_int(stmt, 0);
        reading.temperature = static_cast<float>(sqlite3_column_double(stmt, 1));
        reading.humidity = static_cast<float>(sqlite3_column_double(stmt, 2));
        reading.light = sqlite3_column_int(stmt, 3) != 0;
        reading.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        readings.push_back(reading);
    }

    sqlite3_finalize(stmt);
    return readings;
}

std::optional<SensorReading> Database::getLatestReading() {
    if (!m_db) return std::nullopt;

    const char* sql = "SELECT id, temperature, humidity, light, timestamp FROM sensor_readings "
                      "ORDER BY timestamp DESC LIMIT 1";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    std::optional<SensorReading> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        SensorReading reading;
        reading.id = sqlite3_column_int(stmt, 0);
        reading.temperature = static_cast<float>(sqlite3_column_double(stmt, 1));
        reading.humidity = static_cast<float>(sqlite3_column_double(stmt, 2));
        reading.light = sqlite3_column_int(stmt, 3) != 0;
        reading.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        result = reading;
    }

    sqlite3_finalize(stmt);
    return result;
}
