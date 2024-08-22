# CCDS-0468 Art Buddy

### Board 

Open up the Arduino IDE and go to File->Preferences.

In the dialog that pops up, enter the following URL in the “Additional Boards Manager URLs” field: 

https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

![Additional Boards Manager](https://arduino-pico.readthedocs.io/en/latest/_images/install1.png)

Hit OK to close the dialog.

Go to Tools->Boards->Board Manager in the IDE

Type "pico" in the search box and select "Add":


Additonal information can be found [here](https://arduino-pico.readthedocs.io/en/latest/install.html)

### Libraries

adafruit/Adafruit ST7735 and ST7789 Library@^1.10.4

adafruit/Adafruit NeoPixel@^1.12.3

fastled/FastLED@^3.7.3

### Tools
Flash Size: 2MB (Sketch: 1MB, FS: 1MB)

USB Stack: Adafruit TinyUSB

![tools](/images/Tools.png)

### Flashing 

Arduino IDE 2.x users
- Please see the [arduino-littlefs-plugin](https://github.com/earlephilhower/arduino-littlefs-upload) for use under IDE 2.2.1 or later

Installation
- Make sure you use one of the supported versions of Arduino IDE and have the Arduino-Pico core (https://github.com/earlephilhower/arduino-pico) installed.
- Download the tool archive from (https://github.com/earlephilhower/arduino-pico-littlefs-plugin/releases).
- In your Arduino sketchbook directory, create tools directory if it doesn't exist yet.
- Unpack the tool into tools directory (the path will look like `<home_dir>/Arduino/tools/PicoLittleFS/tool/picolittlefs.jar)`.
- Restart Arduino IDE.
- On the OS X create the tools directory in `~/Documents/Arduino/` and unpack the files there.

Usage
- Open a sketch (or create a new one and save it).
- Go to sketch directory (choose Sketch > Show Sketch Folder).
- Create a directory named `data` and any files you want in the file system there.
- Make sure you have selected a board and port.
- Close the Serial Monitor, or else the tool will not be able to upload the filesystem.
- Select `Tools > Pico LittleFS Data Upload` menu item. This should start uploading the files into flash file system.
  When done, IDE status bar will display LittleFS Image Uploaded message. Might take a few minutes for large file system sizes.
