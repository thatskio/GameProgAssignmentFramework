#include "NPC.h"

NPC::NPC() {    //Don't assign values here, we assign at NPC.Spawn to allow fish variants in future (that gives different rewards)
    health      = 0;
    pointValue  = 0;
    isActive    = false;
}

NPC::~NPC() {}

void NPC::Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 startVel, int hp, int points) {
    position    = startPos;
    velocity    = startVel;
    health      = hp;
    pointValue  = points;
    isActive    = true;
}

void NPC::Update() {
    if (!isActive) { return; }
    position += velocity;
}

void NPC::TakeDamage(int damage) {
    if (!isActive) { return; }

    health -= damage;
    if (health <= 0) {
        isActive = false;
    }
}