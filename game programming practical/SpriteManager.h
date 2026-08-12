#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <unordered_map>

#include "SpriteData.h"
#include "Maid.h"

class SpriteManager {
private:
    LPD3DXSPRITE spriteBrush;
    std::unordered_map<std::string, SpriteData> spriteRegistry; //Create a dictionary to store registered sprite once, so we can easily fetch and create more later on
    Maid* stateMaid; //Maid just to clean up game states

public:
    SpriteManager();
    ~SpriteManager();

    //Creating sprites and putting them into a dictionary
    bool Initialize(IDirect3DDevice9* device, Maid* maid);
    void RegisterSprite(const std::string& key, LPCSTR filePath, RECT defaultRect);
    SpriteData GetSprite(const std::string& key);

    //Rendering
    void Begin();
    void Draw(SpriteData sprite, D3DXVECTOR2 position, float rotation = 0.0f, D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f));
    void End();
};