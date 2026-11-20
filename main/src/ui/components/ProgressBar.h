#ifndef ENOSE_UI_PROGRESSBAR_H
#define ENOSE_UI_PROGRESSBAR_H

/* Rounded progress bar (loading, saving, done, save failed).
 *   ProgressBar bar(19, 129, 202, 17, theme::ACCENT);
 *   bar.draw(0.4f);     // 40 % filled
 */

#include "../Gfx.h"

class ProgressBar {
public:
    ProgressBar(int16_t x, int16_t y, int16_t w, int16_t h,
                uint16_t fill, uint16_t track = theme::TRACK)
        : x(x), y(y), w(w), h(h), fill(fill), track(track) {}

    void draw(float frac);                        // redraws the whole bar
    void setColor(uint16_t c) { fill = c; }

    const int16_t x, y, w, h;

private:
    uint16_t fill, track;
};

#endif
