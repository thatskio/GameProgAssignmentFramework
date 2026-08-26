#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class LineManager {
private:
    LPD3DXLINE line;

public:
    LineManager();
    ~LineManager();

    bool Initialize(IDirect3DDevice9* device);
    void DrawLine(D3DXVECTOR2 p1, D3DXVECTOR2 p2, float thickness, D3DCOLOR color);
    void DrawFilledRect(RECT rect, D3DCOLOR color);
};