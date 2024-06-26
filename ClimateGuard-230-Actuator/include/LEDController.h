#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>

enum Mode
{
    INDOOR,
    OUTDOOR,
    HYBRID
};

class LEDController
{
public:
    LEDController();
    void setupPins();
    static void setMode(Mode mode);
    static void switchMode();
    static Mode getMode();
    void updateLED();
    void updateRGBLed(float temperature);

private:
    static Mode currentMode;
    unsigned long lastBlinkTime;
    bool ledState;
};

#endif
