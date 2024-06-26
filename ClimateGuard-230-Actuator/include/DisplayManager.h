#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Farbdefinitionen
#define HEADER_COLOR 0x4a49
#define FOOTER_COLOR 0x4a49
#define GRID_COLOR TFT_LIGHTGREY
#define INNER_CIRCLE_COLOR TFT_BLACK

#define AREA1_COLOR TFT_BLACK
#define AREA2_COLOR TFT_BLACK
#define AREA3_COLOR TFT_BLACK
#define AREA4_COLOR TFT_BLACK

// Meter Farbschemata
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5

class DisplayManager
{
public:
    DisplayManager();

    void init();
    void drawHeader();
    void drawFooter();
    void drawGrid();
    void updateMeters(float temp, float humidity, float pressure, float lightIntensity);
    void screenOff();
    void showWifiError();
    void showMqttError();
    void showError(String errorType);
    void showStartScreen(String ownIP, String mqttServer, String macAddress);
    void showBootScreen();

private:
    int16_t getTextWidth(String text, int textSize);
    int ringMeter(float value, float vmin, float vmax, int x, int y, int r, const char *units, byte scheme);
    unsigned int rainbow(byte value);
    float sineWave(int phase);

    TFT_eSPI tft;

    int areaWidth;
    int areaHeight;
    int radius;
    int textHeight;

    int xpos1, ypos1;
    int xpos2, ypos2;
    int xpos3, ypos3;
    int xpos4, ypos4;
};

#endif
