#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <string>
#include "SensorManager.h"
#include "WiFiManager.h"
#include "EnvironmentConfig.h" // MQTT_SERVER_URL, MQTT_PORT

class MQTTManager
{
public:
    // Constructors
    MQTTManager(WiFiManager *wifiMgr, SensorManager *sensorMgr);

    void setup(const char *user, const char *password, const char *clientID);
    void setup(const char *clientID);
    void setup(const char *clientID, bool useCredentials);

    void reconnect();
    void publishSensorData();
    void subscribe(const char *topic);
    void loop();

    bool isConnected();

private:
    PubSubClient client;
    std::string macAddress;
    SensorManager *sensorManager; // Pointer to SensorManager
    WiFiManager *wifiManager;     // Pointer to WiFiManager
    const char *mqttUser = nullptr;
    const char *mqttPassword = nullptr;
    const char *mqttClientID;
    static MQTTManager *instance;

    void resolveDomainToIP();
    void callback(char *topic, byte *payload, unsigned int length);
    void handleIncomingMessage(const char *topic, const char *payload);
};

#endif // MQTTMANAGER_H
