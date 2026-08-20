#include "StateGameOver.h"
#include "StateMainMenu.h"
#include "StatePlay.h"
#include "GameStateManager.h"

StateGameOver::StateGameOver(GameStateManager* handlerPtr, IDirect3DDevice9* device, PlayerInput* inputPtr, int width, int height, int score): IGameState(handlerPtr) {
    input           = inputPtr;
    screenWidth     = width;
    screenHeight    = height;
    finalScore      = score;

    localMaid.Init(device);
    lineManager.Initialize(device);
    fontManager.Initialize(device, 24, "Arial");
    uiManager = new UIManager(&lineManager, &fontManager);
}

StateGameOver::~StateGameOver() {
    delete uiManager;
}

void StateGameOver::Input() {
    if (input->IsMouseButtonDown(0)) {
        float mx = input->GetMousePosition().x;
        float my = input->GetMousePosition().y;

        //Main menu button 
        //Note: Because the main menu button only has an active button without any AI logic, so its safe to just pop back to this state
        if (mx > screenWidth / 2 - 100 && mx < screenWidth / 2 + 100 &&
            my > screenHeight / 2 + 50 && my < screenHeight / 2 + 100) {
            //handler->ChangeState(new StateMainMenu(handler, localMaid.GetDevice(), input, screenWidth, screenHeight));
            handler->PopState();
            handler->PopState();
            return;
        }

        //Replay button
        //Note: Because if we return directly to Play State without "refreshing" the state, AI Logic etc are all already disconnected
        //Causing the Play State to be unplayable/uninteractable, so we pop the current state + refresh the play state
        if (mx > screenWidth / 2 - 100 && mx < screenWidth / 2 + 100 &&
            my > screenHeight / 2 + 120 && my < screenHeight / 2 + 170) {
            handler->PopState();
            handler->ChangeState(new StatePlay(handler, localMaid.GetDevice(), input, screenWidth, screenHeight)); //Revive the PlayState 
            return;
        }

        //Exit button
        if (mx > screenWidth / 2 - 100 && mx < screenWidth / 2 + 100 &&
            my > screenHeight / 2 + 190 && my < screenHeight / 2 + 240) {
            PostQuitMessage(0);
        }
    }
}

void StateGameOver::Update(float deltaTime) {}

void StateGameOver::Render() {
    RECT titleRect = { 0, 100, screenWidth, 150 };
    uiManager->DrawTextOnly("TIME'S UP!", titleRect, D3DCOLOR_XRGB(255, 50, 50), DT_CENTER | DT_VCENTER);

    RECT scoreRect = { 0, 200, screenWidth, 250 };
    std::string scoreTxt = "Final Score: " + std::to_string(finalScore);
    uiManager->DrawTextOnly(scoreTxt, scoreRect, D3DCOLOR_XRGB(255, 255, 255), DT_CENTER | DT_VCENTER);

    //                               left                       top                        right                        bottom
    RECT MainMenuButton = { ((screenWidth / 2) - 100), ((screenHeight / 2) + 50) , ((screenWidth / 2) + 100), ((screenHeight / 2) + 100) };
    uiManager->DrawButton(MainMenuButton, "Return To Main Menu", D3DCOLOR_XRGB(50, 50, 150), D3DCOLOR_XRGB(255, 255, 255));

    RECT ReplayButton = { ((screenWidth / 2) - 100), ((screenHeight / 2) + 120) , ((screenWidth / 2) + 100), ((screenHeight / 2) + 170) };
    uiManager->DrawButton(ReplayButton, "Replay Game", D3DCOLOR_XRGB(50, 150, 50), D3DCOLOR_XRGB(255, 255, 255));

    RECT ExitButton = { ((screenWidth / 2) - 100), ((screenHeight / 2) + 190) , ((screenWidth / 2) + 100), ((screenHeight / 2) + 240) };
    uiManager->DrawButton(ExitButton, "Exit Game", D3DCOLOR_XRGB(150, 50, 50), D3DCOLOR_XRGB(255, 255, 255));
}