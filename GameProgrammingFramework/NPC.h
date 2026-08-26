#pragma once
#include "GameObject.h"

class NPC : public GameObject { //NPC is inheriting from GameObject
private:
    int health;
    int pointValue; //Score to be rewarded

    int currentFrame;
    int animationTick;
    int animationSpeed;

public:
    NPC();
    ~NPC();

    void Update() override; //"override" is to replace the "virtual" given to game objects (check GameObject.h)
    void Render(SpriteManager* spriteManager) override;
    void Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 startVel, int hp, int points);
    void TakeDamage(int damage);

    int GetPointValue() const { return pointValue; }
    int GetHealth() const { return health; }
};