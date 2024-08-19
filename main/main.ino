#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#define FASTLED_RP2040_CLOCKLESS_M0_FALLBACK 0
#include <FastLED.h>
#include <SPI.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include "main.h"
#include "animations.h"
#include "motor.h"
#include "display.h"
#include "controls.h"

unsigned long last = 0;

// Initialize the Adafruit_ST7789 display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Initialize the NeoPixel strip object
CRGB leds[NUM_LEDS];

// Initialize the UIState object
UIState uiState;

void setup()
{
  // Init serial communication
  Serial.begin(9600);
  Serial.println("Initialized");

  // Init Button Inputs
  pinMode(KEY_A, INPUT_PULLUP);
  pinMode(KEY_B, INPUT_PULLUP);
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_RIGHT, INPUT_PULLUP);
  pinMode(KEY_CTRL, INPUT_PULLUP);

  // Init TFT Display
  tft.init(135, 240);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);

  // Init Switch and Motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);

  // Initialize the UI state
  EEPROM.begin(sizeof(UIState));

  if (!loadUIState())
  {
    uiState = UIState();
    uiState.currentState = START_SCREEN;
  }

  // Initialize the LED strip
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  // Initialize file system
  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount filesystem!");
    return;
  }

  Serial.println("Filesystem mounted successfully!");

  // Start the UI
  if (uiState.currentState != ANIMATION_MENU)
  {
    start();
    uiState.mainMenuSelection = 0;
    uiState.mainMenuPage = 0;
    uiState.animationMenuSelection = 0;
    uiState.gradientMenuSelection = 0;
    uiState.settingsMenuSelection = 0;
    uiState.currentState = START_SCREEN;
  }
  else
  {
    animationMenu();
  }
}

void loop()
{
  // Handle menu states
  switch (uiState.currentState)
  {
  case START_SCREEN:
    handleStartScreen();
    break;
  case MAIN_MENU:
    handleMainMenu();
    break;
  case ANIMATION_MENU:
    handleAnimationMenu();
    break;
  case GRADIENT_MENU:
    handleGradientMenu();
    break;
  case SETTINGS_MENU:
    handleSettingsMenu();
    break;
  case CHANGE_SETTINGS:
    handleChangeSettingsMenu();
    break;
  }
}

void handleSaveSettings()
{
  if (digitalRead(KEY_A) == LOW)
  {
    saveUIState();
    uiState.currentState = SETTINGS_MENU;
    settingsMenu();
  }
}

bool loadUIState()
{
  UIState loadedState;
  EEPROM.get(EEPROM_ADDRESS, loadedState);

  if (loadedState.validationToken == 0xABCD1234)
  {
    uiState = loadedState;
    return true;
  }
  return false;
}

void saveUIState()
{
  EEPROM.put(EEPROM_ADDRESS, uiState);
  EEPROM.commit();
  Serial.println("Saved UI State");
}

void handleStartScreen()
{
  handleChangeMenu(KEY_A, uiState.currentState, MAIN_MENU, mainMenu);
  handleChangeMenu(KEY_UP, uiState.currentState, SETTINGS_MENU, settingsMenu);
  uiState.mainMenuSelection = 0;
  uiState.mainMenuPage = 0;
  uiState.animationMenuSelection = 0;
  uiState.gradientMenuSelection = 0;
  uiState.settingsMenuSelection = 0;
}

void handleMainMenu()
{
  uiState.gradientMenuSelection = 0;
  uiState.animationMenuSelection = 0;

  // Joystick Controls
  handleRight(uiState.mainMenuSelection, 3, mainMenuChangeSelection);
  handleLeft(uiState.mainMenuSelection, mainMenuChangeSelection);
  handleUp(uiState.mainMenuSelection, mainMenuChangeSelection, 2);
  handleDown(uiState.mainMenuSelection, 3, mainMenuChangeSelection, 2);

  // Selection Controls
  handleChangeMenu(KEY_A, uiState.currentState, ANIMATION_MENU, animationMenu);
  handleChangeMenu(KEY_CTRL, uiState.currentState, START_SCREEN, start);

  // Page Controls
  handleNextPage(uiState.mainMenuPage, uiState.mainMenuSelection, 4, mainMenu);
  handlePreviousPage(uiState.mainMenuPage, uiState.mainMenuSelection, mainMenu);
}

void handleAnimationMenu()
{
  // Gradient Specific Controls
  if (uiState.mainMenuSelection == 1 || uiState.mainMenuSelection == 3 || uiState.mainMenuSelection == 5)
  {
    // Joystick Controls
    handleRight(uiState.animationMenuSelection, 2, animationMenuChangeSelection);
    handleUp(uiState.animationMenuSelection, animationMenuChangeSelection);
    handleDown(uiState.animationMenuSelection, 2, animationMenuChangeSelection);
    if (uiState.animationMenuSelection == 1)
    {
      handleChangeMenu(KEY_A, uiState.currentState, RUN_MENU, runMenu);
    }
    if (uiState.animationMenuSelection == 2)
    {
      handleChangeMenu(KEY_A, uiState.currentState, GRADIENT_MENU, gradientMenu);
    }
  }
  else
  {
    // Joystick Controls
    handleRight(uiState.animationMenuSelection, 1, animationMenuChangeSelection);
    if (uiState.animationMenuSelection == 1)
    {
      handleChangeMenu(KEY_A, uiState.currentState, RUN_MENU, runMenu);
    }
  }

  // Joystick Controls for both cases
  handleLeft(uiState.animationMenuSelection, animationMenuChangeSelection);

  // Selection Controls for both cases
  if (uiState.animationMenuSelection == 0)
  {
    handleChangeMenu(KEY_A, uiState.currentState, MAIN_MENU, mainMenu);
  }
}

