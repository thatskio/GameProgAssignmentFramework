#pragma once 

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dinput.h>
#include <d3dx9.h>

class PlayerInput {
private:
	LPDIRECTINPUT8 dInput;
	LPDIRECTINPUTDEVICE8 dInputKeyboardDevice;
	LPDIRECTINPUTDEVICE8 dInputMouseDevice;
	HWND targetWindow;			//Handle to window, passed around in GameStates to be used to refresh screen)
	BYTE diKeys[256];			//Storing keyboard key states (pressed/release)
	DIMOUSESTATE mouseState;	
	D3DXVECTOR3 mousePosition;

public:
	PlayerInput();
	~PlayerInput();

	bool Initialize(HWND hWnd);
	void Update();

	bool IsKeyDown(int key);
	bool IsMouseButtonDown(int button);
	long GetMouseDeltaX();
	long GetMouseDeltaY();
	D3DXVECTOR3 GetMousePosition();
};