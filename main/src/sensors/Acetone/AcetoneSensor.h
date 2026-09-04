#ifndef Acetone_Sensor_H
#define Acetone_Sensor_H

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

class AcetoneSensor
{
public:
    void begin();
    void update();

private:
    Adafruit_ADS1115 ads;

    // Gain setting, can be reduced
    static const adsGain_t GAIN_SETTING = GAIN_SIXTEEN;
    static const unsigned long SAMPLE_MS = 500;    // 20 Hz
    static const unsigned long BASELINE_MS = 5000; // Get baseline for 5 secs

    // TO be tuned after seeing noise level in clean air
    static constexpr float THRESH_V = 0.003f; // amount that delta must move away from 0 before it's considered "real event"
    static const int CONFIRM_COUNT = 5;   // requires 5 consecutive hts
    float recommended_thresh;

    // Baseline variables
    float baseline = 0.0f;      // average clean air voltage
    bool baseline_done = false; // flag to finish baseline step
    float baseline_sum = 0.0f;  // To get baseline average
    int baseline_n = 0;         // To get baseline average

    // Threshold values for calibrating clean air
    bool calibrating = true;
    unsigned long calib_start = 0;
    static const unsigned long CALIB_MS = 10000; // 10 second calib
    float min_delta = 999.0;
    float max_delta = -999.0;

    int hitCount = 0;     // number of hits
    bool present = false; // acetone present

    unsigned long t_start = 0; // acetone sampling time start
    unsigned long t_last = 0;  // acetone sampling time last

    bool sessionStarted = false;
    int clearCount = 0;

    float readVoltage();
    void resetSession();
    void sendLine(const String& msg);
};

#endif