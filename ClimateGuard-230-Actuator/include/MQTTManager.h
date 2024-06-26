#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>
#include "EnvironmentConfig.h"
#include "WiFiManager.h"
#include "DisplayManager.h"

class MQTTManager
{
public:
    MQTTManager(WiFiClient &wifiClient, DisplayManager &displayManager);
    void setup(const char *clientID);
    void setup(const char *user, const char *password, const char *clientID);
    void loop();
    void subscribeToTopics();
    void publishMode(const char *mode);
    bool waitForAllTopics();
    bool waitForInitialData();
    bool isConnected();

    float getTemperature() const;
    float getHumidity() const;
    float getPressure() const;
    float getAirQuality() const;
    float getLightIntensity() const;
    bool getRainSensor() const;
    IPAddress getIP() const;
    const char *getMAC() const;

private:
    PubSubClient client;
    float temperature;
    float humidity;
    float pressure;
    float airQuality;
    float lightIntensity;
    bool rainSensor;
    IPAddress ip;
    const char *mac;

    bool receivedTemperature = false;
    bool receivedHumidity = false;
    bool receivedPressure = false;
    bool receivedAirQuality = false;
    bool receivedLightIntensity = false;
    bool receivedRainSensor = false;
    bool receivedModeChange = false;
    bool receivedIP = false;
    bool receivedMAC = false;

    const char *mqttUser;
    const char *mqttPassword;
    const char *mqttClientID;
    WiFiManager *wifiManager;
    DisplayManager *displayManager;

    static MQTTManager *instance;
    static void mqttCallback(char *topic, byte *payload, unsigned int length);
    void handleIncomingMessage(const char *topic, const char *payload);
    void resolveDomainToIP();
    void reconnect();
};

#endif
