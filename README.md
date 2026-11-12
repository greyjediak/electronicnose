## General codebase info (devs)
Please make sure any new sketch, cpp, h, and folder names have no spaces.

Layout:
src/

## Sensor Information
Sensiron SGX Hydrogen Sensor:

Sensirion CO2 Sensor: 

Sensirion Ethanol Sensor:
Firmware: Ensure Sensirion 

Figaro Acetone Sensor: 
Firmware - Ensure Adafruit ADS1X15 Library is installed (Arduino IDE)
TGS1820 Please refer to https://www.figarosensor.com/product/entry/tgs1820.html for any sensor info.

## Pin Assignments:
in pin.h in the root folder. To add the assignments file, 
include "pins.h" in your sketch you want to compile. Any setup () pin assignments will overwrite this though.

## Graphics Info
The LCD uses a ST7789 driver from the Adafruit Library. To use it in the Arduino IDE, go to Library Manager and install Adafruit ST7735, ST7789 Library, Adafruit GFX Library. Adafruit ADS1X15 Library.

## USB Info
CDC - What Serial Monitor Uses
MSC - "Mass Storage", opens in File Explorer.
The Xiao has a 2MB flash chip on the board. UsbDrive formats it as FAT, which Windows understands. 
Firmware: TinyUSB, SPIFlash, SdFat alreadycome with the Seeeed, nothing to install

## Circuit and Wiring Functionality
Hydrogen - Output read by anaolog pin
Ethanol
Acetone - The acetone circuit functions by subtracting VRL and VREF with the 
CO2