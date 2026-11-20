#include "Button.h"

void Button::begin()
{
    pinMode(pin, INPUT_PULLUP);
    rawLast = stable = (digitalRead(pin) == LOW);
    holdFired = stable;          // already down at boot: ignore this press
    lastChange = millis();
}

void Button::update()
{
    const uint32_t now = millis();
    const bool raw = (digitalRead(pin) == LOW);   // pressed pulls the pin to GND

    if (raw != rawLast) {                         // edge or bounce: restart the timer
        rawLast = raw;
        lastChange = now;
    }

    if (raw != stable && (now - lastChange) >= DEBOUNCE_MS) {
        stable = raw;
        if (stable) {                             // press started
            downSince = now;
            holdFired = false;
        } else if (!holdFired) {                  // released before HOLD_MS
            pressEvent = true;
        }
    }

    if (stable && !holdFired && (now - downSince) >= holdMs) {
        holdFired = true;
        holdEvent = true;
    }
}

float Button::holdProgress() const
{
    if (!stable || holdFired) return 0.0f;
    const float f = (float)(millis() - downSince) / (float)holdMs;
    return f > 1.0f ? 1.0f : f;
}

bool Button::wasPressed()
{
    const bool e = pressEvent;
    pressEvent = false;
    return e;
}

bool Button::wasHeld()
{
    const bool e = holdEvent;
    holdEvent = false;
    return e;
}
