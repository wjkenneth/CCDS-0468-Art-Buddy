#include <Arduino.h>
#include <FastLED.h>
#include <SPI.h>
#include "main.h"
#include "animations.h"

// Fire animation
#define COOLING 55
#define SPARKING 120
#define FRAMES_PER_SECOND 30
bool gReverseDirection = false;
CRGBPalette16 gPal = HeatColors_p;
uint8_t heat[NUM_LEDS];   // Array of temperature readings at each simulation cell
bool initialized = false; // Flag to check if the heat array is initialized

void fadeall()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i].nscale8(250);
    }
}

void Fire()
{
    if (!initialized)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            heat[i] = random(100, 150); // Random initial heat values
        }
        initialized = true; // Set the flag to true after initialization
    }

    // Add entropy to random number generator; we use a lot of it.
    random16_add_entropy(random());

    // Array of temperature readings at each simulation cell
    // static uint8_t heat[NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < NUM_LEDS; i++)
    {
        heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = NUM_LEDS - 1; k >= 2; k--)
    {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKING)
    {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < NUM_LEDS; j++)
    {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        uint8_t colorindex = scale8(heat[j], 240);
        CRGB color = ColorFromPalette(gPal, colorindex);
        int pixelnumber;
        if (gReverseDirection)
        {
            pixelnumber = (NUM_LEDS - 1) - j;
        }
        else
        {
            pixelnumber = j;
        }
        leds[pixelnumber] = color;
    }

    FastLED.show(); // display this frame
    delay(1000 / FRAMES_PER_SECOND);
}

void Beat()
{
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

    if (uiState.gradientMenuSelection == 0)
    {
        leds[sinBeat] = CRGB::Blue;
        leds[sinBeat2] = CRGB::Red;
        leds[sinBeat3] = CRGB::White;
    }

    if (uiState.gradientMenuSelection == 1)
    {
        leds[sinBeat] = CRGB::HotPink;
        leds[sinBeat2] = CRGB::MediumOrchid;
        leds[sinBeat3] = CRGB::MediumPurple;
    }

    if (uiState.gradientMenuSelection == 2)
    {
        leds[sinBeat] = CRGB::Lime;
        leds[sinBeat2] = CRGB::SpringGreen;
        leds[sinBeat3] = CRGB::DarkGreen;
    }

    if (uiState.gradientMenuSelection == 3)
    {
        leds[sinBeat] = CRGB::Aqua;
        leds[sinBeat2] = CRGB::DeepSkyBlue;
        leds[sinBeat3] = CRGB::Blue;
    }

    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
}

void Fade()
{
    EVERY_N_MILLISECONDS(10)
    {
        // Switch on an LED at random, choosing a random color from the palette
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(gPal, random8(), 255, LINEARBLEND);
    }

    // Fade all LEDs down by 1 in brightness each time this is called
    fadeToBlackBy(leds, NUM_LEDS, 1);

    FastLED.show();
}

void Blur()
{
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3 = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

    if (uiState.gradientMenuSelection == 0)
    {
        leds[sinBeat] = CRGB::Blue;
        leds[sinBeat2] = CRGB::Red;
        leds[sinBeat3] = CRGB::White;
    }

    if (uiState.gradientMenuSelection == 1)
    {
        leds[sinBeat] = CRGB::HotPink;
        leds[sinBeat2] = CRGB::MediumOrchid;
        leds[sinBeat3] = CRGB::MediumPurple;
    }

    if (uiState.gradientMenuSelection == 2)
    {
        leds[sinBeat] = CRGB::Lime;
        leds[sinBeat2] = CRGB::SpringGreen;
        leds[sinBeat3] = CRGB::DarkGreen;
    }

    if (uiState.gradientMenuSelection == 3)
    {
        leds[sinBeat] = CRGB::Aqua;
        leds[sinBeat2] = CRGB::DeepSkyBlue;
        leds[sinBeat3] = CRGB::Blue;
    }

    EVERY_N_MILLISECONDS(10)
    {
        for (int i = 0; i < 4; i++)
        {
            blur1d(leds, NUM_LEDS, 50);
        }
    }

    FastLED.show();
}

