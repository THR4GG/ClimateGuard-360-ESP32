#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <BH1750.h>
#include <MQ135.h>
#include "EnvironmentConfig.h"

class SensorManager
{
public:
    SensorManager();
    void setupSensors();
    void readSensors();

    // Getter für die Sensorwerte
    float getTemperature();
    float getHumidity();
    float getPressure();
    float getPressureBar();
    float getTemperatureF();
    bool isDHTCheck();
    bool isBMPCheck();
    float getAirQuality();
    float getLightIntensity();
    bool isRaining();

private:
    DHT dht;
    Adafruit_BMP280 bmp;
    MQ135 mq135;
    BH1750 lightMeter;

    // Sensorwerte
    float temperature;
    float humidity;
    float pressure;
    float airQuality;
    float lightIntensity;
    bool dhtCheck;
    bool bmpCheck;
    bool rainDetected;
};

#endif // SENSOR_MANAGER_H
