#include "ButtonPressCounter.h"

ButtonPressCounter::ButtonPressCounter()
{
}

void ButtonPressCounter::update(void (*ptr)(int), bool buttonState, int buttonID) {
    if(buttonState != prevButtonState) {
        if(buttonState) {
            ptr(buttonID);
        }
        prevButtonState = buttonState;
    }
}
