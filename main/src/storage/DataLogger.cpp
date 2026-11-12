
#include "DataLogger.h"
#include <nrfx_qspi.h>
#include <string.h>

// QSPI flash configuration
static nrfx_qspi_config_t qspiConfig = {};

// Flash storage layout
static const uint32_t SECTOR_SIZE = 4096;
static const uint32_t PAGE_SIZE = 256;

static const uint32_t LOG_START = 0x001000;
static const uint32_t LOG_END   = 0x1FF000;

// Buffers must be aligned for QSPI operations
alignas(4) static uint8_t pageBuffer[PAGE_SIZE];
alignas(4) static uint8_t sectorBuffer[SECTOR_SIZE];

static const char CSV_HEADER[] =
    "ms,temp_C,rh_pct,co2_ppm,h2_ppm,ethanol_ppm,acetone_dV,acetone_present,acetone_ready,chamber_clear";


bool DataLogger::waitForFlash()
{
    while (nrfx_qspi_mem_busy_check() == NRFX_ERROR_BUSY)
    {
        delay(1);
    }

    return true;
}


bool DataLogger::begin()
{
    // Configure QSPI pins
    qspiConfig.xip_offset = 0;

    qspiConfig.pins.sck_pin = 21;
    qspiConfig.pins.csn_pin = 25;
    qspiConfig.pins.io0_pin = 20;
    qspiConfig.pins.io1_pin = 24;
    qspiConfig.pins.io2_pin = 22;
    qspiConfig.pins.io3_pin = 23;

    qspiConfig.prot_if.readoc = NRF_QSPI_READOC_FASTREAD;
    qspiConfig.prot_if.writeoc = NRF_QSPI_WRITEOC_PP;
    qspiConfig.prot_if.addrmode = NRF_QSPI_ADDRMODE_24BIT;
    qspiConfig.prot_if.dpmconfig = false;

    qspiConfig.phy_if.sck_delay = 0x05;
    qspiConfig.phy_if.dpmen = false;
    qspiConfig.phy_if.spi_mode = NRF_QSPI_MODE_0;
    qspiConfig.phy_if.sck_freq = NRF_QSPI_FREQ_32MDIV16;

    qspiConfig.irq_priority = (uint8_t)NRFX_QSPI_CONFIG_IRQ_PRIORITY;

    // Initialize QSPI
    nrfx_err_t error = nrfx_qspi_init(&qspiConfig, NULL, NULL);

    if (error != NRFX_SUCCESS)
    {
        return false;
    }

    initialized = true;

    // Find the first unused flash page
    return findNextAddress();
}


bool DataLogger::findNextAddress()
{
    writeAddress = LOG_START;

    // Scan one 4 KB sector at a time
    for (uint32_t sectorAddress = LOG_START;
         sectorAddress < LOG_END;
         sectorAddress += SECTOR_SIZE)
    {
        nrfx_err_t error = nrfx_qspi_read(
            sectorBuffer,
            SECTOR_SIZE,
            sectorAddress
        );

        if (error != NRFX_SUCCESS)
        {
            return false;
        }

        waitForFlash();

        // Find the first completely erased page
        for (uint32_t offset = 0;
             offset < SECTOR_SIZE;
             offset += PAGE_SIZE)
        {
            bool erased = true;

            for (uint32_t i = 0; i < PAGE_SIZE; i++)
            {
                if (sectorBuffer[offset + i] != 0xFF)
                {
                    erased = false;
                    break;
                }
            }

            if (erased)
            {
                writeAddress = sectorAddress + offset;
                return true;
            }
        }
    }

    // No unused pages remain
    writeAddress = LOG_END;
    return true;
}


bool DataLogger::logLine(const char* csvLine)
{
    if (!initialized || csvLine == nullptr)
    {
        return false;
    }

    // Check available storage
    if (writeAddress >= LOG_END)
    {
        return false;
    }

    size_t length = strlen(csvLine);

    // Make sure the CSV row fits inside one page
    if (length >= PAGE_SIZE - 1)
    {
        return false;
    }

    // Fill unused space with erased-flash values
    memset(pageBuffer, 0xFF, PAGE_SIZE);

    // Copy the CSV row into the page
    memcpy(pageBuffer, csvLine, length);

    // Add a string terminator
    pageBuffer[length] = '\0';

    // Write one complete 256-byte page
    nrfx_err_t error = nrfx_qspi_write(
        pageBuffer,
        PAGE_SIZE,
        writeAddress
    );

    if (error != NRFX_SUCCESS)
    {
        return false;
    }

    waitForFlash();

    // Move to the next page
    writeAddress += PAGE_SIZE;

    return true;
}


bool DataLogger::exportCSV(Stream& output)
{
    if (!initialized)
    {
        return false;
    }

    output.println(CSV_HEADER);

    alignas(4) uint8_t readBuffer[PAGE_SIZE];

    // Read all previously stored pages
    for (uint32_t address = LOG_START;
         address < writeAddress;
         address += PAGE_SIZE)
    {
        nrfx_err_t error = nrfx_qspi_read(
            readBuffer,
            PAGE_SIZE,
            address
        );

        if (error != NRFX_SUCCESS)
        {
            return false;
        }

        waitForFlash();

        // Skip empty pages
        if (readBuffer[0] == 0xFF)
        {
            continue;
        }

        // Print the stored CSV row
        for (uint32_t i = 0; i < PAGE_SIZE; i++)
        {
            if (readBuffer[i] == 0xFF || readBuffer[i] == '\0')
            {
                break;
            }

            output.write(readBuffer[i]);
        }

        output.println();
    }

    return true;
}
