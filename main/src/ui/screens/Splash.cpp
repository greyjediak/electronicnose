// Splash: "E-Nose", "Loading" with animated dots, blue loading bar.
// Figma: figma/enose_splash1..5.png
#include "../Screens.h"
#include "../components/ProgressBar.h"

static const int16_t  SPLASH_TITLE_BASELINE   = 103;
static const int16_t  SPLASH_LOADING_X        = 87;    // left edge, so the dots don't shift the word
static const int16_t  SPLASH_LOADING_BASELINE = 201;
static const uint32_t SPLASH_DOT_MS           = 350;   // time per dot step

static ProgressBar splashBar(36, 212, 168, 10, theme::ACCENT);

static int16_t  splashDotsX   = 0;
static uint8_t  splashDots    = 0;
static uint32_t splashLastDot = 0;

void screens::splash()
{
    gfx::clear();
    gfx::textCentered("E-Nose", tft.width() / 2, SPLASH_TITLE_BASELINE, theme::FONT_BIG, theme::TEXT);
    gfx::textLeft("Loading", SPLASH_LOADING_X, SPLASH_LOADING_BASELINE, theme::FONT_SMALL, theme::TEXT);
    splashDotsX   = tft.getCursorX();
    splashDots    = 0;
    splashLastDot = millis();
    splashBar.draw(0.0f);
}

void screens::splashTick(float frac)
{
    splashBar.draw(frac);

    // "Loading" -> "Loading." -> "Loading.." -> "Loading..." -> repeat
    if (millis() - splashLastDot < SPLASH_DOT_MS) return;
    splashLastDot = millis();
    splashDots = (splashDots + 1) % 4;
    static const char* const DOTS[] = { "", ".", "..", "..." };
    tft.fillRect(splashDotsX, SPLASH_LOADING_BASELINE - 4, 20, 6, theme::BG);
    gfx::textLeft(DOTS[splashDots], splashDotsX, SPLASH_LOADING_BASELINE, theme::FONT_SMALL, theme::TEXT);
}
