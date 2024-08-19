#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "main.h"
#include "animations.h"
#include "display.h"

// Define the rectangles array
Rectangle rectangles[4] = {
    {15, 30},  // Top Left
    {125, 30}, // Top Right
    {15, 75},  // Bottom Left
    {125, 75}  // Bottom Right
};

void start()
{
  // Draw start screen
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);
  tft.setCursor(60, 10);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.print("Art Buddy LED Painter");

  // Draw instructions
  tft.setTextColor(ST77XX_WHITE);

  tft.setCursor(10, 35);
  tft.print("<- Settings [ UP ]");

  tft.setCursor(10, 50);
  tft.print("<- Back to Start Screen [ CTRL ]");

  tft.setCursor(10, 65);
  tft.print("<- Move [ JOYSTICK ] ");

  tft.setCursor(145, 85);
  tft.print("Select [ A ] ->");

  tft.setCursor(125, 100);
  tft.print("Next Page [ B ] ->");

  // Draw continue prompt
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(35, 115);
  tft.setTextSize(2);
  tft.print("[A] TO CONTINUE");
}

void mainMenu()
{
  int animationIndex = uiState.mainMenuPage * 4;
  int colorIndex = uiState.mainMenuSelection % 4;
  int selectionColor[] = {ST77XX_RED, ST77XX_RED, ST77XX_RED, ST77XX_RED};
  selectionColor[colorIndex] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ A ] Select Animation");

  // Draw menu options
  drawMenuOption(15, 30, 100, 40, selectionColor[0], all_animations[animationIndex]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], all_animations[animationIndex + 1]);
  drawMenuOption(15, 75, 100, 40, selectionColor[2], all_animations[animationIndex + 2]);
  drawMenuOption(125, 75, 100, 40, selectionColor[3], all_animations[animationIndex + 3]);

  // Navigation
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  tft.setCursor(10, 120);
  // tft.print("<- [ CTRL ] Start");

  tft.setCursor(118, 120);
  tft.print(String(uiState.mainMenuPage + 1));

  if (uiState.mainMenuPage != 4)
  {
    tft.setCursor(150, 120);
    tft.print("Next [ B ] ->");
  }
}

void mainMenuChangeSelection(int curIndex, int nextIndex)
{
  int selectionIndex = uiState.mainMenuPage * 4;

  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_RED, all_animations[selectionIndex + curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, all_animations[selectionIndex + nextIndex]);
}

void animationMenu()
{
  int selectionIndex = uiState.mainMenuPage * 4 + uiState.mainMenuSelection;
  int selectionColor[] = {ST77XX_CYAN, ST77XX_CYAN, ST77XX_CYAN};
  selectionColor[uiState.animationMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title and menu options
  tft.setCursor(15, 15);
  tft.setTextSize(1.5);

  if (selectionIndex == 1 || selectionIndex == 3 || selectionIndex == 5)
  {
    tft.print("Animation: " + String(all_animations[selectionIndex]) + " - " + String(all_gradients[uiState.gradientMenuSelection]));

    drawMenuOption(15, 30, 100, 40, selectionColor[0], run_ani_menu[0]);
    drawMenuOption(125, 30, 100, 40, selectionColor[1], run_ani_menu[1]);
    drawMenuOption(15, 75, 210, 40, selectionColor[2], run_ani_menu[2]);
  }
  else
  {
    tft.print("Animation: " + String(all_animations[selectionIndex]));

    drawMenuOption(15, 30, 100, 86, selectionColor[0], "Back");
    drawMenuOption(125, 30, 100, 86, selectionColor[1], "Run");
  }
}

void animationMenuChangeSelection(int curIndex, int nextIndex)
{
  int selectionIndex = uiState.mainMenuPage * 4 + uiState.mainMenuSelection;

  // Change cur to unselected color
  if (selectionIndex == 1 || selectionIndex == 3 || selectionIndex == 5)
  {
    if (curIndex == 2)
    {
      drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 210, 40, ST77XX_CYAN, run_ani_menu[curIndex]);
    }
    else
    {
      drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_CYAN, run_ani_menu[curIndex]);
    }
  }
  else
  {
    drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 86, ST77XX_CYAN, run_ani_menu[curIndex]);
  }

  // Change next to selected color
  if (selectionIndex == 1 || selectionIndex == 3 || selectionIndex == 5)
  {
    if (nextIndex == 2)
    {
      drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 210, 40, ST77XX_GREEN, run_ani_menu[nextIndex]);
    }
    else
    {
      drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, run_ani_menu[nextIndex]);
    }
  }
  else
  {
    drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 86, ST77XX_GREEN, run_ani_menu[nextIndex]);
  }
}

