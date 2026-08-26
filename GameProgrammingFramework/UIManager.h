#pragma once
#include "LineManager.h"
#include "FontManager.h"
#include <string>

class UIManager {
private:
    LineManager* lineManager;
    FontManager* fontManager;

public:
    UIManager(LineManager* lineManager, FontManager* fontManager);
    ~UIManager();

    void DrawButton(RECT buttonPosition, const std::string& text, D3DCOLOR backgroundColor, D3DCOLOR textColor);
    void DrawPanel(RECT panelPosition, D3DCOLOR backgroundColor);
    void DrawTextOnly(const std::string& text, RECT textPosition, D3DCOLOR textColor, DWORD alignment);
    void DrawTopBar(int playerOneScore, int playerTwoScore, float timeRemaining, int screenWidth);
};