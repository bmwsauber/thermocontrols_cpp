#ifndef LedIndicator_h
#define LedIndicator_h
#include <Arduino.h>

class LedIndicator
{
private:
    uint8_t state;
    uint8_t pin;

    void switchState()
    {
        digitalWrite(this->pin, this->state);
    }

public:
    LedIndicator()
    {
    }

    LedIndicator(uint8_t pin, uint8_t startState = 0)
    {
        this->pin = pin;
        this->state = startState;
        pinMode(pin, OUTPUT);
        switchState();
    }

    void on()
    {
        this->state = 1;
        switchState();
    }

    void off()
    {
        this->state = 0;
        switchState();
    }

    void toggle()
    {
        this->state = !this->state;
        switchState();
    }

    uint8_t getState()
    {
        return this->state;
    }
};
#endif