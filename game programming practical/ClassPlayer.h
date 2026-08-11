#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3dx9.h>

class ClassPlayer {
public:
    D3DXVECTOR2 position;
    D3DXVECTOR2 velocity;
    float rotation;
    float enginePower;
    float mass;
    float radius;

    int currentFrame;
    int colorIndex;
    int animationTick;

    int score;
    int hurtTimer;

    ClassPlayer();
    void Init(float startX, float startY, int color);
    void Update(bool up, bool left, bool down, bool right, int screenW, int screenH, int topBound);
};