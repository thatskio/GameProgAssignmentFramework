#include "GraphicsManager.h"
#include <iostream>

GraphicsManager::GraphicsManager() {
    direct3DDevice = nullptr;
    monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    monitorHeight = GetSystemMetrics(SM_CYSCREEN);
}

GraphicsManager::~GraphicsManager() {
    if (direct3DDevice) { direct3DDevice->Release(); }
}

bool GraphicsManager::Initialize(HWND windowHandle, int screenWidth, int screenHeight, bool isFullscreen) {
    IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    ZeroMemory(&direct3DPresentParameters, sizeof(direct3DPresentParameters));

    direct3DPresentParameters.Windowed = !isFullscreen;
    direct3DPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    direct3DPresentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
    direct3DPresentParameters.BackBufferCount = 1;
    direct3DPresentParameters.BackBufferWidth = screenWidth;
    direct3DPresentParameters.BackBufferHeight = screenHeight;
    direct3DPresentParameters.hDeviceWindow = windowHandle;

    HRESULT result = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, windowHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &direct3DPresentParameters, &direct3DDevice);
    if (FAILED(result)) {
        std::cout << "Creating Direct3D9 Device Failed!";
        return false;
    }

    return true;
}

void GraphicsManager::ClearScreen(int red, int green, int blue) {
    direct3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0);
}

void GraphicsManager::BeginScene() {
    direct3DDevice->BeginScene();
}

void GraphicsManager::EndScene() {
    direct3DDevice->EndScene();
    direct3DDevice->Present(NULL, NULL, NULL, NULL);
}