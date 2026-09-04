#include "src/sensors/CO2/CO2Sensor.h"

CO2Sensor co2;

void setup () {
    co2.begin();
}

void loop() {
    co2.update();
}