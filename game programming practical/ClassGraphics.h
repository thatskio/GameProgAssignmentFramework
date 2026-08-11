#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "ClassSprite.h"
#include "ClassPlayer.h"
#include "Maid.h"

class ClassGraphics {
private:
    IDirect3DDevice9* d3dDevice;
    D3DPRESENT_PARAMETERS d3dPP;
    LPD3DXSPRITE spriteBrush;
    LPD3DXLINE line;

    Maid maid;

    int monitorWidth;
    int monitorHeight;

    ClassSprite spriteBackground;
    ClassSprite spriteCursor;
    ClassSprite spriteIronSword, spriteGoldSword, spriteDiamondSword;
    ClassSprite spriteNumbers;
    ClassSprite spritePane;
    ClassSprite spriteFontBG;
    ClassSprite spritePlayer;

    ClassFont fontText;
    ClassSprite* activeCursor;
    D3DXVECTOR2 lineVertices[2];
    float tick; //used for framerate

    void SetSpriteCell(ClassSprite& sprite, int index, int cols, int cellW, int cellH);
    void DrawPlayer(ClassPlayer& player);
    void DrawClock();
    void DrawFilledRect(RECT rect, D3DCOLOR color);
    float RadianConverter(float args);

public:
    ClassGraphics();
    ~ClassGraphics();   //destructor used in maid

    bool Initialize(HWND hWnd);
    void ToggleFullscreen(HWND hWnd);

    void SetActiveCursor(int cursorType);
    void SetCursorPosition(float x, float y);
    ClassSprite* GetActiveCursor();

    void RenderFrame(int bgRed, int bgGreen, int bgBlue, ClassPlayer& p1, ClassPlayer& p2, int timeLeft, int gameState);
};