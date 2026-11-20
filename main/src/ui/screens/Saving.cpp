// Saving: "Saving..." + blue bar filling while the sample is written to flash.
// Figma: figma/saving.png
#include "../Screens.h"
#include "../components/ProgressBar.h"

static ProgressBar savingBar(19, 129, 202, 17, theme::ACCENT);

void screens::saving()
{
    gfx::clear();
    gfx::textCentered("Saving...", tft.width() / 2, 104, theme::FONT_BIG, theme::TEXT);
    savingBar.draw(0.0f);
}

void screens::savingTick(float frac)
{
    savingBar.draw(frac);
}
