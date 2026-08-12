#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() {
    d3dDevice = nullptr;
    monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    monitorHeight = GetSystemMetrics(SM_CYSCREEN);
}

GraphicsManager::~GraphicsManager() {
    maid.DoCleaning();
    if (d3dDevice) d3dDevice->Release();
}

bool GraphicsManager::Initialize(HWND hWnd) {
    IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    ZeroMemory(&d3dPP, sizeof(d3dPP));

    d3dPP.Windowed = true;
    d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPP.BackBufferCount = 1;
    d3dPP.BackBufferWidth = 800;
    d3dPP.BackBufferHeight = 600;
    d3dPP.hDeviceWindow = hWnd;

    HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);
    if (FAILED(hr)) return false;

    maid.Init(d3dDevice);
    return true;
}

void GraphicsManager::ClearScreen(int r, int g, int b) {
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
}

void GraphicsManager::BeginScene() { d3dDevice->BeginScene(); }

void GraphicsManager::EndScene() {
    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}