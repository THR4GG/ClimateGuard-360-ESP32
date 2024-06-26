#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "EnvironmentConfig.h"

class WiFiManager
{
public:
    WiFiManager();
    void setupWiFi();
    WiFiClient &getWiFiClient();
    void sendHTTPData(const char *server, const char *endpoint, const String &data);
    void setupNTP(const char *ntpServer, long utcOffsetInSeconds);
    void refreshNTP();
    String getFormattedTime();
    String getFormattedDate();
    int getDay();
    int getMonth();
    int getYear();
    int getHour();
    int getMinute();
    int getSecond();
    String getMacAddress();
    String getLocalIP();

private:
    bool connectToWiFi(const char *ssid, const char *password);
    void applyProxySettings(const char *proxy);

    WiFiClient wifiClient;
    NTPClient *timeClient;
    WiFiUDP ntpUDP;
};
#endif // WIFI_MANAGER_H
