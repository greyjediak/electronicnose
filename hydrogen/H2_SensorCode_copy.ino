/**
 * nRF52840 H2Sensor Code
 * - We read A0 continuously
 * - We track a rolling average over the last second of readings
 * - This gets converted into volts and ppm
 * - We output this to our computer's monitor
 * - If we type 'r' in the serial monitor, it will reset the sensor
 * - If we type 'z' in the serial monitor, it will zero the sensor
 */

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

const uint8_t  ANALOG_PIN       = A0;     // Defining our analog input pin variable
const float    VREF_VOLTS       = 3.3f;   // Defining vref
const uint8_t  ADC_BITS         = 12;     // Defining our ADC bit resolution
const uint32_t PRINT_PERIOD_MS  = 200;    // Defining the rate that we want to refresh at
const uint32_t runtime = 500;
uint32_t ADC_MAX = (1UL << ADC_BITS) - 1UL; //Defining the max reading from our adc
//uint32_t maxRaw  = 0; 
float maxRaw  = 0.0;                       //Setting up empty variables to use later in our main loop
uint32_t lastPrintMs = 0;
uint32_t arrayforavg[runtime];
float reference = 1.55;
uint32_t rawaverage = 0;

void setup() {
  //To set up, first we set up our baud rate for the monitor where we will display our readings
  Serial.begin(115200);
  //Then we tell our code what the ADC resolution is
  analogReadResolution(ADC_BITS);
  (void)analogRead(ANALOG_PIN); // throw-away read
  // And we print out some basic specs, the reference voltage and bit resolution
  Serial.println(F("nRF52840 A0 Max-Voltage (USB Serial Output)"));
  Serial.print(F("ADC bits: ")); Serial.println(ADC_BITS);
  Serial.print(F("Vref: ")); Serial.print(VREF_VOLTS, 3); Serial.println(F(" V"));
  Serial.println(F("Type 'r' + Enter to reset max.\n"));
}

void loop() {
  // Read and update max
  int count = 0;
  float avg = 0.0;
  //while(count < 500){
  uint32_t raw = (uint32_t)analogRead(ANALOG_PIN);
  //if (raw > maxRaw) maxRaw = raw;
   // maxRaw = raw;
  //  avg += static_cast<float>(raw);
  //  count++;
  //  delay(2);
  //}
  uint32_t oldarray[runtime];
  for (int i = 0; i < runtime ; i++) {
    oldarray[i] = arrayforavg[i];
  }
  for (int i = 0; i < runtime ; i++) {
    if (i == 0) arrayforavg[i] = raw;
    else arrayforavg[i] = oldarray[i-1];
  }
  for (int i = 0; i < runtime ; i++){
    avg+=arrayforavg[i];
  }
  rawaverage = avg/500.0;
  maxRaw = rawaverage;
  // Print periodically to the serial monitor
  uint32_t now = millis();
  // IF the another period has expired
  if (now - lastPrintMs >= PRINT_PERIOD_MS) {
    // THEN we reset the period
    lastPrintMs = now;
    // convert our readings to volts and ppm
    float maxVolts = rawToVolts(maxRaw);
    float ppm = VoltsToPPM(maxVolts, reference);
    ppm = -1.0 * ppm;
    //Then output them to the monitor
    Serial.print(F("Raw: "));   Serial.print(maxRaw);
    Serial.print(F(" | Volts: ")); Serial.print(maxVolts, 4);
    Serial.print(F(" V"));
    Serial.print(F(" | PPM: ")); Serial.print(ppm, 4);
    Serial.println(F(" PPM"));
  }

  // This is where we're going to handle the input for a reset
  while (Serial.available()) {
    int c = Serial.read();
    if (c == 'r' || c == 'R') {
      maxRaw = 0;
      Serial.println(F("[Max reset]"));
    }
    if (c == 'z' || c == 'Z') {
      float newreference = static_cast<float>(raw);
      newreference = rawToVolts(newreference);
      reference = newreference;
      Serial.println(F("[Voltage reference reset]"));
    }
  }

  // Delay just for good code hygiene, don't wanna overclock our poor mcu
  delay(2);
}

float VoltsToPPM(float Volts, float reference) {
    return((Volts - reference)/0.002);
}

float rawToVolts(float raw) {
  if (raw > ADC_MAX) raw = ADC_MAX; // defensive clamp
  return (static_cast<float>(raw) * VREF_VOLTS) / static_cast<float>(ADC_MAX);
}