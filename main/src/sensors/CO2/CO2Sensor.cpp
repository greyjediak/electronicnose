#include "CO2Sensor.h"
#include <Wire.h>


#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

void sendLine(const String &msg)
{
    Serial.println(msg);
}

void CO2Sensor::begin()
{
    Serial.begin(115200);

    while (!Serial)
    {
        delay(100);
    }
    Wire.begin();
    stc3x_sensor.begin(Wire, STC31_C_I2C_ADDR_29);
    sht4x_sensor.begin(Wire);

    uint32_t stc3xProductId = 0;
    uint64_t stc3xSerialNumber = 0;
    uint32_t sht4xSerialNumber = 0;
    int16_t error;

    delay(100);

        error = sht4x_sensor.serialNumber(sht4xSerialNumber);
    if (error == NO_ERROR)
    {
        sendLine("# SHT4x Serial: " + String(sht4xSerialNumber));
    }
 
    error = stc3x_sensor.getProductId(stc3xProductId, stc3xSerialNumber);
    if (error == NO_ERROR)
    {
        sendLine("# STC3x ID: " + String(stc3xProductId));
        sendLine("# STC3x Serial: " + String((uint32_t)stc3xSerialNumber));
    }

    // Set Binary Gas to Mode 19 (Low-cross-sensitivity for CO2 in Air)
    error = stc3x_sensor.setBinaryGas(19);
}


void CO2Sensor::update()
{
    if (millis() - lastRead < READ_MS)
        return;
    lastRead = millis();

    float co2Pct = 0.0f;
    float stcTemp = 0.0f;
    int16_t error;

    // Read SHT4x for compensation
    error = sht4x_sensor.measureHighPrecision(sht4xTemp, sht4xRh);
    if (error == NO_ERROR)
    {
        stc3x_sensor.setRelativeHumidity(sht4xRh);
        stc3x_sensor.setTemperature(sht4xTemp);
    }

    // Measure gas concentration
    error = stc3x_sensor.measureGasConcentration(co2Pct, stcTemp);
    if (error != NO_ERROR)
    {
        //Keep the last good value and dont print into the CSV stream
        return;
    }

    co2Ppm = co2Pct * 10000.0f; //convert percentage into ppm

    //clear detetction goes off if its below the threhold continously for 10 seconds
    if (co2Pct <= clearThreshold)
    {
        if (!isClearing)
        {
            // This is the first time it dropped below the threshold
            clearStartTime = millis();
            isClearing = true;
        }

        // Check if 10,000 milliseconds (10 seconds) have passed
        if (millis() - clearStartTime >= 10000)
        {
            chamberClear = true;
        }
    }
    else
    {
        // If the CO2 goes above the threshold, reset the timer
        if (isClearing)
        {
            isClearing = false;
            chamberClear = false;
        }
    }

}