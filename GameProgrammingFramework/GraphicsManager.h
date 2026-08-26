#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d9.h>

class GraphicsManager {
private:
	IDirect3DDevice9* direct3DDevice;
	D3DPRESENT_PARAMETERS direct3DPresentParameters;

	int monitorWidth;
	int monitorHeight;

public:
	GraphicsManager();
	~GraphicsManager();

	bool Initialize(HWND windowHandle, int screenWidth, int screenHeight, bool isFullscreen);
	void ClearScreen(int red, int green, int blue);
	void BeginScene();
	void EndScene();

	IDirect3DDevice9* GetDevice() const { return direct3DDevice; }
	D3DPRESENT_PARAMETERS GetPresentParameters() const { return direct3DPresentParameters; }
};