#pragma once
#include "Projectile.h"
#include "SpriteManager.h"
#include <vector>   //To use arrays

class BulletsManager {
private:
    std::vector<Projectile*> bulletPool;    //To collect all bullet instances in an array

public:
    BulletsManager(int poolSize = 50);      //Create the array size (and give it a max value)
    ~BulletsManager();

    void SpawnBullet(D3DXVECTOR2 position, D3DXVECTOR2 velocity, int damage, SpriteData sprite);
    void SpawnBullet(D3DXVECTOR2 position, D3DXVECTOR2 velocity, int damage, SpriteData sprite, float screenWidth, float screenHeight);
    void Update();
    void Render(SpriteManager* spriteManager);

    std::vector<Projectile*>& GetBullets() { return bulletPool; }   //Return the bullet instances in an array
};