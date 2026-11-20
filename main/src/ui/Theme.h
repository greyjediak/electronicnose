#ifndef ENOSE_UI_THEME_H
#define ENOSE_UI_THEME_H

/* Look-and-feel shared by every screen: colors, fonts, button timing.
 * Screen-specific positions live in each screens/<Name>.cpp file. */

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "../../graphics/display_config.h"   // panel settings + TFT pins

namespace theme {

// Converts 8-bit-per-channel RGB (Figma hex) to the panel's 16-bit RGB565.
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// ---------- Colors (from Figma) ----------
constexpr uint16_t BG     = TFT_BG_COLOR;              // #0C0E17, set in display_config.h
constexpr uint16_t TEXT   = rgb(0xF2, 0xF2, 0xF2);
constexpr uint16_t MUTED  = rgb(0xA0, 0xA4, 0xAB);    // hints, small print
constexpr uint16_t ACCENT = rgb(0x5E, 0xA6, 0xE6);    // blue: loading/saving bar, heating ring, menu highlight
constexpr uint16_t TRACK  = rgb(0xD9, 0xD9, 0xD9);    // empty part of bars and rings
constexpr uint16_t GOOD   = rgb(0x49, 0xBA, 0x62);    // green: good / complete
constexpr uint16_t WARN   = rgb(0xE0, 0xA6, 0x3D);    // orange
constexpr uint16_t BAD    = rgb(0x9E, 0x03, 0x03);    // red
constexpr uint16_t MARKER = rgb(0xDE, 0xDC, 0x6D);    // yellow value arrow on metric bars

// ---------- Layout ----------
constexpr int16_t MARGIN = 12;

// ---------- Button ----------
constexpr uint32_t HOLD_MS = 3000;   // hold time to "select" (the menu says 3 seconds)

// ---------- Fonts ----------
// Adafruit GFX FreeSans fonts. FONT_TINY = the built-in 6x8 pixel font.
// Defined once in Gfx.cpp so each font is stored in flash only once.
extern const GFXfont* const FONT_TINY;    // small print, metric values
extern const GFXfont* const FONT_SMALL;   // FreeSans 9pt        -- body text, menu
extern const GFXfont* const FONT_BODY;    // FreeSans Bold 9pt
extern const GFXfont* const FONT_TITLE;   // FreeSans 12pt
extern const GFXfont* const FONT_BIG;     // FreeSans 18pt       -- "E-Nose", "Heating...", "Done!"
extern const GFXfont* const FONT_HUGE;    // FreeSans Bold 24pt  -- countdown number

}  // namespace theme

#endif
