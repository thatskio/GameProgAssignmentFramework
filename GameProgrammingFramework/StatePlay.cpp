#include "StatePlay.h"
#include "StateGameOver.h"
#include "GameStateManager.h"

#include <cstdlib>
#include <ctime> 
#include <cmath> //Needed for bullet trajectory math
#include <fstream>

StatePlay::StatePlay(GameStateManager* stateManagerPointer, IDirect3DDevice9* direct3DDevice, PlayerInput* playerInputPointer, LineManager* lineManagerPointer, SpriteManager* spriteManagerPointer, int initialScreenWidth, int initialScreenHeight) : IGameState(stateManagerPointer) {
    //Variables
    input = playerInputPointer;
    lineManager = lineManagerPointer;
    spriteManager = spriteManagerPointer;
    screenWidth = initialScreenWidth;
    screenHeight = initialScreenHeight;
    firingMode = 1;
    highscore = 0;
    fishAmount = 10;            //Change how many fishes to spawn
    fishRespawnCooldown = 2.0f; //Change how long to wait, then check to respawn fishes
    fishRespawnTimer = 0.0f;    //This is just a time tracker, leave at zero
    wasMouseButtonDown = false;
    wasEnterDown = false;
    foregroundBobTime = 0.0f;
    cannonAnimationFrame = 0;
    cannonAnimationTick = 0;
    cannonAnimating = false;
    d3dDevice = direct3DDevice;
    LoadHighscore();

    //Initialize tools
    fontManager.Initialize(direct3DDevice, 20, "Arial");
    uiManager = new UIManager(lineManager, &fontManager);

    //Initialize sprites
    const char* placeholderPath = "Assets/practical9.png";  //Temporarily using the practical spaceship for now (note: it is 64x64 and loads from top to bottom)
    spriteManager->RegisterSprite("Crosshair", placeholderPath, { 0, 0, 32, 32 });
    spriteManager->RegisterSprite("Bullet", placeholderPath, { 0, 0, 32, 32 });
    spriteManager->RegisterAnimatedSprite("rainbow_whale", "Assets/Sprite/rainbow_whale.png", 128, 128, 4, 4, 8);

    const RECT backgroundRect = { 0, 0, 512, 512 };
    spriteManager->RegisterSprite("Background1", "Assets/Sprite/Background/1.png", backgroundRect);
    spriteManager->RegisterSprite("Background2", "Assets/Sprite/Background/2.png", backgroundRect);
    spriteManager->RegisterSprite("Background3", "Assets/Sprite/Background/3.png", backgroundRect);
    spriteManager->RegisterSprite("Background4", "Assets/Sprite/Background/4.png", backgroundRect);
    spriteManager->RegisterSprite("Background5", "Assets/Sprite/Background/5.png", backgroundRect);
    spriteManager->RegisterAnimatedSprite("Cannon", "Assets/Sprite/cannonSheet.png", 60, 100, 3, 3, 9);

    backgroundSprite = spriteManager->GetSprite("Background1");
    foregroundSprites[0] = spriteManager->GetSprite("Background2");
    foregroundSprites[1] = spriteManager->GetSprite("Background3");
    foregroundSprites[2] = spriteManager->GetSprite("Background4");
    foregroundSprites[3] = spriteManager->GetSprite("Background5");
    cannonSprite = spriteManager->GetSprite("Cannon");

    SpriteData crosshairSprite = spriteManager->GetSprite("Crosshair");
    crosshairSprite.red = 255; crosshairSprite.green = 255; crosshairSprite.blue = 255;
    player.SetSprite(crosshairSprite);

    SpriteData rainbowWhaleSprite = spriteManager->GetSprite("rainbow_whale");
    rainbowWhaleSprite.red = 255; rainbowWhaleSprite.green = 255; rainbowWhaleSprite.blue = 255;

    //SpriteData fishSprite = spriteManager->GetSprite("GoldFish");
    //fishSprite.red = 255; fishSprite.green = 50; fishSprite.blue = 50;


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

        aiManager.SpawnFish(D3DXVECTOR2(randomPositionX, randomPositionY), D3DXVECTOR2(velocityX, velocityY), 10, 100, rainbowWhaleSprite);
    }
    for (NPC* fish : aiManager.GetFishes()) {
        int min = 1;
        int max = 3;
        int randomNumber = min + (std::rand() % (max - min + 1));
        fish->SetScale(fish->GetScale() * randomNumber);

    }


    // 1. Small Pebble - Top Far Left
    obstacleManager.AddObstacle({
        D3DXVECTOR2(120, 80),
        D3DXVECTOR2(85, 105),
        D3DXVECTOR2(90, 140),
        D3DXVECTOR2(130, 155),
        D3DXVECTOR2(165, 130),
        D3DXVECTOR2(155, 95)
        });

    // 2. Jagged Rock - Top Mid-Left
    obstacleManager.AddObstacle({
        D3DXVECTOR2(480, 140),
        D3DXVECTOR2(440, 165),
        D3DXVECTOR2(425, 205),
        D3DXVECTOR2(450, 240),
        D3DXVECTOR2(495, 250),
        D3DXVECTOR2(535, 225),
        D3DXVECTOR2(540, 175),
        D3DXVECTOR2(515, 145)
        });

    // 3. Flat Rock - Top Center
    obstacleManager.AddObstacle({
        D3DXVECTOR2(820, 90),
        D3DXVECTOR2(760, 115),
        D3DXVECTOR2(750, 150),
        D3DXVECTOR2(790, 175),
        D3DXVECTOR2(850, 180),
        D3DXVECTOR2(895, 150),
        D3DXVECTOR2(880, 105)
        });

    // 4. Sharp Boulder - Top Mid-Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1260, 110),
        D3DXVECTOR2(1215, 135),
        D3DXVECTOR2(1200, 175),
        D3DXVECTOR2(1225, 210),
        D3DXVECTOR2(1270, 225),
        D3DXVECTOR2(1310, 195),
        D3DXVECTOR2(1315, 150),
        D3DXVECTOR2(1290, 115)
        });

    // 5. Angular Rock - Top Far Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1740, 130),
        D3DXVECTOR2(1690, 160),
        D3DXVECTOR2(1685, 205),
        D3DXVECTOR2(1720, 240),
        D3DXVECTOR2(1775, 245),
        D3DXVECTOR2(1815, 210),
        D3DXVECTOR2(1805, 160)
        });

    // 6. Compact Rock - Upper Left-Center
    obstacleManager.AddObstacle({
        D3DXVECTOR2(310, 340),
        D3DXVECTOR2(270, 365),
        D3DXVECTOR2(260, 405),
        D3DXVECTOR2(285, 440),
        D3DXVECTOR2(330, 450),
        D3DXVECTOR2(370, 420),
        D3DXVECTOR2(375, 375),
        D3DXVECTOR2(345, 345)
        });

    // 7. Small Crag - Upper Center
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1040, 280),
        D3DXVECTOR2(1005, 305),
        D3DXVECTOR2(995, 340),
        D3DXVECTOR2(1020, 370),
        D3DXVECTOR2(1060, 380),
        D3DXVECTOR2(1095, 355),
        D3DXVECTOR2(1090, 310)
        });

    // 8. Rounded Rock - Mid Far Left
    obstacleManager.AddObstacle({
        D3DXVECTOR2(150, 480),
        D3DXVECTOR2(105, 510),
        D3DXVECTOR2(95, 555),
        D3DXVECTOR2(120, 595),
        D3DXVECTOR2(170, 605),
        D3DXVECTOR2(210, 575),
        D3DXVECTOR2(215, 525),
        D3DXVECTOR2(185, 490)
        });

    // 9. Wedge Rock - Mid-Center Left
    obstacleManager.AddObstacle({
        D3DXVECTOR2(670, 430),
        D3DXVECTOR2(625, 460),
        D3DXVECTOR2(615, 505),
        D3DXVECTOR2(645, 540),
        D3DXVECTOR2(695, 550),
        D3DXVECTOR2(740, 520),
        D3DXVECTOR2(745, 470),
        D3DXVECTOR2(710, 435)
        });

    // 10. Center Rock
    obstacleManager.AddObstacle({
        D3DXVECTOR2(910, 490),
        D3DXVECTOR2(865, 515),
        D3DXVECTOR2(850, 555),
        D3DXVECTOR2(875, 595),
        D3DXVECTOR2(925, 610),
        D3DXVECTOR2(970, 580),
        D3DXVECTOR2(975, 535),
        D3DXVECTOR2(945, 495)
        });

    // 11. Diamond-ish Rock - Mid-Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1480, 410),
        D3DXVECTOR2(1435, 440),
        D3DXVECTOR2(1425, 485),
        D3DXVECTOR2(1455, 520),
        D3DXVECTOR2(1505, 530),
        D3DXVECTOR2(1545, 500),
        D3DXVECTOR2(1550, 455),
        D3DXVECTOR2(1520, 420)
        });

    // 12. Pebble - Mid Far Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1800, 460),
        D3DXVECTOR2(1765, 485),
        D3DXVECTOR2(1755, 520),
        D3DXVECTOR2(1780, 550),
        D3DXVECTOR2(1820, 560),
        D3DXVECTOR2(1855, 535),
        D3DXVECTOR2(1850, 490)
        });

    // 13. Low-Left Rock
    obstacleManager.AddObstacle({
        D3DXVECTOR2(390, 660),
        D3DXVECTOR2(345, 690),
        D3DXVECTOR2(335, 735),
        D3DXVECTOR2(365, 770),
        D3DXVECTOR2(415, 780),
        D3DXVECTOR2(455, 750),
        D3DXVECTOR2(460, 705),
        D3DXVECTOR2(430, 670)
        });

    // 14. Wide Slab - Lower Center-Left
    obstacleManager.AddObstacle({
        D3DXVECTOR2(730, 690),
        D3DXVECTOR2(675, 715),
        D3DXVECTOR2(665, 755),
        D3DXVECTOR2(700, 790),
        D3DXVECTOR2(755, 800),
        D3DXVECTOR2(800, 770),
        D3DXVECTOR2(795, 725),
        D3DXVECTOR2(765, 695)
        });

    // 15. Rough Boulder - Lower Center-Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1180, 620),
        D3DXVECTOR2(1135, 650),
        D3DXVECTOR2(1120, 695),
        D3DXVECTOR2(1150, 735),
        D3DXVECTOR2(1200, 750),
        D3DXVECTOR2(1245, 720),
        D3DXVECTOR2(1250, 670),
        D3DXVECTOR2(1220, 630)
        });

    // 16. Medium Rock - Lower Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1630, 660),
        D3DXVECTOR2(1580, 690),
        D3DXVECTOR2(1570, 735),
        D3DXVECTOR2(1600, 770),
        D3DXVECTOR2(1655, 785),
        D3DXVECTOR2(1700, 755),
        D3DXVECTOR2(1705, 705),
        D3DXVECTOR2(1670, 670)
        });

    // 17. Ground Pebble - Bottom Far Left
    obstacleManager.AddObstacle({
        D3DXVECTOR2(170, 870),
        D3DXVECTOR2(130, 895),
        D3DXVECTOR2(120, 935),
        D3DXVECTOR2(145, 970),
        D3DXVECTOR2(190, 980),
        D3DXVECTOR2(230, 950),
        D3DXVECTOR2(230, 905),
        D3DXVECTOR2(205, 875)
        });

    // 18. Ground Rock - Bottom Mid-Left
    obstacleManager.AddObstacle({
        D3DXVECTOR2(560, 860),
        D3DXVECTOR2(515, 890),
        D3DXVECTOR2(505, 930),
        D3DXVECTOR2(535, 965),
        D3DXVECTOR2(585, 975),
        D3DXVECTOR2(625, 945),
        D3DXVECTOR2(630, 900),
        D3DXVECTOR2(600, 865)
        });


    // 19. Ground Rock - Bottom Mid-Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1390, 840),
        D3DXVECTOR2(1345, 870),
        D3DXVECTOR2(1335, 915),
        D3DXVECTOR2(1365, 950),
        D3DXVECTOR2(1415, 960),
        D3DXVECTOR2(1455, 930),
        D3DXVECTOR2(1460, 885),
        D3DXVECTOR2(1430, 845)
        });

    // 20. Ground Slab - Bottom Far Right
    obstacleManager.AddObstacle({
        D3DXVECTOR2(1760, 870),
        D3DXVECTOR2(1710, 900),
        D3DXVECTOR2(1700, 940),
        D3DXVECTOR2(1730, 975),
        D3DXVECTOR2(1785, 985),
        D3DXVECTOR2(1825, 955),
        D3DXVECTOR2(1830, 910),
        D3DXVECTOR2(1800, 875)
        });
}

