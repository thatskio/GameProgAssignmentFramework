#include "UIManager.h"
#include <iomanip>
#include <sstream>

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

void UIManager::DrawTopBar(int playerOneScore, int playerTwoScore, float highscore, int screenWidth) {
    //Draw a semi-transparent black background panel for the HUD
    RECT topBarPosition = { 0, 0, screenWidth, 40 };

    DrawPanel(topBarPosition, D3DCOLOR_ARGB(180, 20, 20, 20));
    //Text label bounds
    RECT leftTextPosition = { 10, 0, 150, 40 };
    RECT leftTipsPosition = { 150, 0, 300, 40 };
    RECT middleTextPosition = { ((screenWidth / 2) - 50), 0, ((screenWidth / 2) + 50), 40 };
    RECT rightTextPosition = { screenWidth - 150, 0, screenWidth - 10, 40 };

    std::string scoreText = "Score: " + std::to_string(playerOneScore);
    std::string leftTips = "(100 score = 1 bullet)";
    std::string timeText = "Highscore: " + std::to_string((int)highscore);
    std::string rightSideText = ""; // + std::to_string(playerTwoScore);

    DrawTextOnly(scoreText, leftTextPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(leftTips, leftTipsPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(timeText, middleTextPosition, D3DCOLOR_XRGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(rightSideText, rightTextPosition, D3DCOLOR_XRGB(255, 255, 255), DT_RIGHT | DT_VCENTER | DT_NOCLIP);
}

void UIManager :: DrawSideBar(int screenHeight, int screenWidth, float bulletMass) { //the controls menu thingy, placed bottom left
    RECT sideBarPosition = { 0, screenHeight - 155 , 200, screenHeight};

    RECT controlTitlePosition = { 0, screenHeight - 150, 200, screenHeight - 125 };
    RECT quitControlPosition = { 0, screenHeight - 125, 200, screenHeight - 100 };
    RECT cannonControlPosition = { 0, screenHeight - 100, 200, screenHeight - 75 };
    RECT addTokenControlPosition = { 0, screenHeight - 75, 200, screenHeight - 50 };
    RECT shootControlPosition = { 0, screenHeight - 50, 200, screenHeight - 25 };
    RECT massControlPosition = { 0, screenHeight - 25, 200, screenHeight };

    std::string controlTitle = "Controls";
    std::string quitText = " Q - Quit";
	std::string cannonText = " 1 / 2 / 3 - Cannon Mode";
	std::string addTokenText = " Enter - Add 1000 Score";
    std::string shootText = " Left Click - Shoot";
    std::ostringstream massValue;
    massValue << std::fixed << std::setprecision(1) << bulletMass;
    std::string massText = " Up/Down - Mass: " + massValue.str();

	DrawPanel(sideBarPosition, D3DCOLOR_ARGB(100, 20, 20, 20));
    DrawTextOnly(controlTitle, controlTitlePosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(quitText, quitControlPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(cannonText, cannonControlPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(addTokenText, addTokenControlPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(shootText, shootControlPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);
    DrawTextOnly(massText, massControlPosition, D3DCOLOR_XRGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_NOCLIP);

}