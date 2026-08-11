#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

//--------------------------------------------------------------------
//DATA CLASSES
class ClassSprite {
public:
	LPDIRECT3DTEXTURE9 texture = nullptr;
	RECT rect{};
	D3DXVECTOR3 position{};
	int red = 255;
	int green = 255;
	int blue = 255;
	int velocity = 1;
};

class ClassFont {
public:
	LPD3DXFONT font = NULL;
	RECT rect{};
	std::string textX = "Hello";
	std::string textY = "World!";
};