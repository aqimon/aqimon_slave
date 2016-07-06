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
    strcpy(config.ssidName, "Floor 5");
    strcpy(config.ssidPassword, "123haiphong");
    strcpy(config.apiKey, "crBurWijvHGLwc51UKeo");
    strcpy(config.clientID, "f96521a9-7b0c-4658-8a6a-18bee82f3dfe");
    configWrite();
    wifiInit();
    wifiConnectToAP();
    lcdClear();
    lcdWrite("E3 client v0.1");
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
