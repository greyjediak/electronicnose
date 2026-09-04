#ifndef CO2_Sensor_H
#define CO2_Sensor_H

#include <Arduino.h>
#include <SensirionI2CSht4x.h>
#include <SensirionI2cStc3x.h>

class CO2Sensor {
public: 
    void begin(); //void setup
    void update(); //void loop

private:
    SensirionI2cStc3x stc3x_sensor;
    SensirionI2CSht4x sht4x_sensor;

    unsigned long clearStartTime = 0; // Stores when the chamber first looked clear
    bool isClearing = false;          // Tracks if we are currently in the 10-second countdown
    float clearThreshold = 0.10;      // Target "clear" ppm/percentage

   
};


#endif