// "Get sample from memory" -- placeholder until data storage is settled.
// (No Figma frame yet.)
#include "../Screens.h"
#include "../Gfx.h"

void screens::memory()
{
    gfx::clear();
    const int16_t cx = tft.width() / 2;
    gfx::textCentered("Saved samples", cx, 100, theme::FONT_TITLE, theme::TEXT);
    gfx::textCentered("coming soon", cx, 128, theme::FONT_SMALL, theme::MUTED);
    gfx::textLeft("Hold for 3 seconds to go back", 12, 231, theme::FONT_TINY, theme::MUTED);
}
