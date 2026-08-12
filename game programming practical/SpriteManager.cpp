#include "SpriteManager.h"

SpriteManager::SpriteManager() {
    spriteBrush = nullptr;
    stateMaid = nullptr;
}

SpriteManager::~SpriteManager() {
    if (spriteBrush) spriteBrush->Release();
}

bool SpriteManager::Initialize(IDirect3DDevice9* device, Maid* maid) {
    stateMaid = maid;
    HRESULT hr = D3DXCreateSprite(device, &spriteBrush);
    return SUCCEEDED(hr);
}

void SpriteManager::RegisterSprite(const std::string& key, LPCSTR filePath, RECT defaultRect) {
    SpriteData newSprite;
    newSprite.rect = defaultRect;

    HRESULT hr = D3DXCreateTextureFromFile(stateMaid->GetDevice(), filePath, &newSprite.texture);
    if (SUCCEEDED(hr) && newSprite.texture) {
        stateMaid->GiveTask(newSprite.texture);
        spriteRegistry[key] = newSprite;
    }
}

SpriteData SpriteManager::GetSprite(const std::string& key) {
    if (spriteRegistry.find(key) != spriteRegistry.end()) {
        return spriteRegistry[key];
    }
    return SpriteData();
}

void SpriteManager::Begin() {
    if (spriteBrush) spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);
}

void SpriteManager::Draw(SpriteData sprite, D3DXVECTOR2 position, float rotation, D3DXVECTOR2 scale) {
    if (!spriteBrush || !sprite.texture) return;

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

void SpriteManager::End() {
    if (spriteBrush) spriteBrush->End();
}

void SpriteManager::OnLostDevice() { if (spriteBrush) spriteBrush->OnLostDevice(); }
void SpriteManager::OnResetDevice() { if (spriteBrush) spriteBrush->OnResetDevice(); }