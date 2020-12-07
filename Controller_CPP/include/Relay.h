#ifndef Relay_h
#define Relay_h
#include <Arduino.h>
#include <pins_arduino.h>
#include "Task.h"

class Relay : public Task
{
private:
    uint8_t state;
    uint8_t pin;
    uint32_t debounce = 0;

    void switchState()
    {
        digitalWrite(this->pin, this->state);
    }

public:
    Relay()
    {
    }

    Relay(uint8_t pin, uint8_t startState = 0)
    {
        this->pin = pin;
        this->state = startState;

        pinMode(pin, OUTPUT);
        switchState();
    }

    uint8_t on(uint32_t setDebounce = 0)
    {
        //Serial.println("ON");
        if (setDebounce && this->debounce >= millis())
        {
            Serial.println("debounce state: on");
            Serial.println(this->debounce - millis());
        }
        else
        {
            if (this->getState() != 1)
            {
                this->state = 1;
                digitalWrite(this->pin, LOW);
                if (setDebounce)
                {
                    //Serial.println("debounce on + ");
                    this->debounce = setDebounce + millis();
                }
            }
        }

        return this->getState();
    }

    uint8_t off(uint32_t setDebounce = 0)
    {
        //Serial.println("OFF");
        if (setDebounce && this->debounce >= millis())
        {
            Serial.println("debounce state: off");
            Serial.println(this->debounce - millis());
        }
        else
        {
            if (this->getState() != 0)
            {
                this->state = 0;
                digitalWrite(this->pin, HIGH);
                if (setDebounce)
                {
                    //Serial.println("debounce off + ");
                    this->debounce = setDebounce + millis();
                }
            }
        }

        return this->getState();
    }

    void toggle()
    {
        this->state = !this->state;
        switchState();
    }

    uint8_t getState()
    {
        return !digitalRead(this->pin);
    }

protected:
    void run()
    {
        this->toggle();
    }
};
#endif