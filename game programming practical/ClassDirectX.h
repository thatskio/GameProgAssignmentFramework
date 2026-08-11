#pragma once
#include <d3d9.h>

// GLobals
extern IDirect3DDevice9* d3dDevice;
extern D3DPRESENT_PARAMETERS d3dPP;
extern HWND                  g_hWnd;

//--------------------------------------------------------------------
class ClassDirectX {
public:
	bool CreateDirectX()
	{
		IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);

		ZeroMemory(&d3dPP, sizeof(d3dPP));

		d3dPP.Windowed = true;
		d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dPP.BackBufferCount = 1;
		d3dPP.BackBufferWidth = 400;
		d3dPP.BackBufferHeight = 300;
		d3dPP.hDeviceWindow = g_hWnd;

		HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice); //	Create a Direct3D 9 device.

		return hr;
	}
};