#ifndef ACTUATORCONTROLLER_H
#define ACTUATORCONTROLLER_H

#include <Arduino.h>
#include <Servo.h>

class ActuatorController
{
public:
    ActuatorController();
    void setupActuators();
    void updateServo(int lightIntensity);
    void updateFan(float temperature);

private:
    Servo servo;
};

#endif
