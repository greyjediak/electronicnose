#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <bluefruit.h>


Adafruit_ADS1115 ads;
BLEUart bleuart;

// Gain setting, can be reduced
static const adsGain_t GAIN_SETTING = GAIN_SIXTEEN;
static const unsigned long SAMPLE_MS = 500; // 20 Hz
static const unsigned long BASELINE_MS = 5000; // Get baseline for 5 secs

// TO be tuned after seeing noise level in clean air
static const float THRESH_V = 0.003f; //amount that delta must move away from 0 before it's considered "real event"
static const int CONFIRM_COUNT = 5; //requires 5 consecutive hts
float recommended_thresh;

// Baseline variables
float baseline = 0.0f; // average clean air voltage
bool baseline_done = false; // flag to finish baseline step
float baseline_sum = 0.0f; // To get baseline average
int baseline_n = 0;  // To get baseline average

// Threshold values for calibrating clean air
bool calibrating = true;
unsigned long calib_start = 0;
static const unsigned long CALIB_MS = 10000; //10 second calib
float min_delta = 999.0;
float max_delta = -999.0;

int hitCount = 0; // number of hits
bool present = false; // acetone present

unsigned long t_start = 0; // acetone sampling time start
unsigned long t_last = 0; // acetone sampling time last

static bool introSent = false;
bool sessionStarted = false;


// Message for both bluetooth and seial helper function
void sendLine (const String& msg)
{
  Serial.println(msg);
  if (Bluefruit.connected())
  {
    bleuart.println (msg);
  }
}

void setupBLE()
{
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Acetone-XIAO");

  bleuart.begin();

  startAdv();
}

float readVoltage()
{
  int16_t raw = ads.readADC_Differential_0_1(); // ADC Pins 0 and 1 are connected to the TGS
  return ads.computeVolts(raw);
}

void resetSession()
{
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


void startAdv(void)
{
  Bluefruit.Advertising.stop();
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start(0);
}

void setup() {
  Serial.begin(115200); // baud rate
  delay(1000);
  setupBLE();

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

//-----------MAIN LOOP-------//
void loop() {

  if (!Bluefruit.connected()) {
    sessionStarted = false;
    delay(100);
    return;
  }
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




