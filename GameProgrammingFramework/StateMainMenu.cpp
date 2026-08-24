#include "StateMainMenu.h"
#include "StatePlay.h"
#include "GameStateManager.h" 

StateMainMenu::StateMainMenu(GameStateManager* stateManagerPointer, IDirect3DDevice9* direct3DDevice, PlayerInput* playerInputPointer, int initialScreenWidth, int initialScreenHeight) : IGameState(stateManagerPointer) {
    input = playerInputPointer;
    screenWidth = initialScreenWidth;
    screenHeight = initialScreenHeight;
    d3dDevice = direct3DDevice;

    lineManager.Initialize(direct3DDevice);
    fontManager.Initialize(direct3DDevice, 24, "Arial");

    //Initializing sounds
    audioManager = new AudioManager();
    audioManager->InitializeAudio();
    audioManager->LoadSounds("MenuMusic", "sasakure.UK - Atropos.mp3", true);

    SoundParams params;
    params.loop = true;
    audioManager->PlayAudio("MenuMusic", params);

    uiManager = new UIManager(&lineManager, &fontManager);
}

StateMainMenu::~StateMainMenu() {
    delete uiManager;
    delete audioManager;
}

void StateMainMenu::Input() {
    if (input->IsMouseButtonDown(0)) {
        float mouseX = input->GetMousePosition().x;
        float mouseY = input->GetMousePosition().y;

        //Simple bounding box for start button in the centre of the screen
        if (mouseX > screenWidth / 2 - 100 && mouseX < screenWidth / 2 + 100 && mouseY > screenHeight / 2 - 25 && mouseY < screenHeight / 2 + 25) {
            handler->PushState(new StatePlay(handler, d3dDevice, input, screenWidth, screenHeight));
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