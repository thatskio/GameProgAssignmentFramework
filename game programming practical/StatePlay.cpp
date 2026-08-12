#include "StatePlay.h"
#include "StateGameOver.h"
#include "GameStateManager.h"

#include <cstdlib>
#include <ctime> 
#include <cmath> //Needed for bullet trajectory math

StatePlay::StatePlay(GameStateManager* handlerPtr, IDirect3DDevice9* pDevice, PlayerInput* inputPtr, int width, int height): IGameState(handlerPtr) {
    //Variables
    input               = inputPtr;
    screenWidth         = width;
    screenHeight        = height;
    roundTimer          = 60.0f;     //Change how long a game lasts here
    fishRespawnTimer    = 0.0f;
    wasMouseDown        = false;

    //Initialize tools
    localMaid.Init(pDevice);
    spriteManager.Initialize(pDevice, &localMaid);
    lineManager.Initialize(pDevice);
    fontManager.Initialize(pDevice, 20, "Arial");
    uiManager = new UIManager(&lineManager, &fontManager);

    //Initialize sprites
    const char* placeholderPath = "Assets/practical9.png";  //Temporarily using the practical spaceship for now (note: it is 64x64 and loads from top to bottom)
    spriteManager.RegisterSprite("Crosshair", placeholderPath, { 0, 0, 32, 32 });
    spriteManager.RegisterSprite("Bullet", placeholderPath, { 0, 0, 32, 32 });
    spriteManager.RegisterSprite("GoldFish", placeholderPath, { 0, 0, 32, 32 });

    SpriteData crosshairSprite = spriteManager.GetSprite("Crosshair");
    crosshairSprite.red = 255; crosshairSprite.green = 255; crosshairSprite.blue = 255;
    player.SetSprite(crosshairSprite);

    SpriteData fishSprite = spriteManager.GetSprite("GoldFish");
    fishSprite.red = 255; fishSprite.green = 50; fishSprite.blue = 50;

    //-----------GAME STARTS HERE-------------
    //Spawning random fishes (x20)
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 20; i++) {
        float randX = (float)(rand() % (screenWidth - 64) + 32);
        float randY = (float)(rand() % (screenHeight - 100) + 50);

        float vX = (float)((rand() % 5) - 2);
        float vY = (float)((rand() % 5) - 2);
        if (vX == 0.0f) { vX = 1.5f; }
        if (vY == 0.0f) { vY = -1.5f; }

        aiManager.SpawnFish(D3DXVECTOR2(randX, randY), D3DXVECTOR2(vX, vY), 10, 100, fishSprite);
    }
}

StatePlay::~StatePlay() {
    delete uiManager;
}

void StatePlay::Input() {
    player.UpdateInput((float)input->GetMousePosition().x, (float)input->GetMousePosition().y);

    bool isMouseDown = input->IsMouseButtonDown(0);
    if (isMouseDown && !wasMouseDown) {

        //Anchor the gun to the bottom middle
        D3DXVECTOR2 gunPos((float)(screenWidth / 2), (float)screenHeight);
        D3DXVECTOR2 targetPos = player.GetPosition(); // The crosshair

        //Find the direction vector
        D3DXVECTOR2 direction = targetPos - gunPos;

        //Normalize the vector (calculate length, then divide x and y by it)
        float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }
        else {
            direction = D3DXVECTOR2(0.0f, -1.0f); // Default to shooting straight up
        }

        //Apply speed multiplier to the normalized direction
        D3DXVECTOR2 bulletVel = direction * 15.0f;

        SpriteData bulletSprite = spriteManager.GetSprite("Bullet");
        bulletSprite.red = 255; bulletSprite.green = 255; bulletSprite.blue = 0;

        bulletsManager.SpawnBullet(gunPos, bulletVel, 10, bulletSprite);
    }
    wasMouseDown = isMouseDown;
}

void StatePlay::Update(float deltaTime) {
    roundTimer -= deltaTime;
    if (roundTimer <= 0.0f) {   //Don't spawn more fishes if wave timer is over
        handler->ChangeState(new StateGameOver(handler, localMaid.GetDevice(), input, screenWidth, screenHeight, player.GetScore()));
        return;
    }

    //Fish respawning logic
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

            fishRespawnTimer = 2.0f; //2-second cooldown before spawning another
        }
    }

    player.Update();
    bulletsManager.Update();
    aiManager.Update();
    physicsManager.ProcessPhysics(&aiManager, &bulletsManager, &player, screenWidth, screenHeight);
}

void StatePlay::Render() {
    //Drawing game objects
    spriteManager.Begin();
    aiManager.Render(&spriteManager);
    bulletsManager.Render(&spriteManager);
    player.Render(&spriteManager);
    spriteManager.End();

    //Drawing topbar/UI
    uiManager->DrawTopBar(player.GetScore(), 0, roundTimer, screenWidth);
}
