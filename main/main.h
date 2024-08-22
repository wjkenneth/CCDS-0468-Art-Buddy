#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <FastLED.h>
#include <SPI.h>
#include "animations.h"

// Define button pins
#define KEY_A 15
#define KEY_B 17
#define KEY_DOWN 18
#define KEY_UP 2
#define KEY_RIGHT 20
#define KEY_LEFT 16
#define KEY_CTRL 3

// Define TFT pins
#define TFT_CS 9
#define TFT_RST 12
#define TFT_DC 8
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_BL 13

// Define NeoPixel pins
#define LED_PIN 22

// Define switch pin
#define SWITCH 13

// Define Motor pins
#define EN 5
#define IN1 6
#define IN2 7

// Define save address
#define EEPROM_ADDRESS 0

// Define menu states
enum MenuState
{
    START_SCREEN,
    MAIN_MENU,
    ANIMATION_MENU,
    GRADIENT_MENU,
    RUN_MENU,
    SETTINGS_MENU,
    CHANGE_SETTINGS,
    BITMAP_MENU,
    USB_MODE,
    STORAGE_MODE,
    BITMAP_ANIMATION_MENU,
    BITMAP_RUN_MENU
};

struct UIState
{
    uint32_t validationToken = 0xABCD1299;
    int mainMenuSelection = 0;
    int mainMenuPage = 0;
    int animationMenuSelection = 0;
    int gradientMenuSelection = 0;
    int settingsMenuSelection = 0;
    int bitmapMenuSelection = 0;
    int storageModeSelection = 0;
    int bitmapAnimationMenuSelection = 0;
    int settings[4] = {255, 7000, 1, 255};
    MenuState currentState = START_SCREEN;
};

// Settings Menu
static const char settings_1[] PROGMEM = "Brightness";
static const char settings_2[] PROGMEM = "M Duration";
static const char settings_3[] PROGMEM = "M Direction";
static const char settings_4[] PROGMEM = "M Speed";

static const char *const settings_menu[] PROGMEM = {
    settings_1, settings_2, settings_3, settings_4};

// Settings Menu Max Values
static const int settings_max[] PROGMEM = {
    255, 20000, 1, 255};

// Settings Menu Increment Values
static const int settings_increment[] PROGMEM = {
    5, 100, 1, 5};

// Function prototypes
void handleSaveSettings();
void saveUIState();
bool loadUIState();
void handleStartScreen();
void handleMainMenu();
void handleAnimationMenu();
void handleGradientMenu();
void handleSettingsMenu();
void handleChangeSettingsMenu();
void handleBitmapMenu();
void handleUsbMode();
void handleStorageMode();
void handleBitmapAnimationMenu();
void displayBrightnessLevel();
void runMenu();
void bitmap();

// Declare global variables
extern Adafruit_ST7789 tft;
extern UIState uiState;
extern CRGB leds[NUM_LEDS];

#endif