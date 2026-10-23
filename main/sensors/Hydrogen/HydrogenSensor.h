#ifndef H2_Sensor_H
#define H2_Sensor_H

#include <Arduino.h>

class HydrogenSensor {
    public:
    void begin();
    void update();

    private: 
    static const uint8_t  ANALOG_PIN       = A0;     // Defining our analog input pin variable
    static constexpr float    VREF_VOLTS       = 3.3f;   // Defining vref
    static const uint8_t  ADC_BITS         = 12;     // Defining our ADC bit resolution
    static const uint32_t PRINT_PERIOD_MS  = 200;    // Defining the rate that we want to refresh at
    static const uint32_t runtime = 500;

    uint32_t ADC_MAX = (1UL << ADC_BITS) - 1UL; //Defining the max reading from our adc
 
    float maxRaw  = 0.0;                       //Setting up empty variables to use later in our main loop
    uint32_t lastPrintMs = 0;
    uint32_t arrayforavg[runtime];
    float reference = 1.55;
    uint32_t rawaverage = 0;

    float VoltsToPPM(float Volts, float reference);
    float rawToVolts(float raw);
};

#endif