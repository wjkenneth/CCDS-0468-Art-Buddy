#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

// Function prototypes
void start();
void mainMenu();
void mainMenuChangeSelection(int curIndex, int nextIndex);
void animationMenu();
void animationMenuChangeSelection(int curIndex, int nextIndex);
void gradientMenu();
void gradientMenuChangeSelection(int curIndex, int nextIndex);
void settingsMenu();
void settingsMenuChangeSelection(int curIndex, int nextIndex);
void changeSettingsMenu();
void changeSettingsMenuChangeSelection(int curIndex, int nextIndex);
void drawMenuOption(int x, int y, int w, int h, int color, String text);

struct Rectangle
{
    int x;
    int y;
};

// Declare global variables
extern Rectangle rectangles[4];

#endif