#ifndef ENOSE_UI_RING_H
#define ENOSE_UI_RING_H

/* Progress ring (heating, baselining, breathe countdown).
 * Fills clockwise from 12 o'clock; the rest is grey track.
 *   Ring ring(120, 146, 76, 10, theme::ACCENT);
 *   ring.draw(0.0f);        // once, when the screen opens
 *   ring.update(0.35f);     // every loop: only the changed slice is redrawn
 * update() works in both directions, so a countdown can go 1.0 -> 0.0. */

#include "../Gfx.h"

class Ring {
public:
    Ring(int16_t cx, int16_t cy, int16_t r, int16_t thick,
         uint16_t fill, uint16_t track = theme::TRACK)
        : cx(cx), cy(cy), r(r), thick(thick), fill(fill), track(track) {}

    void draw(float frac);
    void update(float frac);

    const int16_t cx, cy, r, thick;

private:
    uint16_t fill, track;
    float shownDeg = 0.0f;   // how much of the ring is currently filled
};

#endif
