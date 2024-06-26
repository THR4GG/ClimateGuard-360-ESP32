#include "LEDController.h"
#include "EnvironmentConfig.h"

Mode LEDController::currentMode = INDOOR;

LEDController::LEDController()
{
    lastBlinkTime = 0;
    ledState = LOW;
}

void LEDController::setupPins()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(RGB_RED_PIN, OUTPUT);
    pinMode(RGB_GREEN_PIN, OUTPUT);
    pinMode(RGB_BLUE_PIN, OUTPUT);
}

void LEDController::setMode(Mode mode)
{
    currentMode = mode;
}

void LEDController::switchMode()
{
    currentMode = static_cast<Mode>((currentMode + 1) % 3);
    Environment::setStationMode(currentMode == INDOOR ? "INDOOR" : (currentMode == OUTDOOR ? "OUTDOOR" : "HYBRID"));
}

Mode LEDController::getMode()
{
    // Serial.print("Current Mode: ");
    // Serial.println(currentMode == INDOOR ? "INDOOR" : (currentMode == OUTDOOR ? "OUTDOOR" : "HYBRID"));
    return currentMode;
}

void LEDController::updateLED()
{
    switch (currentMode)
    {
    case INDOOR:
        digitalWrite(LED_PIN, LOW);
        break;
    case OUTDOOR:
        digitalWrite(LED_PIN, HIGH);
        break;
    case HYBRID:
        unsigned long currentTime = millis();
        if (currentTime - lastBlinkTime >= 50)
        {
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
            lastBlinkTime = currentTime;
        }
        break;
    }
}

void LEDController::updateRGBLed(float temperature)
{
    if (isnan(temperature))
    {
        analogWrite(RGB_RED_PIN, 255);
        analogWrite(RGB_GREEN_PIN, 255);
        analogWrite(RGB_BLUE_PIN, 255);
    }
    else
    {

        int redValue = 0;
        int greenValue = 0;
        int blueValue = 0;
        if (temperature <= -300)
        {
            redValue = 0;
            greenValue = 0;
            blueValue = 0;
        }
        else if (temperature <= 0 && temperature > -30)
        {
            blueValue = map(temperature, -20, 0, 255, 100);
        }
        else if (temperature > 0 && temperature <= 20)
        {
            blueValue = map(temperature, 0, 20, 255, 0);
            greenValue = map(temperature, 0, 20, 100, 255);
        }
        else if (temperature > 20 && temperature <= 40)
        {
            greenValue = map(temperature, 20, 30, 255, 0);
            redValue = map(temperature, 20, 40, 100, 255);
        }
        else if (temperature > 40)
        {
            redValue = 255;
        }

        analogWrite(RGB_RED_PIN, redValue);
        analogWrite(RGB_GREEN_PIN, greenValue);
        analogWrite(RGB_BLUE_PIN, blueValue);

        Serial.print("RGB LED (Temperature): ");
        Serial.print("Red: ");
        Serial.print(redValue);
        Serial.print(" Green: ");
        Serial.print(greenValue);
        Serial.print(" Blue: ");
        Serial.println(blueValue);
    }
}