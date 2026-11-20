#ifndef ENOSE_UI_MENULIST_H
#define ENOSE_UI_MENULIST_H

/* Vertical list of options; the selected one sits on a full-width blue bar.
 *   static const char* const ITEMS[] = { "Record breath sample", "Get sample from memory" };
 *   MenuList list(ITEMS, 2, 94, 27, 28);
 *   list.draw(0);            // once
 *   list.select(1);          // on click: redraws only the two rows that changed
 *   list.holdProgress(0.5);  // while the button is held: line under the selection fills */

#include "../Gfx.h"

class MenuList {
public:
    MenuList(const char* const* labels, uint8_t count,
             int16_t firstRowY, int16_t rowPitch, int16_t textX)
        : labels(labels), count(count), firstRowY(firstRowY), rowPitch(rowPitch), textX(textX) {}

    void draw(uint8_t selected);
    void select(uint8_t selected);
    void holdProgress(float frac);

    static const int16_t ROW_H = 19;   // height of the blue highlight (Figma)

private:
    void drawRow(uint8_t i, bool highlighted);
    int16_t rowY(uint8_t i) const { return firstRowY + i * rowPitch; }

    const char* const* labels;
    uint8_t count;
    int16_t firstRowY, rowPitch, textX;
    uint8_t current = 0;
    int16_t holdShown = -1;   // pixels of the hold line already drawn
};

#endif
