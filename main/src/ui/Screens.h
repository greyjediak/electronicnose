#ifndef ENOSE_UI_SCREENS_H
#define ENOSE_UI_SCREENS_H

/* Every screen of the e-nose UI, one file each in screens/.
 *
 *   name()      full redraw -- call ONCE when the screen is entered
 *   nameTick()  redraws only what moves (bar, ring, numbers) -- safe every loop()
 *
 * Positions come from the Figma references in main/graphics/figma/ and live
 * at the top of each screen's file. Reusable pieces are in components/. */

#include <Arduino.h>

namespace screens {

struct Readings {
    float h2Ppm;
    float co2Ppm;
    float acetoneMv;
    float ethanolPpm;
};

enum MenuItem : uint8_t { MENU_RECORD, MENU_MEMORY, MENU_COUNT };

void testPattern();                              // wiring / orientation check

void splash();                                   // "E-Nose", animated "Loading...", bar
void splashTick(float frac);

void menu(uint8_t selected);                     // question + two options
void menuSelect(uint8_t selected);
void menuHoldTick(float frac);                   // line fills while the button is held

void heating();                                  // "Heating..." + blue ring
void heatingTick(float frac);

void baselining();                               // "Baselining..." + blue ring
void baseliningTick(float frac);

void sensing(uint8_t seconds);                   // "Breathe out for 10s...." + countdown
void sensingTick(uint32_t elapsedMs, uint32_t totalMs);

void metrics();                                  // one bar per gas + range strip
void metricsTick(const Readings& r);

void saving();                                   // "Saving..." + blue bar
void savingTick(float frac);

void saveFailed(float frac);                     // "Save failed" + red bar + hint

void done();                                     // "Done!" + green bar

void memory();                                   // saved samples (placeholder)

}  // namespace screens

#endif
