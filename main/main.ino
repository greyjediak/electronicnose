#include "src/sensors/CO2/CO2Sensor.h"
#include "src/sensors/Hydrogen/HydrogenSensor.h"
#include "src/sensors/Acetone/AcetoneSensor.h"
#include "src/sensors/Ethanol/EthanolSensor.h"

CO2Sensor co2;
EthanolSensor ethanol(co2);
AcetoneSensor acetone;
HydrogenSensor h2;
static const uint32_t PRINT_MS = 1000; // one CSV row per second

void setup () {
    Serial.begin(115200);

    uint32_t t0 = millis();
    while (!Serial && (millis() - t0) < 2000)
    {
        delay(10);
    }

    Wire.begin();
    Wire.setClock(400000);   // I2C at 400 kHz (supported by every sensor here)
    delay(100);              // give the I2C sensors a moment to finish powering up

    co2.begin();
    h2.begin();
    acetone.begin();
    ethanol.begin();
    Serial.println("ms,temp_C,rh_pct,co2_ppm,h2_ppm,ethanol_ppm,acetone_dV,acetone_present,acetone_ready,chamber_clear");
}

void loop() {
    co2.update();
    h2.update();
    acetone.update();
    ethanol.update();

    static uint32_t lastPrint = 0;
    if (millis() - lastPrint >= PRINT_MS) {
        lastPrint = millis();

        Serial.print(millis());                       Serial.print(','); //time since start
        Serial.print(co2.getTemperature(), 1);        Serial.print(','); //degree C
        Serial.print(co2.getHumidity(), 1);           Serial.print(','); //% RH
        Serial.print(co2.getPpm(), 0);                Serial.print(','); //CO2 
        Serial.print(h2.getPpm(), 2);                 Serial.print(','); //H2
        Serial.print(ethanol.getPpm(), 2);            Serial.print(','); //ethanol
        Serial.print(acetone.getDelta(), 4);          Serial.print(','); //acetone volts from baseline
        Serial.print(acetone.isPresent() ? 1 : 0);    Serial.print(','); //acetone detected
        Serial.print(acetone.isReady() ? 1 : 0);      Serial.print(','); //acetone baseline done
        Serial.println(co2.isChamberClear() ? 1 : 0); //chamber clear?
    }
}