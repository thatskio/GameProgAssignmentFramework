#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() {}
PhysicsManager::~PhysicsManager() {}

//If the distance squared between 2 objects is less than the combined radius squared, means collision = true
bool PhysicsManager::CheckCollision(GameObject* objA, GameObject* objB) {
    float deltaX = objB->GetPosition().x - objA->GetPosition().x;
    float deltaY = objB->GetPosition().y - objA->GetPosition().y;
    float squaredDistance = (deltaX * deltaX) + (deltaY * deltaY);
    float combinedRadius = objA->GetRadius() + objB->GetRadius();

    return squaredDistance < (combinedRadius * combinedRadius);
}

void PhysicsManager::ProcessPhysics(AILogicManager* aiManager, BulletsManager* projManager, Player* player, AudioManager* audioManager, int screenWidth, int screenHeight) {
    //We wanna compute collisions between all bullets & fishes
    std::vector<NPC*>& fishes = aiManager->GetFishes();
    std::vector<Projectile*>& bullets = projManager->GetBullets();

    for (Projectile* bullet : bullets) {
        if (!bullet->IsActive()) { continue; }

        D3DXVECTOR2 bPos = bullet->GetPosition();

        if (bPos.x < 0 || bPos.x > screenWidth || bPos.y < 0 || bPos.y > screenHeight) {
            bullet->SetActive(false);
            continue;
        }

        //Bullet against fish collision
        for (NPC* fish : fishes) {
            if (!fish->IsActive()) { continue; }

            if (CheckCollision(bullet, fish)) {
                fish->TakeDamage(bullet->GetDamage());
                bullet->SetActive(false);

                if (audioManager) {
                    audioManager->PlayAudio("HitSound");
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