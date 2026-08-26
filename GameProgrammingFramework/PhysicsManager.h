#pragma once
#include "AILogicManager.h"
#include "BulletsManager.h"
#include "Player.h"
#include "AudioManager.h"
#include "ObstacleManager.h"

class PhysicsManager {
private:




    bool SimpleCircleCollision(GameObject* objA, GameObject* objB);
	bool BoundingBoxCollision(GameObject* objA, GameObject* objB);
	bool SATCollision(GameObject* objA, GameObject* objB);
    bool SATCollision(Projectile* projectile, Obstacle* obstacle);

    //bool CheckCollision(GameObject* objA, GameObject* objB, const std::string collisionType); //Circle, BoundingBox, SAT


public:
    PhysicsManager();
    ~PhysicsManager();

    void ProcessPhysics(AILogicManager* aiManager, BulletsManager* projManager, Player* player, AudioManager* audioManager, ObstacleManager* obstacleManager, int screenWidth, int screenHeight);
};