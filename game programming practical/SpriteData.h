#pragma once
#include <d3d9.h>
#include <d3dx9.h>

struct SpriteData {
    LPDIRECT3DTEXTURE9 texture = nullptr;
    RECT rect{};
    int red = 255;
    int green = 255;
    int blue = 255;
};