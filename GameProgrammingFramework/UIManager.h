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

    void DrawButton(RECT rect, const std::string& text, D3DCOLOR bgColor, D3DCOLOR textColor);
    void DrawPanel(RECT rect, D3DCOLOR bgColor);
    void DrawTextOnly(const std::string& text, RECT rect, D3DCOLOR textColor, DWORD alignment);
    void DrawTopBar(int p1Score, int p2Score, float timeLeft, int screenWidth);
};