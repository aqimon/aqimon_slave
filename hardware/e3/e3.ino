#include "commonVariables.h"

void setup(){
    Serial.begin(19200);
   // etherInit();
    lcdInit();
    dustInit();
    dhtInit();
    ledInit();
    wifiInit();
    wifiConnectToAP("Floor 5", "123haiphong");
}

float dustDensity, voltage, temperature, humidity;

void loop(){
    dustRead(&voltage, &dustDensity);
    dhtRead(&temperature, &humidity);
    lcdUpdateTempHumid(temperature, humidity);
    lcdUpdateDustCo(voltage, dustDensity);
    wifiSendHTTPRequest("08e0a9af-214d-4aa3-86d7-94317b3598b", temperature, humidity, voltage, dustDensity);
    delay(5000);
}
