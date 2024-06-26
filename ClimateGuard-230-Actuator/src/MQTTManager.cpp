#include "MQTTManager.h"

MQTTManager *MQTTManager::instance = nullptr;

MQTTManager::MQTTManager(WiFiClient &wifiClient, DisplayManager &displayManager) : client(wifiClient), temperature(0.0), humidity(0.0), pressure(0.0), airQuality(0.0), lightIntensity(0.0), rainSensor(false)
{
    client.setCallback(mqttCallback);
    instance = this;
    this->displayManager = &displayManager;
}

void MQTTManager::setup(const char *clientID)
{
    mqttClientID = clientID;
    resolveDomainToIP();
}

void MQTTManager::setup(const char *user, const char *password, const char *clientID)
{
    mqttUser = user;
    mqttPassword = password;
    mqttClientID = clientID;
    resolveDomainToIP();
}

void MQTTManager::resolveDomainToIP()
{
    IPAddress ip;
    const char *domain = MQTT_SERVER_URL + 7; // Skipping "mqtt://" in the URL

    if (WiFi.hostByName(domain, ip))
    {
        Serial.print("Resolved IP for ");
        Serial.print(domain);
        Serial.print(": ");
        Serial.println(ip.toString());
        client.setServer(ip, MQTT_PORT); // Set server with resolved IP
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
        Serial.println("connected");
        subscribeToTopics();
    }
    else
    {
        Serial.print("Failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
    }
}

void MQTTManager::subscribeToTopics()
{
    client.subscribe(TOPIC_TEMPERATURE);
    client.subscribe(TOPIC_HUMIDITY);
    client.subscribe(TOPIC_PRESSURE);
    client.subscribe(TOPIC_AIRQUALITY);
    client.subscribe(TOPIC_LIGHTINTENSITY);
    client.subscribe(TOPIC_RAINSENSOR);
    client.subscribe(TOPIC_MODE_CHANGE);
    client.subscribe(TOPIC_IP);
    client.subscribe(TOPIC_MAC);
    Serial.println("Subscribed to topics");
}

void MQTTManager::publishMode(const char *mode)
{
    client.publish(TOPIC_MODE, mode);
}

void MQTTManager::loop()
{
    if (!client.connected())
    {
        displayManager->showMqttError();
        reconnect();
        displayManager->screenOff();
    }
    client.loop();
}

void MQTTManager::mqttCallback(char *topic, byte *payload, unsigned int length)
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
    // Directly replace old data with new data as soon as it's received
    if (strcmp(topic, TOPIC_TEMPERATURE) == 0)
    {
        temperature = atof(payload);
        receivedTemperature = true;
    }
    else if (strcmp(topic, TOPIC_HUMIDITY) == 0)
    {
        humidity = atof(payload);
        receivedHumidity = true;
    }
    else if (strcmp(topic, TOPIC_PRESSURE) == 0)
    {
        pressure = atof(payload);
        receivedPressure = true;
    }
    else if (strcmp(topic, TOPIC_AIRQUALITY) == 0)
    {
        airQuality = atof(payload);
        receivedAirQuality = true;
    }
    else if (strcmp(topic, TOPIC_LIGHTINTENSITY) == 0)
    {
        lightIntensity = atof(payload);
        receivedLightIntensity = true;
    }
    else if (strcmp(topic, TOPIC_RAINSENSOR) == 0)
    {
        rainSensor = strcmp(payload, "true") == 0;
        receivedRainSensor = true;
    }
    else if (strcmp(topic, TOPIC_MODE_CHANGE) == 0)
    {
        Serial.print("Received mode change payload: ");
        // Handle mode change immediately
        if (strcmp(payload, "INDOOR") == 0)
        {
            Environment::setStationMode("INDOOR");
        }
        else if (strcmp(payload, "OUTDOOR") == 0)
        {
            Serial.println("Setting station mode to OUTDOOR");
            Environment::setStationMode("OUTDOOR");
        }
        else if (strcmp(payload, "HYBRID") == 0)
        {
            Environment::setStationMode("HYBRID");
        }
        else if (strcmp(payload, "INVALID") == 0)
        {
            // Do nothing
        }
        else
        {
            client.publish(TOPIC_MODE_CHANGE, "INVALID");
        }

        publishMode(Environment::getStationMode());
    }
    else if (strcmp(topic, TOPIC_IP) == 0)
    {
        Serial.print("Received IP payload: ");
        Serial.println(payload);

        if (ip.fromString(payload))
        {
            Serial.print("Valid IP address received: ");
            Serial.println(ip.toString());
            receivedIP = true;
        }
        else
        {
            Serial.println("Invalid IP address format");
        }
    }
    else if (strcmp(topic, TOPIC_MAC) == 0)
    {
        mac = payload;
        receivedMAC = true;
    }
}

bool MQTTManager::waitForAllTopics()
{
    unsigned long startTime = millis();
    unsigned long timeout = 100000;
    while (!(receivedTemperature && receivedHumidity && receivedPressure && receivedAirQuality && receivedLightIntensity && receivedRainSensor))
    {
        loop();
        if (millis() - startTime > timeout)
        {
            Serial.println("Timeout beim Warten auf alle Topics");
            return false;
        }
        delay(100);
    }
    Serial.println("Alle Topics wurden empfangen");
    displayManager->screenOff();
    return true;
}

bool MQTTManager::waitForInitialData()
{
    unsigned long startTime = millis();
    unsigned long timeout = 100000;
    while (!(receivedIP && receivedMAC))
    {
        loop();
        if (millis() - startTime > timeout)
        {
            Serial.println("Timeout beim Warten auf initiale Daten");
            return false;
        }
        delay(100);
    }
    Serial.println("Initiale Daten wurden empfangen");
    return true;
}

bool MQTTManager::isConnected()
{
    return client.connected();
}

float MQTTManager::getTemperature() const { return temperature; }
float MQTTManager::getHumidity() const { return humidity; }
float MQTTManager::getPressure() const { return pressure; }
float MQTTManager::getAirQuality() const { return airQuality; }
float MQTTManager::getLightIntensity() const { return lightIntensity; }
bool MQTTManager::getRainSensor() const { return rainSensor; }
IPAddress MQTTManager::getIP() const { return ip; }
const char *MQTTManager::getMAC() const { return mac; }
