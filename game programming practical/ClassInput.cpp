#include "ClassInput.h"

ClassInput::ClassInput() {
    dInput = NULL;
    dInputKeyboardDevice = NULL;
    dInputMouseDevice = NULL;

    ZeroMemory(diKeys, sizeof(diKeys));
    ZeroMemory(&mouseState, sizeof(mouseState));
    mousePosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

ClassInput::~ClassInput() {
    if (dInputKeyboardDevice) {
        dInputKeyboardDevice->Unacquire();
        dInputKeyboardDevice->Release();
    }
    if (dInputMouseDevice) {
        dInputMouseDevice->Unacquire();
        dInputMouseDevice->Release();
    }
    if (dInput) dInput->Release();
}

bool ClassInput::Initialize(HWND hWnd) {
    targetWindow = hWnd;
    HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);
    if (FAILED(hr)) return false;

    hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
    if (FAILED(hr)) return false;
    dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    dInputKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputKeyboardDevice->Acquire();

    hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);
    if (FAILED(hr)) return false;
    dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
    dInputMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputMouseDevice->Acquire();

    return true;
}

void ClassInput::Update() {
    if (dInputMouseDevice) {
        dInputMouseDevice->Acquire();
        dInputMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
    }

    if (dInputKeyboardDevice) {
        dInputKeyboardDevice->Acquire();
        dInputKeyboardDevice->GetDeviceState(256, (LPVOID)diKeys);
    }

    // --- NEW ABSOLUTE TRACKING: Hijack the OS Cursor ---
    POINT pt;
    GetCursorPos(&pt); // Get the absolute screen position of the cursor
    ScreenToClient(targetWindow, &pt); // Translate it to be relative to your game window

    mousePosition.x = (float)pt.x;
    mousePosition.y = (float)pt.y;
    // ---------------------------------------------------
}

bool ClassInput::IsKeyDown(int key) {
    return (diKeys[key] & 0x80) != 0;
}

bool ClassInput::IsMouseButtonDown(int button) {
    return (mouseState.rgbButtons[button] & 0x80) != 0;
}

long ClassInput::GetMouseDeltaX() { return mouseState.lX; }
long ClassInput::GetMouseDeltaY() { return mouseState.lY; }
D3DXVECTOR3 ClassInput::GetMousePosition() { return mousePosition; }