#ifndef ENOSE_UI_METRICROW_H
#define ENOSE_UI_METRICROW_H

/* One gas on the metrics screen:  "H2:"  [=====----]  with a yellow arrow
 * above the bar pointing at the value, and the value written beside it.
 *
 *   static const MetricSpec H2 = { "H", "2", "ppm", 1, 0.0f, 50.0f, 20.0f, 40.0f };
 *   MetricRow row(H2, 60);   // 60 = top of the bar
 *   row.drawLabel();  row.drawEmpty();   // once
 *   row.update(7.2f);                     // whenever a new reading arrives */

#include "../Gfx.h"

struct MetricSpec {
    const char* name;      // "H"
    const char* sub;       // small lowered digit, e.g. "2" for H2 (nullptr = none)
    const char* unit;      // "ppm"
    uint8_t     decimals;  // digits after the point
    float       floor;     // below this, show "< floor" (0 = never)
    float       maxValue;  // value that fills the bar completely
    float       warnAt;    // bar turns orange at/above this
    float       badAt;     // bar turns red at/above this
};

class MetricRow {
public:
    MetricRow(const MetricSpec& spec, int16_t barTop) : spec(spec), barTop(barTop) {}

    void drawLabel();
    void drawEmpty();          // grey bar, no arrow or value
    void update(float value);

    // Figma geometry shared by every row
    static const int16_t LABEL_X = 21;
    static const int16_t BAR_X   = 107;
    static const int16_t BAR_W   = 100;
    static const int16_t BAR_H   = 11;

private:
    void eraseAbove();
    const MetricSpec& spec;
    const int16_t barTop;
};

#endif
