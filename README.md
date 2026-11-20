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

## Libraries
Install once (Arduino IDE Library Manager, or `arduino-cli lib install "<name>"`):
- Adafruit ST7735 and ST7789 Library, Adafruit GFX Library, Adafruit ADS1X15
- Sensirion I2C STC3x (1.x), Sensirion I2C SHT4x (1.x), Sensirion I2C SGP40 (0.1.0)

## Graphics Info
The LCD is a 240x240 ST7789 (Adafruit 1.54" breakout) on hardware SPI.
- Panel settings (size, rotation, colors, SPI speed): `main/graphics/display_config.h`
- Pins: `main/pins.h`. The display needs its TCS pin on a GPIO (D2); with TCS tied to GND it never starts.
- UI code: `main/src/ui/`. All screens are drawn in code.
  - `Theme.h` - colors (from Figma), fonts, button hold time.
  - `Gfx.*` - the display plus low-level drawing: text, and smooth (anti-aliased) bars and rings.
  - `Button.*` - single button: click = next, hold 3 s = select.
  - `components/` - reusable pieces, each with its own draw/update:
    `ProgressBar`, `Ring`, `MenuList`, `MetricRow`, `RangeLegend`, and `ValueFormat` (how numbers are written, e.g. "30,000 ppm" or "< 1 ppm").
  - `Screens.h` + `screens/` - one small file per screen, holding its positions and the components it uses. `name()` draws the whole screen once, `nameTick()` redraws only the moving parts.
    Screens: Splash, Menu, Heating, Baselining, Sensing, Metrics, Saving, SaveFailed, Done, Memory.
- Test the LCD and button without sensors: open `main/graphics/graphics.ino` (fake data). Hold the button while powering up for a color/orientation test.

To change one screen, edit only `screens/<Name>.cpp`. To change how every bar or ring looks, edit the component.

Command line (from `main/graphics`):
```
arduino-cli compile --upload -p COM12
arduino-cli monitor -p COM12 -c baudrate=115200
```

### Figma screens
`main/graphics/figma/` holds the Figma exports (240x240) as the **reference** for each screen. They are not compiled. Positions in `screens/*.cpp` were measured from these PNGs.

Figma colors (in `Theme.h`): background `#0C0E17`, bar track `#D9D9D9`, blue `#5EA6E6`, green `#49BA62`, orange `#E0A63D`, red `#9E0303`, yellow arrow `#DEDC6D`.

Using a PNG on the device is still possible, if a screen ever needs it:
1. Put it in `main/graphics/assets/`.
2. From `electronicnose/`, run `python main/graphics/tools/png2rgb565.py --bg 0C0E17` (needs `pip install pillow`).
3. Draw it with `gfx::image("name")`.

Numbered frames (`x1.png`..`x5.png`) are stored as one background plus small changed-area crops. A full-screen image costs about 113 KB of flash, out of about 790 KB.

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