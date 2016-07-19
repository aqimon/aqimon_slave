#include "lcd.h"

LiquidCrystal_I2C lcd(LCD_I2C_BUS, 2, 1, 0, 4, 5, 6, 7);

void lcdInit() {
    lcd.setBacklightPin(3, POSITIVE);
    lcd.backlight();
    lcd.begin(20, 4);
    lcd.clear();
    Serial.println(F("[info] lcd initialized"));
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
    lcd.print("%");
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

void lcdWrite(char* msg) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(msg);
}

void lcdUpdateWifiStatus(unsigned char status){
    lcd.setCursor(0, 1);
    switch (status){
        case WIFI_STARTING:
            lcd.print(F("Wifi starting       "));
            break;
        case WIFI_CONNECTING_AP:
            lcd.print(F("Connecting to AP    "));
            break;
        case WIFI_CONNECTING_HTTP:
            lcd.print(F("Connecting to server"));
            break;
        case WIFI_SENDING:
            lcd.print(F("Sending request     "));
            break;
        case WIFI_SENDOK:
            lcd.print(F("Send successful     "));
            break;
        case WIFI_RESET:
            lcd.print(F("Got error, resetting"));
            break;
        case WIFI_TIMEOUT:
            lcd.print(F("Timeout             "));
            break;
        case WIFI_SERVER:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(F("Cfg server started  "));
            lcd.setCursor(0, 1);
            lcd.print(F("SSID: MakerWeek     "));
            lcd.setCursor(0, 2);
            lcd.print(F("Cfg IP: 192.168.69.1"));
            break;
    }
}

void lcdClear() {
    lcd.clear();
}