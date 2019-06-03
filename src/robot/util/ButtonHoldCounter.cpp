#include "ButtonHoldCounter.h"

ButtonHoldCounter::ButtonHoldCounter(/* args */)
{
}

void ButtonHoldCounter::updateScheduled(void (*ptr)(int), int buttonID) {
    if(realtimeButtonState) {
        ptr(buttonID);
    }
}

void ButtonHoldCounter::updateLoop(bool buttonState) {
    realtimeButtonState = buttonState;
}