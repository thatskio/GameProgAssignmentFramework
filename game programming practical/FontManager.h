#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class FontManager {
private:
    LPD3DXFONT font;

public:
    FontManager();
    ~FontManager();

    bool Initialize(IDirect3DDevice9* device, int fontSize, LPCSTR fontName);
    void DrawTextStr(const std::string& text, RECT rect, D3DCOLOR color, DWORD alignment);

    void OnLostDevice();
    void OnResetDevice();
};