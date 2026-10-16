#include "EthanolSensor.h"
#include <Wire.h>
#include <math.h>

EthanolSensor::EthanolSensor() : voc_algorithm(0) {}

void EthanolSensor::printError(uint16_t error, char *errorMessage)
{
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
}

void EthanolSensor::begin()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    Wire.begin();
    uint16_t error;
    char errorMessage[256];

    sgp40.begin(Wire);

    uint16_t serialNumber[3];
    uint8_t serialNumberSize = 3;

    error = sgp40.getSerialNumber(serialNumber, serialNumberSize);

    if (error)
    {
        Serial.print("# Error trying to execute getSerialNumber(): ");
        printError(error, errorMessage);
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
        printError(error, errorMessage);
    }
    else if (testResult != 0xD400)
    {
        Serial.print("# executeSelfTest failed with error: ");
        Serial.println(testResult);
    }
    GasIndexAlgorithm_init(&params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
}

void EthanolSensor::update()
{
    uint16_t error;
    char errorMessage[256];

    if (millis() - lastRead < READ_MS)
        return;
    lastRead = millis();

    error = sgp40.measureRawSignal(defaultRh, defaultT, srawVoc);
    if (error)
        return;

    int32_t voc_index;
    float x = (float)(room - (int32_t)srawVoc);
    float ppm = pow(10, (0.000345345 * x));

    total -= ppmAvg[count];
    ppmAvg[count] = ppm / (float)AVG_N;
    total += ppmAvg[count++];
    //or total += ppmAvg[count]
    //and count = (count + 1) % AVG_N;

    currentPpm = ppm;
    if (ppm > maxPpm)
        maxPpm = ppm;
}