#pragma once
#include <vector>
#include "Projectile.h"
#include "SpriteManager.h"

class BulletsManager {
private:
    std::vector<Projectile*> bulletPool;

public:
    BulletsManager(int poolSize = 500);
    ~BulletsManager();

    void SpawnBullet(D3DXVECTOR2 position, D3DXVECTOR2 velocity, int damage, SpriteData sprite);
    void Update();
    void Render(SpriteManager* spriteManager);

    std::vector<Projectile*>& GetBullets() { return bulletPool; }
};