#include "commonVariables.h"

void setup(){
    Serial.begin(9600);
   // etherInit();
    lcdInit();
    dustInit();
    dhtInit();
    ledInit();
    wifiInit();
}

float dustDensity, voltage, temperature, humidity;

void loop(){
    wifiLoop();
    //dustRead(&voltage, &dustDensity);
    //Serial.print(voltage);
    //Serial.print(" ");
    //Serial.println(dustDensity);
   // dhtRead(&temperature, &humidity);
    //lcdUpdateTempHumid(temperature, humidity);
    //lcdUpdateDustCo(voltage, dustDensity);
    //wifiSendHTTPRequest(temperature, humidity, voltage, dustDensity);
    //for (int i=0; i<10000; i++)
      //etherPacketLoop();
}
