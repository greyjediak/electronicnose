// =====================================================================
//  display_config.h  --  display driver + panel settings.
//  Pins come from main/pins.h (the project-wide pin map).
//
//  Board: Seeed XIAO nRF52840 Plus (NOT Sense)
//  Panel: ST7789 SPI TFT (Adafruit_ST7789 driver)
// =====================================================================
#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

// ---------- Panel geometry ----------
// Native (portrait) resolution of the panel. Common ST7789 sizes:
//   240x240 (1.3"/1.54"), 240x280 (1.69"), 240x320 (2.0"), 170x320 (1.9")
#define TFT_WIDTH        240
#define TFT_HEIGHT       240
#define TFT_ROTATION     0  // 0 = portrait, 1 = landscape, 2 = portrait flipped, 3 = landscape flipped.
#define TFT_INVERT       1
#define TFT_SPI_HZ       8000000UL  // SPI clock. Drop to 8000000 if you see garbage/streaks (long wires).

#define TFT_SPI_MODE     SPI_MODE3

// ---------- Pins ----------
// Pins live in the shared project pin map: main/pins.h (PIN_TFT_*).
#include "../pins.h"
#define TFT_CS           PIN_TFT_CS
#define TFT_DC           PIN_TFT_DC
#define TFT_RST          PIN_TFT_RST
#define TFT_BL           PIN_TFT_BL

// ---------- Theme ----------
// Background used when clearing the screen. RGB565 (0x0000 = black).
// Should match the background color of Figma frames.
// 0x0862 = Figma background #0C0E17. Run png2rgb565.py with --bg 0C0E17
// so transparent pixels in exports blend into the same color.
#define TFT_BG_COLOR     0x0862

#endif
