#include "ClassGraphics.h"
#include <iostream>
#include <string>

const int CURSOR_SIZE = 24;
const int NUM_COLS = 2;
const int NUM_CELL_W = 64;
const int NUM_CELL_H = 64;
const int PANE_COLS = 2;
const int PANE_CELL_W = 2000;
const int PANE_CELL_H = 1333;
const float PI = 3.14159265f;
const int PLAYER_CELL_W = 32;
const int PLAYER_CELL_H = 32;

ClassGraphics::ClassGraphics() {
    d3dDevice = NULL;
    spriteBrush = NULL;
    line = NULL;
    activeCursor = &spriteCursor;
    tick = 0;
    monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    monitorHeight = GetSystemMetrics(SM_CYSCREEN);
}

ClassGraphics::~ClassGraphics() {
    maid.DoCleaning();
    if (d3dDevice) d3dDevice->Release();
}

bool ClassGraphics::Initialize(HWND hWnd) {
    IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    ZeroMemory(&d3dPP, sizeof(d3dPP));
    d3dPP.Windowed = true;
    d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPP.BackBufferCount = 1;
    d3dPP.BackBufferWidth = 400;
    d3dPP.BackBufferHeight = 300;
    d3dPP.hDeviceWindow = hWnd;

    HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);
    if (FAILED(hr)) return false;

    //initialize maid with device to start collecting garbage
    maid.Init(d3dDevice);

    hr = D3DXCreateSprite(d3dDevice, &spriteBrush);
    if (FAILED(hr)) return false;
    maid.GiveTask(spriteBrush);

    D3DXCreateLine(d3dDevice, &line);
    if (line) maid.GiveTask(line);

    spriteBackground = maid.LoadSprite("Assets/Lab03/bg3_2.png", { 220, 8, 301, 57 }, { 100, 100, 0 });
    spriteCursor = maid.LoadSprite("Assets/Lab03/pointer.png", { 0,0,CURSOR_SIZE,CURSOR_SIZE }, { 100, 150, 0 });
    spriteIronSword = maid.LoadSprite("Assets/Lab03/ironSword.png", { 0,0,CURSOR_SIZE,CURSOR_SIZE }, { 100,150,0 });
    spriteGoldSword = maid.LoadSprite("Assets/Lab03/goldSword.png", { 0,0,CURSOR_SIZE,CURSOR_SIZE }, { 100,150,0 });
    spriteDiamondSword = maid.LoadSprite("Assets/Lab03/diamondSword.png", { 0,0,CURSOR_SIZE,CURSOR_SIZE }, { 100,150,0 });

    spriteNumbers = maid.LoadSprite("Assets/Lab03/numbers.bmp", { 0,0,NUM_CELL_W,NUM_CELL_H }, { 0,0,0 });
    spritePlayer = maid.LoadSprite("Assets/practical9.png", { 0, 0, PLAYER_CELL_W, PLAYER_CELL_H }, { 0, 0, 0 });

    spritePane = maid.LoadSpriteKeyed("Assets/Lab03/HUD.png", { 0,0,PANE_CELL_W,PANE_CELL_H }, { 0,0,0 }, D3DCOLOR_XRGB(255, 255, 255));
    SetSpriteCell(spritePane, 1, PANE_COLS, PANE_CELL_W, PANE_CELL_H);

    spriteFontBG = maid.LoadSpriteKeyed("Assets/Lab03/HUD.png", { 1500, 1800, 2660, 2250 }, { 0,0,0 }, D3DCOLOR_XRGB(255, 255, 255));

    fontText = maid.LoadFontAsset({ 110, 8, 301, 57 }, 20, "Arial");

    return true;
}

