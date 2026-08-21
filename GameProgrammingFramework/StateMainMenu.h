#pragma once
#include "IGameState.h"
#include "LineManager.h"
#include "FontManager.h"
#include "UIManager.h"
#include "PlayerInput.h"
#include "AudioManager.h" 

class StateMainMenu : public IGameState {
private:
    IDirect3DDevice9* d3dDevice;
    LineManager lineManager;
    FontManager fontManager;
    UIManager* uiManager;
    PlayerInput* input;
    AudioManager* audioManager;

    int screenWidth;
    int screenHeight;

public:
    StateMainMenu(GameStateManager* handlerPtr, IDirect3DDevice9* device, PlayerInput* inputPtr, int width, int height);
    ~StateMainMenu();

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;
};