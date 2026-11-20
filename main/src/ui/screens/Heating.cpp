// Heating: "Heating..." + ring that fills blue while the sensors warm up.
// Figma: figma/heating_blank.png, figma/heating_partial.png
#include "../Screens.h"
#include "../components/Ring.h"

static Ring heatingRing(120, 146, 76, 10, theme::ACCENT);   // center x/y, radius, thickness

void screens::heating()
{
    gfx::clear();
    gfx::textCentered("Heating...", tft.width() / 2, 61, theme::FONT_BIG, theme::TEXT);
    heatingRing.draw(0.0f);
}

void screens::heatingTick(float frac)
{
    heatingRing.update(frac);
}
