#include "ProgressBar.h"

void ProgressBar::draw(float frac)
{
    gfx::bar(x, y, w, h, frac, fill, track);
}