void gradientMenu()
{
  int selectionColor[] = {ST77XX_RED, ST77XX_RED, ST77XX_RED, ST77XX_RED};
  selectionColor[uiState.gradientMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ A ] Select Gradient");

  // Draw menu options
  drawMenuOption(15, 30, 100, 40, selectionColor[0], all_gradients[0]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], all_gradients[1]);
  drawMenuOption(15, 75, 100, 40, selectionColor[2], all_gradients[2]);
  drawMenuOption(125, 75, 100, 40, selectionColor[3], all_gradients[3]);

  // Draw instructions
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(165, 120);
  tft.print("Back [ B ]");
}

void gradientMenuChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_RED, all_gradients[curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, all_gradients[nextIndex]);
}

void settingsMenu()
{
  int selectionColor[] = {ST77XX_RED, ST77XX_RED, ST77XX_RED, ST77XX_RED};
  selectionColor[uiState.settingsMenuSelection] = ST77XX_GREEN;

  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ A ] Select Settings");

  // Draw menu options
  drawMenuOption(15, 30, 100, 40, selectionColor[0], settings_menu[0]);
  drawMenuOption(125, 30, 100, 40, selectionColor[1], settings_menu[1]);
  drawMenuOption(15, 75, 100, 40, selectionColor[2], settings_menu[2]);
  drawMenuOption(125, 75, 100, 40, selectionColor[3], settings_menu[3]);

  // Draw instructions
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(165, 120);
  tft.print("Back [ B ]");
}

void settingsMenuChangeSelection(int curIndex, int nextIndex)
{
  // Change cur to unselected color
  drawMenuOption(rectangles[curIndex].x, rectangles[curIndex].y, 100, 40, ST77XX_RED, settings_menu[curIndex]);

  // Change next to selected color
  drawMenuOption(rectangles[nextIndex].x, rectangles[nextIndex].y, 100, 40, ST77XX_GREEN, settings_menu[nextIndex]);
}

void changeSettingsMenu()
{
  // Draw menu
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 0, 240, 135, ST77XX_BLUE);

  // Draw title
  tft.setCursor(15, 10);
  tft.setTextSize(1);
  tft.print("[ UP ] [ DOWN ] Change Settings");

  // Draw settings
  tft.setCursor(15, 60);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(uiState.settings[uiState.settingsMenuSelection]));

  // Draw instructions

  tft.setTextColor(ST77XX_WHITE);

  if (uiState.settingsMenuSelection == 2)
  {
    tft.setCursor(15, 100);
    tft.setTextSize(1);
    tft.print("[ 0 ] Anticlockwise [ 1 ] Clockwise");
  }

  tft.setCursor(15, 120);
  tft.setTextSize(1);
  tft.print("[ A ] Save [ B ] Back");
}

void changeSettingsMenuChangeSelection(int curIndex, int nextIndex)
{
  // Update settings value
  tft.setCursor(15, 60);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(curIndex));

  tft.setCursor(15, 60);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print(String(settings_menu[uiState.settingsMenuSelection]) + " " + String(nextIndex));
}

void drawMenuOption(int x, int y, int w, int h, int color, String text)
{
  tft.drawRect(x, y, w, h, color);
  tft.setCursor(x + 15, y + 10);
  tft.setTextColor(color);
  tft.setTextSize(1);
  tft.print(text);
}