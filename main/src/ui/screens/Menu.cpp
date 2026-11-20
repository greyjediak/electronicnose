// Main menu: question, two options on a highlight bar, hint at the bottom.
// Figma: figma/menu1.png, figma/menu2.png
#include "../Screens.h"
#include "../components/MenuList.h"

static const char* const MENU_ITEMS[screens::MENU_COUNT] = {
    "Record breath sample",
    "Get sample from memory",
};

static MenuList menuList(MENU_ITEMS, screens::MENU_COUNT,
                         94,    // top of the first highlight row
                         27,    // distance between rows
                         28);   // text x

void screens::menu(uint8_t selected)
{
    gfx::clear();
    gfx::textLeft("What do you want to do", 20, 52, theme::FONT_SMALL, theme::TEXT);
    gfx::textLeft("today?",                 20, 72, theme::FONT_SMALL, theme::TEXT);
    menuList.draw(selected);
    gfx::textLeft("Click once to navigate, hold for 3", 12, 217, theme::FONT_TINY, theme::TEXT);
    gfx::textLeft("seconds to select option",           12, 231, theme::FONT_TINY, theme::TEXT);
}

void screens::menuSelect(uint8_t selected)
{
    menuList.select(selected);
}

void screens::menuHoldTick(float frac)
{
    menuList.holdProgress(frac);
}
