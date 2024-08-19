#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>

#define NUM_LEDS 144
#define OFF 0

// Function prototypes
void Fire();
void Beat();
void Fade();
void Blur();
void ColorChange();
void MovingDot();
void Cylon();
void Boom();
void FirstLight();
void Strange();
void RainbowCycle();
void OuterS();
void RainbowLines();
void JagWhite();
void Stars();
void Pride();
void ColorFrenzy();
void Histo();
void RandHisto();
void Bang();

// Type definition for animation function pointer
typedef void (*AnimationFunction)();

// Array of function pointers
extern const AnimationFunction animationFunctions[];

// Animations
static const char ani_1[] PROGMEM = "Fire";
static const char ani_2[] PROGMEM = "Beat";
static const char ani_3[] PROGMEM = "Fade";
static const char ani_4[] PROGMEM = "Blur";
static const char ani_5[] PROGMEM = "Color Change";
static const char ani_6[] PROGMEM = "Moving Dot";
static const char ani_7[] PROGMEM = "Cylon";
static const char ani_8[] PROGMEM = "Boom";
static const char ani_9[] PROGMEM = "FirstLight";
static const char ani_10[] PROGMEM = "Strange";
static const char ani_11[] PROGMEM = "RainbowCycle";
static const char ani_12[] PROGMEM = "OuterS";
static const char ani_13[] PROGMEM = "RaingbowLines";
static const char ani_14[] PROGMEM = "JagWhite";
static const char ani_15[] PROGMEM = "Stars";
static const char ani_16[] PROGMEM = "Pride";
static const char ani_17[] PROGMEM = "ColorFrenzy";
static const char ani_18[] PROGMEM = "Histo";
static const char ani_19[] PROGMEM = "RandHisto";
static const char ani_20[] PROGMEM = "Bang";

static const char *const all_animations[] PROGMEM = {
    ani_1, ani_2, ani_3, ani_4, ani_5, ani_6, ani_7, ani_8, ani_9, ani_10,
    ani_11, ani_12, ani_13, ani_14, ani_15, ani_16, ani_17, ani_18, ani_19, ani_20};

// Gradient Menu
static const char grad_1[] PROGMEM = "Gradient 1";
static const char grad_2[] PROGMEM = "Gradient 2";
static const char grad_3[] PROGMEM = "Gradient 3";
static const char grad_4[] PROGMEM = "Gradient 4";

static const char *const all_gradients[] PROGMEM = {
    grad_1, grad_2, grad_3, grad_4};

// Run Animation Menu
static const char run_ani_1[] PROGMEM = "Back";
static const char run_ani_2[] PROGMEM = "Run";
static const char run_ani_3[] PROGMEM = "Config Ani";

static const char *const run_ani_menu[] PROGMEM = {
    run_ani_1, run_ani_2, run_ani_3};

// Calibration Menu
static const char cali_1[] PROGMEM = "Clockwise";
static const char cali_2[] PROGMEM = "Anti-Clockwise";
static const char cali_3[] PROGMEM = "Mini Clockwise";
static const char cali_4[] PROGMEM = "Back";

static const char *const cali_menu[] PROGMEM = {
    cali_1, cali_2, cali_3, cali_4};

#endif
