#include "PlayerInput.h"
#include <iostream>

PlayerInput::PlayerInput() {
    //Initialization
    dInput = NULL;
    dInputKeyboardDevice = NULL;
    dInputMouseDevice = NULL;
    mousePosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //Clearing memory
    ZeroMemory(diKeys, sizeof(diKeys));
    ZeroMemory(&mouseState, sizeof(mouseState));
}

PlayerInput::~PlayerInput() { //Deconstructing the class (clear memory)
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

bool PlayerInput::Initialize(HWND hWnd) {
    targetWindow = hWnd;

    //API to handle input devices (keyboard, controllers, mouse)
    HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);
    if (FAILED(hr)) {
        std::cout << "DirectInput8Create Failed!" << std::endl;
        return false;
    }

    //Creating keyboard
    hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
    if (FAILED(hr)) {
        std::cout << "Creating Keyboard Device Failed!" << std::endl;
        return false;
    }
    //Foreground to focus when game is active tab. Nonexclusive to allow window controls like alt+tab.
    dInputKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard); //256 byte from Input.h
    dInputKeyboardDevice->Acquire();

    //Creating mouse
    hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);
    if (FAILED(hr)) {
        std::cout << "Creating Mouse Device Failed!" << std::endl;
        return false;
    }
    dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
    dInputMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    dInputMouseDevice->Acquire();

    return true;
}

void PlayerInput::Update() {
    // Windows (Native) Input - Update this first
    POINT pt;
    GetCursorPos(&pt);                  //Get the absolute screen position of the cursor
    ScreenToClient(targetWindow, &pt); //Translate it to be relative to your game window
    mousePosition.x = (float)pt.x;
    mousePosition.y = (float)pt.y;

    if (dInputMouseDevice) {
        HRESULT hr = dInputMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
        if (FAILED(hr)) {
            dInputMouseDevice->Acquire();
        }
        //else  std::cout << "DirectX Mouse Device fetch failed!" << std::endl;
    }
    if (dInputKeyboardDevice) {
        HRESULT hr = dInputKeyboardDevice->GetDeviceState(256, (LPVOID)diKeys);
        if (FAILED(hr)) {
            dInputKeyboardDevice->Acquire();
        }
        //else  std::cout << "DirectX Keyboard Device fetch failed!" << std::endl;
    }
}

bool PlayerInput::IsKeyDown(int key) {
    return (diKeys[key] & 0x80) != 0; //FYI: "& 0x80" is just a template
}

bool PlayerInput::IsMouseButtonDown(int button) {
    return (mouseState.rgbButtons[button] & 0x80) != 0;
}

long PlayerInput::GetMouseDeltaX() { return mouseState.lX; }
long PlayerInput::GetMouseDeltaY() { return mouseState.lY; }
D3DXVECTOR3 PlayerInput::GetMousePosition() { return mousePosition; }