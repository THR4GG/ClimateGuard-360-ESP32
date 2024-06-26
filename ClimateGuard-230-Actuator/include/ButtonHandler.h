#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler
{
public:
    ButtonHandler();
    void update();
    bool getModusState();
    bool getLeiserState();
    bool getPlayPauseState();
    bool getLauterState();

private:
    bool buttonStates[4];
};

#endif
