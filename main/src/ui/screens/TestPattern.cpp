// Wiring / orientation check: color bars, a yellow edge, labelled corners.
#include "../Screens.h"
#include "../Gfx.h"

void screens::testPattern()
{
    const int16_t w = tft.width(), h = tft.height();
    const uint16_t bars[] = { ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_WHITE, ST77XX_BLACK };
    for (uint8_t i = 0; i < 5; i++) {
        tft.fillRect(i * (w / 5), 0, w / 5 + 1, h, bars[i]);
    }
    tft.drawRect(0, 0, w, h, ST77XX_YELLOW);   // whole border should be visible

    char size[24];
    snprintf(size, sizeof(size), "%dx%d r%d", w, h, TFT_ROTATION);
    tft.fillRect(0, h / 2 - 12, w, 24, ST77XX_BLACK);
    gfx::textCentered(size, w / 2, h / 2 + 6, theme::FONT_BODY, ST77XX_YELLOW);
    gfx::textLeft("TL", 4, 18, theme::FONT_BODY, ST77XX_YELLOW);
    gfx::textRight("BR", w - 4, h - 6, theme::FONT_BODY, ST77XX_YELLOW);
}
