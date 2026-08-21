#pragma once
#include "AILogicManager.h"
#include "BulletsManager.h"
#include "Player.h"
#include "AudioManager.h"

class PhysicsManager {
private:
    bool CheckCollision(GameObject* objA, GameObject* objB);

public:
    PhysicsManager();
    ~PhysicsManager();

    void ProcessPhysics(AILogicManager* aiManager, BulletsManager* projManager, Player* player, AudioManager* audioManager, int screenWidth, int screenHeight);
};