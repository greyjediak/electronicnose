// Metrics: one row per gas (label, colored bar, yellow arrow + value) and the
// red/orange/green range strip. Figma: figma/metrics1.png, figma/metrics2.png
#include "../Screens.h"
#include "../components/MetricRow.h"
#include "../components/RangeLegend.h"

// Bar scale and color thresholds per gas.
// TODO(sensor team): replace these placeholder ranges with real ones.
//                               name       sub   unit  dec floor  max       warnAt    badAt
static const MetricSpec SPEC_H2      = { "H",       "2",     "ppm", 1, 0.0f, 50.0f,    20.0f,    40.0f    };
static const MetricSpec SPEC_CO2     = { "CO",      "2",     "ppm", 0, 0.0f, 50000.0f, 20000.0f, 40000.0f };
static const MetricSpec SPEC_ACETONE = { "Acetone", nullptr, "mV",  1, 1.0f, 20.0f,    5.0f,     10.0f    };
static const MetricSpec SPEC_ETHANOL = { "Ethanol", nullptr, "ppm", 2, 0.0f, 2.0f,     0.5f,     1.0f     };

// Top of each bar, from Figma.
static MetricRow metricRows[] = {
    MetricRow(SPEC_H2,      60),
    MetricRow(SPEC_CO2,     96),
    MetricRow(SPEC_ACETONE, 131),
    MetricRow(SPEC_ETHANOL, 166),
};
static const uint8_t  METRIC_COUNT      = sizeof(metricRows) / sizeof(metricRows[0]);
static const uint32_t METRIC_REFRESH_MS = 500;
static uint32_t metricLastDraw = 0;

void screens::metrics()
{
    gfx::clear();
    gfx::textCentered("Metrics:", tft.width() / 2, 37, theme::FONT_SMALL, theme::TEXT);
    for (uint8_t i = 0; i < METRIC_COUNT; i++) {
        metricRows[i].drawLabel();
        metricRows[i].drawEmpty();
    }
    rangeLegend::draw(7, 217, 228, 16);
    metricLastDraw = 0;
}

void screens::metricsTick(const Readings& r)
{
    if (metricLastDraw != 0 && millis() - metricLastDraw < METRIC_REFRESH_MS) return;
    metricLastDraw = millis();

    const float values[METRIC_COUNT] = { r.h2Ppm, r.co2Ppm, r.acetoneMv, r.ethanolPpm };
    for (uint8_t i = 0; i < METRIC_COUNT; i++) metricRows[i].update(values[i]);
}
