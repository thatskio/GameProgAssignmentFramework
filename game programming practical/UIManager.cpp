#include "UIManager.h"

UIManager::UIManager(LineManager* lm, FontManager* fm) {
    lineManager = lm;
    fontManager = fm;
}

UIManager::~UIManager() {}

void UIManager::DrawButton(RECT rect, const std::string& text, D3DCOLOR bgColor, D3DCOLOR textColor) {
    if (lineManager) lineManager->DrawFilledRect(rect, bgColor);
    if (fontManager) fontManager->DrawTextStr(text, rect, textColor, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
}

void UIManager::DrawPanel(RECT rect, D3DCOLOR bgColor) {
    if (lineManager) lineManager->DrawFilledRect(rect, bgColor);
}

void UIManager::DrawTextOnly(const std::string& text, RECT rect, D3DCOLOR textColor, DWORD alignment) {
    if (fontManager) fontManager->DrawTextStr(text, rect, textColor, alignment);
}

void UIManager::DrawTopBar(int p1Score, int p2Score, float timeLeft, int screenWidth) {
    // 1. Draw a semi-transparent black background panel for the HUD
    RECT topBarRect = { 0, 0, screenWidth, 40 };
    DrawPanel(topBarRect, D3DCOLOR_ARGB(180, 20, 20, 20));

    // 2. Define text bounds
    RECT rectP1 = { 10, 0, 150, 40 };
    RECT rectTime = { screenWidth / 2 - 50, 0, screenWidth / 2 + 50, 40 };
    RECT rectP2 = { screenWidth - 150, 0, screenWidth - 10, 40 };

    // 3. Format strings
    std::string p1Str = "Score: " + std::to_string(p1Score);
    std::string timeStr = "Time: " + std::to_string((int)timeLeft);
    std::string p2Str = "Score: " + std::to_string(p2Score);

    // 4. Draw the text on top of the panel
    DrawTextOnly(p1Str, rectP1, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(timeStr, rectTime, D3DCOLOR_XRGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(p2Str, rectP2, D3DCOLOR_XRGB(255, 255, 255), DT_RIGHT | DT_VCENTER | DT_NOCLIP);
}