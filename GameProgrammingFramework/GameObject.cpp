#include "GameObject.h"

GameObject::GameObject() {
    position    = D3DXVECTOR2(0.0f, 0.0f);
    velocity    = D3DXVECTOR2(0.0f, 0.0f);
    rotation    = 0.0f;
    radius      = 10.0f;
    isActive    = true;
}

GameObject::~GameObject() {}

void GameObject::Render(SpriteManager* spriteManager) {
    if (isActive && spriteManager) {
        spriteManager->Draw(sprite, position, rotation);
    }
}