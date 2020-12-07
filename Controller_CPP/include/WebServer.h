#ifndef WebServer_h
#define WebServer_h
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

ESP8266WebServer server(80);
WiFiManager wifiManager;

class WebServer
{

public:
    String postForms;
    
    WebServer(){}
    
    WebServer(uint8_t port)
    {
        //reset settings - for testing
        //wifiManager.resetSettings();

        wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 0, 99), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
        wifiManager.setSTAStaticIPConfig(IPAddress(192, 168, 0, 99), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0), IPAddress(192, 168, 0, 1));
        wifiManager.setConfigPortalTimeout(180);
        wifiManager.autoConnect("NodeMCU");
        
        server.begin();
        server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        server.sendHeader("Pragma", "no-cache");
        server.sendHeader("Expires", "-1");
        server.sendHeader("Access-Control-Allow-Origin", "*");

        server.on("/", std::bind(&WebServer::handleRoot, this));
        server.on("/get", std::bind(&WebServer::handleGet, this));
        server.on("/set", std::bind(&WebServer::handleSet, this));
    }

    void handleRoot()
    {
        String postForms = "<!doctype html><html lang='en'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0'> <meta http-equiv='X-UA-Compatible' content='ie=edge'> <title>NodeMcu</title> <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css' integrity='sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh' crossorigin='anonymous'> <script src='https://kit.fontawesome.com/d92bc170ca.js' crossorigin='anonymous'></script> <script src='https://cdn.jsdelivr.net/npm/vue'></script> <script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script> <script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.22.2/moment.min.js'></script> <script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/axios/0.19.0/axios.js'></script> <script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/tempusdominus-bootstrap-4/5.0.1/js/tempusdominus-bootstrap-4.min.js'></script> <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/tempusdominus-bootstrap-4/5.0.1/css/tempusdominus-bootstrap-4.min.css'/> <link href='https://cdn.jsdelivr.net/gh/gitbrent/bootstrap4-toggle@3.6.1/css/bootstrap4-toggle.min.css' rel='stylesheet'> <script src='https://cdn.jsdelivr.net/gh/gitbrent/bootstrap4-toggle@3.6.1/js/bootstrap4-toggle.min.js'></script> <style>#app{color: #7A7A7A}.modes .active{background-color: #7a7a7a; color: #fff;}.controls button{font-size: 35px; border: solid 1px #7a7a7a; color: #7A7A7A; background-color: #fff;}.mode-advanced .digits{font-size: 66px;}.top-data{font-size: 17px;}.fa-cogs{font-size: 17px;}.fa-cogs.on{color: #ff7676;}.fa-wifi{color: #eeeeee;}.fa-wifi.on{color: #ff7676;}.fa-cogs.off{opacity: 0.1;}.mode-simple .digits, .mode-direct .fa-power-off{font-size: 116px;}.fa-sun, .fa-moon{margin-top: 20px; font-size: 31px;}.mode-simple .plus-minus{font-size: 60px;}.mode-advanced .plus-minus{font-size: 40px;}.day{}.night{}</style></head><body><div id='app'> <div class='container mt-3' style='max-width:544px; display:none;'> <div class='row top-data'> <div class='col-4 border-right text-center py-3'> <i class='fas fa-temperature-low'></i>{{tempData}}<span class='sign'>&#8451;</span> </div><div class='col-5 text-center border-right py-3'> <i class='far fa-clock'></i>{{clockData}}</div><div class='col-3 py-3' style='text-align: center;'> <span><i class='fas fa-wifi' :class='{on : recivingData}'></i></span> <span style='color: #dee2e6;'>|</span> <span v-if='relay'><i class='fas fa-cogs on'></i></span> <span v-else><i class='fas fa-cogs off'></i></span> </div></div><div class='border-top border-bottom'> <div class='row mode-simple' v-show='mode==1'> <div class='col-4 border-right plus-minus text-center'> <div class='my-2 plus-minus border-bottom' @click='setTempSimple(++tempSimple)'><i class='fas fa-plus'></i></div><div class='my-2 plus-minus' @click='setTempSimple(--tempSimple)'><i class='fas fa-minus'></i></div></div><div class='col-8 text-center'> <div><span class='digits'>{{tempSimple}}</span> <span class='sign' style='font-size: 36px;'>&#8451;</span></div></div></div><div class='row mode-direct text-center m-5' v-show='mode==3'> <div class='col-4'></div><div class='col-4'><input type='checkbox' id='directMode' :checked='directMode' data-toggle='toggle' data-width='100' data-height='75' data-onstyle='danger'></div><div class='col-4'></div></div><div class='mode-advanced' v-show='mode==2'> <div class='row night'> <div class='col-4 border-right plus-minus text-center'> <div class='my-2 plus-minus border-bottom' @click='setTempNignt(++tempNight)'><i class='fas fa-plus'></i></div><div class='my-2 plus-minus' @click='setTempNignt(--tempNight)'><i class='fas fa-minus'></i> </div></div><div class='col-8 text-center row'> <div class='col-3'><i class='fas fa-moon'></i></div><div class='col-9'><span class='digits'>{{tempNight}}</span> <span class='sign' style='font-size: 36px;'>&#8451;</span> </div><div class='col-12'> <div class='input-group date mb-1 text-center' id='timepicker-night' data-target-input='nearest'><label for='timepicker-night' style='margin: 8px 10px 0 0'> Starts At: </label> <input type='text' class='form-control datetimepicker-input' data-target='#timepicker-night' data-date-format='HH:mm' v-model='nightStarts' style='max-width:70px;'/> <div class='input-group-append' data-target='#timepicker-night' data-toggle='datetimepicker'> <div class='input-group-text'><i class='fa fa-clock-o'></i></div></div></div></div></div></div><div class='row day border-top'> <div class='col-4 border-right plus-minus text-center'> <div class='my-2 plus-minus border-bottom' @click='setTempDay(++tempDay)'><i class='fas fa-plus'></i></div><div class='my-2 plus-minus' @click='setTempDay(--tempDay)'><i class='fas fa-minus'></i></div></div><div class='col-8 text-center row'> <div class='col-3'><i class='fas fa-sun'></i></div><div class='col-9'><span class='digits'>{{tempDay}}</span> <span class='sign' style='font-size: 36px;'>&#8451;</span> </div><div class='col-12'> <div class='input-group date mb-1 text-center' id='timepicker-day' data-target-input='nearest'><label for='timepicker-day' style='margin: 8px 10px 0 0'> Starts At: </label> <input type='text' class='form-control datetimepicker-input' data-target='#timepicker-day' data-date-format='HH:mm' v-model='dayStarts' style='max-width:70px;'/> <div class='input-group-append' data-target='#timepicker-day' data-toggle='datetimepicker'> <div class='input-group-text'><i class='fa fa-clock-o'></i></div></div></div></div></div></div></div></div><div class='row p-3 controls border-bottom'> <div class='modes col-4'> <button class='w-100 rounded' :class='{active : (mode==1)}' @click='setMode(1)'><i class='fas fa-thermometer-quarter'></i></button> </div><div class='modes col-4'> <button class='w-100 rounded' :class='{active : (mode==2)}' @click='setMode(2)'><i class='far fa-calendar-check'></i></button> </div><div class='modes col-4'> <button class='w-100 rounded' :class='{active : (mode==3)}' @click='setMode(3)'><i class='fas fa-tools'></i></button> </div></div><div class='row mt-3'> <div class='col-3'></div><div class='col-6 controls'> <button class='w-100 rounded' @click='sendParams'><i class='far fa-save'></i> SAVE</button> </div><div class='col-3'></div></div></div></div><script>var app=new Vue({el: '#app', data:{clockData: '--:--', tempData: '--', dayStarts: '--:--', nightStarts: '--:--', mode: 1, relay: 1, tempSimple: '--', tempDay: '--', tempNight: '--', directMode: 0, recivingData: false,}, mounted(){$startDatepicker=$('#timepicker-night, #timepicker-day').datetimepicker({format: 'HH:mm',}); $startDatepicker.on('change.datetimepicker', (e)=>{let target=$(e.target).find('input'); if(target.attr('data-target')==='#timepicker-night'){this.nightStarts=target.val();}else{this.dayStarts=target.val();}}); $('#directMode').change(()=>{this.directMode=!this.directMode;}); this.initParams(); this.resiveParams(); $('.container').show();}, methods:{setMode(mode){this.mode=mode;}, setTempSimple(temp){this.tempSimple=temp;}, setTempDay(temp){this.tempDay=temp;}, setTempNignt(temp){this.tempNight=temp;}, sendParams(){this.recivingData=true; let day=this.dayStarts.split(':'); let night=this.nightStarts.split(':'); const params=new URLSearchParams(); params.append('day_starts_hour', day[0]); params.append('day_starts_mins', day[1]); params.append('night_starts_hour', night[0]); params.append('night_starts_mins', night[1]); params.append('mode', this.mode); params.append('temp_simple', this.tempSimple); params.append('temp_day', this.tempDay); params.append('temp_night', this.tempNight); params.append('direct_mode', Number(this.directMode)); axios.post('http://192.168.0.99/set', params) .then(function (response){this.recivingData=false; console.log(response);}).catch(function (error){console.log(error);});}, resiveParams(){setInterval(()=>{this.doAjax();}, 3000);}, doAjax(){this.recivingData=true; axios.get('http://192.168.0.99/get') .then((response)=>{this.relay=response.data.relay; this.clockData=response.data.hour + ':' + response.data.minut; this.tempData=response.data.temp.toFixed(1); this.recivingData=false;}).catch((error)=>{console.log(error);});}, initParams(){this.recivingData=true; axios.get('http://192.168.0.99/get') .then((response)=>{this.mode=response.data.mode; this.tempData=response.data.temp.toFixed(1); this.clockData=response.data.hour + ':' + response.data.minut; this.dayStarts=response.data.day_starts_hour + ':' + response.data.day_starts_mins; this.nightStarts=response.data.night_starts_hour + ':' + response.data.night_starts_mins; this.tempSimple=response.data.temp_simple; this.tempDay=response.data.temp_day; this.tempNight=response.data.temp_night; this.directMode=response.data.direct_mode; if(this.directMode){$('#directMode').bootstrapToggle('on');}else{$('#directMode').bootstrapToggle('off');}this.recivingData=false;}).catch((error)=>{console.log(error);});}}})</script></body></html>";
        server.send(200, "text/html", postForms);
        
    }

    void handleGet()
    {
        // sizeof(tcs)/sizeof(tcs[0])

        RTC& rtc = RTC::getInstance();
        uint8_t i = 0;
        //uint8_t i = server.arg("tc").toInt();
        char json[300];
        snprintf(
            json, sizeof(json),
            "{\"mode\":%u,\"relay\":%u,\"temp\":%f,\"hour\":\"%02d\",\"minut\":\"%02d\",\"temp_simple\":%u,\"temp_day\":%u,\"temp_night\":%u,\"day_starts_hour\":\"%02d\",\"day_starts_mins\":\"%02d\",\"night_starts_hour\":\"%02d\",\"night_starts_mins\":\"%02d\",\"direct_mode\":%u}",
            tcs[i].data.mode, tcs[i].data.relayState, tcs[i].data.temp, rtc.hours, rtc.mins, tcs[i].data.tempSimple, tcs[i].data.tempDay, tcs[i].data.tempNight, tcs[i].data.dayStartHour, tcs[i].data.dayStartMins, tcs[i].data.nightStartHour, tcs[i].data.nightStartMins, tcs[i].data.directModeState);
        server.send(200, "application/json", json);
    }

    void handleSet()
    {
        if (server.method() != HTTP_POST)
        {
            server.send(405, "text/plain", "Method Not Allowed");
        }
        else
        {
            uint8_t i = 0;
            //uint8_t i = server.arg("tc").toInt();
            
            tcs[i].data.mode =  server.arg("mode").toInt();
            tcs[i].data.tempSimple = server.arg("temp_simple").toInt();
            tcs[i].data.tempDay = server.arg("temp_day").toInt();
            tcs[i].data.tempNight = server.arg("temp_night").toInt();
            tcs[i].data.dayStartHour = server.arg("day_starts_hour").toInt();
            tcs[i].data.dayStartMins = server.arg("day_starts_mins").toInt();
            tcs[i].data.nightStartHour = server.arg("night_starts_hour").toInt();
            tcs[i].data.nightStartMins = server.arg("night_starts_mins").toInt();
            tcs[i].data.directModeState = server.arg("direct_mode").toInt();

            tcs[i].save();

            server.send(200, "text/plain", "success!");
        }
    }

    void watch()
    {
        server.handleClient();
    }
};

#endif