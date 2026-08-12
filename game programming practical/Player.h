#pragma once
#include "GameObject.h"

class Player : public GameObject {
private:
    int score;

public:
    Player();
    ~Player();

    void Update() override;
    void UpdateInput(float mouseX, float mouseY);

    int GetScore() const { return score; }
    void AddScore(int points) { score += points; }
    void ResetScore() { score = 0; }
};