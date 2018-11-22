#include "lcd.h"

LiquidCrystal_I2C lcd(LCD_I2C_BUS, 2, 1, 0, 4, 5, 6, 7);

void lcdInit() {
    lcd.setBacklightPin(3, POSITIVE);
    lcd.backlight();
    lcd.begin(20, 4);
    lcd.clear();
    Serial.println(F("[info] lcd initialized"));
    lcd.print(F("E3 client v1"));
}

void lcdUpdateTempHumid(float temperature, float humidity) {
    char tempStr[5], humidStr[3];
    dtostrf(temperature, 4, 1, tempStr);
    dtostrf(humidity, 2, 0, humidStr);
    lcd.setCursor(0, 2);
    lcd.print(F("Temp:"));
    lcd.print(tempStr);
    lcd.print(F("C"));
    lcd.print(F(" Humid:"));
    lcd.print(humidStr);
    if (humidity >= 100.0)
        lcd.print(F("%"));
}

void lcdUpdateDustCo(float dustLevel, float coLevel) {
    char dustLevelStr[5], coLevelStr[5];
    dtostrf(dustLevel, 4, 2, dustLevelStr);
    dtostrf(coLevel, 4, 2, coLevelStr);
    lcd.setCursor(0, 3);
    lcd.print(F("Dust: "));
    lcd.print(dustLevelStr);
    lcd.print(F("  CO: "));
    lcd.print(coLevelStr);
}

void lcdUpdateWifiStatus(unsigned char n){
    lcd.setCursor(0, 1);
    lcd.print(F("                    "));
    lcd.setCursor(0, 1);
    switch(n){
        case 1: lcd.print(F("Restarting SIM900"));
        break;
        case 2: lcd.print(F("Waiting for cellular"));
        break;
        case 3: lcd.print(F("Establishing GPRS"));
        break;
        case 4: lcd.print(F("Openning connection"));
        break;
        case 5: lcd.print(F("Sending request"));
        break;
        case 6: lcd.print(F("Send OK"));
        break;
    }
}