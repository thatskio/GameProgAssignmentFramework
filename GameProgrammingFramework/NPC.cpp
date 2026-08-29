#include "NPC.h"

NPC::NPC() {    //Don't assign values here, we assign at NPC.Spawn to allow fish variants in future (that gives different rewards)
    health = 0;
    pointValue = 0;
    isActive = false;
    currentFrame = 0;
    animationTick = 0;
    animationSpeed = 10; //Frames to wait before swapping animation cell (60fps = switches 6 times a second)
}

NPC::~NPC() {}

void NPC::Spawn(D3DXVECTOR2 startPos, D3DXVECTOR2 startVel, int hp, int points) {
    position = startPos;
    velocity = startVel;
    health = hp;
    pointValue = points;
    isActive = true;
    currentFrame = 0;
    animationTick = 0;
}

void NPC::Update() {
    if (!isActive) { return; }
    position += velocity;

    //Animation ticking logic
    if (sprite.isAnimated) {
        animationTick++;
        if (animationTick >= animationSpeed) {
            currentFrame++;
            if (currentFrame >= sprite.totalFrames) {
                currentFrame = 0;
            }
            animationTick = 0;
        }
    }
}

void NPC::Render(SpriteManager* spriteManager) {
    if (isActive && spriteManager) {
        //If animated, use DrawAnimationFrame instead of static Draw
        if (sprite.isAnimated) {
            spriteManager->DrawAnimationFrame(sprite, currentFrame, position, rotation, scale);
        }
        else {
            spriteManager->Draw(sprite, position, rotation, scale);
        }
    }
}

void NPC::TakeDamage(int damage) {
    if (!isActive) { return; }

    health -= damage;
    if (health <= 0) {
        isActive = false;
    }
}