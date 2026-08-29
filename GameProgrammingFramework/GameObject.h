#pragma once
#include <d3dx9.h>
#include "SpriteData.h"
#include "SpriteManager.h"

class GameObject {
protected:
    D3DXVECTOR2 position;
    D3DXVECTOR2 velocity;
    SpriteData sprite;
    D3DXVECTOR2 scale;
    float rotation;
    float radius;
    float screenWidth;
    float screenHeight;
    bool isActive;

public:
    GameObject();
    virtual ~GameObject();  

    virtual void Update() = 0;     //We use "virtual" to ensure instanced objects can have their own functions.
    virtual void Render(SpriteManager* spriteManager);

    //FYI: Use const to ensure read-only.
    D3DXVECTOR2 GetPosition() const { return position; }
    D3DXVECTOR2 GetVelocity() const { return velocity; }
    SpriteData GetSprite() const { return sprite; }
    D3DXVECTOR2 GetScale() const { return scale; }
    float GetRotation() const { return rotation; }
    float GetRadius() const { return radius; }
    bool IsActive() const { return isActive; }

    void SetPosition(D3DXVECTOR2 newPos) { position = newPos; }
    void SetVelocity(D3DXVECTOR2 newVelocity) { velocity = newVelocity; }
    void SetSprite(SpriteData newSprite) { sprite = newSprite; }
    void SetScale(D3DXVECTOR2 newScale) { scale = newScale; }
    void SetRotation(float newRot) { rotation = newRot; }
    void SetRadius(float newRadius) { radius = newRadius; }
    void SetActive(bool active) { isActive = active; }  
};