void ClassGraphics::ToggleFullscreen(HWND hWnd) {
    //suspend all drawing tools
    if (spriteBrush) spriteBrush->OnLostDevice();
    if (line) line->OnLostDevice();
    if (fontText.font) fontText.font->OnLostDevice();

    if (d3dPP.Windowed == true) {
        d3dPP.Windowed = false;
        d3dPP.BackBufferWidth = monitorWidth;
        d3dPP.BackBufferHeight = monitorHeight;
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
    }
    else {
        d3dPP.Windowed = true;
        d3dPP.BackBufferWidth = 400;
        d3dPP.BackBufferHeight = 300;
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    }

    //reset graphics driver
    d3dDevice->Reset(&d3dPP);

    if (d3dPP.Windowed == true)
        SetWindowPos(hWnd, HWND_TOP, 0, 100, 400, 300, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
    else
        SetWindowPos(hWnd, HWND_TOP, 0, 0, monitorWidth, monitorHeight, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

    //wake all drawing tools
    if (spriteBrush) spriteBrush->OnResetDevice();
    if (line) line->OnResetDevice();
    if (fontText.font) fontText.font->OnResetDevice();
}

void ClassGraphics::DrawFilledRect(RECT rect, D3DCOLOR color) {
    float height = (float)(rect.bottom - rect.top);
    float width = (float)(rect.right - rect.left);
    line->SetWidth(height);
    line->Begin();

    //draw horizontally through the center of the bounding box
    D3DXVECTOR2 verts[2];
    verts[0] = D3DXVECTOR2((float)rect.left, rect.top + (height / 2.0f));
    verts[1] = D3DXVECTOR2((float)rect.right, rect.top + (height / 2.0f));

    line->Draw(verts, 2, color);
    line->End();
}

void ClassGraphics::RenderFrame(int bgRed, int bgGreen, int bgBlue, ClassPlayer& p1, ClassPlayer& p2, int timeLeft, int gameState) {
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(bgRed, bgGreen, bgBlue), 1.0f, 0);
    d3dDevice->BeginScene();

    line->SetWidth(40.0f);
    line->Begin();
    lineVertices[0] = D3DXVECTOR2(0.0f, 20.0f);
    lineVertices[1] = D3DXVECTOR2((float)d3dPP.BackBufferWidth, 20.0f);
    line->Draw(lineVertices, 2, D3DCOLOR_XRGB(20, 20, 20));
    line->End();

    if (SUCCEEDED(spriteBrush->Begin(D3DXSPRITE_ALPHABLEND))) {
        //topbar
        std::string p1Str = "Score: " + std::to_string(p1.score);
        std::string timeStr = "Time Left: " + std::to_string(timeLeft);
        std::string p2Str = "Score: " + std::to_string(p2.score);

        RECT rectP1 = { 10, 10, 150, 40 };
        RECT rectTime = { d3dPP.BackBufferWidth / 2 - 50, 10, d3dPP.BackBufferWidth / 2 + 50, 40 };
        RECT rectP2 = { d3dPP.BackBufferWidth - 100, 10, d3dPP.BackBufferWidth - 10, 40 };

        fontText.font->DrawText(spriteBrush, p1Str.c_str(), -1, &rectP1, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
        fontText.font->DrawText(spriteBrush, timeStr.c_str(), -1, &rectTime, DT_CENTER | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
        fontText.font->DrawText(spriteBrush, p2Str.c_str(), -1, &rectP2, DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

        DrawPlayer(p1);
        DrawPlayer(p2);

        spriteBrush->End();
    }

    //UI (game state) overlay
    if (gameState == 0 || gameState == 2) {
        //semi black transparent background
        RECT fullScreen = { 0, 0, d3dPP.BackBufferWidth, d3dPP.BackBufferHeight };
        DrawFilledRect(fullScreen, D3DCOLOR_ARGB(180, 0, 0, 0));

        if (gameState == 0) { //start menu
            RECT btnPlay = { 150, 130, 250, 170 };
            DrawFilledRect(btnPlay, D3DCOLOR_XRGB(80, 80, 80));

            if (SUCCEEDED(spriteBrush->Begin(D3DXSPRITE_ALPHABLEND))) {
                fontText.font->DrawText(spriteBrush, "PLAY", -1, &btnPlay, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
                spriteBrush->End();
            }
        }
        else if (gameState == 2) { //gameover
            RECT btnMenu = { 40, 200, 120, 240 };
            RECT btnReplay = { 160, 200, 240, 240 };
            RECT btnExit = { 280, 200, 360, 240 };

            DrawFilledRect(btnMenu, D3DCOLOR_XRGB(80, 80, 80));
            DrawFilledRect(btnReplay, D3DCOLOR_XRGB(80, 80, 80));
            DrawFilledRect(btnExit, D3DCOLOR_XRGB(80, 80, 80));

            if (SUCCEEDED(spriteBrush->Begin(D3DXSPRITE_ALPHABLEND))) {
                RECT titleRect = { 0, 60, d3dPP.BackBufferWidth, 100 };
                RECT scoreRect = { 0, 100, d3dPP.BackBufferWidth, 140 };

                fontText.font->DrawText(spriteBrush, "GAME OVER", -1, &titleRect, DT_CENTER | DT_NOCLIP, D3DCOLOR_XRGB(255, 100, 100));

                std::string scoreStr = "P1 Score: " + std::to_string(p1.score) + "      P2 Score: " + std::to_string(p2.score);
                fontText.font->DrawText(spriteBrush, scoreStr.c_str(), -1, &scoreRect, DT_CENTER | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

                fontText.font->DrawText(spriteBrush, "MENU", -1, &btnMenu, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
                fontText.font->DrawText(spriteBrush, "REPLAY", -1, &btnReplay, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
                fontText.font->DrawText(spriteBrush, "EXIT", -1, &btnExit, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

                spriteBrush->End();
            }
        }
    }

    //render cursor on top of game state UIs
    if (SUCCEEDED(spriteBrush->Begin(D3DXSPRITE_ALPHABLEND))) {
        D3DXVECTOR3 drawCursorPos = spriteCursor.position;
        drawCursorPos.x -= ((activeCursor->rect.right + activeCursor->rect.left) / 2.0f);
        drawCursorPos.y -= ((activeCursor->rect.bottom + activeCursor->rect.top) / 2.0f);
        spriteBrush->Draw(activeCursor->texture, &activeCursor->rect, NULL, &drawCursorPos, D3DCOLOR_XRGB(activeCursor->red, activeCursor->green, activeCursor->blue));

        spriteBrush->End();
    }

    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void ClassGraphics::SetActiveCursor(int cursorType) {
    switch (cursorType) {
    case 1: activeCursor = &spriteIronSword; break;
    case 2: activeCursor = &spriteGoldSword; break;
    case 3: activeCursor = &spriteDiamondSword; break;
    default: activeCursor = &spriteCursor; break;
    }
}

ClassSprite* ClassGraphics::GetActiveCursor() {
    return activeCursor;
}

void ClassGraphics::SetCursorPosition(float x, float y) {
    spriteCursor.position.x = x;
    spriteCursor.position.y = y;
}

void ClassGraphics::SetSpriteCell(ClassSprite& sprite, int index, int cols, int cellW, int cellH) {
    int col = index % cols;
    int row = index / cols;
    sprite.rect.left = col * cellW;
    sprite.rect.top = row * cellH;
    sprite.rect.right = sprite.rect.left + cellW;
    sprite.rect.bottom = sprite.rect.top + cellH;
}

float ClassGraphics::RadianConverter(float args) {
    return args * (PI / 180.0f);
}

void ClassGraphics::DrawClock() {
}

void ClassGraphics::DrawPlayer(ClassPlayer& player) {
    if (!spritePlayer.texture) return;

    spritePlayer.rect.left = PLAYER_CELL_W * player.colorIndex;
    spritePlayer.rect.top = player.currentFrame * PLAYER_CELL_H;
    spritePlayer.rect.right = spritePlayer.rect.left + PLAYER_CELL_W;
    spritePlayer.rect.bottom = spritePlayer.rect.top + PLAYER_CELL_H;

    D3DXVECTOR2 scalingCenter(PLAYER_CELL_W * 0.5f, PLAYER_CELL_H * 0.5f);
    D3DXVECTOR2 playerScaling(1.0f, 1.0f);
    D3DXVECTOR2 playerRotationCenter(PLAYER_CELL_W * 0.5f, PLAYER_CELL_H * 0.5f);
    D3DXMATRIX matrix;

    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, &playerScaling, &playerRotationCenter, player.rotation, &player.position);
    spriteBrush->SetTransform(&matrix);

    D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);

    //damaged indicator
    D3DCOLOR tintColor = D3DCOLOR_XRGB(255, 255, 255);
    if (player.hurtTimer > 0 && (player.hurtTimer % 10 < 5)) {
        tintColor = D3DCOLOR_XRGB(255, 0, 0);
    }

    spriteBrush->Draw(spritePlayer.texture, &spritePlayer.rect, NULL, &pos, tintColor);

    D3DXMatrixIdentity(&matrix);
    spriteBrush->SetTransform(&matrix);
}