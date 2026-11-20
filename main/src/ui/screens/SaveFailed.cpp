// Save failed: "Save failed" + red bar (how far it got) + what to do.
// Figma: figma/save_fail.png
#include "../Screens.h"
#include "../components/ProgressBar.h"

static ProgressBar failBar(19, 129, 202, 17, theme::BAD);

void screens::saveFailed(float frac)
{
    gfx::clear();
    gfx::textCentered("Save failed", tft.width() / 2, 104, theme::FONT_BIG, theme::TEXT);
    failBar.draw(frac);
    gfx::textLeft("Connect to device, clear out memory.", 19, 189, theme::FONT_TINY, theme::TEXT);
}
