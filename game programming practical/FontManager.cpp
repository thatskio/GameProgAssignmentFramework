#include "FontManager.h"

FontManager::FontManager() : font(nullptr) {}
FontManager::~FontManager() { if (font) font->Release(); }

bool FontManager::Initialize(IDirect3DDevice9* device, int fontSize, LPCSTR fontName) {
    HRESULT hr = D3DXCreateFont(device, fontSize, 0, FW_BOLD, 1, false,
        DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontName, &font);
    return SUCCEEDED(hr);
}

void FontManager::DrawTextStr(const std::string& text, RECT rect, D3DCOLOR color, DWORD alignment) {
    if (font) font->DrawTextA(NULL, text.c_str(), -1, &rect, alignment, color);
}

void FontManager::OnLostDevice() { if (font) font->OnLostDevice(); }
void FontManager::OnResetDevice() { if (font) font->OnResetDevice(); }