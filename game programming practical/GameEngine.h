#pragma once
#include "ClassWindow.h"
#include "GraphicsManager.h" 
#include "ClassInput.h"
#include "FrameTimer.h"
#include "GameStateManager.h" 

class GameEngine {
private:
    ClassWindow Window;
    GraphicsManager Graphics;
    ClassInput Input;
    FrameTimer gameTimer;
    GameStateManager StateManager;

    bool isRunning;

public:
    GameEngine();

    bool Initialize(HINSTANCE hInstance);
    void Run();
    void Shutdown();

    // Allows individual GameStates to access global tools via the Engine
    GraphicsManager* GetGraphics() { return &Graphics; }
    ClassInput* GetInput() { return &Input; }
    void Quit() { isRunning = false; }
};