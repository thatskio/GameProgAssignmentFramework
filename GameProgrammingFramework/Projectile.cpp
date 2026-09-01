#include "Projectile.h"
#define BOUNCE_FACTOR -1.0f



Projectile::Projectile() {
    damage      = 0;
    obstacleHitCount = 0;
    isActive    = false;
    radius      = 5.0f;
}

Projectile::~Projectile() {}

void Projectile::Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 vel, int dmg) {
    position    = startPos;
    velocity    = vel;
    damage      = dmg;
    obstacleHitCount = 0;
    isActive    = true;
}

void Projectile::Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 vel, int dmg, float scrWidth, float scrnHeight) {
    position = startPos;
    velocity = vel;
    damage = dmg;
    obstacleHitCount = 0;
    isActive = true;
    screenWidth = scrWidth;
    screenHeight = scrnHeight;
}

bool Projectile::RegisterObstacleHit() {
    obstacleHitCount++;
    return obstacleHitCount >= 5;
}

D3DXVECTOR2 Projectile::GetCenter()
{
	return D3DXVECTOR2(position.x + (sprite.rect.right - sprite.rect.left) / 2.0f,
		position.y + (sprite.rect.bottom - sprite.rect.top) / 2.0f);
}

void Projectile::Update() {
    if (!isActive) { return; }
    UpdatePosition();
    BorderCollision();


}

void Projectile::UpdatePosition()
{
    position += velocity;
}

void Projectile::BorderCollision() {
    const float bulletWidth = static_cast<float>(sprite.rect.right - sprite.rect.left);
    const float bulletHeight = static_cast<float>(sprite.rect.bottom - sprite.rect.top);
    const float topEdge = 0;
    const float leftEdge = 0;
    const float rightEdge = screenWidth - bulletWidth;
    const float bottomEdge = screenHeight - bulletHeight;

    if (position.x <= topEdge) {
        position.x = 0.0f;
        velocity.x = velocity.x * BOUNCE_FACTOR * mass;
    }
    else if (position.x >= rightEdge) {
        position.x = rightEdge;
        velocity.x = velocity.x * BOUNCE_FACTOR * mass;
    }

    if (position.y <= topEdge) {
        position.y = 0.0f;
        velocity.y = velocity.y * BOUNCE_FACTOR * mass;
    }
    else if (position.y >= bottomEdge) {
        position.y = bottomEdge;
        velocity.y = velocity.y * BOUNCE_FACTOR * mass;
    }
}