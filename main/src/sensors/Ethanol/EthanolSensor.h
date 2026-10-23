#ifndef Ethanol_Sensor_H
#define Ethanol_Sensor_H

#include <Arduino.h>
#include <SensirionI2CSgp40.h>
#include "../CO2/CO2Sensor.h"

class EthanolSensor {
    public:
    //CO2Sensor passed so EthanolSensor can borrow the temp and humidity readings
    explicit EthanolSensor(const CO2Sensor& co2Sensor) : co2(co2Sensor) {}
    void begin();
    void update();

    float getPpm() const { return avgPpm; } 
    float getCurrentPpm() const { return currentPpm; }
    float getMaxPpm() const { return maxPpm; }

    private:
    SensirionI2CSgp40 sgp40;
    const CO2Sensor&  co2;   // source of temperature / humidity (read-only)

    uint16_t srawVoc = 0;
    int32_t room = 31500;

    static const int AVG_N = 100;
    int count = 0;
    float total = 0.0f;
    float ppmAvg[AVG_N] = {0.0f};
    float currentPpm = 0.0f;
    float maxPpm = 0.0f;

    unsigned long lastRead = 0;  //millis() of last SGP40 read
    static const unsigned long READ_MS =  100;

};

#endif