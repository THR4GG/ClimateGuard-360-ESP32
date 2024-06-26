#include "SensorManager.h"

SensorManager::SensorManager()
    : dht(DHT11_PIN, DHT11), bmp(), mq135(MQ135_PIN), lightMeter(),
      temperature(0), humidity(0), pressure(0), airQuality(0), lightIntensity(0),
      dhtCheck(false), bmpCheck(false), rainDetected(false) {}

void SensorManager::setupSensors()
{
    // DHT11 Setup
    dht.begin();

    // BMP280 Setup
    bmpCheck = bmp.begin(0x76);
    if (!bmpCheck)
    {
        Serial.println("BMP280 nicht gefunden, überprüfe die Verkabelung!");
    }
    else
    {
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                        Adafruit_BMP280::SAMPLING_X2,
                        Adafruit_BMP280::SAMPLING_X16,
                        Adafruit_BMP280::FILTER_X16,
                        Adafruit_BMP280::STANDBY_MS_500);
    }

    // BH1750 Setup
    Wire.begin(BH1750_SDA_PIN, BH1750_SCL_PIN);
    if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
    {
        Serial.println("BH1750 nicht gefunden, überprüfe die Verkabelung!");
    }

    // Rain Sensor Setup
    pinMode(RAIN_SENSOR_PIN, INPUT);
}

void SensorManager::readSensors()
{
    Serial.println("Reading sensors...");
    float newTemperature = dht.readTemperature();
    float newHumidity = dht.readHumidity();
    if (!isnan(newTemperature) && !isnan(newHumidity))
    {
        temperature = newTemperature;
        humidity = newHumidity;
        dhtCheck = true;
        Serial.println("DHT11 reading successful.");
    }
    else
    {
        dhtCheck = false;
        Serial.println("DHT11 reading failed. Using BMP280 if available.");
    }

    if (bmpCheck)
    {
        if (!dhtCheck)
        {
            temperature = bmp.readTemperature();
            Serial.println("Using BMP280 temperature.");
        }
        pressure = bmp.readPressure() / 100.0F;
        Serial.println("BMP280 reading successful.");
    }

    lightIntensity = lightMeter.readLightLevel();
    Serial.println("BH1750 reading successful.");

    airQuality = mq135.getPPM();
    Serial.println("MQ135 reading successful.");

    rainDetected = digitalRead(RAIN_SENSOR_PIN) == LOW;
    Serial.println("Rain sensor reading successful.");
}

// Getter-Methoden für Sensorwerte
float SensorManager::getTemperature()
{
    return temperature;
}

float SensorManager::getHumidity()
{
    return humidity;
}

float SensorManager::getPressure()
{
    return pressure;
}

float SensorManager::getPressureBar()
{
    return pressure / 1000; // Convert to bar
}

float SensorManager::getTemperatureF()
{
    return temperature * 9 / 5 + 32;
}

bool SensorManager::isDHTCheck()
{
    return dhtCheck;
}

bool SensorManager::isBMPCheck()
{
    return bmpCheck;
}

float SensorManager::getAirQuality()
{
    return airQuality;
}

float SensorManager::getLightIntensity()
{
    return lightIntensity;
}

bool SensorManager::isRaining()
{
    return rainDetected;
}
