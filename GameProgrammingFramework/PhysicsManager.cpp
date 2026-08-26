#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() {}
PhysicsManager::~PhysicsManager() {}

//If the distance squared between 2 objects is less than the combined radius squared, means collision = true
bool PhysicsManager::SimpleCircleCollision(GameObject* objA, GameObject* objB) {
    float deltaX = objB->GetPosition().x - objA->GetPosition().x;
    float deltaY = objB->GetPosition().y - objA->GetPosition().y;
    float squaredDistance = (deltaX * deltaX) + (deltaY * deltaY);
    float combinedRadius = objA->GetRadius() + objB->GetRadius();

    return squaredDistance < (combinedRadius * combinedRadius);
}

bool SATCollision(Projectile* projectile, Obstacle* obstacle) {
    std::vector<D3DXVECTOR2> axes(obstacle->GetNodeCount());

	// Getting the normal for each side of the polygon (obstacle)
    for (int i = 0; i < obstacle->GetNodeCount(); i++) {
        // get the current vertex
        D3DXVECTOR2 p1 = obstacle->GetNode(i);

        // get the next vertex
        D3DXVECTOR2 p2 = obstacle->GetNode(i + 1 == obstacle->GetNodeCount() ? 0 : i + 1);

        // subtract the two to get the edge vector
        D3DXVECTOR2 edge = p1 - p2;

        // get either perpendicular vector
        D3DXVECTOR2 normal = D3DXVECTOR2(-edge.y, edge.x);

        // the perp method is just (x, y) =&gt; (-y, x) or (y, -x)
        axes[i] = normal;
    }



}


void PhysicsManager::ProcessPhysics(AILogicManager* aiManager, BulletsManager* projManager, Player* player, AudioManager* audioManager, ObstacleManager* obstacleManager, int screenWidth, int screenHeight) {
    std::vector<Projectile*>& bullets = projManager->GetBullets();
    std::vector<NPC*>& fishes = aiManager->GetFishes();
    std::vector<Obstacle*>& obstacles = obstacleManager->GetObstacles();

    
    // Compute collisions for bullets
    for (Projectile* bullet : bullets) {
        if (!bullet->IsActive()) { continue; }

        D3DXVECTOR2 bPos = bullet->GetPosition();

        if (bPos.x < 0 || bPos.x > screenWidth || bPos.y < 0 || bPos.y > screenHeight) {
            bullet->SetActive(false);
            continue;
        }

		//Bullet against obstacle collision
        for (Obstacle* obstacle : obstacles) {
			if (SATCollision(bullet, obstacle)) {
				bullet->SetActive(false);
				break;
			}
        }



        //Bullet against fish collision
        for (NPC* fish : fishes) {
            if (!fish->IsActive()) { continue; }

            if (SimpleCircleCollision(bullet, fish)) {
                fish->TakeDamage(bullet->GetDamage());
                bullet->SetActive(false);

                if (audioManager) {
                    audioManager->PlayAudio("Boing");
                }

                if (!fish->IsActive()) {
                    player->AddScore(fish->GetPointValue());
                }
                break;
            }
        }
    }

    for (NPC* fish : fishes) {
        if (!fish->IsActive()) { continue; }

        D3DXVECTOR2 fPos = fish->GetPosition();
        D3DXVECTOR2 fVel = fish->GetVelocity();
        float radius = fish->GetRadius();

        if (fPos.x - radius < 0) {
            fPos.x = radius;
            fVel.x *= -1.0f;
        }
        else if (fPos.x + radius > screenWidth) {
            fPos.x = screenWidth - radius;
            fVel.x *= -1.0f;
        }

        if (fPos.y - radius < 0) {
            fPos.y = radius;
            fVel.y *= -1.0f;
        }
        else if (fPos.y + radius > screenHeight) {
            fPos.y = screenHeight - radius;
            fVel.y *= -1.0f;
        }

        fish->SetPosition(fPos);
        fish->SetVelocity(fVel);
    }

}
