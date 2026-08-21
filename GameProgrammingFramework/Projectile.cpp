#include "Projectile.h"

Projectile::Projectile() {
    damage      = 0;
    isActive    = false;
    radius      = 5.0f;
}

Projectile::~Projectile() {}

void Projectile::Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 vel, int dmg) {
    position    = startPos;
    velocity    = vel;
    damage      = dmg;
    isActive    = true;
}

void Projectile::Update() {
    if (!isActive) { return; }
    position += velocity;
}