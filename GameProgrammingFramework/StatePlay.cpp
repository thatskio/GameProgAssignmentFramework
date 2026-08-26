#include "StatePlay.h"
#include "StateGameOver.h"
#include "GameStateManager.h"

#include <cstdlib>
#include <ctime> 
#include <cmath> //Needed for bullet trajectory math

StatePlay::StatePlay(GameStateManager* stateManagerPointer, IDirect3DDevice9* direct3DDevice, PlayerInput* playerInputPointer, LineManager* lineManagerPointer, SpriteManager* spriteManagerPointer, int initialScreenWidth, int initialScreenHeight) : IGameState(stateManagerPointer) {
    //Variables
    input = playerInputPointer;
    lineManager = lineManagerPointer;
    spriteManager = spriteManagerPointer;
    screenWidth = initialScreenWidth;
    screenHeight = initialScreenHeight;
    roundTimer = 30.0f;         //Change how long a game lasts here
    fishAmount = 20;            //Change how many fishes to spawn
    fishRespawnCooldown = 2.0f; //Change how long to wait, then check to respawn fishes
    fishRespawnTimer = 0.0f;    //This is just a time tracker, leave at zero
    wasMouseButtonDown = false;
    d3dDevice = direct3DDevice;

    //Initialize tools
    fontManager.Initialize(direct3DDevice, 20, "Arial");
    uiManager = new UIManager(lineManager, &fontManager);

    //Initialize sprites
    const char* placeholderPath = "Assets/practical9.png";  //Temporarily using the practical spaceship for now (note: it is 64x64 and loads from top to bottom)
    spriteManager->RegisterSprite("Crosshair", placeholderPath, { 0, 0, 32, 32 });
    spriteManager->RegisterSprite("Bullet", placeholderPath, { 0, 0, 32, 32 });
    spriteManager->RegisterAnimatedSprite("GoldFish", placeholderPath, 32, 32, 2, 2, 4);

    SpriteData crosshairSprite = spriteManager->GetSprite("Crosshair");
    crosshairSprite.red = 255; crosshairSprite.green = 255; crosshairSprite.blue = 255;
    player.SetSprite(crosshairSprite);

    SpriteData fishSprite = spriteManager->GetSprite("GoldFish");
    fishSprite.red = 255; fishSprite.green = 50; fishSprite.blue = 50;

    //Initialize sounds
    audioManager = new AudioManager();
    audioManager->InitializeAudio();
    audioManager->LoadSounds("Boing", "boing.mp3", false);

    //Spawning random fishes (x20, from fishAmount)
    srand((unsigned int)time(NULL));
    for (int i = 0; i < fishAmount; i++) {
        float randomPositionX = (float)(rand() % (screenWidth - 64) + 32);
        float randomPositionY = (float)(rand() % (screenHeight - 100) + 50);

        float velocityX = (float)((rand() % 5) - 2);
        float velocityY = (float)((rand() % 5) - 2);
        if (velocityX == 0.0f) { velocityX = 1.5f; }
        if (velocityY == 0.0f) { velocityY = -1.5f; }

        aiManager.SpawnFish(D3DXVECTOR2(randomPositionX, randomPositionY), D3DXVECTOR2(velocityX, velocityY), 10, 100, fishSprite);
    }

    //Spawning obstacles
    obstacleManager.AddObstacle({
    D3DXVECTOR2(250.0f, 200.0f),
    D3DXVECTOR2(450.0f, 200.0f),
    D3DXVECTOR2(450.0f, 300.0f)
        });
}

StatePlay::~StatePlay() {
    delete uiManager;
    delete audioManager;
}

void StatePlay::Input() {
    // FIX: Subtract 16 from X and Y to perfectly center the 32x32 crosshair sprite on the mouse
    player.UpdateInput((float)input->GetMousePosition().x - 16.0f, (float)input->GetMousePosition().y - 16.0f);

    bool isMouseButtonDown = input->IsMouseButtonDown(0);
    if (isMouseButtonDown && !wasMouseButtonDown) {

        //Anchor the gun to the bottom middle
        D3DXVECTOR2 gunPosition((float)(screenWidth / 2), (float)screenHeight);

        //Finding mouse current position
        D3DXVECTOR2 targetPosition = player.GetPosition();
        targetPosition.x += 16.0f;
        targetPosition.y += 16.0f;

        //Find the direction vector
        D3DXVECTOR2 direction = targetPosition - gunPosition;

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
        D3DXVECTOR2 bulletVelocity = direction * 15.0f;

        SpriteData bulletSprite = spriteManager->GetSprite("Bullet");
        bulletSprite.red = 255; bulletSprite.green = 255; bulletSprite.blue = 0;

        bulletsManager.SpawnBullet(gunPosition, bulletVelocity, 10, bulletSprite, screenWidth, screenHeight);
    }
    wasMouseButtonDown = isMouseButtonDown;
}

void StatePlay::Update(float deltaTime) {
    roundTimer -= deltaTime;
    if (roundTimer <= 0.0f) {   //Don't spawn more fishes if wave timer is over
        handler->PushState(new StateGameOver(handler, d3dDevice, input, lineManager, spriteManager, screenWidth, screenHeight, player.GetScore()));
        return;
    }

    //Fish respawning logic
    fishRespawnTimer -= deltaTime;
    if (fishRespawnTimer <= 0.0f) {
        int activeFishes = 0;
        for (NPC* fish : aiManager.GetFishes()) {
            if (fish->IsActive()) activeFishes++;
        }

        if (activeFishes < fishAmount) {
            float randomPositionX = (float)(rand() % (screenWidth - 64) + 32);
            float randomPositionY = (float)(rand() % (screenHeight - 100) + 50);

            float velocityX = (float)((rand() % 5) - 2);
            float velocityY = (float)((rand() % 5) - 2);
            if (velocityX == 0.0f) velocityX = 1.5f;
            if (velocityY == 0.0f) velocityY = -1.5f;

            SpriteData fishSprite = spriteManager->GetSprite("GoldFish");
            fishSprite.red = 255; fishSprite.green = 50; fishSprite.blue = 50;

            aiManager.SpawnFish(D3DXVECTOR2(randomPositionX, randomPositionY), D3DXVECTOR2(velocityX, velocityY), 10, 100, fishSprite);

            fishRespawnTimer = fishRespawnCooldown;
        }
    }

    player.Update();
    bulletsManager.Update();
    aiManager.Update();
    obstacleManager.Update();
    physicsManager.ProcessPhysics(&aiManager, &bulletsManager, &player, audioManager, &obstacleManager, screenWidth, screenHeight);
}

void StatePlay::Render() {
    //Drawing game objects
    spriteManager->Begin();
    aiManager.Render(spriteManager);
    bulletsManager.Render(spriteManager);
    player.Render(spriteManager);
    obstacleManager.Render(lineManager);

    //Drawing topbar/UI
    uiManager->DrawTopBar(player.GetScore(), 0, roundTimer, screenWidth);
    spriteManager->End();
}