#pragma once
#include "IGameState.h"
#include "LineManager.h"
#include "SpriteManager.h"
#include "FontManager.h"
#include "UIManager.h"
#include "PlayerInput.h"
#include "AudioManager.h" 

class StateMainMenu : public IGameState {
private:
    IDirect3DDevice9* d3dDevice;
    LineManager* lineManager;
    SpriteManager* spriteManager;
    FontManager fontManager;
    UIManager* uiManager;
    PlayerInput* input;
    AudioManager* audioManager;

    SpriteData startButtonSprite;

    int screenWidth;
    int screenHeight;

public:
    StateMainMenu(GameStateManager* stateManagerPointer, IDirect3DDevice9* direct3DDevice, PlayerInput* playerInputPointer, LineManager* lineManagerPointer, SpriteManager* spriteManagerPointer, int initialScreenWidth, int initialScreenHeight);
    ~StateMainMenu();

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;
};