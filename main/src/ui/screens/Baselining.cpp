// Baselining: same layout as Heating, while the clean-air baseline is collected.
// (No Figma frame yet -- copies heating.)
#include "../Screens.h"
#include "../components/Ring.h"

static Ring baselineRing(120, 146, 76, 10, theme::ACCENT);

void screens::baselining()
{
    gfx::clear();
    gfx::textCentered("Baselining...", tft.width() / 2, 61, theme::FONT_BIG, theme::TEXT);
    baselineRing.draw(0.0f);
}

void screens::baseliningTick(float frac)
{
    baselineRing.update(frac);
}
