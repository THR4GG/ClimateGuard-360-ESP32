#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H

#include <HTTPClient.h>
#include <WiFi.h>

class SQLManager
{
public:
    SQLManager();

    // Funktionen zum Senden von Daten
    void sendSensorData(float temperature = NAN, float humidity = NAN, float pressure = NAN, float lightIntensity = NAN, float airQuality = -1, bool rain = false);
    void sendStationData();

private:
    void sendPostRequest(const String &payload);
    String serverUrl;
    String macAddress;
};

#endif
