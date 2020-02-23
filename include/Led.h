#ifndef Led_h
#define Led_h
#include <Arduino.h>
#include "Task.h"

class Led : public Task
{
private:
    uint8_t state;
    uint8_t pin;

    void switchState()
    {
        digitalWrite(this->pin, this->state);
    }

public:
    Led()
    {
    }

    Led(uint8_t pin, uint8_t startState = 0)
    {
        this->pin = pin;
        this->state = startState;
        pinMode(pin, OUTPUT);
        switchState();
    }

    void on()
    {
        this->state = 1;
        this->setInterval(0);
        switchState();
    }

    void off()
    {
        this->state = 0;
        this->setInterval(0);
        switchState();
    }

    void toggle()
    {
        this->state = !this->state;
        switchState();
    }

    void blink(uint32_t msInterval)
    {
        this->setInterval(msInterval);
        if (msInterval)
        {
            toggle();
        }
    }

    uint8_t getState()
    {
        return this->state;
    }

protected:
    void run()
    {
        this->toggle();
    }
};
#endif