void ColorChange()
{
    int i = 0;
    for (int j = 0; j < 6; j++)
    {
        if (i == 0)
            fill_solid(leds, NUM_LEDS, CRGB::Red);
        if (i == 1)
            fill_solid(leds, NUM_LEDS, CRGB::Yellow);
        if (i == 2)
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
        if (i == 3)
            fill_solid(leds, NUM_LEDS, CRGB::HotPink);
        if (i == 4)
            fill_solid(leds, NUM_LEDS, CRGB::MediumOrchid);
        if (i == 5)
            fill_solid(leds, NUM_LEDS, CRGB::MediumPurple);

        i++;
        FastLED.show();
        if (i == 6)
            i = 0;
        if (digitalRead(SWITCH) == HIGH) break;
        delay(50);
        if (digitalRead(SWITCH) == HIGH) break;
    }
}

void MovingDot()
{
    uint16_t sinBeat = beatsin16(30, 0, NUM_LEDS - 1, 0, 0); //NUM_LEDS - 1

    //leds[sinBeat] = CRGB::Blue;
    if (uiState.animationMenuSelection==0)leds[sinBeat].setRGB(255,128,0);
    if (uiState.animationMenuSelection==1)leds[sinBeat].setRGB(255,0,0);
    if (uiState.animationMenuSelection==2)leds[sinBeat].setRGB(0,255,0);
    if (uiState.animationMenuSelection==3)leds[sinBeat].setRGB(0,0,255);

    fadeToBlackBy(leds, NUM_LEDS, 10); //NUM_LEDS 

    EVERY_N_MILLISECONDS(10){
        Serial.println(sinBeat);
    }

    FastLED.show();
}

void Cylon()
{
    static uint8_t hue = 0;
    // First slide the led in one direction
    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (digitalRead(SWITCH) == HIGH) break;

        // Set the i'th led to red
        leds[i] = CHSV(hue++, 255, 255);

        // Show the leds
        FastLED.show();

        // now that we've shown the leds, reset the i'th led to black
        fadeall();

        if (digitalRead(SWITCH) == HIGH) break;
    }
}
void Boom()
{
    int red = 255;
    int lred = 120;
    int green = 0;
    int blue = 0;

    EVERY_N_MILLISECONDS(200)
    {
        int cutoff = random8(30, 90);
        for (int i = 0; i < cutoff; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i].setRGB(213, 240, 76);
        }
        for (int j = cutoff; j < NUM_LEDS; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[j].setRGB(red, green, blue);
        }
    }
    FastLED.show();
}
void FirstLight()
{
    EVERY_N_MILLISECONDS(200)
    {
        for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
        {
            // Turn our current led on to white, then show the leds
            if (digitalRead(SWITCH) == HIGH) break;
            for (int i = whiteLed; i < whiteLed + 20; i++)
            {
                leds[i] = CRGB::White;
                if (digitalRead(SWITCH) == HIGH) break;
            }
            // Show the leds (only one of which is set to white, from above)
            FastLED.show();
            // Turn our current led back to black for the next loop around
            for (int j = whiteLed; j < whiteLed + 20; j++)
            {
                leds[j] = CRGB::Black;
                if (digitalRead(SWITCH) == HIGH) break;
            }
        }
    }
}
void Strange()
{
    uint16_t sinBeat = beatsin16(30, 0, 85, 0, 0); // NUM_LEDS - 1

    // leds[sinBeat] = CRGB::Blue;
    leds[sinBeat].setRGB(255, 128, 0);

    fadeToBlackBy(leds, 85, 10); // NUM_LEDS

    FastLED.show();
}

