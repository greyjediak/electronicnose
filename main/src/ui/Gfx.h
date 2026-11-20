#ifndef ENOSE_UI_GFX_H
#define ENOSE_UI_GFX_H

/* The TFT itself plus small drawing helpers shared by every screen.
 * Text y-positions are the BASELINE (bottom of letters like "a"), which is
 * how Adafruit GFX custom fonts work. */

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "Theme.h"

extern Adafruit_ST7789 tft;

namespace gfx {

void begin();
void clear();
void backlight(bool on);

// ---------- Text ----------
void textLeft    (const char* s, int16_t x,     int16_t baseline, const GFXfont* f, uint16_t color);
void textCentered(const char* s, int16_t cx,    int16_t baseline, const GFXfont* f, uint16_t color);
void textRight   (const char* s, int16_t right, int16_t baseline, const GFXfont* f, uint16_t color);
// Centers text inside a box both ways.
void textInBox   (const char* s, int16_t x, int16_t y, int16_t w, int16_t h,
                  const GFXfont* f, uint16_t color);
int16_t textWidth(const char* s, const GFXfont* f);

// ---------- Building blocks ----------
// Small print at the bottom of the screen (line2 optional).
void footer(const char* line1, const char* line2 = nullptr);

// Rounded progress bar: `frac` (0..1) of it in `fill`, the rest in `track`.
void bar(int16_t x, int16_t y, int16_t w, int16_t h, float frac, uint16_t fill, uint16_t track);

// Thick ring segment. Angles in degrees, 0 = 12 o'clock, increasing clockwise.
void arc(int16_t cx, int16_t cy, int16_t r, int16_t thick,
         float fromDeg, float toDeg, uint16_t color);

// ---------- Figma images ----------
// Draws assets/<name>.png (centered) if it was converted into images.h.
// Returns false if there is no such image, so the screen can draw itself in code.
bool image(const char* name);

}  // namespace gfx

#endif
