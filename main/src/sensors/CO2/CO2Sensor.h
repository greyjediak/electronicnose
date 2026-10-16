#ifndef CO2_Sensor_H
#define CO2_Sensor_H

#include <Arduino.h>
#include <SensirionI2CSht4x.h>
#include <SensirionI2cStc3x.h>

class CO2Sensor {
public: 
    void begin(); //void setup
    void update(); //void loop
    float getPpm() const         { return co2Ppm; }
    float getTemperature() const { return sht4xTemp; }
    float getHumidity() const    { return sht4xRh; }
    bool isChamberClear() const { return chamberClear; }

private:
    SensirionI2cStc3x stc3x_sensor;
    SensirionI2CSht4x sht4x_sensor;

    float co2Ppm = 0.0f;
    float sht4xTemp = 0.0f;
    float sht4xRh = 0.0f;

    unsigned long lastRead = 0;
    static const unsigned long READ_MS = 250;

    unsigned long clearStartTime = 0; // Stores when the chamber first looked clear
    bool isClearing = false;          // Tracks if we are currently in the 10-second countdown
    bool chamberClear = false;
    float clearThreshold = 0.10;      // Target "clear" ppm/percentage

   
};


#endif