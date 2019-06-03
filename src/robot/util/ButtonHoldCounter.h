class ButtonHoldCounter
{
private:
    bool realtimeButtonState = false;
public:
    ButtonHoldCounter(/* args */);
    void updateScheduled(void (*ptr)(int), int buttonID);
    void updateLoop(bool buttonState);
};

