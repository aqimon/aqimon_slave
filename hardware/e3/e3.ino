#include "const.h"
#include "sensors/DHT.h"
#include "sensors/dust.h"
#include "config/config.h"
#include "lcd/lcd.h"
#include "notification/led.h"
#include "wifi/wifiCore.h"
#include "wifi/wifiClient.h"
#include "wifi/wifiServer.h"
#include <avr/wdt.h>

void __attribute__((naked)) __attribute__((section(".init3"))) wdt_init(void) {
    MCUSR = 0;
    wdt_disable();
}

float temperature, humidity, dustDensity, coLevel, voltage, coDensity;
unsigned long t;

void setup() {
    wdt_enable(WDTO_2S);
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
    lcdUpdateDustCo(dustDensity, voltage);
    if (dustDensity > 0.2) {
        ledSetRight(LED_RED);
        ledSetColor();
    } else {
        ledSetRight(LED_GREEN);
        ledSetColor();
    }
    wifiInit();
    wifiClientInit();
}


void loop() {
    if (!wifiSendHTTPRequest(temperature, humidity, dustDensity, voltage)){
        wifiInit();
        wifiClientInit();
    }
    t = millis();
    if (config.sleepTime > 30) 
        wifiDeepSleep((config.sleepTime - 15) * 1000);
    lcdUpdateWifiStatus(WIFI_IDLE);
    while (millis() - t <= (config.sleepTime * 1000)) {
        wdt_reset();
        if (configEnabled) {
            configStarted = 1;
            wifiInit();
            wifiServerInit();
            wifiServerListener();
        }
    }   
    dustRead(&voltage, &dustDensity);
    //coRead(&coDensity);
    dhtRead(&temperature, &humidity);
    lcdUpdateTempHumid(temperature, humidity);
    lcdUpdateDustCo(dustDensity, voltage);
    if (dustDensity > 0.2) {
        ledSetRight(LED_RED);
        ledSetColor();
    } else {
        ledSetRight(LED_GREEN);
        ledSetColor();
    }
    if (config.sleepTime > 30){
        wifiInit();
        wifiClientInit();
    }
}
