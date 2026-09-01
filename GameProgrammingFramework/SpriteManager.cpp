#include "SpriteManager.h"
#include <iostream>
#include <cmath>

SpriteManager::SpriteManager() {
    spriteBrush = nullptr;
    d3dDevice = nullptr;
}

SpriteManager::~SpriteManager() {
    for (auto& pair : spriteRegistry) {
        if (pair.second.texture) {
            pair.second.texture->Release();
        }
    }
    spriteRegistry.clear();

    if (spriteBrush) { spriteBrush->Release(); }
}

bool SpriteManager::Initialize(IDirect3DDevice9* device) {
    d3dDevice = device;
    HRESULT hr = D3DXCreateSprite(device, &spriteBrush);
    return SUCCEEDED(hr);
}

void SpriteManager::RegisterSprite(const std::string& key, LPCSTR filePath, RECT defaultRect) {
    SpriteData newSprite;
    newSprite.rect = defaultRect;

    HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, filePath, &newSprite.texture);
    if (SUCCEEDED(hr) && newSprite.texture) {
        spriteRegistry[key] = newSprite;
    }
    else std::cout << "Normal Sprite Registration failed!" << key << std::endl;
}

void SpriteManager::RegisterAnimatedSprite(const std::string& key, LPCSTR filePath, int frameWidth, int frameHeight, int totalColumns, int totalRows, int totalFrames) {
    SpriteData newSprite;
    newSprite.isAnimated = true;
    newSprite.frameWidth = frameWidth;
    newSprite.frameHeight = frameHeight;
    newSprite.totalColumns = totalColumns;
    newSprite.totalRows = totalRows;
    newSprite.totalFrames = totalFrames;
    newSprite.rect = { 0, 0, frameWidth, frameHeight };

    HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, filePath, &newSprite.texture);
    if (SUCCEEDED(hr) && newSprite.texture) {
        spriteRegistry[key] = newSprite;
    }
    else std::cout << "Animated Sprite Registration failed!" << key << std::endl;
}

SpriteData SpriteManager::GetSprite(const std::string& key) {
    if (spriteRegistry.find(key) != spriteRegistry.end()) {
        return spriteRegistry[key];
    }
    else std::cout << "Sprite unable to be found, key invalid! [" << key << "]" << std::endl;

    return SpriteData();
}

void SpriteManager::Begin() {
    if (spriteBrush) { spriteBrush->Begin(D3DXSPRITE_ALPHABLEND); }
}

void SpriteManager::Draw(SpriteData sprite, D3DXVECTOR2 position, float rotation, D3DXVECTOR2 scale) {
    if (!spriteBrush || !sprite.texture) {
        std::cout << "Sprite brush or sprite texture invalid!" << std::endl;
        return;
    }

    D3DXMATRIX matrix;
    D3DXVECTOR2 rotationCenter((sprite.rect.right - sprite.rect.left) * 0.5f, (sprite.rect.bottom - sprite.rect.top) * 0.5f);

    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, &scale, &rotationCenter, rotation, &position);
    spriteBrush->SetTransform(&matrix);

    D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
    D3DCOLOR tintColor = D3DCOLOR_XRGB(sprite.red, sprite.green, sprite.blue);

    spriteBrush->Draw(sprite.texture, &sprite.rect, NULL, &pos, tintColor);

    D3DXMatrixIdentity(&matrix);
    spriteBrush->SetTransform(&matrix);
}

void SpriteManager::DrawBobbing(SpriteData sprite, D3DXVECTOR2 position, float time, float speed, float amplitude, D3DXVECTOR2 scale) {
    position.y += std::sinf(time * speed) * amplitude * scale.y;
    Draw(sprite, position, 0.0f, scale);
}

void SpriteManager::DrawAnimationFrame(SpriteData sprite, int frameIndex, D3DXVECTOR2 position, float rotation, D3DXVECTOR2 scale) {
    if (!spriteBrush || !sprite.texture) {
        std::cout << "Sprite brush or sprite texture invalid!" << std::endl;
        return;
    }

    if (sprite.isAnimated && sprite.totalFrames > 0 && sprite.totalColumns > 0) {
        int safeFrameIndex = frameIndex % sprite.totalFrames;
        int currentColumn = safeFrameIndex % sprite.totalColumns;
        int currentRow = safeFrameIndex / sprite.totalColumns;

        sprite.rect.left = currentColumn * sprite.frameWidth;
        sprite.rect.top = currentRow * sprite.frameHeight;
        sprite.rect.right = sprite.rect.left + sprite.frameWidth;
        sprite.rect.bottom = sprite.rect.top + sprite.frameHeight;
    }

    Draw(sprite, position, rotation, scale);
}

void SpriteManager::End() {
    if (spriteBrush) { spriteBrush->End(); }
}