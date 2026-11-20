#ifndef ENOSE_UI_RANGELEGEND_H
#define ENOSE_UI_RANGELEGEND_H

/* The red | orange | green | orange | red strip along the bottom of the
 * metrics screen. Segment widths are in RangeLegend.cpp (from Figma). */

#include "../Gfx.h"

namespace rangeLegend {
void draw(int16_t x, int16_t y, int16_t w, int16_t h);
}

#endif
