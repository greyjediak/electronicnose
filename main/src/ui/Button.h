#ifndef ENOSE_UI_BUTTON_H
#define ENOSE_UI_BUTTON_H

/* Push button to GND (INPUT_PULLUP) with debounce. Reports, once each:
 *   wasPressed() -- short press, fired on release
 *   wasHeld()    -- long press, fired as soon as holdMs is reached
 * Call update() at the top of every loop(). */

#include <Arduino.h>

class Button {
public:
    explicit Button(int pin, uint32_t holdMs = 3000) : pin(pin), holdMs(holdMs) {}
    void begin();
    void update();
    bool isDown() const { return stable; }
    // 0..1 while the button is down: how close the press is to counting as a hold.
    float holdProgress() const;

    bool wasPressed();   // reading clears the event
    bool wasHeld();

private:
    static const uint32_t DEBOUNCE_MS = 25;

    int      pin;
    uint32_t holdMs;
    bool     rawLast    = false;   // true = down
    bool     stable     = false;   // debounced state
    uint32_t lastChange = 0;
    uint32_t downSince  = 0;
    bool     holdFired  = false;   // hold already reported for this press
    bool     pressEvent = false;
    bool     holdEvent  = false;
};

#endif
