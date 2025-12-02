cmake_minimum_required(VERSION 3.16)
project(iot_garden_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${MOSQUITTO_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/mqtt_client.cpp
    src/database.cpp
)

# Server executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${MOSQUITTO_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    pthread
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${MOSQUITTO_LIBRARY_DIRS}
    ${SQLITE3_LIBRARY_DIRS}
)

# Simulator executable (for testing without ESP32)
add_executable(esp32_simulator
    src/simulator.cpp
    src/mqtt_client.cpp
)

target_link_libraries(esp32_simulator
    ${MOSQUITTO_LIBRARIES}
    pthread
)

target_link_directories(esp32_simulator PRIVATE
    ${MOSQUITTO_LIBRARY_DIRS}
)
