#include "Ring.h"

static float toDegrees(float frac)
{
    return 360.0f * constrain(frac, 0.0f, 1.0f);
}

void Ring::draw(float frac)
{
    const float deg = toDegrees(frac);
    if (deg < 360.0f) gfx::ringSegment(cx, cy, r, thick, deg, 360.0f, track, fill, fill);
    if (deg > 0.0f)   gfx::ringSegment(cx, cy, r, thick, 0.0f, deg, fill, track, track);
    shownDeg = deg;
}

void Ring::update(float frac)
{
    const float deg = toDegrees(frac);
    if (fabsf(deg - shownDeg) < 1.0f) return;     // nothing visible changed

    if (deg > shownDeg) {   // grow: paint fill over the new slice
        gfx::ringSegment(cx, cy, r, thick, shownDeg, deg, fill, fill, track);
    } else {                // shrink: paint track back over the slice
        gfx::ringSegment(cx, cy, r, thick, deg, shownDeg, track, fill, track);
    }
    shownDeg = deg;
}
