#include "SQLManager.h"
#include "EnvironmentConfig.h"

SQLManager::SQLManager()
{
    macAddress = WiFi.macAddress();
}

void SQLManager::sendSensorData(float temperature, float humidity, float pressure, float lightIntensity, float airQuality, bool rain)
{
    String payload = "station_id=" + macAddress;

    if (!isnan(temperature))
        payload += "&temperature=" + String(temperature);
    if (!isnan(humidity))
        payload += "&humidity=" + String(humidity);
    if (!isnan(pressure))
        payload += "&pressure=" + String(pressure);
    if (!isnan(lightIntensity))
        payload += "&light_intensity=" + String(lightIntensity);
    if (airQuality != -1)
        payload += "&air_quality=" + String(airQuality);
    if (rain != -1)
        payload += "&rain=" + String(rain);

    sendPostRequest(payload);
}

void SQLManager::sendStationData()
{
    String payload = "station_id=" + macAddress;
    payload += "&station_name=" + String(STATION_NAME);
    payload += "&station_description=" + String(STATION_DESCRIPTION);
    payload += "&station_mode=" + String(Environment::getStationMode());
    sendPostRequest(payload);
}

void SQLManager::sendPostRequest(const String &payload)
{
    HTTPClient http;
    http.begin(PHP_SERVER_URL); // Root-URL des Servers verwenden
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}
