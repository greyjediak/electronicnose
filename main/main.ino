#include <SensirionI2cSht4x.h>


#include "src/sensors/CO2/CO2Sensor.h"
#include "src/sensors/Hydrogen/HydrogenSensor.h"
#include "src/sensors/Acetone/AcetoneSensor.h"
#include "src/sensors/Ethanol/EthanolSensor.h"

#include "src/storage/DataLogger.h"

CO2Sensor co2;
EthanolSensor ethanol(co2);
AcetoneSensor acetone;
HydrogenSensor h2;

DataLogger logger;

static const uint32_t PRINT_MS = 1000; // one CSV row per second

void setup()
{
    Serial.begin(115200);

    uint32_t t0 = millis();
    while (!Serial && (millis() - t0) < 2000)
    {
        delay(10);
    }

    // Initialize onboard QSPI flash
    if (logger.begin())
    {
        Serial.println("# QSPI logger initialized successfully");
    }
    else
    {
        Serial.println("# ERROR: QSPI logger initialization failed");
    }

    Wire.begin();
    Wire.setClock(400000);
    delay(100);

    co2.begin();
    h2.begin();
    acetone.begin();
    ethanol.begin();

    Serial.println("ms,temp_C,rh_pct,co2_ppm,h2_ppm,ethanol_ppm,acetone_dV,acetone_present,acetone_ready,chamber_clear");
}


void loop()
{
    // Check for the CSV export command before h2.update()
    // because the H2 module also reads Serial input.
    if (Serial.available())
    {
        char command = Serial.peek();

        if (command == 'e' || command == 'E')
        {
            Serial.read();

            Serial.println("# Exporting stored CSV data...");

            if (logger.exportCSV(Serial))
            {
                Serial.println("# Export complete");
            }
            else
            {
                Serial.println("# ERROR: CSV export failed");
            }

            return;
        }
    }

    co2.update();
    h2.update();
    acetone.update();
    ethanol.update();

    static uint32_t lastPrint = 0;

    if (millis() - lastPrint >= PRINT_MS)
    {
        lastPrint = millis();

        // Build the CSV row
        String csvRow;
        csvRow.reserve(128);

        csvRow += String(millis());
        csvRow += ',';

        csvRow += String(co2.getTemperature(), 1);
        csvRow += ',';

        csvRow += String(co2.getHumidity(), 1);
        csvRow += ',';

        csvRow += String(co2.getPpm(), 0);
        csvRow += ',';

        csvRow += String(h2.getPpm(), 2);
        csvRow += ',';

        csvRow += String(ethanol.getPpm(), 2);
        csvRow += ',';

        csvRow += String(acetone.getDelta(), 4);
        csvRow += ',';

        csvRow += String(acetone.isPresent() ? 1 : 0);
        csvRow += ',';

        csvRow += String(acetone.isReady() ? 1 : 0);
        csvRow += ',';

        csvRow += String(co2.isChamberClear() ? 1 : 0);

        // Print the row to USB Serial
        Serial.println(csvRow);

        // Save the same row to onboard QSPI flash
        static bool loggerErrorReported = false;

        if (!logger.logLine(csvRow.c_str()) && !loggerErrorReported)
        {
            Serial.println("# ERROR: Could not save data to QSPI flash");
            loggerErrorReported = true;
        }
    }
}
