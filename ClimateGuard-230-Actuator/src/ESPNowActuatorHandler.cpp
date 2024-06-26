#include "ESPNowActuatorHandler.h"
#include "EnvironmentConfig.h"

ESPNowActuatorHandler *ESPNowActuatorHandler::instance = nullptr;

ESPNowActuatorHandler::ESPNowActuatorHandler()
    : temperature(0.0), pressure(0.0), humidity(0.0), airQuality(0.0), lightIntensity(0.0), rainSensor(false)
{
    ESPNowActuatorHandler::instance = this;
}

void ESPNowActuatorHandler::setup()
{
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Fehler bei der Initialisierung von ESP-NOW");
        return;
    }

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);

    addPeer();
}

void ESPNowActuatorHandler::loop()
{
    // Nichts Spezifisches im Loop
}

float ESPNowActuatorHandler::getTemperature() const { return temperature; }
float ESPNowActuatorHandler::getPressure() const { return pressure; }
float ESPNowActuatorHandler::getHumidity() const { return humidity; }
float ESPNowActuatorHandler::getAirQuality() const { return airQuality; }
float ESPNowActuatorHandler::getLightIntensity() const { return lightIntensity; }
bool ESPNowActuatorHandler::getRainSensor() const { return rainSensor; }
const char *ESPNowActuatorHandler::getMQTTBroker() const { return mqttBroker; }
const char *ESPNowActuatorHandler::getOwnIP() const { return ownIp; }
const char *ESPNowActuatorHandler::getSQLServer() const { return sqlServer; }

void ESPNowActuatorHandler::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("Sende-Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Erfolgreich" : "Fehlgeschlagen");
}

void ESPNowActuatorHandler::onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    instance->handleReceive(mac_addr, incomingData, len, instance);
}

void ESPNowActuatorHandler::handleReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len, ESPNowActuatorHandler *instance)
{
    if (memcmp(mac_addr, sensorAddress, 6) != 0) // Verwenden Sie sensorAddress aus EnvironmentConfig
    {
        Serial.println("Unbekannter Absender");
        return;
    }

    char receivedString[250];
    memcpy(receivedString, incomingData, len);
    receivedString[len] = '\0';

    Serial.print("Empfangene Daten: ");
    Serial.println(receivedString);

    instance->parseReceivedData(receivedString);
    instance->sendMode();
}

void ESPNowActuatorHandler::addPeer()
{
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, sensorAddress, 6); // Verwenden Sie sensorAddress aus EnvironmentConfig
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    esp_now_del_peer(sensorAddress);

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Fehler beim Hinzufügen des Peers");
    }
    else
    {
        Serial.println("Peer erfolgreich hinzugefügt oder bereits vorhanden");
    }
}

void ESPNowActuatorHandler::sendMode()
{

    const char *mode = Environment::getStationMode();
    char payload[100];
    snprintf(payload, sizeof(payload), "%s", mode);

    esp_err_t result = esp_now_send(sensorAddress, (uint8_t *)payload, sizeof(payload));
    if (result == ESP_OK)
    {
        Serial.println("Modus erfolgreich gesendet");
    }
    else
    {
        Serial.print("Fehler beim Senden des Modus: ");
        Serial.println(result);
    }
}

void ESPNowActuatorHandler::parseReceivedData(const char *data)
{
    String dataString = String(data);
    int separatorIndex = 0;

    temperature = dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)).toFloat();
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    pressure = dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)).toFloat();
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    humidity = dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)).toFloat();
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    airQuality = dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)).toFloat();
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    lightIntensity = dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)).toFloat();
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    rainSensor = dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)) == "1";
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)).toCharArray(mqttBroker, sizeof(mqttBroker));
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    dataString.substring(separatorIndex, dataString.indexOf('/', separatorIndex)).toCharArray(ownIp, sizeof(ownIp));
    separatorIndex = dataString.indexOf('/', separatorIndex) + 1;

    dataString.substring(separatorIndex).toCharArray(sqlServer, sizeof(sqlServer));

    Serial.println("Daten erfolgreich geparst:");
    Serial.print("Temperatur: ");
    Serial.println(temperature);
    Serial.print("Druck: ");
    Serial.println(pressure);
    Serial.print("Luftfeuchtigkeit: ");
    Serial.println(humidity);
    Serial.print("Luftqualität: ");
    Serial.println(airQuality);
    Serial.print("Lichtintensität: ");
    Serial.println(lightIntensity);
    Serial.print("Regen: ");
    Serial.println(rainSensor ? "Ja" : "Nein");
    Serial.print("MQTT-Broker: ");
    Serial.println(mqttBroker);
    Serial.print("Eigene IP: ");
    Serial.println(ownIp);
    Serial.print("SQL-Server: ");
    Serial.println(sqlServer);
}
