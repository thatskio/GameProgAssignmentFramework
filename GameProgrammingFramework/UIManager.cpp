#include "UIManager.h"

UIManager::UIManager(LineManager* lineManager, FontManager* fontManager) {
    this->lineManager = lineManager;
    this->fontManager = fontManager;
}

UIManager::~UIManager() {}

void UIManager::DrawButton(RECT buttonPosition, const std::string& text, D3DCOLOR backgroundColor, D3DCOLOR textColor) {
    if (lineManager) {
        lineManager->DrawFilledRect(buttonPosition, backgroundColor);
    }
    if (fontManager) {
        fontManager->DrawText(text, buttonPosition, textColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    }
}

void UIManager::DrawPanel(RECT panelPosition, D3DCOLOR backgroundColor) {
    if (lineManager) {
        lineManager->DrawFilledRect(panelPosition, backgroundColor);
    }
}

void UIManager::DrawTextOnly(const std::string& text, RECT textPosition, D3DCOLOR textColor, DWORD alignment) {
    if (fontManager) {
        fontManager->DrawText(text, textPosition, textColor, alignment);
    }
}

void UIManager::DrawTopBar(int playerOneScore, int playerTwoScore, float timeRemaining, int screenWidth) {
    //Draw a semi-transparent black background panel for the HUD
    RECT topBarPosition = { 0, 0, screenWidth, 40 };
    DrawPanel(topBarPosition, D3DCOLOR_ARGB(180, 20, 20, 20));

    //Text label bounds
    RECT leftTextPosition = { 10, 0, 150, 40 };
    RECT middleTextPosition = { ((screenWidth / 2) - 50), 0, ((screenWidth / 2) + 50), 40 };
    RECT rightTextPosition = { screenWidth - 150, 0, screenWidth - 10, 40 };

    std::string scoreText = "Score: " + std::to_string(playerOneScore);
    std::string timeText = "Time: " + std::to_string((int)timeRemaining);
    std::string rightSideText = ""; // + std::to_string(playerTwoScore);

    DrawTextOnly(scoreText, leftTextPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(timeText, middleTextPosition, D3DCOLOR_XRGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(rightSideText, rightTextPosition, D3DCOLOR_XRGB(255, 255, 255), DT_RIGHT | DT_VCENTER | DT_NOCLIP);
}