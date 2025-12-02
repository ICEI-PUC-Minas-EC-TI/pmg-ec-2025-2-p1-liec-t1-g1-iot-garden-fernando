#include "mqtt_client.h"
#include <iostream>
#include <cstring>

MQTTClient::MQTTClient(const std::string& host, int port, const std::string& client_id)
    : m_host(host), m_port(port), m_clientId(client_id), m_connected(false), m_mosq(nullptr) {

    mosquitto_lib_init();
    m_mosq = mosquitto_new(client_id.c_str(), true, this);

    if (m_mosq) {
        mosquitto_connect_callback_set(m_mosq, onConnect);
        mosquitto_disconnect_callback_set(m_mosq, onDisconnect);
        mosquitto_message_callback_set(m_mosq, onMessage);
    }
}

MQTTClient::~MQTTClient() {
    disconnect();
    if (m_mosq) {
        mosquitto_destroy(m_mosq);
    }
    mosquitto_lib_cleanup();
}

bool MQTTClient::connect() {
    if (!m_mosq) return false;

    int rc = mosquitto_connect(m_mosq, m_host.c_str(), m_port, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "[MQTT] Connection failed: " << mosquitto_strerror(rc) << std::endl;
        return false;
    }

    return true;
}

void MQTTClient::disconnect() {
    if (m_mosq && m_connected) {
        mosquitto_disconnect(m_mosq);
        m_connected = false;
    }
}

bool MQTTClient::subscribe(const std::string& topic) {
    if (!m_mosq) return false;

    if (!m_connected) {
        m_pendingSubscriptions.push_back(topic);
        return true;
    }

    int rc = mosquitto_subscribe(m_mosq, nullptr, topic.c_str(), 0);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "[MQTT] Subscribe failed: " << mosquitto_strerror(rc) << std::endl;
        return false;
    }

    std::cout << "[MQTT] Subscribed to: " << topic << std::endl;
    return true;
}

bool MQTTClient::publish(const std::string& topic, const std::string& payload) {
    if (!m_mosq || !m_connected) return false;

    int rc = mosquitto_publish(m_mosq, nullptr, topic.c_str(),
                               payload.length(), payload.c_str(), 0, false);
    return rc == MOSQ_ERR_SUCCESS;
}

void MQTTClient::setMessageCallback(MessageCallback callback) {
    m_messageCallback = callback;
}

void MQTTClient::loop() {
    if (m_mosq) {
        mosquitto_loop(m_mosq, -1, 1);
    }
}

bool MQTTClient::isConnected() const {
    return m_connected;
}

void MQTTClient::onConnect(struct mosquitto* mosq, void* obj, int rc) {
    MQTTClient* client = static_cast<MQTTClient*>(obj);

    if (rc == 0) {
        std::cout << "[MQTT] Connected to broker" << std::endl;
        client->m_connected = true;

        for (const auto& topic : client->m_pendingSubscriptions) {
            mosquitto_subscribe(mosq, nullptr, topic.c_str(), 0);
            std::cout << "[MQTT] Subscribed to: " << topic << std::endl;
        }
        client->m_pendingSubscriptions.clear();
    } else {
        std::cerr << "[MQTT] Connection failed: " << mosquitto_connack_string(rc) << std::endl;
        client->m_connected = false;
    }
}

void MQTTClient::onDisconnect(struct mosquitto* mosq, void* obj, int rc) {
    MQTTClient* client = static_cast<MQTTClient*>(obj);
    client->m_connected = false;

    if (rc != 0) {
        std::cerr << "[MQTT] Unexpected disconnect, will reconnect..." << std::endl;
        mosquitto_reconnect(mosq);
    } else {
        std::cout << "[MQTT] Disconnected" << std::endl;
    }
}

void MQTTClient::onMessage(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {
    MQTTClient* client = static_cast<MQTTClient*>(obj);

    if (client->m_messageCallback && msg->payload) {
        std::string topic(msg->topic);
        std::string payload(static_cast<char*>(msg->payload), msg->payloadlen);
        client->m_messageCallback(topic, payload);
    }
}
