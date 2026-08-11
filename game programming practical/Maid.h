#pragma once

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include "ClassSprite.h"

class Maid {
private:
    IDirect3DDevice9* d3dDevice;
    std::vector<IUnknown*> cleanupTasks;    //we use unknown so that it collects every d3dx data types

public:
    void Init(IDirect3DDevice9* device);

    ClassSprite LoadSprite(LPCSTR filePath, RECT rect, D3DXVECTOR3 position);
    ClassSprite LoadSpriteKeyed(LPCSTR filePath, RECT rect, D3DXVECTOR3 position, D3DCOLOR colourKey);
    ClassFont LoadFontAsset(RECT rect, int fontSize, LPCSTR fontName);

    void GiveTask(IUnknown* resource);
    void DoCleaning();
};