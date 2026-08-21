#include "UIManager.h"

UIManager::UIManager(LineManager* lineManager, FontManager* fontManager) {
    this->lineManager = lineManager;
    this->fontManager = fontManager;
}

UIManager::~UIManager() {}

void UIManager::DrawButton(RECT rect, const std::string& text, D3DCOLOR bgColor, D3DCOLOR textColor) {
    if (lineManager) {
        lineManager->DrawFilledRect(rect, bgColor);
    }
    if (fontManager) {
        fontManager->DrawText(text, rect, textColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    }
}

void UIManager::DrawPanel(RECT rect, D3DCOLOR bgColor) {
    if (lineManager) {
        lineManager->DrawFilledRect(rect, bgColor);
    }
}

void UIManager::DrawTextOnly(const std::string& text, RECT rect, D3DCOLOR textColor, DWORD alignment) {
    if (fontManager) {
        fontManager->DrawText(text, rect, textColor, alignment);
    }
}

void UIManager::DrawTopBar(int p1Score, int p2Score, float timeLeft, int screenWidth) {
    //Draw a semi-transparent black background panel for the HUD
    RECT topBarRect = { 0, 0, screenWidth, 40 };
    DrawPanel(topBarRect, D3DCOLOR_ARGB(180, 20, 20, 20));

    //Text label bounds
    RECT leftRect = { 10, 0, 150, 40 };
    RECT middleRect = { ((screenWidth / 2) - 50), 0, ((screenWidth / 2) + 50), 40 };
    RECT rightRect = { screenWidth - 150, 0, screenWidth - 10, 40 };

    std::string scoreText = "Score: " + std::to_string(p1Score);
    std::string timeText = "Time: " + std::to_string((int)timeLeft);
    std::string rightSideText = ""; // + std::to_string(p2Score);

    DrawTextOnly(scoreText, leftRect, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(timeText, middleRect, D3DCOLOR_XRGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(rightSideText, rightRect, D3DCOLOR_XRGB(255, 255, 255), DT_RIGHT | DT_VCENTER | DT_NOCLIP);
}