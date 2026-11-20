#include "AcetoneSensor.h"
#include <math.h>
#include <Wire.h>

void AcetoneSensor::sendLine(const String& msg) {
    Serial.println(msg);
}

float AcetoneSensor::readVoltage() {
    int16_t raw = ads.readADC_Differential_0_1(); // ADC Pins 0 and 1 are connected to the TGS
    return ads.computeVolts(raw);
}

void AcetoneSensor::resetSession() {
    t_start = millis();
  t_last = 0;

  baseline = 0.0f;
  baseline_done = false;
  baseline_sum = 0.0f;
  baseline_n = 0;

  calibrating = true;
  calib_start = 0;
  min_delta = 999.0f;
  max_delta = -999.0f;

  hitCount = 0;
  present = false;
}

void AcetoneSensor::begin() {
    Serial.begin(115200); // baud rate
  delay(1000);

  Wire.begin();
  Wire.setClock(400000);

  if (!ads.begin(0x48))
  {
    // Error statement for inaccurate ADC
     sendLine("ADS1115 not found. Check wiring/address.");
    while(1) {} // Stay here till the ADC is found
  }

  ads.setGain(GAIN_SETTING); // set the ADC gain
}

void AcetoneSensor::update() {
    if (!sessionStarted) {
    sendLine("Starting baseline");
    sendLine("Keep sensor in clean air.");
    sendLine("t,vdiff,delta,state");

    resetSession();

    sessionStarted = true;
  } 

  unsigned long t_now = millis();
  if (t_now - t_last < SAMPLE_MS) return;
  t_last = t_now;

  float vdiff = readVoltage() * -1;

  // BASELINE phase
  if (!baseline_done)
  {
    // Get running average baseline

    baseline_sum += vdiff;
    baseline_n++;

    if (t_now - t_start >= BASELINE_MS) // If baseline time read is done
    {
      baseline = baseline_sum / (float)baseline_n; //Compute average of voltage points
      baseline_done = true; // Set baseline flag to true
      sendLine("BASELINE = " + String(baseline, 4));
    }

    // Keep printing values while baselining
    
    return;
  }

  // DETECTION Phase - Baseline done
  float delta = vdiff - baseline; // in clean air it should be 0

  if (calibrating)
  {
    if(calib_start == 0) calib_start = millis();
    if(delta < min_delta) min_delta = delta;
    if(delta > max_delta) max_delta = delta;
    String msg = String(t_now - t_start) + "ms," + String(vdiff, 4) + ",0,BASE";
    sendLine(msg);

    if (millis() - calib_start >= CALIB_MS)
    {
      calibrating = false; // Calibrating complete

      recommended_thresh = max(fabs(min_delta), fabs(max_delta)) * 3.0f;
      if (recommended_thresh < 0.002f) recommended_thresh = 0.002f;

      sendLine("calibration complete");
      //msg = "CALIBRATION COMPLETE. Min delta: " + String(min_delta, 8) + 
      //"Max delta " + String(max_delta, 8) + 
      //Serial.print("Center offset: "); Serial.println(center, 8);
      //"New baseline: " + String(baseline, 8) +
      //Serial.print("Spread: "); Serial.println(spread, 8);
      //"Recommended threshold is " + String(recommended_thresh, 6);
      
    }
    return;
  }

  // With updated threshold
  if (fabs(delta) > recommended_thresh) //if the volt reading - baseline is deviated so far from baseline
  {
    hitCount++; // increment number of hits taken
  }
  else
  {
    hitCount = 0;
  }

  if(!present && hitCount >= CONFIRM_COUNT)
  {
    present = true; // update state
    sendLine("#DETECTED#");

  }

  // Hysteresis 
  const float CLEAR_THRESH = recommended_thresh * 0.8f;
  static int clearCount = 0;
  if (present)
  {
    if (fabs(delta) < CLEAR_THRESH) clearCount++;
    else clearCount = 0;

    if (clearCount >= 10)
    {
      present = false;
      clearCount = 0;
      sendLine("#CLEARED#");
    }
  }

  // Write CSV ouput
  String msg = String(vdiff, 3) + "," +
             String(delta, 3) + "," +
             String(hitCount) + ",P=" +
             String(present ? 1 : 0);
  sendLine(msg);

}