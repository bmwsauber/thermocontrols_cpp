#ifndef RTC_h
#define RTC_h
#include <Arduino.h>
#include "Task.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

class RTC : public Task
{

private:
    WiFiUDP ntpUDP;
    NTPClient timeClient = NTPClient(this->ntpUDP);

public:
    uint8_t hours;
    uint8_t mins;
    
    static RTC &getInstance()
    {
        static RTC instance;
        return instance;
    }

    void setRTCUpd(uint32_t updateInterval, uint16_t runInterval, const char *poolServerName, uint16_t timeZoneOffset)
    {
        this->timeClient = NTPClient(this->ntpUDP, poolServerName, timeZoneOffset, updateInterval);
        this->timeClient.begin();
        this->timeClient.update();
        
        this->setInterval(runInterval);
    }

    uint16_t static getConcatTime(uint8_t a, uint8_t b)
    {
        char chars[4];
        sprintf(chars, "%d%02d", a, b);
        uint16_t result = atoi(chars);
        return result;
    }

    uint16_t getConcatTime()
    {
        char chars[4];
        sprintf(chars, "%d%02d", this->hours, this->mins);
        uint16_t result = atoi(chars);
        return result;
    }

    String getFormattedTime()
    {
        char chars[5];
        sprintf(chars, "%02d:%02d", this->hours, this->mins);
        return chars;
    }

protected:
    void run()
    {
        this->timeClient.update();
        this->hours = this->timeClient.getHours();
        this->mins = this->timeClient.getMinutes();
    }

private:
    RTC() {}
    RTC(const RTC &);
    RTC &operator=(RTC &);
};
#endif