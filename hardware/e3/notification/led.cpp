#include "led.h"

unsigned char ledColorLeft = LED_GREEN, ledColorRight = LED_GREEN;

void ledInit() {
    pinMode(LED_DS, OUTPUT);
    pinMode(LED_STCP, OUTPUT);
    pinMode(LED_SHCP, OUTPUT);
    ledSetColor();
}

void ledSetLeft(const char color) {
    ledColorLeft = color;
}

void ledSetRight(const char color) {
    ledColorRight = color;
}

void ledSetColor() {
    unsigned char color = ~((ledColorLeft << 4) | ledColorRight);
    digitalWrite(LED_STCP, LOW);
    shiftOut(LED_DS, LED_SHCP, MSBFIRST, color);
    digitalWrite(LED_STCP, HIGH);
}

