#include "ActuatorController.h"
#include "EnvironmentConfig.h"

ActuatorController::ActuatorController()
{
}

void ActuatorController::setupActuators()
{
    servo.attach(SERVO_PIN);
    pinMode(FAN_PIN, OUTPUT);
}

void ActuatorController::updateServo(int lightIntensity)
{

    int servoAngle = 0;

    if (lightIntensity < 10)
    {
        servoAngle = 0;
    }
    else{
        servoAngle = map(lightIntensity, 10, 50, 0, 180);
    }

    servo.write(SERVO_PIN, servoAngle);

    Serial.print("Servo Angle (Light Intensity): ");
    Serial.println(servoAngle);
}

void ActuatorController::updateFan(float temperature)
{
    int fanSpeed = 0;
    if (temperature <= 20)
    {
        fanSpeed = 0;
        return;
    }
    else
    {
        fanSpeed = map(temperature, 20, 40, 150, 255);
    }

    analogWrite(FAN_PIN, fanSpeed);

    Serial.print("Fan Speed (Temperature): ");
    Serial.println(fanSpeed);
}