StatePlay::~StatePlay() {
    SaveHighscore();
    delete uiManager;
    delete audioManager;
}

void StatePlay::LoadHighscore() {
    std::ifstream highscoreFile("highscore.txt");
    if (highscoreFile) {
        highscoreFile >> highscore;
        if (highscore < 0) {
            highscore = 0;
        }
    }
}

void StatePlay::SaveHighscore() {
    if (player.GetScore() > highscore) {
        highscore = player.GetScore();
    }

    std::ofstream highscoreFile("highscore.txt");
    if (highscoreFile) {
        highscoreFile << highscore;
    }
}

void StatePlay::Input() {
    bool isEnterDown = input->IsKeyDown(DIK_RETURN);
    if (isEnterDown && !wasEnterDown) {
        player.AddScore(1000);
    }
    wasEnterDown = isEnterDown;

    int previousFiringMode = firingMode;
    if (input->IsKeyDown(DIK_1)) {
        firingMode = 1;
    }
    else if (input->IsKeyDown(DIK_2)) {
        firingMode = 2;
    }
    else if (input->IsKeyDown(DIK_3)) {
        firingMode = 3;
    }
    if (firingMode != previousFiringMode) {
        cannonAnimationFrame = 0;
        cannonAnimationTick = 0;
        cannonAnimating = false;
    }

    //Subtract 16 from X and Y to perfectly center the 32x32 crosshair sprite on the mouse
    player.UpdateInput((float)input->GetMousePosition().x - 16.0f, (float)input->GetMousePosition().y - 16.0f);

    bool isMouseButtonDown = input->IsMouseButtonDown(0);
    if (isMouseButtonDown && !wasMouseButtonDown) {
        const int shotCost = firingMode * 100;
        if (player.GetScore() < shotCost) {
            wasMouseButtonDown = isMouseButtonDown;
            return;
        }

        cannonAnimationFrame = 0;
        cannonAnimationTick = 0;
        cannonAnimating = true;

        //Anchor the gun to the bottom middle
        D3DXVECTOR2 gunPosition((float)(screenWidth / 2), (float)screenHeight - 50.0f);

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

        SpriteData bulletSprite = spriteManager->GetSprite("Bullet");
        bulletSprite.red = 255; bulletSprite.green = 255; bulletSprite.blue = 0;

        const float bulletPositionOffsets[3] = { -16.0f, 0.0f, 16.0f };
        for (int i = 0; i < 3; i++) {
            if (firingMode == 1 && i != 1) {
                continue;
            }
            if (firingMode == 2 && i == 1) {
                continue;
            }

            D3DXVECTOR2 bulletPosition = gunPosition + D3DXVECTOR2(bulletPositionOffsets[i], 0.0f);
            bulletsManager.SpawnBullet(bulletPosition, direction * 15.0f, 10, bulletSprite, screenWidth, screenHeight);
        }

        player.AddScore(-shotCost);
    }
    wasMouseButtonDown = isMouseButtonDown;
}

