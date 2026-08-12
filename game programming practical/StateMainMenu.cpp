#include "StateMainMenu.h"
#include "StatePlay.h"
#include "GameStateManager.h" 

StateMainMenu::StateMainMenu(GameStateManager* handlerPtr, IDirect3DDevice9* pDevice, PlayerInput* inputPtr, int width, int height): IGameState(handlerPtr) {
    input           = inputPtr;
    screenWidth     = width;
    screenHeight    = height;

    localMaid.Init(pDevice); 
    lineManager.Initialize(pDevice); 
    fontManager.Initialize(pDevice, 24, "Arial"); 
    uiManager = new UIManager(&lineManager, &fontManager);
}

StateMainMenu::~StateMainMenu() {
    delete uiManager;
}

void StateMainMenu::Input() {
    if (input->IsMouseButtonDown(0)) {
        float mouseX = input->GetMousePosition().x;
        float mouseY = input->GetMousePosition().y;

        //Simple bounding box for start button in the centre of the screen
        if (mouseX > screenWidth / 2 - 100 && mouseX < screenWidth / 2 + 100 && mouseY > screenHeight / 2 - 25 && mouseY < screenHeight / 2 + 25) {
            handler->ChangeState(new StatePlay(handler, localMaid.GetDevice(), input, screenWidth, screenHeight));
        }
    }
}

void StateMainMenu::Update(float deltaTime) {}

void StateMainMenu::Render() {
    RECT titleRect = { 0, 100, screenWidth, 150 };
    uiManager->DrawTextOnly("Fishing Game DEMO", titleRect, D3DCOLOR_XRGB(255, 255, 255), DT_CENTER | DT_VCENTER);

    RECT btnRect = { ((screenWidth / 2) - 100), ((screenHeight / 2) - 25), ((screenWidth / 2) + 100), screenHeight / 2 + 25 };
    uiManager->DrawButton(btnRect, "Go fissin", D3DCOLOR_XRGB(50, 150, 50), D3DCOLOR_XRGB(255, 255, 255));
}