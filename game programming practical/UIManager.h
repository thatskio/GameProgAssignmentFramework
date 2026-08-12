#pragma once
#include "LineManager.h"
#include "FontManager.h"
#include <string>

class UIManager {
private:
    LineManager* lineManager;
    FontManager* fontManager;

public:
    // Dependency Injection: We hand it the tools it needs to draw
    UIManager(LineManager* lm, FontManager* fm);
    ~UIManager();

    void DrawButton(RECT rect, const std::string& text, D3DCOLOR bgColor, D3DCOLOR textColor);
    void DrawPanel(RECT rect, D3DCOLOR bgColor);
    void DrawTextOnly(const std::string& text, RECT rect, D3DCOLOR textColor, DWORD alignment);

    // Custom Ocean King blockout HUD
    void DrawTopBar(int p1Score, int p2Score, float timeLeft, int screenWidth);
};