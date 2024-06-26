#include "DisplayManager.h"

void DisplayManager::initDisplay(int rotation, uint16_t bgColor)
{
    tft.init();
    tft.setRotation(rotation);
    tft.fillScreen(bgColor);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setCursor(0, 0, 1);
    tft.println("Display initialisiert!");
}

void DisplayManager::showWifiError()
{
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setCursor(5, 5, 1);
    tft.setTextSize(3);
    tft.println("ClimateGuard-360");
    tft.fillRect(0, 30, 240, 3, TFT_WHITE);

    tft.fillTriangle(30, 220, 210, 220, 120, 64, TFT_RED);
    tft.fillTriangle(40, 215, 200, 215, 120, 74, TFT_WHITE);
    tft.fillRoundRect(110, 110, 20, 70, 10, TFT_BLACK);
    tft.fillCircle(120, 200, 10, TFT_BLACK);

    tft.setCursor(32, 230, 1);
    tft.setTextSize(3);
    tft.setTextColor(TFT_RED);
    tft.println("WiFi-Error");

    tft.setCursor(90, 260, 1);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.println("verbinde...");
}

void DisplayManager::showMqttError()
{
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setCursor(5, 5, 1);
    tft.setTextSize(3);
    tft.println("ClimateGuard-360");
    tft.fillRect(0, 30, 240, 3, TFT_WHITE);

    tft.fillTriangle(30, 220, 210, 220, 120, 64, TFT_RED);
    tft.fillTriangle(40, 215, 200, 215, 120, 74, TFT_WHITE);
    tft.fillRoundRect(110, 110, 20, 70, 10, TFT_BLACK);
    tft.fillCircle(120, 200, 10, TFT_BLACK);

    tft.setCursor(32, 230, 1);
    tft.setTextSize(3);
    tft.setTextColor(TFT_RED);
    tft.println("MQTT-Error");

    tft.setCursor(90, 260, 1);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.println("verbinde...");
}

void DisplayManager::showStartScreen(String ownIP, String mqttServer, String macAddress)
{
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setCursor(5, 5, 1);
    tft.setTextSize(3);
    tft.println("ClimateGuard-360");
    tft.fillRect(0, 30, 240, 3, TFT_WHITE);

    tft.setCursor(60, 50, 1);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("IP-Adresse");
    tft.setCursor(48, 70, 1);
    tft.setTextColor(TFT_GREEN);
    tft.println(ownIP);
    tft.drawRoundRect(10, 45, 220, 45, 5, TFT_BLUE);

    tft.setTextColor(TFT_WHITE);
    tft.setCursor(55, 100, 1);
    tft.println("MAC-Adresse");
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(19, 120, 1);
    tft.println(macAddress);
    tft.drawRoundRect(10, 95, 220, 45, 5, TFT_BLUE);

    tft.setTextColor(TFT_WHITE);
    tft.setCursor(55, 150, 1);
    tft.println("MQTT-Server");
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(50, 170, 1);
    tft.println(mqttServer);
    tft.drawRoundRect(10, 145, 220, 45, 5, TFT_BLUE);

    tft.setTextColor(TFT_RED);
    tft.setTextSize(1);
    tft.setCursor(5, 310, 1);
    tft.print("2024 - Made by: Magnus Jung");
}

