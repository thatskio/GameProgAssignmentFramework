#include "ClassPlayer.h"
#include <math.h>

ClassPlayer::ClassPlayer() {
    position = D3DXVECTOR2(0.0f, 0.0f);
    velocity = D3DXVECTOR2(0.0f, 0.0f);
    rotation = 0.0f;
    enginePower = 0.08f; 
    mass = 1.0f;
    radius = 16.0f;
    currentFrame = 0;
    colorIndex = 0;
    animationTick = 0;
    score = 0;
    hurtTimer = 0;
}

void ClassPlayer::Init(float startX, float startY, int color) {
    position.x = startX;
    position.y = startY;
    colorIndex = color;
    velocity = D3DXVECTOR2(0.0f, 0.0f);
    rotation = 0.0f;
    score = 0;
    hurtTimer = 0;
}

void ClassPlayer::Update(bool up, bool left, bool down, bool right, int screenW, int screenH, int topBound) {
    D3DXVECTOR2 force(0.0f, 0.0f);

    //object oriented movement
    if (up) {
        force.x += enginePower * sinf(rotation);
        force.y -= enginePower * cosf(rotation);
    }
    if (down) {
        force.x -= enginePower * sinf(rotation);
        force.y += enginePower * cosf(rotation);
    }

    //rotation speed increased
    if (left) {
        rotation -= 0.08f;
    }
    if (right) {
        rotation += 0.08f;
    }

    D3DXVECTOR2 acceleration = force / mass;
    velocity += acceleration;
    velocity *= 0.98f;
    position += velocity;

    //boundaries
    if (position.x < 0) {
        position.x = 0;
        velocity.x *= -1.0f;
    }
    if (position.x > screenW - (radius * 2)) {
        position.x = screenW - (radius * 2);
        velocity.x *= -1.0f;
    }

    //new top boundary check
    if (position.y < topBound) {
        position.y = topBound;
        velocity.y *= -1.0f;
    }

    if (position.y > screenH - (radius * 2)) {
        position.y = screenH - (radius * 2);
        velocity.y *= -1.0f;
    }

    animationTick++;
    if (animationTick >= 10) {
        currentFrame = (currentFrame + 1) % 2;
        animationTick = 0;
    }

    if (hurtTimer > 0) {
        hurtTimer--;
    }
}