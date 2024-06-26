#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <TFT_eSPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class DisplayManager
{
public:
    void initDisplay(int rotation, uint16_t bgColor);
    void showWifiError();
    void showMqttError();
    void showStartScreen(String ownIP, String mqttBroker, String macAddress);
    void showSensorData(float temperature, float humidity, float pressure, bool isWifiConnected, NTPClient &timeClient, bool isFahrenheit = false, bool isBar = false);

private:
    TFT_eSPI tft = TFT_eSPI();
    char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
    char daysOfTheWeekShort[7][12] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
};

#endif
