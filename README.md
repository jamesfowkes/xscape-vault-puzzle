# xscape-vault-puzzle
Arduino project for a two-player vault doors puzzle

## Required Libraries

### TaskAction from https://github.com/blackhack/ArduLibraries

To install:
  - Click "Clone or Download" and then "Download ZIP"
  - Unzip the ZIP file anywhere
  - In Arduino IDE, click Sketch... Include Library... Add ZIP Library
  - Select the TaskAction folder in the unzipped folder and click OK.

### Adafruit NeoPixel library

To install:
 - In Arduino IDE, click Sketch... Include Library... Manage Libraries.
 - In the search bar, enter "adafruit neopixel".
 - Select the "Adafruit NeoPixel" library in the list and click "Install" (there are a few libraries with similar names, makes sure you select the correct one)

### Keypad

To install:
 - In Arduino IDE, click Sketch... Include Library... Manage Libraries.
 - In the search bar, enter "keypad".
 - Scroll down the list to find "Keypad by Mark Stanley, Alexander Brevig". Select it and click "Install"

## Settings

All end-user settings are in settings.h.

You can modify keypad codes, dial combinations, timeouts and display colours.

After changing a setting in code, the Arduino program must be re-uploaded. 
 