#include "Maid.h"
#include <string>

void Maid::Init(IDirect3DDevice9* device) {
    d3dDevice = device;
}

ClassSprite Maid::LoadSprite(LPCSTR filePath, RECT rect, D3DXVECTOR3 position) {
    ClassSprite outAsset;
    outAsset.rect = rect;
    outAsset.position = position;

    HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, filePath, &outAsset.texture);

    if (SUCCEEDED(hr) && outAsset.texture) {
        cleanupTasks.push_back(outAsset.texture);
    }
    else {
        std::string gotErrorMsg = std::string("failed to load sprite!\n") + filePath;
        MessageBox(NULL, gotErrorMsg.c_str(), "error", MB_OK | MB_ICONINFORMATION);
    }

    return outAsset;
}

ClassSprite Maid::LoadSpriteKeyed(LPCSTR filePath, RECT rect, D3DXVECTOR3 position, D3DCOLOR colourKey) {
    ClassSprite outAsset;
    outAsset.rect = rect;
    outAsset.position = position;

    HRESULT hr = D3DXCreateTextureFromFileEx(d3dDevice, filePath, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
        colourKey, NULL, NULL, &outAsset.texture);

    if (SUCCEEDED(hr) && outAsset.texture) {
        cleanupTasks.push_back(outAsset.texture);
    }
    else {
        std::string errorMsg = std::string("failed to load keyed sprite!\n") + filePath;
        MessageBox(NULL, errorMsg.c_str(), "error", MB_OK | MB_ICONINFORMATION);
    }

    return outAsset;
}

ClassFont Maid::LoadFontAsset(RECT rect, int fontSize, LPCSTR fontName) {
    ClassFont outAsset;
    outAsset.rect = rect;

    HRESULT hr = D3DXCreateFont(d3dDevice, fontSize, 0, FW_BOLD, 1, false,
        DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontName, &outAsset.font);

    if (SUCCEEDED(hr) && outAsset.font) {
        cleanupTasks.push_back(outAsset.font);
    }

    return outAsset;
}

void Maid::GiveTask(IUnknown* resource) {
    if (resource) {
        cleanupTasks.push_back(resource);
    }
}

void Maid::DoCleaning() {
    // iterate backwards to safely release
    for (int i = cleanupTasks.size() - 1; i >= 0; i--) {
        if (cleanupTasks[i]) {
            cleanupTasks[i]->Release();
        }
    }
    cleanupTasks.clear();
}