void handleGradientMenu()
{
  // Joystick Controls
  handleRight(uiState.gradientMenuSelection, 3, gradientMenuChangeSelection);
  handleLeft(uiState.gradientMenuSelection, gradientMenuChangeSelection);
  handleUp(uiState.gradientMenuSelection, gradientMenuChangeSelection, 2);
  handleDown(uiState.gradientMenuSelection, 3, gradientMenuChangeSelection, 2);

  // Selection Controls
  handleChangeMenu(KEY_A, uiState.currentState, ANIMATION_MENU, animationMenu);
  handleChangeMenu(KEY_B, uiState.currentState, ANIMATION_MENU, animationMenu);
}

void handleSettingsMenu()
{
  FastLED.clear();
  FastLED.show();

  // Joystick Controls
  handleRight(uiState.settingsMenuSelection, 3, settingsMenuChangeSelection);
  handleLeft(uiState.settingsMenuSelection, settingsMenuChangeSelection);
  handleUp(uiState.settingsMenuSelection, settingsMenuChangeSelection, 2);
  handleDown(uiState.settingsMenuSelection, 3, settingsMenuChangeSelection, 2);

  // Selection Controls
  handleChangeMenu(KEY_A, uiState.currentState, CHANGE_SETTINGS, changeSettingsMenu);
  handleChangeMenu(KEY_B, uiState.currentState, START_SCREEN, start);

  unsigned long currentTime = millis();

  if (digitalRead(KEY_UP) == LOW)
  {
    if (currentTime - last > 200)
    {
      Serial.println("A pressed");
      // spin_motor(uiState.settings[0], uiState.settings[1], uiState.setting[2]);
      listDir();
      displayBMP("/linear-gradient.bmp");
      last = currentTime;
    }
  }
}

void handleChangeSettingsMenu()
{
  // -
  handleUp(uiState.settings[uiState.settingsMenuSelection], changeSettingsMenuChangeSelection, settings_increment[uiState.settingsMenuSelection]);
  // +
  handleDown(uiState.settings[uiState.settingsMenuSelection], settings_max[uiState.settingsMenuSelection], changeSettingsMenuChangeSelection, settings_increment[uiState.settingsMenuSelection]);

  // Selection Controls
  handleSaveSettings();
  handleChangeMenu(KEY_B, uiState.currentState, SETTINGS_MENU, settingsMenu);
  handleChangeMenu(KEY_CTRL, uiState.currentState, CHANGE_SETTINGS, spinMotor);
}

void displayBrightnessLevel()
{
  FastLED.setBrightness(uiState.settings[0]);

  fill_solid(leds, NUM_LEDS, CRGB::Red);

  FastLED.show();
}

void runMenu()
{

  unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long duration = uiState.settings[1];

  // Display running animation
  tft.fillScreen(ST77XX_RED);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(90, 60);
  tft.print("Running");
  tft.setCursor(85, 80);
  tft.print("Animation");

  FastLED.setBrightness(uiState.settings[0]);

  // Run the selected animation
  while (true)
  {
    // Run code here
    animationFunctions[uiState.mainMenuPage * 4 + uiState.mainMenuSelection]();

    // Check if the duration has passed
    currentTime = millis();
    delay(100);
    runMotor();
    if (currentTime - startTime > duration || digitalRead(KEY_B) == LOW)
    {
      break;
    }
  }

  stopMotor();
  FastLED.clear();
  FastLED.show();

  // Return to the animation menu
  uiState.currentState = ANIMATION_MENU;
  animationMenu();
  saveUIState();
}

void listDir()
{
  File root = LittleFS.open("/", "r");
  if (!root)
  {
    Serial.println("Failed to open root directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Root is not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("DIR : ");
      Serial.println(file.name());
    }
    else
    {
      Serial.print("FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void displayBMP(const char *filename)
{
  File bmpFile = LittleFS.open(filename, "r");

  if (!bmpFile)
  {
    Serial.println("Failed to open BMP file");
    return;
  }

  bmpFile.seek(18);
  int width;
  bmpFile.read((uint8_t *)&width, 4);

  bmpFile.seek(22);
  int height;
  bmpFile.read((uint8_t *)&height, 4);

  // Calculate row size (including padding)
  int rowSize = ((width * 3 + 3) & ~3);

  // Calculate expected pixel data size
  int pixelDataSize = rowSize * height;

  // Get the total file size
  int fileSize = bmpFile.size();

  // Calculate the header size
  int headerSize = fileSize - pixelDataSize;

  // Reading the file column by column
  for (int col = 0; col < width + 0; col++)
  {
    for (int row = 0; row < height; row++)
    {
      int pos = headerSize + (height - 1 - row) * rowSize + col * 3;
      bmpFile.seek(pos);

      uint8_t bmpLine[3];
      bmpFile.read(&bmpLine[0], 3);

      leds[row] = CRGB(bmpLine[2], bmpLine[1], bmpLine[0]);
    }

    FastLED.show();
    delay(20);
  }

  bmpFile.close();
}