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