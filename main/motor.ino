#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "main.h"
#include "motor.h"
#include "display.h"

void spinMotor()
{
    tft.fillScreen(ST77XX_ORANGE);  // Fill the screen with a color (e.g., black)
    tft.setTextColor(ST77XX_WHITE); // Set text color
    tft.setTextSize(2);             // Set text size
    tft.setCursor(90, 60);          // Set the cursor position
    tft.print("Spinning");          // Print text on the screen
    tft.setCursor(100, 80);         // Set the cursor position
    tft.print("Motor");             // Print text on the screen
    unsigned long startTime = millis();
    unsigned long currentTime = millis();

    int duration = uiState.settings[1]; // 10750 for dual, 5400 for 1m    1 rot 8000-8200
    Serial.println("Spinning motor" + String(uiState.settings[2]) + " at speed " + String(uiState.settings[3]) + " for " + String(uiState.settings[1]) + " milliseconds");
    while (currentTime - startTime <= duration)
    {
        currentTime = millis();
        runMotor();
    }

    stopMotor();

    // Return to the animation menu
    uiState.currentState = CHANGE_SETTINGS;
    changeSettingsMenu();
}

void runMotor()
{
    digitalWrite(EN, HIGH);
    if (uiState.settings[2] == 1)
    {
        analogWrite(IN1, uiState.settings[3]);
        analogWrite(IN2, 0);
    }
    else if (uiState.settings[2] == 0)
    {
        analogWrite(IN1, 0);
        analogWrite(IN2, uiState.settings[3]);
    }
}

void stopMotor()
{

    digitalWrite(EN, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
}
