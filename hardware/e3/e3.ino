#include "const.h"
#include "sensors/DHT.h"
#include "sensors/dust.h"
#include "config/config.h"
#include "lcd/lcd.h"
#include "notification/led.h"
#include "wifi/wifiCore.h"
#include "wifi/wifiClient.h"
#include "wifi/wifiServer.h"

float temperature, humidity, dustDensity, coLevel, voltage;
unsigned long t;

void setup() {
    Serial.begin(38400);
    attachInterrupt(digitalPinToInterrupt(2), configInterrupt, CHANGE);
    lcdInit();
    dustInit();
    dhtInit();
    ledInit();
    configRead();
    dustRead(&voltage, &dustDensity);
    dhtRead(&temperature, &humidity);
    lcdUpdateTempHumid(temperature, humidity);
    lcdUpdateDustCo(voltage, dustDensity);
    wifiInit();
    wifiClientInit();
}


void loop() {
    if (!wifiSendHTTPRequest(temperature, humidity, dustDensity, voltage)){
       wifiInit();
       wifiClientInit();
    }
    t=millis();
    while (millis()-t<=5000){
        if (configEnabled==1){
            configStarted=1;
            wifiInit();
            wifiServerInit();
            wifiServerListener();
        }
    }
    dustRead(&voltage, &dustDensity);
    dhtRead(&temperature, &humidity);
    lcdUpdateTempHumid(temperature, humidity);
    lcdUpdateDustCo(voltage, dustDensity);
    if (dustDensity>0.2){
        ledSetRight(LED_RED);
        ledSetColor();
    } else {
        ledSetRight(LED_GREEN);
        ledSetColor();
    }
}
