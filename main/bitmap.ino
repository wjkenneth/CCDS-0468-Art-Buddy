#include <Arduino.h>
#include <LittleFS.h>
#include <SPI.h>
#include <FastLED.h>
#include "main.h"
#include "bitmap.h"
#include "display.h"

// Define file info array
FileInfo files[MAX_FILES];
int fileCount = 0;

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
    while (file && fileCount < MAX_FILES)
    {
        files[fileCount].name = file.name();
        files[fileCount].size = file.size();
        files[fileCount].isDirectory = file.isDirectory();
        Serial.println(files[fileCount].name + " " + files[fileCount].size + " " + files[fileCount].isDirectory);

        fileCount++;
        file = root.openNextFile();
    }
}

void deleteFile()
{
    String path = "/" + String(files[uiState.storageModeSelection].name);

    if (LittleFS.remove(path))
    {
        Serial.println(String(path) + " deleted successfully.");
        // List files again to verify deletion
        fileCount = 0; // Reset fileCount
        listDir();
    }
    else
    {
        Serial.println("Failed to delete " + String(path));
    }
    storageMode();
}

void displayBMP()
{
    String path = "/" + String(files[uiState.storageModeSelection].name);
    File bmpFile = LittleFS.open(path.c_str(), "r");

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

    // Calculate delay
    int delayTime = uiState.settings[1] / width;

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
        delay(delayTime);
    }

    FastLED.clear();
    FastLED.show();
    bmpFile.close();
}