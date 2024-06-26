#include "WiFiManager.h"

WiFiManager::WiFiManager()
    : timeClient(nullptr) {}

bool WiFiManager::connectToWiFi(const char *ssid, const char *password)
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC address: ");
        Serial.println(WiFi.macAddress());
        return true;
    }
    else
    {
        Serial.println("");
        Serial.println("Failed to connect");
        return false;
    }
}

void WiFiManager::applyProxySettings(const char *proxy)
{
    if (proxy)
    {
        String proxyStr = String(proxy);
        int colonIndex = proxyStr.indexOf(':');
        if (colonIndex > 0)
        {
            String proxyIP = proxyStr.substring(0, colonIndex);
            int proxyPort = proxyStr.substring(colonIndex + 1).toInt();

            // Verbindungsaufbau zum Proxy
            if (wifiClient.connect(proxyIP.c_str(), proxyPort))
            {
                Serial.println("Proxy connected");
            }
            else
            {
                Serial.println("Failed to connect to Proxy");
            }
        }
    }
}

void WiFiManager::setupWiFi()
{
    for (int i = 0; i < networkCount; i++)
    {
        if (connectToWiFi(ssids[i], passwords[i]))
        {
            applyProxySettings(proxies[i]);
            setupNTP("de.pool.ntp.org", 3600);
            break;
        }
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi-Verbindung fehlgeschlagen");
    }
    else
    {
        Serial.println("WiFi verbunden");
    }
}

WiFiClient &WiFiManager::getWiFiClient()
{
    return wifiClient;
}

void WiFiManager::sendHTTPData(const char *server, const char *endpoint, const String &data)
{
    WiFiClient client;

    if (wifiClient.connected())
    {
        client = wifiClient;
    }
    else
    {
        if (!client.connect(server, 80))
        {
            Serial.println("Connection to server failed");
            return;
        }
    }

    client.print(String("POST ") + endpoint + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: " + data.length() + "\r\n\r\n" +
                 data);

    while (client.connected() && !client.available())
        ;
    String response = client.readStringUntil('\n');
    Serial.println(response);

    client.stop();
}

void WiFiManager::setupNTP(const char *ntpServer, long utcOffsetInSeconds)
{
    if (timeClient != nullptr)
    {
        delete timeClient;
    }
    timeClient = new NTPClient(ntpUDP, ntpServer, utcOffsetInSeconds);
    timeClient->begin();
    timeClient->update();
}

void WiFiManager::refreshNTP()
{
    if (timeClient)
    {
        timeClient->update();
    }
}

String WiFiManager::getFormattedTime()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getFormattedTime();
    }
    return "";
}

String WiFiManager::getFormattedDate()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getFormattedDate();
    }
    return "";
}

int WiFiManager::getDay()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getDay();
    }
    return -1;
}

int WiFiManager::getMonth()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getMonth();
    }
    return -1;
}

int WiFiManager::getYear()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getYear();
    }
    return -1;
}

int WiFiManager::getHour()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getHours();
    }
    return -1;
}

int WiFiManager::getMinute()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getMinutes();
    }
    return -1;
}

int WiFiManager::getSecond()
{
    if (timeClient)
    {
        timeClient->update();
        return timeClient->getSeconds();
    }
    return -1;
}

String WiFiManager::getMacAddress()
{
    return WiFi.macAddress();
}

String WiFiManager::getLocalIP()
{
    return WiFi.localIP().toString();
}