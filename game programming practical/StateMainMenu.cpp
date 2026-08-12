#include "StateMainMenu.h"
#include "StatePlay.h"
#include "GameStateManager.h" 

// Update to pDevice
StateMainMenu::StateMainMenu(GameStateManager* handlerPtr, IDirect3DDevice9* pDevice, ClassInput* inputPtr, int width, int height)
    : IGameState(handlerPtr) {
    input = inputPtr;
    screenWidth = width;
    screenHeight = height;

    localMaid.Init(pDevice); // Update to pDevice
    lineManager.Initialize(pDevice); // Update to pDevice
    fontManager.Initialize(pDevice, 24, "Arial"); // Update to pDevice
    uiManager = new UIManager(&lineManager, &fontManager);
}

StateMainMenu::~StateMainMenu() {
    delete uiManager;
}

void StateMainMenu::Input() {
    if (input->IsMouseButtonDown(0)) {
        float mx = input->GetMousePosition().x;
        float my = input->GetMousePosition().y;

        // Simple bounding box for a 200x50 Start Button in the center of the screen
        if (mx > screenWidth / 2 - 100 && mx < screenWidth / 2 + 100 &&
            my > screenHeight / 2 - 25 && my < screenHeight / 2 + 25) {
            handler->ChangeState(new StatePlay(handler, localMaid.GetDevice(), input, screenWidth, screenHeight));
        }
    }
}

void StateMainMenu::Update(float deltaTime) {}

void StateMainMenu::Render() {
    RECT titleRect = { 0, 100, screenWidth, 150 };
    uiManager->DrawTextOnly("OCEAN KING DEMO", titleRect, D3DCOLOR_XRGB(255, 255, 255), DT_CENTER | DT_VCENTER);

    RECT btnRect = { screenWidth / 2 - 100, screenHeight / 2 - 25, screenWidth / 2 + 100, screenHeight / 2 + 25 };
    uiManager->DrawButton(btnRect, "Start Game", D3DCOLOR_XRGB(50, 150, 50), D3DCOLOR_XRGB(255, 255, 255));
}