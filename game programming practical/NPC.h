#pragma once
#include "GameObject.h"

class NPC : public GameObject {
private:
    int health;
    int pointValue;

public:
    NPC();
    ~NPC();

    void Update() override;
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 startVel, int hp, int points);
    void TakeDamage(int damage);

    int GetPointValue() const { return pointValue; }
    int GetHealth() const { return health; }
};