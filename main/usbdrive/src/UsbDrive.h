// =====================================================================
//  UsbDrive -- exposes the XIAO's 2 MB QSPI flash as a USB thumb drive
//  (shows up in File Explorer) while Serial (CDC) keeps working.
//  The firmware writes CSV files to the same FAT filesystem.
//
//  Rule: the PC and the firmware must not write at the same time.
//  appendLine()/format() take the drive "offline" to the PC for the
//  few ms they take, so Windows re-reads it afterwards.
//
//  Designed to be lifted into main/src/storage/ unchanged.
// =====================================================================
#ifndef USB_DRIVE_H
#define USB_DRIVE_H

#include <Arduino.h>

namespace UsbDrive {
    bool begin();
    bool isFormatted(); // True once a FAT filesystem is present (format() if not).
    bool format(Print* log = nullptr);// ERASES the whole flash and creates an empty FAT filesystem.
    bool appendLine(const char* path, const char* line); // Append one line (newline added) to a file, creating it if needed.
    bool exists(const char* path); // True if path exists.
    void list(Print& out); // Print the root directory listing (name + size) to out.
    void poll(); // Call from loop(): picks up changes the PC made (e.g. deleted files).
    uint32_t sizeKB();
}

#endif
