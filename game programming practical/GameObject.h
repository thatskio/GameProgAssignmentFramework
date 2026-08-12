#pragma once
#include <d3dx9.h>
#include "SpriteData.h"
#include "SpriteManager.h"

class GameObject {
protected:
    D3DXVECTOR2 position;
    D3DXVECTOR2 velocity;
    float rotation;
    float radius;
    bool isActive;
    SpriteData sprite;

public:
    GameObject();
    virtual ~GameObject();

    virtual void Update() = 0;
    virtual void Render(SpriteManager* spriteManager);

    D3DXVECTOR2 GetPosition() const { return position; }
    D3DXVECTOR2 GetVelocity() const { return velocity; }
    float GetRotation() const { return rotation; }
    float GetRadius() const { return radius; }
    bool IsActive() const { return isActive; }
    SpriteData GetSprite() const { return sprite; }

    void SetPosition(D3DXVECTOR2 pos) { position = pos; }
    void SetVelocity(D3DXVECTOR2 vel) { velocity = vel; }
    void SetRotation(float rot) { rotation = rot; }
    void SetRadius(float r) { radius = r; }
    void SetActive(bool active) { isActive = active; }
    void SetSprite(SpriteData newSprite) { sprite = newSprite; }
};