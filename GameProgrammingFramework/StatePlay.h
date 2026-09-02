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
#include "Obstacle.h"

class StatePlay : public IGameState {
private:
    IDirect3DDevice9* d3dDevice;
    SpriteManager* spriteManager;
    LineManager* lineManager;
    FontManager fontManager;
    UIManager* uiManager;
    AudioManager* audioManager;

    Player player;
    AILogicManager aiManager;
    BulletsManager bulletsManager;
    PhysicsManager physicsManager;
    ObstacleManager obstacleManager;
    PlayerInput* input;

    // Controls
    int quitKey;
    int alternateQuitKey;
    int addScoreKey;
    int cannonModeOneKey;
    int cannonModeTwoKey;
    int cannonModeThreeKey;
    int increaseMassKey;
    int decreaseMassKey;
    int shootMouseButton;

    int firingMode;
    float bulletMass;
    int highscore;
    int screenWidth;
    int screenHeight;
    int fishAmount;
    float fishRespawnTimer;
    float fishRespawnCooldown;
    bool wasMouseButtonDown;
    bool wasEnterDown;
    bool wasArrowUpDown;
    bool wasArrowDownDown;
    SpriteData backgroundSprite;
    SpriteData foregroundSprites[4];
    SpriteData fishSprite;
    SpriteData cannonSprite;
    float foregroundBobTime;
    int cannonAnimationFrame;
    int cannonAnimationTick;
    bool cannonAnimating;

    void LoadHighscore();
    void SaveHighscore();

public:
    StatePlay(GameStateManager* stateManagerPointer, IDirect3DDevice9* direct3DDevice, PlayerInput* playerInputPointer, LineManager* lineManagerPointer, SpriteManager* spriteManagerPointer, int initialScreenWidth, int initialScreenHeight);
    ~StatePlay();

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;
};