#include "Player.h"

Player::Player() {
    score   = 0;
    radius  = 16.0f;
}

Player::~Player() {}

void Player::Update() {}

void Player::UpdateInput(float mouseX, float mouseY) {
    position.x = mouseX;
    position.y = mouseY;
}