void StatePlay::Update(float deltaTime) {
    foregroundBobTime += deltaTime;

    if (cannonAnimating) {
        cannonAnimationTick++;
        if (cannonAnimationTick >= 10) {
            cannonAnimationTick = 0;
            cannonAnimationFrame++;
            if (cannonAnimationFrame >= 3) {
                cannonAnimationFrame = 0;
                cannonAnimating = false;
            }
        }
    }

    //Fish respawning logic
    fishRespawnTimer -= deltaTime;
    if (fishRespawnTimer <= 0.0f) {
        int activeFishes = 0;
        for (NPC* fish : aiManager.GetFishes()) {
            if (fish->IsActive()) activeFishes++;
            if (fish->GetVelocity().x < 0 && fish->GetScale().x > 0) {
                fish->SetScale(D3DXVECTOR2(-fish->GetScale().x, fish->GetScale().y));
            }
            else if (fish->GetVelocity().x > 0 && fish->GetScale().x < 0) {
                fish->SetScale(D3DXVECTOR2(-fish->GetScale().x, fish->GetScale().y));
            }
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
    // 1. Draw Background layers
    spriteManager->Begin();

    D3DXVECTOR2 backgroundScale(screenWidth / 512.0f, screenHeight / 512.0f);
    spriteManager->Draw(backgroundSprite, D3DXVECTOR2(0.0f, 0.0f), 0.0f, backgroundScale);

    const float bobSpeeds[4] = { 0.8f, 1.1f, 1.4f, 1.7f };
    const float bobAmplitude = 8.0f;
    for (int i = 0; i < 4; i++) {
        spriteManager->DrawBobbing(foregroundSprites[i], D3DXVECTOR2(0.0f, 0.0f), foregroundBobTime, bobSpeeds[i], bobAmplitude, backgroundScale);
    }

    spriteManager->End();

    obstacleManager.Render(lineManager);

    spriteManager->Begin();
    aiManager.Render(spriteManager);
    bulletsManager.Render(spriteManager);

    D3DXVECTOR2 cannonPosition((float)(screenWidth / 2), (float)screenHeight - 70.0f);
    D3DXVECTOR2 cannonTarget = player.GetPosition() + D3DXVECTOR2(16.0f, 16.0f);
    D3DXVECTOR2 cannonDirection = cannonTarget - cannonPosition;
    float cannonRotation = atan2(cannonDirection.y, cannonDirection.x) + D3DX_PI / 2.0f;
    int cannonFrame = ((firingMode - 1) * 3) + cannonAnimationFrame;
    spriteManager->DrawAnimationFrame(cannonSprite, cannonFrame, cannonPosition, cannonRotation);

    uiManager->DrawTopBar(player.GetScore(), 0, highscore, screenWidth);
	uiManager->DrawSideBar(screenHeight, screenWidth);
    spriteManager->End();
}