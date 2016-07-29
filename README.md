# Requirements

* Hardware:
    * Any Arduino with ATmega328P MCU (only Nano tested but Uno could be fine)
    * Optiboot bootloader on the Arduino (otherwise watchdog/restart would not work)
    * Sharp GP2Y1010AU0F
    * An 20x4 LCD with I2C backpack
    * DHT21
    * A custom designed LED circuit
    * ESP-12 with the latest AT firmware (only NONOS_SDK 2.0 tested)
* Software:
    * Arduino IDE
    * Libraries:
        * AltSoftSerial
        * Adafruit DHT
        * NewLiquidCrystal

# Compiler flags:
    -Os -mrelax -Wl,-u,vfprintf -lprintf_flt -lm -Wall -Wextra -Wno-unused-parameter