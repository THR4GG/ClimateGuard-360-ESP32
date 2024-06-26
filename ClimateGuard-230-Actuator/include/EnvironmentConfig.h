#include <stdint.h>

#ifndef ENVIRONMENT_CONFIG_H
#define ENVIRONMENT_CONFIG_H

// Sensor Pins
#define DHT11_PIN 4
#define BMP280_SDA_PIN 21
#define BMP280_SCL_PIN 22
#define BH1750_SDA_PIN 21
#define BH1750_SCL_PIN 22
#define MQ135_PIN 36
#define RAIN_SENSOR_PIN 39

// LED Pins
#define LED_PIN 32
#define RGB_RED_PIN 33
#define RGB_GREEN_PIN 22
#define RGB_BLUE_PIN 21

// Button Pins
#define MODUS_PIN 25
#define LEISER_PIN 26
#define PLAY_PAUSE_PIN 27
#define LAUTER_PIN 14

// Actuator Pins
#define SERVO_PIN 13
#define FAN_PIN 12
#define BUSY_PIN 5

// DFPlayer Pins

#define RX_PIN 16 // RX
#define TX_PIN 17 // TX

// Server URLs
#define PHP_SERVER_URL "http://climateguard-360.local:8080/insert_data.php"

// WiFi Settings
extern const char *ssids[];
extern const char *passwords[];
extern const char *proxies[];

extern const int networkCount;

// MQTT Settings
#define STATION_NAME "ClimateGuard 360"
#define STATION_DESCRIPTION "A weather station for your home"
#define MQTT_SERVER_URL "mqtt://climateguard-360.local"
#define MQTT_PORT 1883

// ESP-NOW Identification Keys
#define KEY_TEMPERATURE 1
#define KEY_PRESSURE 2
#define KEY_HUMIDITY 3
#define KEY_AIRQUALITY 4
#define KEY_LIGHTINTENSITY 5
#define KEY_RAINSENSOR 6
#define KEY_MODE 10

// ESP-NOW Configuration Keys
#define KEY_MQTT_BROKER 7
#define KEY_OWN_IP 8
#define KEY_SQL_SERVER 9

// ESP-NOW Control Keys
#define KEY_ACK 99

// ESP-NOW Peer Addresses
const uint8_t sensorAddress[] = {0xA8, 0x42, 0xE3, 0xA9, 0x55, 0x38};   // Sensor MacAddress A8:42:E3:A9:55:38
const uint8_t actuatorAddress[] = {0x30, 0xC6, 0xF7, 0xF8, 0x90, 0x08}; // Actuator MacAddress 30:C6:F7:F8:90:08

// MQTT Topics
#define TOPIC_TEMPERATURE "A8:42:E3:A9:55:38/Temperature"
#define TOPIC_HUMIDITY "A8:42:E3:A9:55:38/Humidity"
#define TOPIC_PRESSURE "A8:42:E3:A9:55:38/Pressure"
#define TOPIC_AIRQUALITY "A8:42:E3:A9:55:38/AirQuality"
#define TOPIC_LIGHTINTENSITY "A8:42:E3:A9:55:38/LightIntensity"
#define TOPIC_RAINSENSOR "A8:42:E3:A9:55:38/RainSensor"
#define TOPIC_MODE "A8:42:E3:A9:55:38/Mode"
#define TOPIC_MODE_CHANGE "A8:42:E3:A9:55:38/ModeChange"
#define TOPIC_IP "A8:42:E3:A9:55:38/IP"
#define TOPIC_MAC "A8:42:E3:A9:55:38/MacAddress"

class Environment
{
public:
    static void setStationMode(const char *mode);
    static const char *getStationMode();

private:
    static const char *stationMode;
};
#endif // ENVIRONMENT_CONFIG_H
