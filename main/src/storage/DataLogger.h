
#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>

class DataLogger
{
public:
    bool begin();
    bool logLine(const char* csvLine);
    bool exportCSV(Stream& output);

private:
    bool findNextAddress();
    bool waitForFlash();

    bool initialized = false;
    uint32_t writeAddress = 0;
};

#endif
