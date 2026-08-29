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
    D3DXVECTOR2 Project(const D3DXVECTOR2& axis, const Obstacle& obstacle);
    void SATCollision(Obstacle* obstacle1, Obstacle* obstacle2);
    void SATCollision(Projectile* projectile, Obstacle* obstacle); // for circle vs polygon

    //bool CheckCollision(GameObject* objA, GameObject* objB, const std::string collisionType); //Circle, BoundingBox, SAT


public:
    PhysicsManager();
    ~PhysicsManager();

    void ProcessPhysics(AILogicManager* aiManager, BulletsManager* projManager, Player* player, AudioManager* audioManager, ObstacleManager* obstacleManager, int screenWidth, int screenHeight);
};