#include "src/sensors/CO2/CO2Sensor.h"
#include "src/sensors/Hydrogen/HydrogenSensor.h"
#include "src/sensors/Acetone/AcetoneSensor.h"
#include "src/sensors/Ethanol/EthanolSensor.h"

EthanolSensor ethanol;
AcetoneSensor acetone;
HydrogenSensor h2;
CO2Sensor co2;
static const uint32_t PRINT_MS = 1000; // one CSV row per second

void setup () {
    warmupDisplay();//needs code
    co2.begin();
    h2.begin();
    acetone.begin();
    ethanol.begin();
    Serial.println("ms,temp_C,rh_pct,co2_ppm,h2_ppm,ethanol_ppm,acetone_dV,acetone_present,acetone_ready,chamber_clear");
}

void loop() {
    menuDisplay();//needs code
    if (buttonpressed){
        //set to next option 0 or 1
        moveHighlight();//move UI highlight to other option
    }
    if (buttonHeld && option == 0){
        displayWarmUp();//need code
        if (warmupdone){
            displayReady();//needs code
            if (button pressed){
                co2.update();
                h2.update();
                acetone.update();
                ethanol.update();

                static uint32_t lastPrint = 0;
                if (millis() - lastPrint >= PRINT_MS) {
                    lastPrint = millis();

                    Serial.print(millis());                       Serial.print(',');
                    Serial.print(co2.getTemperature(), 1);        Serial.print(',');
                    Serial.print(co2.getHumidity(), 1);           Serial.print(',');
                    Serial.print(co2.getPpm(), 0);                Serial.print(',');
                    Serial.print(h2.getPpm(), 2);                 Serial.print(',');
                    Serial.print(ethanol.getPpm(), 2);            Serial.print(',');
                    Serial.print(acetone.getDelta(), 4);          Serial.print(',');
                    Serial.print(acetone.isPresent() ? 1 : 0);    Serial.print(',');
                    Serial.print(acetone.isReady() ? 1 : 0);      Serial.print(',');
                    Serial.println(co2.isChamberClear() ? 1 : 0);
                }
                displayResults();//needs code
                //save results for CSV writing
            }
        }
    }
    if (buttonHeld && option == 1){
        displayDataHandling();
        //DataHandling
    }
}