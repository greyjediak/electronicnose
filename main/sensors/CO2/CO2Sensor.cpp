#include "CO2Sensor.h"
#include <Wire.h>


#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0
static char errorMessage[64];

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
        sendLine("SHT4x Serial: " + String(sht4xSerialNumber));
    }

    error = stc3x_sensor.getProductId(stc3xProductId, stc3xSerialNumber);
    if (error == NO_ERROR)
    {
        sendLine("STC3x ID: " + String(stc3xProductId));
        sendLine("Serial Number: " + String((uint32_t)stc3xSerialNumber));
    }

    // Set Binary Gas to Mode 19 (Low-cross-sensitivity for CO2 in Air)
    error = stc3x_sensor.setBinaryGas(19);
}


void CO2Sensor::update()
{
    float stc3xCo2Concentration = 0.0;
    float stc3xTemperature = 0.0;
    float sht4xTemperature = 0.0;
    float sht4xRelativeHumidity = 0.0;
    int16_t error;

    delay(250); // responsiveness

    // Read SHT4x for compensation
    error = sht4x_sensor.measureHighPrecision(sht4xTemperature, sht4xRelativeHumidity);
    if (error == NO_ERROR)
    {
        stc3x_sensor.setRelativeHumidity(sht4xRelativeHumidity);
        stc3x_sensor.setTemperature(sht4xTemperature);
    }

    // Measure gas concentration
    error = stc3x_sensor.measureGasConcentration(stc3xCo2Concentration, stc3xTemperature);
    if (error != NO_ERROR)
    {
        errorToString(error, errorMessage, sizeof errorMessage);
        sendLine("Error: " + String(errorMessage));
        return;
    }

    // 2. Calculate Corrected PPM (Relative to baseline)
    float co2Ppm = stc3xCo2Concentration * 10000.0;

    // 4. Clearance Notification Logic
    // If gas drops below 500ppm and was previously high
    float currentCO2 = stc3xCo2Concentration;

    if (currentCO2 <= clearThreshold)
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
            Serial.println("CHAMBER CONFIRMED CLEAR");
            // Trigger your next step here (e.g., turn off pump, allow next test)
            isClearing = false;
        }
    }
    else
    {
        // If the CO2 goes above the threshold, reset the timer
        if (isClearing)
        {
            isClearing = false;
        }
    }

    String dataOutput = "T:" + String(stc3xTemperature, 1);
    dataOutput += " H:" + String(sht4xRelativeHumidity, 1);
    dataOutput += " CO2%:" + String(stc3xCo2Concentration, 4);
    dataOutput += " PPM:" + String(co2Ppm, 0);

    sendLine(dataOutput);
}
