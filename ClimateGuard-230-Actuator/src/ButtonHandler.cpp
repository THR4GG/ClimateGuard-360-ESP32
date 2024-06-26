#include "ButtonHandler.h"
#include "EnvironmentConfig.h"

ButtonHandler::ButtonHandler()
{
    pinMode(MODUS_PIN, INPUT_PULLDOWN);
    pinMode(LEISER_PIN, INPUT_PULLDOWN);
    pinMode(PLAY_PAUSE_PIN, INPUT_PULLDOWN);
    pinMode(LAUTER_PIN, INPUT_PULLDOWN);

    for (int i = 0; i < 4; i++)
    {
        buttonStates[i] = false;
    }
}

void ButtonHandler::update()
{
    buttonStates[0] = digitalRead(MODUS_PIN);
    buttonStates[1] = digitalRead(LEISER_PIN);
    buttonStates[2] = digitalRead(PLAY_PAUSE_PIN);
    buttonStates[3] = digitalRead(LAUTER_PIN);
}

bool ButtonHandler::getModusState()
{
    this->update();
    return buttonStates[0];
}

bool ButtonHandler::getLeiserState()
{
    this->update();
    return buttonStates[1];
}

bool ButtonHandler::getPlayPauseState()
{
    this->update();
    return buttonStates[2];
}

bool ButtonHandler::getLauterState()
{
    this->update();
    return buttonStates[3];
}
