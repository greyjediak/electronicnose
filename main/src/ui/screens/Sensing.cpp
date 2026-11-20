// Sensing: "Breathe out for 10s...." + green ring that empties while the
// number counts down. Figma: figma/breathe_start.png, figma/breathe_partial.png
#include "../Screens.h"
#include "../components/Ring.h"

static Ring    senseRing(120, 146, 76, 10, theme::GOOD);
static int32_t senseShownSec = -1;

void screens::sensing(uint8_t seconds)
{
    gfx::clear();
    char caption[32];
    snprintf(caption, sizeof(caption), "Breathe out for %us....", seconds);
    gfx::textCentered(caption, tft.width() / 2, 51, theme::FONT_SMALL, theme::TEXT);
    senseRing.draw(1.0f);
    senseShownSec = -1;
}

void screens::sensingTick(uint32_t elapsedMs, uint32_t totalMs)
{
    if (elapsedMs > totalMs) elapsedMs = totalMs;
    senseRing.update(1.0f - (float)elapsedMs / (float)totalMs);

    // Seconds left, rounded up so it shows 10 ... 1. Only redrawn when it changes.
    const int32_t left = (int32_t)((totalMs - elapsedMs + 999) / 1000);
    if (left == senseShownSec) return;
    senseShownSec = left;
    char num[6];
    snprintf(num, sizeof(num), "%ld", (long)left);
    tft.fillRect(senseRing.cx - 45, senseRing.cy - 30, 90, 60, theme::BG);
    gfx::textCentered(num, senseRing.cx, senseRing.cy + 17, theme::FONT_HUGE, theme::TEXT);
}
