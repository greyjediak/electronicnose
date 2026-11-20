#include "HydrogenSensor.h"
#include <math.h>

void HydrogenSensor::begin()
{
    // To set up, first we set up our baud rate for the monitor where we will display our readings
    Serial.begin(115200);

    // Then we tell our code what the ADC resolution is
    analogReadResolution(ADC_BITS);

    (void)analogRead(ANALOG_PIN); // throw-away read

    // And we print out some basic specs, the reference voltage and bit resolution
    Serial.println(F("nRF52840 A0 Max-Voltage (USB Serial Output)"));
    Serial.print(F("ADC bits: "));
    Serial.println(ADC_BITS);
    Serial.print(F("Vref: "));
    Serial.print(VREF_VOLTS, 3);
    Serial.println(F(" V"));
    Serial.println(F("Type 'r' + Enter to reset max.\n"));
}

void HydrogenSensor::update()
{
    // Read and update max
    int count = 0;
    float avg = 0.0;
    // while(count < 500){
    uint32_t raw = (uint32_t)analogRead(ANALOG_PIN);
    // if (raw > maxRaw) maxRaw = raw;
    //  maxRaw = raw;
    //  avg += static_cast<float>(raw);
    //  count++;
    //  delay(2);
    //}
    uint32_t oldarray[runtime];
    for (int i = 0; i < runtime; i++)
    {
        oldarray[i] = arrayforavg[i];
    }
    for (int i = 0; i < runtime; i++)
    {
        if (i == 0)
            arrayforavg[i] = raw;
        else
            arrayforavg[i] = oldarray[i - 1];
    }
    for (int i = 0; i < runtime; i++)
    {
        avg += arrayforavg[i];
    }
    rawaverage = avg / 500.0;
    maxRaw = rawaverage;
    // Print periodically to the serial monitor
    uint32_t now = millis();
    // IF the another period has expired
    if (now - lastPrintMs >= PRINT_PERIOD_MS)
    {
        // THEN we reset the period
        lastPrintMs = now;
        // convert our readings to volts and ppm
        float maxVolts = rawToVolts(maxRaw);
        float ppm = VoltsToPPM(maxVolts, reference);
        ppm = 1.0 * ppm;
        // Then output them to the monitor
        Serial.print(F("Raw: "));
        Serial.print(maxRaw);
        Serial.print(F(" | Volts: "));
        Serial.print(maxVolts, 4);
        Serial.print(F(" V"));
        Serial.print(F(" | PPM: "));
        Serial.print(ppm, 4);
        Serial.println(F(" PPM"));
    }

    while (Serial.available()) {
    int c = Serial.read();
    if (c == 'r' || c == 'R') {
      maxRaw = 0;
      Serial.println(F("[Max reset]"));
    }
    if (c == 'z' || c == 'Z') {
      float newreference = maxRaw;
      newreference = rawToVolts(newreference);
      reference = newreference;
      Serial.println(F("[Voltage reference reset]"));
    }
  }

  // Delay just for good code hygiene, don't wanna overclock our poor mcu
  delay(2);
}

float HydrogenSensor::VoltsToPPM(float Volts, float reference) {
    return(fabs(Volts - reference)/0.002);
}

float HydrogenSensor::rawToVolts(float raw) {
    if (raw > ADC_MAX) //defensive clamp
        raw = ADC_MAX;

        return (static_cast<float>(raw) * VREF_VOLTS) / static_cast<float>(ADC_MAX);
}