byte *Wheel(byte WheelPosition)
{
    static byte c[3];

    if (WheelPosition < 85)
    {
        c[0] = WheelPosition * 3;
        c[1] = 255 - WheelPosition * 3;
        c[2] = 0;
    }
    else if (WheelPosition < 170)
    {
        WheelPosition -= 85;
        c[0] = 255 - WheelPosition * 3;
        c[1] = 0;
        c[2] = WheelPosition * 3;
    }
    else
    {
        WheelPosition -= 170;
        c[0] = 0;
        c[1] = WheelPosition * 3;
        c[2] = 255 - WheelPosition * 3;
    }

    return c;
}

void RainbowCycle()
{
    byte *c;
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        if (digitalRead(SWITCH) == HIGH) break;
        for (i = 0; i < NUM_LEDS; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
            leds[NUM_LEDS - 1 - i].setRGB(*c, *(c + 1), *(c + 2));
        }
        FastLED.show();
    }
}

void OuterS()
{
    uint16_t sinBeat = beatsin16(30, 0, 85, 0, 0); // NUM_LEDS - 1

    // leds[sinBeat] = CRGB::Blue;
    leds[sinBeat].setRGB(255, 128, 0);

    fadeToBlackBy(leds, 85, 10); // NUM_LEDS

    FastLED.show();

    EVERY_N_MILLISECONDS(20)
    {
        for (int i = 125; i <= 144; i++)
        {
            // if (digitalRead(SWITCH) == HIGH) break;
            int random = random8(0, 10);
            leds[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds[i].setRGB(255, 128, 0);
            }
            else
            {
                leds[i].setRGB(0, 0, 0);
            }
        }

        for (int i = 110; i <= 120; i++)
        {
            // if (digitalRead(SWITCH) == HIGH) break;
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 128, 0);
        }

        for (int i = 95; i <= 105; i++)
        {
            // if (digitalRead(SWITCH) == HIGH) break;
            int random = random8(0, 10);
            leds[i] = CHSV(30, 255, 255);
            if (random % 5 == 1)
            {
                leds[i].setRGB(255, 128, 0);
            }
            else
            {
                leds[i].setRGB(0, 0, 0);
            }
        }

        for (int i = 85; i <= 90; i++)
        {
            // if (digitalRead(SWITCH) == HIGH) break;
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 128, 0);
        }

        FastLED.show();
    }
}

void RainbowLines()
{
    for (int i = 130; i <= 140; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }

    for (int i = 110; i <= 120; i++)
    {
        leds[i] = CHSV(60, 255, 255);
        leds[i].setRGB(255, 255, 0);
    }

    for (int i = 90; i <= 100; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 255, 0);
    }

    for (int i = 70; i <= 80; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 0, 255);
    }
    FastLED.show();
}

void JagWhite()
{
    EVERY_N_MILLISECONDS(20)
    {
        for (int i = 130; i <= 140; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i] = CHSV(30, 255, 255);
            leds[i].setRGB(255, 255, 255);
        }

        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show(); // Send the updated color data to the LED strip

        for (int i = 110; i <= 120; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i].setRGB(255, 255, 255);
        }
        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show(); // Send the updated color data to the LED strip

        for (int i = 90; i <= 100; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i].setRGB(255, 255, 255);
        }

        FastLED.show(); // Send the updated color data to the LED strip
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show(); // Send the updated color data to the LED strip

        for (int i = 70; i <= 80; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i].setRGB(255, 255, 255);
        }
        FastLED.show();
    }
}

void Stars()
{
    EVERY_N_MILLISECONDS(20)
    {
        Serial.println("Stars");
        for (int i = 85; i <= 144; i++)
        {
            Serial.println(i);
            // if (digitalRead(SWITCH) == HIGH) break;
            int randNum = random8(0, 10);
            leds[i] = CHSV(30, 255, 255);
            if (randNum % 5 == 1)
            {
                leds[i].setRGB(255, 255, 255);
            }
            else
            {
                leds[i].setRGB(0, 0, 0);
            }
        }
        FastLED.show();
    }
}

