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
// Hydrogen: SGX-H2-1000 analog output
#define PIN_H2_ANALOG        A3          // = A3

// All of these share the one I2C bus on D4/D5:
//   STC31-C  CO2        0x29 (fixed)
//   SHT41    RH/T       0x44 (fixed)
//   SGP40    ethanol    0x59 (fixed)
//   ADS1115  acetone ADC  -- address set by its ADDR pin:
//            GND = 0x48, VDD = 0x49, SDA = 0x4A, SCL = 0x4B
#define I2C_ADDR_ADS1115     0x48
#define I2C_CLOCK_HZ         400000UL

// ---------------------------------------------------------------------
//  Display  (ST7789 on hardware SPI: SCK = D8, MOSI = D10)
//  Panel settings (size, rotation, colors) are in graphics/display_config.h
// ---------------------------------------------------------------------
#define PIN_TFT_CS           D1
#define PIN_TFT_DC           D2
#define PIN_TFT_RST          -1          // tied to reset; set a pin (e.g. D3) if wired
#define PIN_TFT_BL           D0          

// ---------------------------------------------------------------------
//  User interface  (placeholders -- update to match the wiring)
// ---------------------------------------------------------------------
#define PIN_BUTTON           D6          // to GND, uses INPUT_PULLUP
#define PIN_BUZZER           D2

// Free: D7, D9, D11-D19 (back pads)

#endif
