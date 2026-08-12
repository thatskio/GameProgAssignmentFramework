#include "StatePlay.h"
#include "StateGameOver.h"
#include "GameStateManager.h" 

#include <cstdlib>
#include <ctime> 
#include <cmath> // Needed for bullet trajectory math

StatePlay::StatePlay(GameStateManager* handlerPtr, IDirect3DDevice9* pDevice, ClassInput* inputPtr, int width, int height)
    : IGameState(handlerPtr) {

    input = inputPtr;
    screenWidth = width;
    screenHeight = height;
    roundTimer = 5.0f;
    fishRespawnTimer = 0.0f;
    wasMouseDown = false;

    localMaid.Init(pDevice);
    spriteManager.Initialize(pDevice, &localMaid);
    lineManager.Initialize(pDevice);
    fontManager.Initialize(pDevice, 20, "Arial");
    uiManager = new UIManager(&lineManager, &fontManager);

    const char* placeholderPath = "Assets/practical9.png";
    spriteManager.RegisterSprite("Crosshair", placeholderPath, { 0, 0, 32, 32 });
    spriteManager.RegisterSprite("Bullet", placeholderPath, { 0, 0, 32, 32 });
    spriteManager.RegisterSprite("GoldFish", placeholderPath, { 0, 0, 32, 32 });

    spriteManager.RegisterSprite("PrototypeCurrency", "Assets/icon_generic.png", { 0, 0, 32, 32 });
    SpriteData currencySprite = spriteManager.GetSprite("PrototypeCurrency");
    currencySprite.red = 50; currencySprite.green = 255; currencySprite.blue = 50;

    SpriteData crosshairSprite = spriteManager.GetSprite("Crosshair");
    crosshairSprite.red = 255; crosshairSprite.green = 255; crosshairSprite.blue = 255;
    player.SetSprite(crosshairSprite);

    SpriteData fishSprite = spriteManager.GetSprite("GoldFish");
    fishSprite.red = 255; fishSprite.green = 50; fishSprite.blue = 50;

    srand((unsigned int)time(NULL));
    for (int i = 0; i < 20; i++) {
        float randX = (float)(rand() % (screenWidth - 64) + 32);
        float randY = (float)(rand() % (screenHeight - 100) + 50);

        float vX = (float)((rand() % 5) - 2);
        float vY = (float)((rand() % 5) - 2);
        if (vX == 0.0f) vX = 1.5f;
        if (vY == 0.0f) vY = -1.5f;

        aiManager.SpawnFish(D3DXVECTOR2(randX, randY), D3DXVECTOR2(vX, vY), 10, 100, fishSprite);
    }
}

StatePlay::~StatePlay() {
    delete uiManager;
}

void StatePlay::Input() {
    player.UpdateInput((float)input->GetMousePosition().x, (float)input->GetMousePosition().y);

    bool isMouseDown = input->IsMouseButtonDown(0);

    // --- NEW: Tap-to-fire mechanic to prevent 60fps bullet spam ---
    if (isMouseDown && !wasMouseDown) {

        // 1. Anchor the gun to the bottom middle
        D3DXVECTOR2 gunPos((float)(screenWidth / 2), (float)screenHeight);
        D3DXVECTOR2 targetPos = player.GetPosition(); // The crosshair

        // 2. Find the direction vector
        D3DXVECTOR2 direction = targetPos - gunPos;

        // 3. Normalize the vector (calculate length, then divide x and y by it)
        float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }
        else {
            direction = D3DXVECTOR2(0.0f, -1.0f); // Default to shooting straight up
        }

        // 4. Apply speed multiplier to the normalized direction
        D3DXVECTOR2 bulletVel = direction * 15.0f;

        SpriteData bulletSprite = spriteManager.GetSprite("Bullet");
        bulletSprite.red = 255; bulletSprite.green = 255; bulletSprite.blue = 0;

        bulletsManager.SpawnBullet(gunPos, bulletVel, 10, bulletSprite);
    }
    wasMouseDown = isMouseDown;
}

void StatePlay::Update(float deltaTime) {
    roundTimer -= deltaTime;
    if (roundTimer <= 0.0f) {
        handler->ChangeState(new StateGameOver(handler, localMaid.GetDevice(), input, screenWidth, screenHeight, player.GetScore()));
        return;
    }

    // --- NEW: Fish Respawning Logic ---
    fishRespawnTimer -= deltaTime;
    if (fishRespawnTimer <= 0.0f) {
        int activeFishes = 0;
        for (NPC* fish : aiManager.GetFishes()) {
            if (fish->IsActive()) activeFishes++;
        }

        if (activeFishes < 20) {
            float randX = (float)(rand() % (screenWidth - 64) + 32);
            float randY = (float)(rand() % (screenHeight - 100) + 50);

            float vX = (float)((rand() % 5) - 2);
            float vY = (float)((rand() % 5) - 2);
            if (vX == 0.0f) vX = 1.5f;
            if (vY == 0.0f) vY = -1.5f;

            SpriteData fishSprite = spriteManager.GetSprite("GoldFish");
            fishSprite.red = 255; fishSprite.green = 50; fishSprite.blue = 50;

            aiManager.SpawnFish(D3DXVECTOR2(randX, randY), D3DXVECTOR2(vX, vY), 10, 100, fishSprite);

            fishRespawnTimer = 2.0f; // 2-second cooldown before spawning another
        }
    }
    // ----------------------------------

    player.Update();
    bulletsManager.Update();
    aiManager.Update();
    physicsManager.ProcessPhysics(&aiManager, &bulletsManager, &player, screenWidth, screenHeight);
}

void StatePlay::Render() {
    spriteManager.Begin();
    aiManager.Render(&spriteManager);
    bulletsManager.Render(&spriteManager);
    player.Render(&spriteManager);
    spriteManager.Draw(spriteManager.GetSprite("PrototypeCurrency"), D3DXVECTOR2(120, 20));
    spriteManager.End();

    uiManager->DrawTopBar(player.GetScore(), 0, roundTimer, screenWidth);
}
