#include "const.h"
#include "sensors/DHT.h"
#include "sensors/dust.h"
#include "config/config.h"
#include "lcd/lcd.h"
#include "notification/led.h"
// #include "wifi/wifiCore.h"
// #include "wifi/wifiClient.h"
// #include "wifi/wifiServer.h"
#include "sim900/sim900Core.h"
#include "sim900/sim900Client.h"
#include "buffer/buffer.h"
#include <avr/wdt.h>

void __attribute__((naked)) __attribute__((section(".init3"))) wdt_init(void) {
    MCUSR = 0;
    wdt_disable();
}

float temperature, humidity, dustDensity, coLevel, voltage, coDensity;
unsigned long t;

void busyloop(unsigned long duration){
    unsigned long t = millis();
    while (millis() - t <= duration){
        wdt_reset();
    }
}

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
    sim900Init();
    busyloop(3000);
    sim900ClientInit();
}

void loop() {
    while (!sim900SendHTTPRequest(temperature, humidity, dustDensity, voltage)){
        sim900Init();
        busyloop(3000);
        sim900ClientInit();
    }
    t = millis();
    while (millis() - t <= (config.sleepTime * 1000)) {
        wdt_reset();
        // while (get_buffer_count != 0){
        //     struct record_s record;
        //   if (buffer_pop(*record)){
        //         while (!wifiSendHTTPRequest(temperature, humidity, dustDensity, voltage)){
        //             wifiInit();
        //            wifiClientInit();
        //         }           
        //     }
            // }
        // if (configEnabled) {
        //     configStarted = 1;
        //     wifiInit();
        //     wifiServerInit();
        //     wifiServerListener();
        // }
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
    // if (config.sleepTime > 30){
    //     wifiInit();
    //     wifiClientInit();
    //}
}