void Pride()
{
    for (int i = 130; i <= 140; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }

    for (int i = 120; i <= 130; i++)
    {
        leds[i].setRGB(255, 165, 0);
    }

    for (int i = 110; i <= 120; i++)
    {
        leds[i] = CHSV(60, 255, 255);
        leds[i].setRGB(255, 255, 0);
    }

    for (int i = 100; i <= 110; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 255, 0);
    }

    for (int i = 110; i <= 120; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(0, 0, 255);
    }

    for (int i = 100; i <= 110; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(160, 32, 240);
    }
    FastLED.show();
}

void ColorFrenzy()
{
    int i = 0;
    for (int j = 0; j < 6; j++)
    {
        if (i == 0)
            fill_solid(leds, NUM_LEDS, CRGB::Red);
        if (i == 1)
            fill_solid(leds, NUM_LEDS, CRGB::Yellow);
        if (i == 2)
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
        if (i == 3)
            fill_solid(leds, NUM_LEDS, CRGB::HotPink);
        if (i == 4)
            fill_solid(leds, NUM_LEDS, CRGB::MediumOrchid);
        if (i == 5)
            fill_solid(leds, NUM_LEDS, CRGB::MediumPurple);
        i++;
        FastLED.show();
        if (i == 6)
            i = 0;
        if (digitalRead(SWITCH) == HIGH) break;
        delay(50);
        if (digitalRead(SWITCH) == HIGH) break;
    }
}

void Histo()
{
    EVERY_N_MILLISECONDS(500)
    {
        int cutoff = random8(30, 90);
        for (int i = 0; i < cutoff; i++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            leds[i].setRGB(255, 255, 255);
        }
        for (int j = cutoff; j < NUM_LEDS; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            int cutoff2 = random8(1, 10);
            if (cutoff2 % 3 == 0)
                leds[j].setRGB(255, 0, 0);
            if (cutoff2 % 3 == 1)
                leds[j].setRGB(0, 255, 0);
            if (cutoff2 % 3 == 2)
                leds[j].setRGB(0, 0, 255);
        }
        FastLED.show();
    }
}

void RandHisto()
{
    EVERY_N_MILLISECONDS(500)
    {
        int cutoffsize = random8(40, NUM_LEDS / 2);
        int cutoffcolor = random8(1, 10);
        for (int j = cutoffsize; j < NUM_LEDS / 2; j++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            if (cutoffcolor % 3 == 0)
                leds[j].setRGB(255, 0, 0);
            if (cutoffcolor % 3 == 1)
                leds[j].setRGB(0, 255, 0);
            if (cutoffcolor % 3 == 2)
                leds[j].setRGB(0, 0, 255);
        }

        for (int k = NUM_LEDS / 2; k < NUM_LEDS / 2 + cutoffsize; k++)
        {
            if (digitalRead(SWITCH) == HIGH) break;
            if (cutoffcolor % 3 == 0)
                leds[k].setRGB(255, 0, 0);
            if (cutoffcolor % 3 == 1)
                leds[k].setRGB(0, 255, 0);
            if (cutoffcolor % 3 == 2)
                leds[k].setRGB(0, 0, 255);
        }
        FastLED.show();
    }
}

void Bang()
{
    for (int i = 130; i <= 140; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }
    static uint8_t hue = 0;
    // First slide the led in one direction
    for (int i = 80; i < 120; i++)
    {
        // Set the i'th led to red
        leds[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        fadeall();
    }

    for (int i = 60; i <= 70; i++)
    {
        leds[i] = CHSV(30, 255, 255);
        leds[i].setRGB(255, 0, 0);
    }
}

// Array of function pointers
const AnimationFunction animationFunctions[] = {
    Fire,
    Beat,
    Fade,
    Blur,
    ColorChange,
    MovingDot,
    Cylon,
    Boom,
    FirstLight,
    Strange,
    RainbowCycle,
    OuterS,
    RainbowLines,
    JagWhite,
    Stars,
    Pride,
    ColorFrenzy,
    Histo,
    RandHisto,
    Bang
};
