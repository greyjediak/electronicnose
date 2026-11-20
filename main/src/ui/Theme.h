#ifndef ENOSE_UI_THEME_H
#define ENOSE_UI_THEME_H

/* Look-and-feel for every screen: colors, fonts, layout numbers.
 * Colors are eyeballed from the Figma mockups -- replace them with the exact
 * hex values from the Figma inspector: #5DA9E9 -> rgb(0x5D, 0xA9, 0xE9). */

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "../../graphics/display_config.h"   // panel settings + TFT pins

namespace theme {

// Converts 8-bit-per-channel RGB (Figma hex) to the panel's 16-bit RGB565.
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// ---------- Colors ----------
constexpr uint16_t BG     = TFT_BG_COLOR;              // dark navy, set in display_config.h
constexpr uint16_t TEXT   = rgb(0xF2, 0xF2, 0xF2);
constexpr uint16_t MUTED  = rgb(0xA0, 0xA4, 0xAB);    // hints, small print
constexpr uint16_t ACCENT = rgb(0x5D, 0xA9, 0xE9);    // blue: loading bar, heating ring, menu highlight
constexpr uint16_t TRACK  = rgb(0xD9, 0xD9, 0xD9);    // light grey: empty part of bars and rings
constexpr uint16_t GOOD   = rgb(0x4C, 0xB8, 0x5C);    // green
constexpr uint16_t WARN   = rgb(0xE8, 0xA1, 0x3A);    // orange
constexpr uint16_t BAD    = rgb(0xA6, 0x0D, 0x0D);    // red
constexpr uint16_t MARKER = rgb(0xE6, 0xE3, 0x5A);    // yellow value marker on metric bars

// ---------- Layout (pixels) ----------
constexpr int16_t MARGIN   = 12;
constexpr int16_t FOOTER_H = 26;    // room for two lines of small print

constexpr int16_t RING_R     = 72;  // outer radius of the heating / breathe rings
constexpr int16_t RING_THICK = 10;

// Button hold time to "select" (the menu text says 3 seconds).
constexpr uint32_t HOLD_MS = 3000;

// ---------- Fonts ----------
// Adafruit GFX FreeSans fonts. nullptr = the built-in 6x8 pixel font.
// Defined once in Gfx.cpp so each font is stored in flash only once.
extern const GFXfont* const FONT_TINY;    // built-in, for footers and bar values
extern const GFXfont* const FONT_SMALL;   // FreeSans 9pt  -- body text
extern const GFXfont* const FONT_BODY;    // FreeSans Bold 9pt
extern const GFXfont* const FONT_TITLE;   // FreeSans 12pt -- "Heating..."
extern const GFXfont* const FONT_BIG;     // FreeSans 18pt -- "E-Nose", "Done!"
extern const GFXfont* const FONT_HUGE;    // FreeSans Bold 24pt -- countdown number

}  // namespace theme

#endif
