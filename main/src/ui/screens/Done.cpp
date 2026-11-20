// Done: "Done!" + full green bar. Figma: figma/done.png
#include "../Screens.h"
#include "../components/ProgressBar.h"

static ProgressBar doneBar(19, 129, 202, 17, theme::GOOD);

void screens::done()
{
    gfx::clear();
    gfx::textCentered("Done!", tft.width() / 2, 104, theme::FONT_BIG, theme::TEXT);
    doneBar.draw(1.0f);
}
