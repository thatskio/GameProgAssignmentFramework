#include "ClassInput.h"
#include <iostream>

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
        dInputKeyboardDevice = NULL;
    }

    if (dInputMouseDevice) {
        dInputMouseDevice->Unacquire();
        dInputMouseDevice->Release();
        dInputMouseDevice = NULL;
    }

    if (dInput) {
        dInput->Release();
        dInput = NULL;
    }
}

// --- INITIALIZATION ---
bool ClassInput::Initialize(HWND hWnd) {
    // Create the Direct Input object
    HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);
    if (FAILED(hr)) return false;

    // Create the keyboard device
    hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
    if (FAILED(hr)) return false;
    dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    dInputKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputKeyboardDevice->Acquire();

    // Create the mouse device
    hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);
    if (FAILED(hr)) return false;
    dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
    dInputMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputMouseDevice->Acquire();

    return true;
}

// --- UPDATE LOOP ---
void ClassInput::Update() {
    // Get mouse
    if (dInputMouseDevice) {
        dInputMouseDevice->Acquire();
        dInputMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
        mousePosition.x += mouseState.lX;
        mousePosition.y += mouseState.lY; 
    }

    // Get keyboard
    if (dInputKeyboardDevice) {
        dInputKeyboardDevice->Acquire();
        dInputKeyboardDevice->GetDeviceState(256, (LPVOID)diKeys);
    }
}

// --- HELPERS ---
bool ClassInput::IsKeyDown(int key) {
    return (diKeys[key] & 0x80) != 0;
}

bool ClassInput::IsMouseButtonDown(int button) {
    // 0 = Left Click, 1 = Right Click, 2 = Middle Click
    return (mouseState.rgbButtons[button] & 0x80) != 0;
}

long ClassInput::GetMouseDeltaX() {
    return mouseState.lX;
}

long ClassInput::GetMouseDeltaY() {
    return mouseState.lY;
}

D3DXVECTOR3 ClassInput::GetMousePosition() {
    return mousePosition;
}