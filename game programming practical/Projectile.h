#pragma once
#include "GameObject.h"

class Projectile : public GameObject {
private:
    int damage;

public:
    Projectile();
    ~Projectile();

    void Update() override;
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 vel, int dmg);

    int GetDamage() const { return damage; }
};