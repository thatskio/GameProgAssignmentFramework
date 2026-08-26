#pragma once
#include <d3d9.h>
#include <d3dx9.h>

//Every sprite carries this exact data when they are initialized 
struct SpriteData {
    LPDIRECT3DTEXTURE9 texture = nullptr;
    RECT rect{};
    int red = 255;
    int green = 255;
    int blue = 255;

    //Animation Metadata
    bool isAnimated = false;
    int frameWidth = 0;
    int frameHeight = 0;
    int totalColumns = 1;
    int totalRows = 1;
    int totalFrames = 1;
};