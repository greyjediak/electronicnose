// =====================================================================
//  Test script only! XIAO shows up in File Explorer as a USB drive
//  AND as a serial port (CDC) at the same time.
//
//  Serial commands (arduino-cli monitor -p COMx):
//    w  append a test row to log.csv
//    l  list files on the drive
//    F  format the flash (asks for confirmation; ERASES everything)
//
//  First run on a fresh/raw flash: Windows will say the drive needs
//  formatting -- either click Format (FAT) there, or send F here.
// =====================================================================
#include "src/UsbDrive.h"

static const char* LOG_PATH = "log.csv";
static const char* CSV_HEADER =
    "ms,temp_C,rh_pct,co2_ppm,h2_ppm,ethanol_ppm,acetone_dV,acetone_present,acetone_ready,chamber_clear";

void printHelp() {
    Serial.println(F("Commands: w = write test row, l = list files, F = format (erase!)"));
}

void writeTestRow() {
    if (!UsbDrive::exists(LOG_PATH)) {
        UsbDrive::appendLine(LOG_PATH, CSV_HEADER);
    }
    char row[96];
    snprintf(row, sizeof(row), "%lu,25.0,50.0,420,0.00,0.00,0.0000,0,1,1",
             (unsigned long)millis());
    Serial.println(UsbDrive::appendLine(LOG_PATH, row) ? F("wrote row") : F("write FAILED (formatted?)"));
}

void confirmFormat() {
    Serial.println(F("Type YES within 10 s to ERASE the flash and format it."));
    Serial.setTimeout(10000);
    String reply = Serial.readStringUntil('\n');
    reply.trim();
    if (reply != "YES") { Serial.println(F("format cancelled")); return; }
    Serial.println(F("formatting..."));
    Serial.println(UsbDrive::format(&Serial) ? F("format OK") : F("format FAILED"));
}

void setup() {
    // Start the drive BEFORE waiting on Serial so the PC enumerates both.
    bool flashOk = UsbDrive::begin();

    Serial.begin(115200);
    uint32_t t0 = millis();
    while (!Serial && millis() - t0 < 3000) delay(10);

    if (!flashOk) {
        Serial.println(F("QSPI flash not found!"));
        return;
    }
    Serial.print(F("flash: ")); Serial.print(UsbDrive::sizeKB()); Serial.println(F(" KB"));
    Serial.println(UsbDrive::isFormatted() ? F("FAT filesystem mounted")
                                           : F("no filesystem yet -- send F, or format from Windows"));
    printHelp();
}

void loop() {
    UsbDrive::poll(); // tells firmware to drop cached copy if the PC changes something

    if (Serial.available()) {
        char c = Serial.read();
        switch (c) {
            case 'w': writeTestRow();          break;
            case 'l': UsbDrive::list(Serial);  break;
            case 'F': confirmFormat();         break;
            case '\r': case '\n':              break;
            default:  printHelp();             break;
        }
    }
}
