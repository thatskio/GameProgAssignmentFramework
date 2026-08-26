#pragma once
#include "GameObject.h"

class Player : public GameObject {
private:
    int score;

public:
    Player();
    ~Player();

    void Update() override; //"override" is to replace the "virtual" given to game objects (check GameObject.h)
    void UpdateInput(float mouseX, float mouseY);

    int GetScore() const { return score; }
    void AddScore(int points) { score += points; }
    void ResetScore() { score = 0; }
};