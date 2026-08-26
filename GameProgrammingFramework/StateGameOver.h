#pragma once
#include "IGameState.h"
#include "LineManager.h"
#include "SpriteManager.h"
#include "FontManager.h"
#include "UIManager.h"
#include "PlayerInput.h"

class StateGameOver : public IGameState {
private:
    IDirect3DDevice9* d3dDevice;
    LineManager* lineManager;
    SpriteManager* spriteManager;
    FontManager fontManager;
    UIManager* uiManager;
    PlayerInput* input;

    int screenWidth;
    int screenHeight;
    int finalScore;

public:
    StateGameOver(GameStateManager* handlerPtr, IDirect3DDevice9* device, PlayerInput* inputPtr, LineManager* lineManagerPointer, SpriteManager* spriteManagerPointer, int width, int height, int score);
    ~StateGameOver();

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;
};