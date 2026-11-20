// =====================================================================
//  graphics.ino -- UI test harness: LCD + button only, fake data and timers.
//
//  The screens live in main/src/ui (shared with main.ino); this sketch
//  walks through them in the Figma order:
//    splash -> menu --hold "Record"--> heating -> baselining -> sensing (10 s)
//           -> metrics --click--> done --click--> menu
//    menu --hold "Get sample from memory"--> memory --hold--> menu
//  click = next option / continue      hold 3 s = select / cancel to menu
//  Hold the button while powering up to get the wiring test pattern.
//
//  Panel settings: display_config.h   Pins: ../pins.h
//  Figma images:   assets/*.png -> tools/png2rgb565.py -> images.h
// =====================================================================
#include "../src/ui/Gfx.h"
#include "../src/ui/Screens.h"
#include "../src/ui/Button.h"

// Declared before any function so Arduino's auto-generated prototypes see it.
enum Demo : uint8_t { D_MENU, D_HEATING, D_BASELINING, D_SENSING, D_METRICS, D_DONE, D_MEMORY };

static const uint32_t FAKE_LOADING_MS    = 2000;
static const uint32_t FAKE_HEATING_MS    = 6000;
static const uint32_t FAKE_BASELINING_MS = 4000;
static const uint32_t SENSING_MS         = 10000;

Button   button(PIN_BUTTON, theme::HOLD_MS);
Demo     demo      = D_MENU;
uint8_t  menuIndex = screens::MENU_RECORD;
uint32_t enteredAt = 0;

uint32_t elapsed() { return millis() - enteredAt; }

void enter(Demo d)
{
    demo = d;
    enteredAt = millis();
    switch (d) {
        case D_MENU:       screens::menu(menuIndex);                               break;
        case D_HEATING:    screens::progressRing("Heating...", "heating");          break;
        case D_BASELINING: screens::progressRing("Baselining...", "baselining");    break;
        case D_SENSING:    screens::sensing(SENSING_MS / 1000);                     break;
        case D_METRICS:    screens::metrics();                                      break;
        case D_DONE:       screens::done();                                         break;
        case D_MEMORY:     screens::memory();                                       break;
    }
    Serial.print("screen ");
    Serial.println((int)d);
}

// Slowly wandering numbers so the metrics screen has something to show.
screens::Readings fakeReadings()
{
    const float t = millis() / 1000.0f;
    return { 7.2f + 4.0f * sinf(t * 0.7f), 30000.0f + 12000.0f * sinf(t * 0.4f),
             4.0f + 3.0f * sinf(t * 0.3f), 0.30f + 0.08f * sinf(t * 0.5f) };
}

void setup()
{
    Serial.begin(115200);
    gfx::begin();
    button.begin();

    if (button.isDown()) {                       // held at power-up: wiring test
        screens::testPattern();
        while (digitalRead(PIN_BUTTON) == LOW) delay(10);
        delay(3000);
    }

    screens::splash();
    const uint32_t t0 = millis();
    while (millis() - t0 < FAKE_LOADING_MS) {
        screens::splashProgress((float)(millis() - t0) / FAKE_LOADING_MS);
        delay(30);
    }
    screens::splashProgress(1.0f);
    enter(D_MENU);
}

void loop()
{
    button.update();
    const bool click = button.wasPressed();
    const bool hold  = button.wasHeld();

    // Hold = select in the menu, cancel back to the menu everywhere else.
    if (hold) {
        if (demo == D_MENU) enter(menuIndex == screens::MENU_RECORD ? D_HEATING : D_MEMORY);
        else                enter(D_MENU);
        return;
    }

    switch (demo) {
        case D_MENU:
            if (click) {
                menuIndex = (menuIndex + 1) % screens::MENU_COUNT;
                screens::menuSelect(menuIndex);
            }
            screens::menuHoldTick(menuIndex, button.holdProgress());
            break;

        case D_HEATING:
            screens::progressRingTick((float)elapsed() / FAKE_HEATING_MS);
            if (elapsed() >= FAKE_HEATING_MS) enter(D_BASELINING);
            break;

        case D_BASELINING:
            screens::progressRingTick((float)elapsed() / FAKE_BASELINING_MS);
            if (elapsed() >= FAKE_BASELINING_MS) enter(D_SENSING);
            break;

        case D_SENSING:
            screens::sensingTick(elapsed(), SENSING_MS);
            if (elapsed() >= SENSING_MS) enter(D_METRICS);
            break;

        case D_METRICS:
            screens::metricsTick(fakeReadings());
            if (click) enter(D_DONE);
            break;

        case D_DONE:
            if (click) enter(D_MENU);
            break;

        case D_MEMORY:
            break;
    }
}
