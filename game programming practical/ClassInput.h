#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <dinput.h>
#include <d3dx9.h>

class ClassInput {
private:
    LPDIRECTINPUT8 dInput;
    LPDIRECTINPUTDEVICE8 dInputKeyboardDevice;
    LPDIRECTINPUTDEVICE8 dInputMouseDevice;

    BYTE diKeys[256];
    DIMOUSESTATE mouseState;
    D3DXVECTOR3 mousePosition;

public:
    ClassInput();  
    ~ClassInput(); // Cleanup

    // Core 
    bool Initialize(HWND hWnd);
    void Update();

    // Helpers 
    bool IsKeyDown(int key);
    bool IsMouseButtonDown(int key);
    long GetMouseDeltaX();
    long GetMouseDeltaY();
    D3DXVECTOR3 GetMousePosition();
};