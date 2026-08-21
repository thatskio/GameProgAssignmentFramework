#pragma once
#include "IGameState.h"
#include "SpriteManager.h"
#include "LineManager.h"
#include "FontManager.h"
#include "UIManager.h"
#include "Player.h"
#include "AILogicManager.h"
#include "BulletsManager.h"
#include "PhysicsManager.h"
#include "PlayerInput.h"
#include "AudioManager.h"

class StatePlay : public IGameState {
private:
    IDirect3DDevice9* d3dDevice;
    SpriteManager spriteManager;
    LineManager lineManager;
    FontManager fontManager;
    UIManager* uiManager;
    AudioManager* audioManager;

    Player player;
    AILogicManager aiManager;
    BulletsManager bulletsManager;
    PhysicsManager physicsManager;
    PlayerInput* input;

    float roundTimer;
    int screenWidth;
    int screenHeight;
    float fishRespawnTimer;
    bool wasMouseButtonDown;

public:
    StatePlay(GameStateManager* stateManagerPointer, IDirect3DDevice9* direct3DDevice, PlayerInput* playerInputPointer, int initialScreenWidth, int initialScreenHeight);
    ~StatePlay();

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;
};