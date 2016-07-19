#include "const.h"
#include "sensors/DHT.h"
#include "sensors/dust.h"
#include "config/config.h"
#include "lcd/lcd.h"
#include "wifi/wifiCore.h"
#include "wifi/wifiClient.h"
#include "wifi/wifiServer.h"

float dustDensity, voltage, temperature, humidity;
volatile int cfgMode=0;

void setup() {
    Serial.begin(38400);
    lcdInit();
    dustInit();
    dhtInit();
    ledInit();
    configRead();
    lcdClear();
    lcdWrite("E3 client v0.1");
    dustRead(&voltage, &dustDensity);
    dhtRead(&temperature, &humidity);
    lcdUpdateTempHumid(temperature, humidity);
    lcdUpdateDustCo(voltage, dustDensity);
    wifiInit();
    wifiClientInit();
    attachInterrupt(digitalPinToInterrupt(2), configurationMode, LOW);
}

void configurationMode(){
    cfgMode=1;
}

void loop() {
    if (!wifiSendHTTPRequest(temperature, humidity, dustDensity, voltage)){
       wifiInit();
       wifiClientInit();
    }
    if (cfgMode){
        wifiInit();
        wifiServerInit();
        wifiServerListener();
    }
    delay(1000);
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
