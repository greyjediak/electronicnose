#include "RangeLegend.h"

void rangeLegend::draw(int16_t x, int16_t y, int16_t w, int16_t h)
{
    // Inside widths from Figma (228 px strip): 15 | 20 | 156 | 20 | 15, as fractions.
    const float    parts[]  = { 15, 20, 156, 20, 15 };
    const uint16_t colors[] = { theme::BAD, theme::WARN, theme::GOOD, theme::WARN, theme::BAD };
    const float    total    = 226.0f;

    tft.drawRect(x, y, w, h, theme::TRACK);                 // 1 px light border
    const int16_t ix = x + 1, iw = w - 2;
    int16_t px = ix;
    for (uint8_t i = 0; i < 5; i++) {
        const int16_t pw = (i == 4) ? (ix + iw - px) : (int16_t)lroundf(iw * parts[i] / total);
        tft.fillRect(px, y + 1, pw, h - 2, colors[i]);
        px += pw;
    }
}
