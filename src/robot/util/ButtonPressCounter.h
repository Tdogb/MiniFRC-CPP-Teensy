class ButtonPressCounter
{
private:
    bool prevButtonState = false;
public:
    ButtonPressCounter();
    void update(void (*ptr)(int), bool buttonState, int buttonID);
};


