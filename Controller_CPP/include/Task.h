#ifndef Task_h
#define Task_h
#include <Arduino.h>

class Task
{
private:
    uint32_t msInterval = 0; //0 = off
    uint32_t _tick = 0;
    bool init = true; //run at first time

public:
    Task()
    {
    }
    virtual void run();
    void setInterval(uint16_t msInterval)
    {
        this->msInterval = msInterval;
    }
    void watch()
    {
        if (this->msInterval || this->init)
        {
            uint32_t measurement = millis() - _tick;
            if (measurement >= msInterval || this->init)
            {
                _tick = millis();
                this->run();
                this->init = false;
            }
        }
    }
};
#endif