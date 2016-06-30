#include "commonVariables.h"
#include "config.h"

void setup() {
    Serial.begin(9600);
//  etherInit();
    lcdInit();
    dustInit();
    dhtInit();
    ledInit();
    configRead();
    lcdWrite("Wifi starting...");
    wifiInit();
    wifiConnectToAP();
    lcdClear();
}

float dustDensity, voltage, temperature, humidity;

void loop() {
    dustRead(&voltage, &dustDensity);
    dhtRead(&temperature, &humidity);
    lcdUpdateTempHumid(temperature, humidity);
    lcdUpdateDustCo(voltage, dustDensity);
    wifiSendHTTPRequest(temperature, humidity, voltage, dustDensity);
    delay(5000);
}
