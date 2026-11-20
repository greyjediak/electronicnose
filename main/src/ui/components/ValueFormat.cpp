#include "ValueFormat.h"

namespace fmt {

// 1 -> "1", 0.5 -> "0.5"
static String plain(float v)
{
    return (v == (float)(long)v) ? String((long)v) : String(v, 2);
}

// 30000 -> "30,000"
static String withCommas(long v)
{
    String digits = String(v < 0 ? -v : v);
    String out;
    for (int i = 0; i < (int)digits.length(); i++) {
        if (i > 0 && (digits.length() - i) % 3 == 0) out += ',';
        out += digits[i];
    }
    return v < 0 ? "-" + out : out;
}

void reading(float value, uint8_t decimals, float floor, const char* unit, char* out, size_t n)
{
    String num;
    if (floor > 0.0f && value < floor) {
        num = "< " + plain(floor);
    } else if (fabsf(value) >= COMPACT_FROM) {
        num = withCommas(lroundf(value / 1000.0f)) + "k";
    } else if (fabsf(value) >= 1000.0f) {
        num = withCommas(lroundf(value));
    } else {
        num = String(value, (unsigned int)decimals);
    }
    snprintf(out, n, "%s %s", num.c_str(), unit);
}

}  // namespace fmt
