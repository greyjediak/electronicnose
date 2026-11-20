#include "MetricRow.h"
#include "ValueFormat.h"

void MetricRow::drawLabel()
{
    const int16_t baseline = barTop + 8;
    gfx::textLeft(spec.name, LABEL_X, baseline, theme::FONT_SMALL, theme::TEXT);
    if (spec.sub) {
        gfx::textLeft(spec.sub, tft.getCursorX() + 1, baseline + 3, theme::FONT_TINY, theme::TEXT);
    }
    gfx::textLeft(":", tft.getCursorX() + 1, baseline, theme::FONT_SMALL, theme::TEXT);
}

void MetricRow::eraseAbove()
{
    // Strip above the bar that holds the arrow and the value text (the label
    // starts below this strip, so it isn't touched).
    tft.fillRect(0, barTop - 11, tft.width(), 10, theme::BG);
}

void MetricRow::drawEmpty()
{
    eraseAbove();
    gfx::bar(BAR_X, barTop, BAR_W, BAR_H, 0.0f, theme::TRACK, theme::TRACK);
}

void MetricRow::update(float value)
{
    const float frac = constrain(value / spec.maxValue, 0.0f, 1.0f);
    const uint16_t color = value >= spec.badAt  ? theme::BAD
                         : value >= spec.warnAt ? theme::WARN
                                                : theme::GOOD;
    eraseAbove();
    gfx::bar(BAR_X, barTop, BAR_W, BAR_H, frac, color, theme::TRACK);

    // Yellow arrow (9 x 7 px) pointing down at the end of the fill.
    const int16_t mx = BAR_X + (int16_t)(BAR_W * frac);
    tft.fillTriangle(mx - 4, barTop - 8, mx + 4, barTop - 8, mx, barTop - 2, theme::MARKER);

    // Value to the right of the arrow, or to its left if it would run off the screen.
    char text[24];
    fmt::reading(value, spec.decimals, spec.floor, spec.unit, text, sizeof(text));
    const int16_t tw = gfx::textWidth(text, theme::FONT_TINY);
    if (mx + 8 + tw <= tft.width() - 2) gfx::textLeft (text, mx + 8,  barTop - 3, theme::FONT_TINY, theme::TEXT);
    else                                gfx::textRight(text, mx - 10, barTop - 3, theme::FONT_TINY, theme::TEXT);
}
