#ifndef Ethanol_Sensor_H
#define Ethanol_Sensor_H

#include <Arduino.h>
#include <SensirionI2CSgp40.h>
#include <SensirionGasIndexAlgorithm.h>

class EthanolSensor {
    public:
    EthanolSensor();
    void begin();
    void update();

    float getPpm() const { return total; } 
    float getCurrentPpm() const { return currentPpm; }
    float getMaxPpm() const { return maxPpm; }

    private:
    SensirionI2CSgp40 sgp40;
    SensirionGasIndexAlgorithm voc_algorithm;
    GasIndexAlgorithmParams params;

    uint16_t defaultRh = 0x8000;
    uint16_t defaultT = 0x6666;
    uint16_t srawVoc = 0;
    int32_t room = 31500;

    static const int AVG_N = 100;
    int count = 0;
    float total = 0.0f;
    float ppmAvg[AVG_N] = {0.0f};
    float currentPpm = 0.0f;
    float maxPpm = 0.0f;

    unsigned long lastRead = 0;
    static const unsigned long READ_MS =  100;

    void printError(uint16_t error, char* errorMessage);
};

#endif