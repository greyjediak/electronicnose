#include "UsbDrive.h"

#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_TinyUSB.h>

// on-board flash chip isn't adafruit built in.
// description here.
static const SPIFlash_Device_t P25Q16H = {
    .total_size = (1UL << 21),
    .start_up_time_us = 10000,
    .manufacturer_id = 0x85, 
    .memory_type = 0x60,
    .capacity = 0x15,
    .max_clock_speed_mhz = 55,
    .quad_enable_bit_mask = 0x02,
    .has_sector_protection = false,
    .supports_fast_read = true,
    .supports_qspi = true,
    .supports_qspi_writes = true,
    .write_status_register_split = false,
    .single_status_byte = false,
    .is_fram = false,
};

// QSPI pins come from the board variant (on-board P25Q16H, not on headers).
static Adafruit_FlashTransport_QSPI flashTransport; // 4 wire SPI
static Adafruit_SPIFlash            flash(&flashTransport); // reads and writes the chip in 512 byte blocks
static FatVolume                    fatfs; // filesystem
static Adafruit_USBD_MSC            usbMsc; // usb drive interface Windows  sees

static bool          formatted   = false; // did fatfs mount?
static volatile bool hostChanged = false;   // did PC write to the drive?

// ---------- USB mass-storage callbacks (run from USB task) ----------
// Read blocks for Windows from flash
static int32_t mscRead(uint32_t lba, void* buffer, uint32_t bufsize) {
    return flash.readBlocks(lba, (uint8_t*)buffer, bufsize / 512) ? (int32_t)bufsize : -1;
}

// Windows saving blocks, so read them from flash
static int32_t mscWrite(uint32_t lba, uint8_t* buffer, uint32_t bufsize) {
    return flash.writeBlocks(lba, buffer, bufsize / 512) ? (int32_t)bufsize : -1;
}

// Windows done writing, push RAM copy to chip and set hostChanged
static void mscFlush() {
    flash.syncBlocks();
    hostChanged = true;
}

// Hide the drive from the PC while the firmware touches the filesystem.
static void hostOffline() { usbMsc.setUnitReady(false); }
static void hostOnline()  { usbMsc.setUnitReady(true);  }

namespace UsbDrive {

// TODO: Call first thing in setup(). Starts flash + USB mass storage.
// Returns false only if the flash chip didn't respond.
bool begin() {
    if (!flash.begin(&P25Q16H, 1)) {
        return false;
    }

    usbMsc.setID("ENose", "Data Logger", "1.0");   // name Windows shows in Device Manager, vendor(8) product(16) rev(4)
    usbMsc.setReadWriteCallback(mscRead, mscWrite, mscFlush);
    usbMsc.setCapacity(flash.size() / 512, 512);
    usbMsc.setUnitReady(true); // Lets disconnect -> reconnect make Windows notice the new drive
    usbMsc.begin();

    // Try to mount the filesystem
    if (TinyUSBDevice.mounted()) {
        TinyUSBDevice.detach();
        delay(10);
        TinyUSBDevice.attach();
    }

    formatted = fatfs.begin(&flash);
    return true;
}

bool isFormatted() { return formatted; } // True once a FAT filesystem is present (format() if not).

uint32_t sizeKB() { return flash.size() / 1024; }

// Erases everyting and creates blank filesystem
bool format(Print* log) {
    hostOffline(); // Hide drive
    static uint8_t secBuf[512];
    FatFormatter fmt; 
    bool ok = fmt.format(&flash, secBuf, log);
    flash.syncBlocks(); // WRtie to the chip
    formatted = ok && fatfs.begin(&flash); // remount
    hostOnline(); // Unhide drive
    return formatted;
}

// handles line addition to csv, if one does not yet exist, it creates on
bool appendLine(const char* path, const char* line) {
    if (!formatted) return false;
    hostOffline(); // hide drive
    File32 f = fatfs.open(path, FILE_WRITE);   // create / append
    bool ok = f;
    if (ok) {
        f.println(line);
        f.close();
    }
    flash.syncBlocks();
    hostOnline();
    return ok;
}

bool exists(const char* path) {
    return formatted && fatfs.exists(path);
}

void list(Print& out) {
    if (!formatted) { out.println(F("(not formatted)")); return; }
    File32 root = fatfs.open("/");
    File32 entry;
    while (entry.openNext(&root, O_RDONLY)) {
        out.print(F("  "));
        entry.printName(&out);
        if (entry.isDir()) out.print('/');
        out.print(F("  "));
        out.print((uint32_t)entry.fileSize());
        out.println(F(" B"));
        entry.close();
    }
    root.close();
}

void poll() {
    if (hostChanged) {
        hostChanged = false;
        fatfs.cacheClear();   // drop stale cached sectors after PC edits
    }
}

} // namespace UsbDrive
