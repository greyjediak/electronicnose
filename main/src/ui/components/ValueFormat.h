#ifndef ENOSE_UI_VALUEFORMAT_H
#define ENOSE_UI_VALUEFORMAT_H

/* How sensor numbers are written on screen -- one place to decide the policy.
 *
 *   7.2      -> "7.2 ppm"
 *   30000    -> "30,000 ppm"     (thousands separator)
 *   250000   -> "250k ppm"       (at/above COMPACT_FROM)
 *   0.4, floor 1 -> "< 1 ppm"    (below what the sensor can really detect)
 *
 * TODO(team): decide how large numbers should look, then change the rules here
 * (e.g. raise/lower COMPACT_FROM, switch to %, clamp to a max like "> 50,000"). */

#include <Arduino.h>

namespace fmt {

constexpr float COMPACT_FROM = 100000.0f;   // numbers this big are shortened to "123k"

// value: the reading; decimals: digits after the point; floor: show "< floor"
// below this (0 = never); unit: e.g. "ppm". Writes into out (n bytes).
void reading(float value, uint8_t decimals, float floor, const char* unit, char* out, size_t n);

}  // namespace fmt

#endif
