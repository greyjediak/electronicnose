#include "MenuList.h"

void MenuList::draw(uint8_t selected)
{
    current = selected;
    holdShown = -1;
    for (uint8_t i = 0; i < count; i++) drawRow(i, i == selected);
}

void MenuList::select(uint8_t selected)
{
    if (selected == current) return;
    drawRow(current, false);
    drawRow(selected, true);
    current = selected;
    holdShown = -1;
}

void MenuList::drawRow(uint8_t i, bool highlighted)
{
    tft.fillRect(0, rowY(i), tft.width(), ROW_H, highlighted ? theme::ACCENT : theme::BG);
    gfx::textLeft(labels[i], textX, rowY(i) + 15, theme::FONT_SMALL, theme::TEXT);
}

void MenuList::holdProgress(float frac)
{
    const int16_t w = tft.width();
    const int16_t filled = (int16_t)(w * constrain(frac, 0.0f, 1.0f));
    if (filled == holdShown) return;
    const int16_t y = rowY(current) + ROW_H - 2;            // 2 px line along the bottom of the highlight
    tft.fillRect(0, y, filled, 2, theme::TEXT);
    tft.fillRect(filled, y, w - filled, 2, theme::ACCENT);
    holdShown = filled;
}
