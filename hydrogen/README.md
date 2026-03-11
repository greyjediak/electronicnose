README for hydrogen sensor
This code is pretty simple. You need to use the arduino IDE version 2.3.8 or newer, and you need to select the board "Seeed XIAO nRF52840 Plus" This board can be downloaded from this link: 

In this code, what's happening is that the microcontroller is reading the analog input from pin A0 (or D0) constantly, and then reporting the rolling average over the last second of measurements on the serial monitor. If you type "z" into the serial monitor, it will zero the reading.
