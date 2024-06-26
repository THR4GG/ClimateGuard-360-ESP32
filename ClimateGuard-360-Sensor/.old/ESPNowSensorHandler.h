#ifndef ESPNOWSENSORHANDLER_H
#define ESPNOWSENSORHANDLER_H

#include <esp_now.h>
#include <WiFi.h>
#include "EnvironmentConfig.h"
#include "WiFiManager.h"

class ESPNowSensorHandler
{
public:
    ESPNowSensorHandler(WiFiManager *wifiManager);
    void setup();
    void loop();

    void sendSensorData(const String &data);
    bool isModeReceived() const;
    bool isHandshakeComplete() const;

private:
    WiFiManager *wifiManager;
    bool modeReceived;
    bool handshakeComplete;
    unsigned long lastCheckTime; // Zeitpunkt des letzten Checks

    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
    static void handleReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len, ESPNowSensorHandler *instance);

    static ESPNowSensorHandler *instance;
    void addPeer();
    void performHandshake();
};

#endif
