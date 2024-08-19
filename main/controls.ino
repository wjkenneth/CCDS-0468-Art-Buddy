#include <Arduino.h>
#include "main.h"

const unsigned long DEBOUNCE_DELAY = 200;
unsigned long lastDebounceTime = 0;

void handleRight(int &currentSelection, int maxSelection, void (*updateMenu)(int curIndex, int nextIndex))
{
    unsigned long currentTime = millis();

    if (digitalRead(KEY_RIGHT) == LOW && currentSelection < maxSelection)
    {
        if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
        {
            int previousSelection = currentSelection;
            currentSelection++;
            updateMenu(previousSelection, currentSelection);

            lastDebounceTime = currentTime;

            Serial.println(String(previousSelection) + " -> " + String(currentSelection));
        }
    }
}

void handleLeft(int &currentSelection, void (*updateMenu)(int curIndex, int nextIndex))
{
    unsigned long currentTime = millis();

    if (digitalRead(KEY_LEFT) == LOW && currentSelection > 0)
    {
        if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
        {
            int previousSelection = currentSelection;
            currentSelection--;
            updateMenu(previousSelection, currentSelection);

            lastDebounceTime = currentTime;

            Serial.println(String(previousSelection) + " -> " + String(currentSelection));
        }
    }
}

void handleUp(int &currentSelection, void (*updateMenu)(int curIndex, int nextIndex), int increment)
{
    unsigned long currentTime = millis();

    if (digitalRead(KEY_UP) == LOW && currentSelection >= increment)
    {
        if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
        {
            int previousSelection = currentSelection;
            currentSelection -= increment;
            updateMenu(previousSelection, currentSelection);
            lastDebounceTime = currentTime;

            Serial.println(String(previousSelection) + " -> " + String(currentSelection));
        }
    }

    if (uiState.currentState == CHANGE_SETTINGS && uiState.settingsMenuSelection == 0)
    {
        displayBrightnessLevel();
    }
}

void handleDown(int &currentSelection, int maxSelection, void (*updateMenu)(int curIndex, int nextIndex), int increment)
{
    unsigned long currentTime = millis();

    if (digitalRead(KEY_DOWN) == LOW && currentSelection <= maxSelection - increment)
    {
        if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
        {
            int previousSelection = currentSelection;
            currentSelection += increment;
            updateMenu(previousSelection, currentSelection);

            lastDebounceTime = currentTime;

            Serial.println(String(previousSelection) + " -> " + String(currentSelection));
        }
    }

    if (uiState.currentState == CHANGE_SETTINGS && uiState.settingsMenuSelection == 0)
    {
        Serial.println("Display Brightness Level");
        displayBrightnessLevel();
    }
}

void handleNextPage(int &currentPage, int &currentSelection, int maxPage, void (*updateMenu)())
{
    if (digitalRead(KEY_B) == LOW && currentPage != maxPage)
    {
        currentPage++;
        currentSelection = 0;
        updateMenu();
    }
}

void handlePreviousPage(int &currentPage, int &currentSelection, void (*updateMenu)())
{
    if (digitalRead(KEY_A) == LOW && currentPage != 0)
    {
        currentPage--;
        currentSelection = 0;
        updateMenu();
    }
}

void handleChangeMenu(int key, MenuState &currentState, MenuState newState, void (*updateMenu)())
{
    if (digitalRead(key) == LOW)
    {
        currentState = newState;
        updateMenu();
    }
}