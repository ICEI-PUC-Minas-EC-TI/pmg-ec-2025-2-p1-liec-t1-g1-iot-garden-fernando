#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <mosquitto.h>
#include <string>
#include <functional>
#include <vector>

class MQTTClient {
public:
    using MessageCallback = std::function<void(const std::string& topic, const std::string& payload)>;

    MQTTClient(const std::string& host, int port, const std::string& client_id);
    ~MQTTClient();

    bool connect();
    void disconnect();
    bool subscribe(const std::string& topic);
    bool publish(const std::string& topic, const std::string& payload);
    void setMessageCallback(MessageCallback callback);
    void loop();
    bool isConnected() const;

private:
    static void onConnect(struct mosquitto* mosq, void* obj, int rc);
    static void onDisconnect(struct mosquitto* mosq, void* obj, int rc);
    static void onMessage(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);

    struct mosquitto* m_mosq;
    std::string m_host;
    int m_port;
    std::string m_clientId;
    bool m_connected;
    MessageCallback m_messageCallback;
    std::vector<std::string> m_pendingSubscriptions;
};

#endif
