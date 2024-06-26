#include <Arduino.h>
#include <SensorManager.h>
#include <WiFiManager.h>
#include <MQTTManager.h>
#include <SQLManager.h>

// Objekte
SensorManager sensorManager;
WiFiManager wifiManager;
MQTTManager mqttManager(&wifiManager, &sensorManager);
SQLManager sqlManager;

// Variablen
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 30 * 60 * 1000;

unsigned long ESPNowlastSendTime = 0;
const unsigned long ESPNowsendInterval = 5000;


// Methoden
void SQL();
void SerialCheck();
// void ESPNowSend();

void setup()
{
  Serial.begin(9600);
  delay(2000);

  sensorManager.setupSensors();
  wifiManager.setupWiFi();
  mqttManager.setup("esp32");
  mqttManager.subscribe(TOPIC_MODE);

  Serial.println("Setup abgeschlossen!");
  delay(2000);
}

void loop()
{
  Serial.println("Loop gestartet!");
  sensorManager.readSensors();
  SerialCheck();

  mqttManager.loop();
  mqttManager.publishSensorData();
  if (!mqttManager.isConnected())
  {
    SQL();
  }

  Serial.println(Environment::getStationMode());

  delay(5000);
}

void SQL()
{
  if (millis() - lastSendTime >= sendInterval)
  {
    lastSendTime = millis();
    // sqlManager.sendSensorData(sensorManager.getTemperature(), sensorManager.getHumidity(), sensorManager.getPressure(), sensorManager.getLightIntensity(), sensorManager.getAirQuality(), sensorManager.isRaining());

    Serial.println("Daten an SQL-Datenbank gesendet!");
  }
}

void SerialCheck()
{
  Serial.print("Temperatur: ");
  Serial.print(sensorManager.getTemperature());
  Serial.println(" °C");
  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(sensorManager.getHumidity());
  Serial.println(" %");
  Serial.print("Luftdruck: ");
  Serial.print(sensorManager.getPressure());
  Serial.println(" hPa");
  Serial.print("Luftqualität: ");
  Serial.print(sensorManager.getAirQuality());
  Serial.println(" PPM");
  Serial.print("Lichtintensität: ");
  Serial.print(sensorManager.getLightIntensity());
  Serial.println(" lx");
  Serial.print("Regen: ");
  Serial.println(sensorManager.isRaining() ? "Ja" : "Nein");
  Serial.println();
}