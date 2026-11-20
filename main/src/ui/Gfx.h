#ifndef ENOSE_UI_GFX_H
#define ENOSE_UI_GFX_H

/* The TFT itself plus low-level drawing shared by every component and screen.
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
int16_t textWidth(const char* s, const GFXfont* f);

// ---------- Smooth (anti-aliased) shapes ----------
// Mixes two colors: t = 0 gives `a`, t = 1 gives `b`.
uint16_t blend(uint16_t a, uint16_t b, float t);

// Rounded bar (pill): the first `frac` (0..1) in `fill`, the rest in `track`.
// Redraws the whole bar rectangle, so it also erases whatever was there.
void bar(int16_t x, int16_t y, int16_t w, int16_t h, float frac,
         uint16_t fill, uint16_t track, uint16_t bg = theme::BG);

// Thick ring segment centered on (cx, cy). Angles in degrees, 0 = 12 o'clock,
// clockwise. `beforeColor` / `afterColor` are what the ring already shows just
// before `fromDeg` / after `toDeg`, so the ends blend smoothly into it.
void ringSegment(int16_t cx, int16_t cy, int16_t r, int16_t thick,
                 float fromDeg, float toDeg, uint16_t color,
                 uint16_t beforeColor, uint16_t afterColor, uint16_t bg = theme::BG);

// ---------- Figma images (optional) ----------
// Draws assets/<name>.png at its screen position if it was converted into
// images.h by tools/png2rgb565.py. Returns false if there is no such image.
bool image(const char* name);
bool hasImage(const char* name);

}  // namespace gfx

#endif
