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

    //We use "const std::string& " to ensure all passed string formats are same standard, and can be safely read/reference.
    void DrawText(const std::string& text, RECT rect, D3DCOLOR color, DWORD alignment);
};