#include <Arduino.h>
#include <pins_arduino.h>
#include "RTC.h"
#include "TC.h"

TC tcs[1];

#include "WebServer.h"

RTC& realTimeClock = RTC::getInstance();
WebServer webServer;

void setup()
{
    Serial.begin(74880);
    while (!Serial) {}
    
    //Disable builtin AP
    //WiFi.mode(WIFI_STA);

    realTimeClock.setRTCUpd((1000 * 60 * 10), 1000, "europe.pool.ntp.org", 7200);

    tcs[0] = TC("Room", D1, D7, D8, 0, 10000);
    //tcs[1] = TC("Kitchen", D1, D7, D8, 1024, 10000);

    webServer = WebServer(80);
    
}

void loop()
{ 
    realTimeClock.watch();
    webServer.watch();

    for(uint8_t i = 0; i < sizeof(tcs)/sizeof(tcs[0]); i++){
        tcs[i].watch();
    }
}