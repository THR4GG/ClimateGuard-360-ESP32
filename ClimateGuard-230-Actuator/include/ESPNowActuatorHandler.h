#ifndef ESPNOWACTUATORHANDLER_H
#define ESPNOWACTUATORHANDLER_H

#include <esp_now.h>
#include <WiFi.h>
#include "EnvironmentConfig.h"

class ESPNowActuatorHandler
{
public:
    ESPNowActuatorHandler();
    void setup();
    void loop();

    float getTemperature() const;
    float getPressure() const;
    float getHumidity() const;
    float getAirQuality() const;
    float getLightIntensity() const;
    bool getRainSensor() const;

    const char *getMQTTBroker() const;
    const char *getOwnIP() const;
    const char *getSQLServer() const;

private:
    float temperature;
    float pressure;
    float humidity;
    float airQuality;
    float lightIntensity;
    bool rainSensor;
    char mqttBroker[100];
    char ownIp[100];
    char sqlServer[100];

    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
    static void handleReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len, ESPNowActuatorHandler *instance);

    static ESPNowActuatorHandler *instance;
    void addPeer();
    void sendMode();
    void parseReceivedData(const char *data);
};

#endif
