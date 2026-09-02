#pragma once
#include "GameObject.h"

class Projectile : public GameObject {
private:
    int damage;
    int obstacleHitCount;

public:
    Projectile();
    ~Projectile();

    void Update() override; //"override" is to replace the "virtual" given to game objects (check GameObject.h)
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 vector, int dmg);
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 vel, int dmg, float scrWidth, float scrnHeight);
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 vel, int dmg, float scrWidth, float scrnHeight, float bulletMass);

    int GetDamage() const { return damage; }
    bool RegisterObstacleHit();
    D3DXVECTOR2 GetCenter();


    void UpdatePosition();

    //Collsion
    void BorderCollision();

};