#include "ESPNowSensorHandler.h"

ESPNowSensorHandler *ESPNowSensorHandler::instance = nullptr;

ESPNowSensorHandler::ESPNowSensorHandler(WiFiManager *wifiManager)
    : wifiManager(wifiManager), modeReceived(false), handshakeComplete(false), lastCheckTime(0)
{
    ESPNowSensorHandler::instance = this;
}

void ESPNowSensorHandler::setup()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Fehler bei der Initialisierung von ESP-NOW");
        return;
    }

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);

    addPeer();

    performHandshake();
}

void ESPNowSensorHandler::loop()
{
    // Überprüfen, ob es Zeit für einen Verbindungscheck ist (alle 10 Minuten)
    if (millis() - lastCheckTime >= 10 * 60 * 1000)
    {
        lastCheckTime = millis();
        performHandshake();
    }
}

void ESPNowSensorHandler::sendSensorData(const String &data)
{
    if (modeReceived)
    {
        modeReceived = false;

        char payload[250];
        data.toCharArray(payload, sizeof(payload));

        esp_err_t result = esp_now_send(actuatorAddress, (uint8_t *)payload, strlen(payload));
        if (result == ESP_OK)
        {
            Serial.println("Daten erfolgreich gesendet");
        }
        else
        {
            Serial.print("Fehler beim Senden der Daten: ");
            Serial.println(result);
        }
    }
}

bool ESPNowSensorHandler::isModeReceived() const
{
    return modeReceived;
}

bool ESPNowSensorHandler::isHandshakeComplete() const
{
    return handshakeComplete;
}

void ESPNowSensorHandler::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("Sende-Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Erfolgreich" : "Fehlgeschlagen");
}

void ESPNowSensorHandler::onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    instance->handleReceive(mac_addr, incomingData, len, instance);
}

void ESPNowSensorHandler::handleReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len, ESPNowSensorHandler *instance)
{
    if (memcmp(mac_addr, actuatorAddress, 6) != 0)
    {
        Serial.println("Unbekannter Absender");
        return;
    }

    char receivedString[250];
    memcpy(receivedString, incomingData, len);
    receivedString[len] = '\0';

    Serial.print("Empfangene Nachricht: ");
    Serial.println(receivedString);

    // Überprüfen, ob die Nachricht eine Handshake-Bestätigung ist
    if (String(receivedString) == "HANDSHAKE_ACK")
    {
        instance->handshakeComplete = true;
        Serial.println("Handshake erfolgreich abgeschlossen");
    }
    else
    {
        instance->modeReceived = true;
    }
}

void ESPNowSensorHandler::addPeer()
{
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, actuatorAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    esp_now_del_peer(actuatorAddress);

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Fehler beim Hinzufügen des Peers");
    }
    else
    {
        Serial.println("Peer erfolgreich hinzugefügt oder bereits vorhanden");
    }
}

void ESPNowSensorHandler::performHandshake()
{
    char handshakeMessage[] = "HANDSHAKE";
    esp_err_t result = esp_now_send(actuatorAddress, (uint8_t *)handshakeMessage, strlen(handshakeMessage));
    if (result != ESP_OK)
    {
        Serial.print("Fehler beim Senden der Handshake-Nachricht: ");
        Serial.println(result);
    }

    unsigned long startTime = millis();
    while (!handshakeComplete && (millis() - startTime < 5000)) // Warte bis zu 5 Sekunden auf das Handshake
    {
        delay(100);
    }

    if (!handshakeComplete)
    {
        Serial.println("Handshake fehlgeschlagen");
    }
}
