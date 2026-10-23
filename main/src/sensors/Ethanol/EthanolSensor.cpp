#include "EthanolSensor.h"
#include <Wire.h>
#include <math.h>

void EthanolSensor::begin()
{
    sgp40.begin(Wire);

    uint16_t serialNumber[3];
    uint8_t serialNumberSize = 3;

    error = sgp40.getSerialNumber(serialNumber, serialNumberSize);

    if (error)
    {
        Serial.print("# Error trying to execute getSerialNumber(): ");
    }
    else {
        Serial.print("# SGP40 SerialNumber: 0x");
        for (size_t i = 0; i < serialNumberSize; i++)
        {
            uint16_t value = serialNumber[i];
            Serial.print(value < 4096 ? "0" : "");
            Serial.print(value < 256 ? "0" : "");
            Serial.print(value < 16 ? "0" : "");
            Serial.print(value, HEX);
        }
        Serial.println();
    }

    uint16_t testResult;

    error = sgp40.executeSelfTest(testResult);
    if (error)
    {
        Serial.print("# Error trying to execute executeSelfTest(): ");
    }
    else if (testResult != 0xD400)
    {
        Serial.print("# executeSelfTest failed with error: ");
        Serial.println(testResult);
    }
}

void EthanolSensor::update()
{
    // Convert deg C / %RH to the SGP40's 16-bit "ticks".
    float rh = constrain(co2.getHumidity(), 0.0f, 100.0f);  // keep the humidity clamp
    uint16_t rhTicks   = rh * 65535.0f / 100.0f;
    uint16_t tempTicks = (co2.getTemperature() + 45.0f) * 65535.0f / 175.0f;

    if (millis() - lastRead < READ_MS)
        return;
    lastRead = millis();

    error = sgp40.measureRawSignal(rhTicks, tempTicks, srawVoc);
    if (error)
        return;

    float x = (float)(room - (int32_t)srawVoc);
    float ppm = powf(10, (0.000345345 * x));

    total -= ppmAvg[count];
    ppmAvg[count] = ppm / (float)AVG_N;
    total += ppmAvg[count];
    count = (count + 1) % AVG_N;

    currentPpm = ppm;
    if (ppm > maxPpm)
        maxPpm = ppm;
}