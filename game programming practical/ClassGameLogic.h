#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdlib>

enum BgColour { WHITE = 1, YELLOW = 2, BLUE = 3 };

class ClassGameLogic {
public:
    // Game state variables 
    int red, green, blue;
    int rgbCtrl;
    int RANDOM_NUM;
    bool WRAP;
    bool PINGPONG;
    bool DEBUG_COLOUR_CHANGE;

    DWORD flashUntil;
    int bgColourNow;
    int score;
    int currentDigit;

    ClassGameLogic();

    // Helpers
    int WrapRGB(int gotColour);
    bool AttackColour(int cursorType);
    void RngBackground(int& outBgRed, int& outBgGreen, int& outBgBlue);
    bool CheckFlashReset();
};