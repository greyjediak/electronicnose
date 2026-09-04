#include "src/sensors/CO2/CO2Sensor.h"
#include "src/sensors/Hydrogen/HydrogenSensor.h"
#include "src/sensors/Acetone/AcetoneSensor.h"
#include "src/sensors/Ethanol/EthanolSensor.h"

EthanolSensor ethanol;
AcetoneSensor acetone;
HydrogenSensor h2;
CO2Sensor co2;

void setup () {
    co2.begin();
    h2.begin();
    acetone.begin();
    ethanol.begin();
}

void loop() {
    co2.update();
    h2.update();
    acetone.update();
    ethanol.update();
}