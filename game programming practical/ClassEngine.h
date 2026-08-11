#pragma once

#include "ClassWindow.h"
#include "ClassGraphics.h"
#include "ClassInput.h"
#include "ClassGameLogic.h"
#include "ClassPlayer.h"
#include "ClassCollision.h"
#include "FrameTimer.h"

//game states
enum GameState { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER };

class ClassEngine {
private:
    ClassWindow Window;
    ClassGraphics Graphics;
    ClassInput Input;
    ClassGameLogic Logic;
    ClassPlayer Player1;
    ClassPlayer Player2;
    FrameTimer gameTimer;

    int currentCursorType;
    int bgRed, bgGreen, bgBlue;
    int cursorRed, cursorGreen, cursorBlue;

    bool isRunning;
    bool wasLeftMouseDown;
    bool wasSpaceDown;
    bool wasFDown; //for fullscreen toggle debouncing

    float roundTimer;
    int topBoundary;

    //state machine variables
    GameState currentState;
    RECT btnPlay, btnMenu, btnReplay, btnExit;

    bool IsMouseInRect(POINT pt, RECT r);
    void ResetGame();

public:
    ClassEngine();
    bool Initialize(HINSTANCE hInstance);
    void Run();
    void Shutdown();
};