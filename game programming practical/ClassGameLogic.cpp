#include "ClassGameLogic.h"
#include <iostream>

ClassGameLogic::ClassGameLogic() { // Variables
    red = 255; green = 0; blue = 0;
    rgbCtrl         = 1;
    RANDOM_NUM      = (std::rand() % 10) + 1;
    WRAP            = false;
    PINGPONG        = true;

    flashUntil      = 0;
    bgColourNow     = WHITE;
    score           = 0;
    currentDigit    = 0;
}

int ClassGameLogic::WrapRGB(int gotColour) {
    if (gotColour >= 255) {
        if (PINGPONG) {
            rgbCtrl = -RANDOM_NUM;
            gotColour += rgbCtrl;
        }
        gotColour = WRAP ? 0 : gotColour;
        return gotColour;
    }
    else if (gotColour <= 0) {
        if (PINGPONG) { rgbCtrl = RANDOM_NUM; }
    }

    gotColour += rgbCtrl;

    // Sanity check
    if (gotColour <= 0) { gotColour = 0; }
    else if (gotColour >= 255) { gotColour = 255; }

    return gotColour;
}

// cursorType: 1 = Iron, 2 = Gold, 3 = Diamond
bool ClassGameLogic::AttackColour(int cursorType) {
    if (cursorType == 1 && bgColourNow == WHITE) return true;
    else if (cursorType == 2 && bgColourNow == YELLOW) return true;
    else if (cursorType == 3 && bgColourNow == BLUE) return true;
    return false;
}

void ClassGameLogic::RngBackground(int& outBgRed, int& outBgGreen, int& outBgBlue) {
    switch (std::rand() % 3) {
    case 0: // white
        outBgRed = 255; outBgGreen = 255; outBgBlue = 255;
        bgColourNow = WHITE;
        break;
    case 1: // blue
        outBgRed = 0; outBgGreen = 0; outBgBlue = 255;
        bgColourNow = BLUE;
        break;
    case 2: // yellow
        outBgRed = 255; outBgGreen = 255; outBgBlue = 0;
        bgColourNow = YELLOW;
        break;
    }
}

bool ClassGameLogic::CheckFlashReset() { // Minecraft sword crit effect
    if (flashUntil != 0 && GetTickCount() >= flashUntil) {
        flashUntil = 0;
        return true; // Tells the engine it is time to reset the sword color
    }
    return false;
}