#include <AltSoftSerial.h>
AltSoftSerial wifi(1, 2);

void setup() {
  wifi.begin(38400);
  Serial.begin(38400);
}

void loop() {
    while (1) {
        if (wifi.available())
            Serial.write(wifi.read());
        if (Serial.available())
            wifi.write(Serial.read());
    }
}
