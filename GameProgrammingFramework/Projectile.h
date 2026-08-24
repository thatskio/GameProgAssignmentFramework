#pragma once
#include "GameObject.h"

class Projectile : public GameObject {
private:
    int damage;

public:
    Projectile();
    ~Projectile();

    void Update() override; //"override" is to replace the "virtual" given to game objects (check GameObject.h)
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 velocity, int dmg);
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 velocity, int dmg, float screenWidth, float screenHeight);

    int GetDamage() const { return damage; }
};