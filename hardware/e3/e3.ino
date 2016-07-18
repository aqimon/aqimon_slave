#include "const.h"
#include "sensors/DHT.h"
#include "sensors/dust.h"
#include "config/config.h"
#include "lcd/lcd.h"
#include "wifi/wifiCore.h"
#include "wifi/wifiClient.h"
#include "wifi/wifiServer.h"

float dustDensity, voltage, temperature, humidity;

void setup() {
    Serial.begin(9600);
//    lcdInit();
//    dustInit();
//    dhtInit();
//    ledInit();
    configRead();
//    lcdClear();
//   lcdWrite("E3 client v0.1");
//   dustRead(&voltage, &dustDensity);
//    dhtRead(&temperature, &humidity);
//    lcdUpdateTempHumid(temperature, humidity);
//    lcdUpdateDustCo(voltage, dustDensity);
    wifiInit();
    wifiServerInit();
    //wifiLoop();
    wifiServerListener();
}

void loop() {
//    if (!wifiSendHTTPRequest(temperature, humidity, voltage, dustDensity))
//        wifiInit();
//    delay(5000);
//    dustRead(&voltage, &dustDensity);
//    dhtRead(&temperature, &humidity);
//    lcdUpdateTempHumid(temperature, humidity);
//    lcdUpdateDustCo(voltage, dustDensity);
}