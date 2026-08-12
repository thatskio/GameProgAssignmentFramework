#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d9.h>

#include "Maid.h"

class GraphicsManager {
private:
	IDirect3DDevice9* d3dDevice;
	D3DPRESENT_PARAMETERS d3dPP;
	Maid maid;

	int monitorWidth;
	int monitorHeight;

public:
	GraphicsManager();
	~GraphicsManager();

	bool Initialize(HWND hWnd);
	void ClearScreen(int red, int green, int blue);
	void BeginScene();
	void EndScene();

	IDirect3DDevice9* GetDevice() const { return d3dDevice; }
	D3DPRESENT_PARAMETERS GetPP() const { return d3dPP; }
};