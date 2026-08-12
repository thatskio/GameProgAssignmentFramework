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
#include "Maid.h"
#include "ClassInput.h"

class StatePlay : public IGameState {
private:
    Maid localMaid;
    SpriteManager spriteManager;
    LineManager lineManager;
    FontManager fontManager;
    UIManager* uiManager;

    Player player;
    AILogicManager aiManager;
    BulletsManager bulletsManager;
    PhysicsManager physicsManager;
    ClassInput* input;

    float roundTimer;
    int screenWidth;
    int screenHeight;
    float fishRespawnTimer;
    bool wasMouseDown;

public:
    StatePlay(GameStateManager* handlerPtr, IDirect3DDevice9* pDevice, ClassInput* inputPtr, int width, int height);
    ~StatePlay();

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;
};