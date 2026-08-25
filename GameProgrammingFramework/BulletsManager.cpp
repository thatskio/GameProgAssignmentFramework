#include "BulletsManager.h"

BulletsManager::BulletsManager(int poolSize) {
    for (int i = 0; i < poolSize; i++) {
        bulletPool.push_back(new Projectile());
    }
}

BulletsManager::~BulletsManager() {
    for (Projectile* bullet : bulletPool) {
        delete bullet;
    }
    bulletPool.clear();
}

void BulletsManager::SpawnBullet(D3DXVECTOR2 position, D3DXVECTOR2 velocity, int damage, SpriteData sprite) {
    for (Projectile* bullet : bulletPool) {
        if (!bullet->IsActive()) {
            bullet->SetSprite(sprite);
            bullet->Spawn(position, velocity, damage);
            return;
        }
    }
}

void BulletsManager::SpawnBullet(D3DXVECTOR2 position, D3DXVECTOR2 velocity, int damage, SpriteData sprite, float screenWidth, float screenHeight) {
    for (Projectile* bullet : bulletPool) {
        if (!bullet->IsActive()) {
            bullet->SetSprite(sprite);
            bullet->Spawn(position, velocity, damage, screenWidth, screenHeight);
            return;
        }
    }
}

void BulletsManager::Update() {
    for (Projectile* bullet : bulletPool) {
        if (bullet->IsActive()) { bullet->Update(); }
    }
}

void BulletsManager::Render(SpriteManager* spriteManager) {
    for (Projectile* bullet : bulletPool) {
        if (bullet->IsActive()) { bullet->Render(spriteManager); }
    }
}