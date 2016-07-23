#include <AltSoftSerial.h>
AltSoftSerial wifi(1, 2);

void setup() {
  wifi.begin(74880);
  Serial.begin(74880);
}

void loop() {
  digitalWrite(11, LOW);
  delay(1000);
  digitalWrite(11, HIGH);
  delay(1000);
    while (1) {
        if (wifi.available())
            Serial.write(wifi.read());
        if (Serial.available())
            wifi.write(Serial.read());
    }
}