void DisplayManager::showSensorData(float temperature, float humidity, float pressure, bool isWifiConnected, NTPClient &timeClient, bool showBar, bool showFahrenheit)
{
    float temperatureF = temperature * 1.8 + 32;
    float pressureBar = pressure / 1000;

    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setCursor(5, 5, 1);
    tft.setTextSize(3);
    tft.println("ClimateGuard-360");
    tft.fillRect(0, 30, 240, 3, TFT_WHITE);
    if (isWifiConnected)
    {
        tft.fillRoundRect(45, 45, 145, 50, 5, TFT_WHITE);
        tft.fillRoundRect(48, 48, 139, 44, 5, TFT_SKYBLUE);

        tft.setCursor(65, 45, 2);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);

        tft.println(timeClient.getFormattedTime());

        tft.setTextSize(1);

        tft.setCursor(68, 75, 2);

        tft.print(daysOfTheWeekShort[timeClient.getDay()]);
        tft.print(", ");
        tft.println(timeClient.getFormattedDate());
    }
    else
    {
        tft.setCursor(65, 50, 2);
        tft.setTextColor(TFT_RED);
        tft.println("Verbinde WiFi...");
    }

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setCursor(40, 110, 2);

    if (!isnan(humidity))
    {
        tft.print("humidity: ");
        tft.print(humidity);
        tft.println("%");
    }
    else
    {
        tft.println("humidity: DHT pruefen");
    }

    tft.drawRoundRect(35, 105, 160, 25, 5, TFT_SILVER);

    tft.setCursor(0, 130, 2);
    tft.print("0%");
    tft.setCursor(110, 130, 2);
    tft.print("50%");
    tft.setCursor(205, 130, 2);
    tft.print("100%");
    tft.fillRect(0, 145, map(humidity, 0, 100, 0, 240), 10, TFT_BLUE);

    tft.setCursor(45, 190, 2);

    if (showFahrenheit)
    {
        if (!isnan(temperature))
        {
            tft.print("temperature: ");
            tft.print(temperatureF);
            tft.println(" *F");
        }
        else
        {
            tft.print("temperature: BMP & DHT pruefen");
        }
        tft.drawRoundRect(40, 185, 150, 25, 5, TFT_SILVER);

        tft.setCursor(0, 210, 2);
        tft.print("-4F");
        tft.setCursor(110, 210, 2);
        tft.print("68F");
        tft.setCursor(205, 210, 2);
        tft.print("140F");
        tft.fillRect(0, 225, map(temperatureF, -4, 140, 0, 240), 10, TFT_RED);
    }
    else
    {
        if (!isnan(temperature))
        {
            tft.print("temperature: ");
            tft.print(temperature);
            tft.println(" *C");
        }
        else
        {
            tft.print("temperature: BMP & DHT pruefen");
        }
        tft.drawRoundRect(40, 185, 150, 25, 5, TFT_SILVER);

        tft.setCursor(0, 210, 2);
        tft.print("-20C");
        tft.setCursor(110, 210, 2);
        tft.print("20C");
        tft.setCursor(210, 210, 2);
        tft.print("60C");
        tft.fillRect(0, 225, map(temperature, -20, 60, 0, 240), 10, TFT_RED);
    }

    if (showBar)
    {
        tft.setCursor(45, 270);
        if (!isnan(pressureBar))
        {
            tft.print("pressure: ");
            tft.print(pressureBar);
            tft.println(" Bar");
        }
        else
        {
            tft.println("pressure: BMP pruefen!");
        }
        tft.drawRoundRect(40, 265, 150, 25, 5, TFT_SILVER);

        // Totes Meer (-425m) = 1.07 Bar
        // NN = 1.013 Bar
        // 1.2 daher eher unwahrscheinlich
        tft.setCursor(0, 290, 2);
        tft.print("0");
        tft.setCursor(110, 290, 2);
        tft.print("0.6");
        tft.setCursor(215, 290, 2);
        tft.print("1.2");
        tft.fillRect(0, 310, map(pressure, 0, 1200, 0, 240), 10, TFT_CYAN);
    }
    else
    {

        tft.setCursor(45, 270);
        if (!isnan(pressure))
        {
            tft.print("pressure: ");
            tft.print(pressure);
            tft.println(" hPa");
        }
        else
        {
            tft.println("pressure: BMP pruefen!");
        }
        tft.drawRoundRect(40, 265, 150, 25, 5, TFT_SILVER);

        // Totes Meer (-425m) = 1070 hPa
        // NN = 1013 hPa
        // 1200 daher eher unwahrscheinlich
        tft.setCursor(0, 290, 2);
        tft.print("0");
        tft.setCursor(110, 290, 2);
        tft.print("600");
        tft.setCursor(205, 290, 2);
        tft.print("1200");
        tft.fillRect(0, 310, map(pressure, 0, 1200, 0, 240), 10, TFT_CYAN);
    }
}
