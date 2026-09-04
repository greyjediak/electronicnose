#include "EthanolSensor.h"
#include <Wire.h>
#include <math.h>

EthanolSensor::EthanolSensor() : voc_algorithm(0) {}

void EthanolSensor::printError(uint16_t error, char* errorMessage) {
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
}

void EthanolSensor::begin() {
    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }
    Wire.begin();
    uint16_t error;
    char errorMessage[256];

    sgp40.begin(Wire);

    uint16_t serialNumber[3];
    uint8_t serialNumberSize = 3;

    error = sgp40.getSerialNumber(serialNumber, serialNumberSize);

    if (error) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        printError(error, errorMessage);
    } else {
        Serial.print("SerialNumber:");
        Serial.print("0x");
        for (size_t i = 0; i < serialNumberSize; i++) {
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
    if (error) {
        Serial.print("Error trying to execute executeSelfTest(): ");
        printError(error, errorMessage);
    } else if (testResult != 0xD400) {
        Serial.print("executeSelfTest failed with error: ");
        Serial.println(testResult);
    }
    GasIndexAlgorithm_init(&params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
}

void EthanolSensor::update() {
    uint16_t error;
    char errorMessage[256];

    error = sgp40.measureRawSignal(defaultRh, defaultT, srawVoc);
    if (error) {
        Serial.print("Error trying to execute measureRawSignal(): ");
        printError(error, errorMessage);

    } else {
        while(true){
            //Serial.print("SRAW_VOC:");
            //Serial.println(srawVoc);
            //total = 0.0;
            sgp40.measureRawSignal(defaultRh, defaultT, srawVoc);
            int32_t voc_index;
            float x = (float)(room - (int32_t)srawVoc);
            float ppm = pow(10, (0.000345345 * x));
            total -= ppmAvg[count];
            ppmAvg[count] = ppm / 100.0;
            total += ppmAvg[count++];
            char cmd = 0;
            bool measure = false;
            
            if (ppm > max) max = ppm;
            
            if (count == 100) count = 0;
            if (time == 10){
                Serial.print("SRAW_VOC:");
                Serial.println(srawVoc);
                Serial.print("avgppm: ");
                Serial.println(total);
                Serial.print("current ppm: ");
                Serial.println(ppm);
                Serial.print("max ppm: ");
                Serial.println(max);
                time = 0;
            }
            time++;
            delay(100);
        }
    }
}