#pragma once
#include "AILogicManager.h"
#include "BulletsManager.h"
#include "Player.h"

class PhysicsManager {
private:
    bool CheckCollision(GameObject* objA, GameObject* objB);

public:
    PhysicsManager();
    ~PhysicsManager();

    void ProcessPhysics(AILogicManager* aiManager, BulletsManager* projManager, Player* player, int screenWidth, int screenHeight);
};