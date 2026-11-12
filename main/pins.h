// =====================================================================
//  pins.h  --  ONE pin map for the whole e-nose (sensors, display, UI).
//  Every .ino / .cpp should take its pins from here; never hard-code.
//
//  Board: Seeed XIAO nRF52840 Plus (NOT Sense)
//    Front pins:     D0-D10
//    Back pads:      D11-D19 (Plus only)
//    Fixed buses:    I2C  (Wire)  SDA = D4, SCL = D5
//                    SPI          SCK = D8, MISO = D9, MOSI = D10
//                    UART Serial1 TX = D6, RX = D7 (free if unused)
//    On-board QSPI flash (2 MB) uses internal pins -- not on the headers.
// =====================================================================
#ifndef ENOSE_PINS_H
#define ENOSE_PINS_H

#include <Arduino.h>

// ---------------------------------------------------------------------
//  Sensors
// ---------------------------------------------------------------------
// Hydrogen input
#define PIN_H2_ANALOG        A3          // = D3 (H2_Out on schematic)

// I2C Sensors:
// All share the one I2C bus on D4/D5:
//   STC31-C  CO2        0x29 (fixed)
//   SHT41    RH/T       0x44 (fixed)
//   SGP40    ethanol    0x59 (fixed)
//   ADS1115  acetone ADC  -- address set by its ADDR pin:
//            GND = 0x48, VDD = 0x49, SDA = 0x4A, SCL = 0x4B
#define I2C_ADDR_ADS1115     0x48
#define I2C_CLOCK_HZ         400000UL

// ---------------------------------------------------------------------
//  Display  (ST7789 on hardware SPI: SCK = D8, MOSI = D10)
// ---------------------------------------------------------------------
// LCD header JLCD1: MISO (pin 5) also goes to D9; CCS (SD card, pin 10) unused.
#define PIN_TFT_BL           D0          // header pin 11 "Lite"
#define PIN_TFT_DC           D1          // header pin 9 "D/C"
#define PIN_TFT_CS           -1          // header pin 7 "TCS" tied to GND (always selected)
#define PIN_TFT_RST          -1          // not on a GPIO

// For size and color related settings, check the /graphics folder.

// ---------------------------------------------------------------------
//  User interface
// ---------------------------------------------------------------------
#define PIN_BUTTON           D6          // PTS1 to GND, uses INPUT_PULLUP
#define PIN_BUZZER           D2          // U11 CPT-7502 beeper

// ---------------------------------------------------------------------
//  Power
// ---------------------------------------------------------------------
#define PIN_LOAD_SW          D7          // LD_SW_CTRL -- load switch enable

// Free: D11-D19 (back pads)

// ---------------------------------------------------------------------
//  Build-time check: fails compilation if two signals share a pin.
//  Add any new pin to this list. (-1 = not connected, ignored.)
// ---------------------------------------------------------------------
namespace pin_check {
    constexpr int ALL[] = {
        PIN_H2_ANALOG, PIN_TFT_BL, PIN_TFT_DC, PIN_TFT_CS, PIN_TFT_RST,
        PIN_BUTTON, PIN_BUZZER, PIN_LOAD_SW,
        PIN_WIRE_SDA, PIN_WIRE_SCL, PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI
    };
    constexpr int N = sizeof(ALL) / sizeof(ALL[0]);
    constexpr bool dupOf(int i, int j) {
        return j >= N ? false : ((ALL[i] >= 0 && ALL[i] == ALL[j]) || dupOf(i, j + 1));
    }
    constexpr bool anyDup(int i) {
        return i >= N ? false : (dupOf(i, i + 1) || anyDup(i + 1));
    }
    static_assert(!anyDup(0), "pins.h: two signals are assigned the same pin");
}

#endif
