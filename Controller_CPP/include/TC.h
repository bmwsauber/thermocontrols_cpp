#ifndef TC_h
#define TC_h
#include <Arduino.h>
#include <pins_arduino.h>
#include "Task.h"
#include "Relay.h"
#include "LedIndicator.h"
#include <OneWire.h>
#include "RTC.h"
#include <DallasTemperature.h>
#include <EEPROM.h>

class TC : public Task
{
public: 
    String name;
    OneWire oneWire;
    DallasTemperature sensor;
    Relay relay; 
    LedIndicator led;
    uint16_t epromAddr = 0;

    struct Data
    {
        uint8_t mode = 1;
        uint8_t relayState = 0;
        uint8_t hours = 0;
        uint8_t mins = 0;
        uint8_t tempSimple = 20;
        uint8_t tempDay = 20;
        uint8_t tempNight = 19;
        uint8_t dayStartHour = 8;
        uint8_t dayStartMins = 0;
        uint8_t nightStartHour = 0;
        uint8_t nightStartMins = 0;
        uint8_t directModeState = 0;
        float temp = 0;
    } data;

    TC() {}
    
    TC(String name, uint8_t sensorPin, uint8_t relayPin, uint8_t ledPin, uint16_t epromAddr = 0, uint32_t updateInterval = 10000)
    {
        this->name = name;
        this->oneWire = OneWire(sensorPin);
        this->sensor = DallasTemperature(&this->oneWire);
        this->sensor.begin();
        this->relay = Relay(relayPin);
        this->led = LedIndicator(ledPin);
        this->epromAddr = epromAddr;
        this->setInterval(updateInterval);
        this->dataLoad(); //comment this to setup values in new device
    }

    void save()
    {
        this->dataSave();
    }

    void run()
    {
        this->led.on();
        this->sensor = DallasTemperature(&this->oneWire);
        this->sensor.requestTemperaturesByIndex(0);
        this->data.temp = this->sensor.getTempCByIndex(0);
        
        switch (this->data.mode)
        {
        case 1:
            this->simpleMode();
            break;
        case 2:
            this->advancedMode();
            break;
        case 3:
            this->directMode();
            break;
        default:
            this->simpleMode();
            break;
        }
        this->led.off();
    }

protected: 
    void simpleMode()
    {
        if (this->data.tempSimple > this->data.temp)
        {
            this->data.relayState = relay.on(1000);
        }
        else
        {
            this->data.relayState = relay.off(1000);
        }
    }

    void advancedMode()
    {
        RTC& rtc = RTC::getInstance();

        uint16_t dayStarts = RTC::getConcatTime(this->data.dayStartHour, this->data.dayStartMins);
        uint16_t nightStarts = RTC::getConcatTime(this->data.nightStartHour, this->data.nightStartMins);
        uint16_t now = rtc.getConcatTime();

        if (dayStarts <= nightStarts)
        {
            if (now < nightStarts && now >= dayStarts) //isDAY
                this->handleTempDay();
            else //is Night
                this->handleTempNight();
        }
        else
        {
            if (now < dayStarts && now >= nightStarts) //Night
                this->handleTempNight();
            else //is isDAY
                this->handleTempDay();
        }
    }

    void directMode()
    {
        if (this->data.directModeState == 1)
        {
            this->data.relayState = relay.on();
        }
        else
        {
            this->data.relayState = relay.off();
        }
    }

    void handleTempDay()
    {
        if (this->data.tempDay > this->data.temp)
        {
            this->data.relayState = relay.on(1000);
        }
        else
        {
            this->data.relayState = relay.off(1000);
        }
    }

    void handleTempNight()
    {
        if (this->data.tempNight > this->data.temp)
        {
            this->data.relayState = relay.on(1000);
        }
        else
        {
            this->data.relayState = relay.off(1000);
        }
    }

    void dataSave()
    {
        EEPROM.begin(1024);
        EEPROM.put(this->epromAddr, this->data);
        EEPROM.commit();
    }

    void dataLoad()
    {
        EEPROM.begin(1024);
        EEPROM.get(this->epromAddr, this->data);
    }
};
#endif