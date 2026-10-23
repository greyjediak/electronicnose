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

    private:
    SensirionI2CSgp40 sgp40;
    SensirionGasIndexAlgorithm voc_algorithm;
    GasIndexAlgorithmParams params;

    uint16_t defaultRh = 0x8000;
    uint16_t defaultT = 0x6666;
    uint16_t srawVoc = 0;
    int32_t room = 31500;
    int count = 0;
    float total = 0.0;
    int time = 0;
    float ppmAvg[100] = {0.0};
    float max = 0.0;

    void printError(uint16_t error, char* errorMessage);
};

#endif