#include "MQTTManager.h"
#include <WiFi.h>
#include <PubSubClient.h>

MQTTManager *MQTTManager::instance = nullptr;

MQTTManager::MQTTManager(WiFiManager *wifiMgr, SensorManager *sensorMgr)
    : client(wifiMgr->getWiFiClient()), sensorManager(sensorMgr), wifiManager(wifiMgr)
{
    instance = this;
}

void MQTTManager::setup(const char *user, const char *password, const char *clientID)
{
    mqttUser = user;
    mqttPassword = password;
    mqttClientID = clientID;
    resolveDomainToIP();
    client.setCallback([this](char *topic, byte *payload, unsigned int length)
                       { this->callback(topic, payload, length); });
}

void MQTTManager::setup(const char *clientID)
{
    mqttClientID = clientID;
    resolveDomainToIP();
    client.setCallback([this](char *topic, byte *payload, unsigned int length)
                       { this->callback(topic, payload, length); });
}

void MQTTManager::setup(const char *clientID, bool useCredentials)
{
    if (useCredentials)
    {
        mqttUser = MQTT_USER;
        mqttPassword = MQTT_PASSWORD;
    }
    mqttClientID = clientID;
    resolveDomainToIP();
    client.setCallback([this](char *topic, byte *payload, unsigned int length)
                       { this->callback(topic, payload, length); });
}

void MQTTManager::resolveDomainToIP()
{
    IPAddress ip;
    const char *domain = MQTT_SERVER_URL + 7;

    if (WiFi.hostByName(domain, ip))
    {
        Serial.print("Resolved IP for ");
        Serial.print(domain);
        Serial.print(": ");
        Serial.println(ip.toString());
        client.setServer(ip, MQTT_PORT);
    }
    else
    {
        Serial.println("Failed to resolve domain to IP");
    }
}

void MQTTManager::reconnect()
{
    Serial.print("Attempting MQTT connection...");
    bool connected;
    if (mqttUser && mqttPassword)
    {
        connected = client.connect(mqttClientID, mqttUser, mqttPassword);
    }
    else
    {
        connected = client.connect(mqttClientID);
    }

    if (connected)
    {
        Serial.println("MQTT connected");
    }
    else
    {
        Serial.print("MQTT connection failed, rc=");
        Serial.print(client.state());
    }
}

void MQTTManager::publishSensorData()
{
    if (client.connected())
    {
        client.publish((wifiManager->getMacAddress() + "/Temperature").c_str(), String(sensorManager->getTemperature()).c_str());
        client.publish((wifiManager->getMacAddress() + "/Humidity").c_str(), String(sensorManager->getHumidity()).c_str());
        client.publish((wifiManager->getMacAddress() + "/Pressure").c_str(), String(sensorManager->getPressure()).c_str());
        client.publish((wifiManager->getMacAddress() + "/LightIntensity").c_str(), String(sensorManager->getLightIntensity()).c_str());
        client.publish((wifiManager->getMacAddress() + "/AirQuality").c_str(), String(sensorManager->getAirQuality()).c_str());
        client.publish((wifiManager->getMacAddress() + "/RainSensor").c_str(), sensorManager->isRaining() ? "true" : "false");
        client.publish((wifiManager->getMacAddress() + "/IP").c_str(), WiFi.localIP().toString().c_str());
        client.publish((wifiManager->getMacAddress() + "/MacAddress").c_str(), wifiManager->getMacAddress().c_str());
    }
    else
    {
        Serial.println("MQTT connection lost! Trying to reconnect...");
        reconnect();
    }
}

void MQTTManager::subscribe(const char *topic)
{
    if (client.connected())
    {
        client.subscribe(topic);
    }
    else
    {
        Serial.println("MQTT connection lost");
    }
}

void MQTTManager::loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}

void MQTTManager::callback(char *topic, byte *payload, unsigned int length)
{
    payload[length] = '\0';
    String payloadStr = String((char *)payload);

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println(payloadStr);

    instance->handleIncomingMessage(topic, payloadStr.c_str());
}

void MQTTManager::handleIncomingMessage(const char *topic, const char *payload)
{
    if (strcmp(topic, TOPIC_MODE) == 0)
    {
        Serial.print("Received mode: ");
        Serial.println(payload);

        Environment::setStationMode(payload);
    }
}

bool MQTTManager::isConnected()
{
    return client.connected();
}