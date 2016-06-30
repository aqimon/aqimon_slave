#include "commonVariables.h"
#include <EEPROM.h>
#include "config.h"

void eeprom_serial_dump_table(int bytesPerRow = 16) {
  int i, j;
  byte b;
  char buf[10];
  j = 0;
  for (i = 0; i <= 1023; i++) {
    b = EEPROM.read(i);
    sprintf(buf, "%02X ", b);
    j++;
    if (j == bytesPerRow) {
      j = 0;
      Serial.println(buf);
    }
    else {
      Serial.print(buf);
    }
  }
}

void setup() {
  Serial.begin(19200);
//  etherInit();
  lcdInit();
  dustInit();
  dhtInit();
  ledInit();
  wifiInit();
  configRead();

  wifiConnectToAP();
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
