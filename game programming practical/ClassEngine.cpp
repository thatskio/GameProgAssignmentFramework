#include "ClassEngine.h"
#include <string>
#include <iostream>

ClassEngine::ClassEngine() {
    isRunning = false;
    currentCursorType = 4;
    bgRed = 30;
    bgGreen = 10;
    bgBlue = 50;
    cursorRed = 255;
    cursorGreen = 255;
    cursorBlue = 255;
    wasLeftMouseDown = false;
    wasSpaceDown = false;
    wasFDown = false;

    roundTimer = 5.0f;
    topBoundary = 40;

    currentState = STATE_MENU;

    //simple UI blockout boxes
    btnPlay = { 150, 130, 250, 170 };
    btnMenu = { 40, 200, 120, 240 };
    btnReplay = { 160, 200, 240, 240 };
    btnExit = { 280, 200, 360, 240 };
}

bool ClassEngine::IsMouseInRect(POINT pt, RECT r) {
    return (pt.x >= r.left && pt.x <= r.right && pt.y >= r.top && pt.y <= r.bottom);
}

void ClassEngine::ResetGame() {
    roundTimer = 5.0f;
    Player1.Init(30.0f, 50.0f, 0);
    Player2.Init(320.0f, 220.0f, 1);
}

bool ClassEngine::Initialize(HINSTANCE hInstance) {
    Window.CreateMyWindow();

    if (!Graphics.Initialize(Window.GetHandle())) return false;
    if (!Input.Initialize(Window.GetHandle())) return false;

    ResetGame();

    gameTimer.Init(60);
    isRunning = true;
    return true;
}

void ClassEngine::Run() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (isRunning) {
        if (!Window.WindowIsRunning(msg)) {
            isRunning = false;
            break;
        }

        Input.Update();

        if (Input.IsKeyDown(DIK_ESCAPE) || Input.IsKeyDown(DIK_Q)) isRunning = false;

        //fullscreen toggle test bound to F
        bool isFDown = Input.IsKeyDown(DIK_F);
        if (isFDown && !wasFDown) {
            Graphics.ToggleFullscreen(Window.GetHandle());
        }
        wasFDown = isFDown;

        //get dynamic screen dimensions
        RECT clientRect;
        GetClientRect(Window.GetHandle(), &clientRect);
        int currentScreenWidth = clientRect.right - clientRect.left;
        int currentScreenHeight = clientRect.bottom - clientRect.top;

        //UI click detection
        bool isLeftMouseDown = Input.IsMouseButtonDown(0);
        if (isLeftMouseDown && !wasLeftMouseDown) {
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(Window.GetHandle(), &pt);

            if (currentState == STATE_MENU) {
                if (IsMouseInRect(pt, btnPlay)) {
                    ResetGame();
                    currentState = STATE_PLAYING;
                }
            }
            else if (currentState == STATE_GAMEOVER) {
                if (IsMouseInRect(pt, btnMenu)) {
                    currentState = STATE_MENU;
                }
                else if (IsMouseInRect(pt, btnReplay)) {
                    ResetGame();
                    currentState = STATE_PLAYING;
                }
                else if (IsMouseInRect(pt, btnExit)) {
                    isRunning = false;
                }
            }
        }
        wasLeftMouseDown = isLeftMouseDown;

        int framesToProcess = gameTimer.FramesToUpdate();
        for (int i = 0; i < framesToProcess; i++) {

            //only process physics and time if the game is active
            if (currentState == STATE_PLAYING) {
                roundTimer -= (1.0f / 60.0f);
                if (roundTimer <= 0.0f) {
                    roundTimer = 0.0f;
                    currentState = STATE_GAMEOVER;
                }

                bool p1W = Input.IsKeyDown(DIK_W);
                bool p1A = Input.IsKeyDown(DIK_A);
                bool p1S = Input.IsKeyDown(DIK_S);
                bool p1D = Input.IsKeyDown(DIK_D);
                Player1.Update(p1W, p1A, p1S, p1D, currentScreenWidth, currentScreenHeight, topBoundary);

                bool p2Up = Input.IsKeyDown(DIK_UP);
                bool p2Left = Input.IsKeyDown(DIK_LEFT);
                bool p2Down = Input.IsKeyDown(DIK_DOWN);
                bool p2Right = Input.IsKeyDown(DIK_RIGHT);
                Player2.Update(p2Up, p2Left, p2Down, p2Right, currentScreenWidth, currentScreenHeight, topBoundary);

                float p1cx = Player1.position.x + Player1.radius;
                float p1cy = Player1.position.y + Player1.radius;
                float p2cx = Player2.position.x + Player2.radius;
                float p2cy = Player2.position.y + Player2.radius;

                if (ClassCollision::CheckRadianCollision(p1cx, p1cy, p2cx, p2cy, Player1.radius + Player2.radius)) {
                    float speed1 = (Player1.velocity.x * Player1.velocity.x) + (Player1.velocity.y * Player1.velocity.y);
                    float speed2 = (Player2.velocity.x * Player2.velocity.x) + (Player2.velocity.y * Player2.velocity.y);

                    if (speed1 > speed2) {
                        Player1.score++;
                        Player2.hurtTimer = 30;
                    }
                    else if (speed2 > speed1) {
                        Player2.score++;
                        Player1.hurtTimer = 30;
                    }

                    D3DXVECTOR2 tempVel = Player1.velocity;
                    Player1.velocity = Player2.velocity;
                    Player2.velocity = tempVel;

                    Player1.position += Player1.velocity;
                    Player2.position += Player2.velocity;
                }
            }
        }

        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(Window.GetHandle(), &pt);
        Graphics.SetCursorPosition((float)pt.x, (float)pt.y);

        Graphics.RenderFrame(bgRed, bgGreen, bgBlue, Player1, Player2, (int)roundTimer, currentState);
    }
}

void ClassEngine::Shutdown() {
    Window.CleanupWindow();
}