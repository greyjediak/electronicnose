#include "HydrogenSensor.h"
#include <math.h>

void HydrogenSensor::begin()
{
     // Then we tell our code what the ADC resolution is
    analogReadResolution(ADC_BITS);

    (void)analogRead(ANALOG_PIN); // throw-away read

    // And we print out some basic specs, the reference voltage and bit resolution
    /*Serial.println(F("nRF52840 A0 Max-Voltage (USB Serial Output)"));
    Serial.print(F("ADC bits: "));
    Serial.println(ADC_BITS);
    Serial.print(F("Vref: "));
    Serial.print(VREF_VOLTS, 3);
    Serial.println(F(" V"));
    Serial.println(F("Type 'r' + Enter to reset max.\n"));
    */

    // Prefill the averaging buffer so the average doesn't ramp up from zero
    uint32_t first = (uint32_t)analogRead(ANALOG_PIN);
    for (uint32_t i = 0; i < AVG_SAMPLES; i++)
    {
        samples[i] = first;
    }
    sum = (uint32_t)first * AVG_SAMPLES;
    idx = 0;

    // Calculate initial average voltage
    avgRaw = (float)sum / (float)AVG_SAMPLES;
    avgVolts = rawToVolts(avgRaw);

    // Automatically zero the sensor at startup
    reference = avgVolts;

    // Initial PPM should be zero
    ppm = 0.0f;
 
    Serial.print(F("# H2: ADC bits "));
    Serial.print(ADC_BITS);
    Serial.print(F(", Vref "));
    Serial.print(VREF_VOLTS, 3);
    Serial.println(F(" V. Send 'z' to re-zero the voltage reference."));

    }

void HydrogenSensor::update()
{
    // Sample on a timer instead of delay()
    uint32_t now = millis();
    if (now - lastSampleMs >= SAMPLE_MS)
    {
        lastSampleMs = now;
 
        uint32_t raw = (uint32_t)analogRead(ANALOG_PIN);
 
        // Ring buffer + running sum: O(1) instead of shifting 500 values
        sum -= samples[idx];
        samples[idx] = raw;
        sum += raw;
        idx = (idx + 1) % AVG_SAMPLES;
 
        avgRaw = (float)sum / (float)AVG_SAMPLES;   // float, no truncation
        avgVolts = rawToVolts(avgRaw);
        ppm = VoltsToPPM(avgVolts, reference);
    }
 
    // Serial command: 'z' re-zeros the reference to the current average voltage
    while (Serial.available())
    {
        int c = Serial.read();
        if (c == 'z' || c == 'Z')
        {
            reference = avgVolts;
            ppm = 0.0f;

            Serial.print(F("# H2 Reference Reset: "));
            Serial.print(reference, 4);
            Serial.println(F(" V"));
        }
    }
}
 
float HydrogenSensor::VoltsToPPM(float Volts, float reference) const
{
    return fabsf(Volts - reference) / 0.002f;
}
 
float HydrogenSensor::rawToVolts(float raw) const
{
    if (raw > ADC_MAX) // defensive clamp
        raw = ADC_MAX;
 
    return (raw * VREF_VOLTS) / static_cast<float>(ADC_MAX);
}
