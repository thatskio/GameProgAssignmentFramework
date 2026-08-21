#pragma once
#include "Windows.h"
#include "GraphicsManager.h" 
#include "PlayerInput.h"
#include "FrameTimer.h"
#include "GameStateManager.h" 

class GameEngine {
private:
    Windows window;
    GraphicsManager graphics;
    PlayerInput input;
    FrameTimer gameTimer;
    GameStateManager stateManager;

    bool isRunning;

public:
    GameEngine();

    bool Initialize(HINSTANCE hInstance);
    void Run();
    void Shutdown();

    //Allows individual game states to access global tools via game engine
    GraphicsManager* GetGraphics() { return &graphics; }
    PlayerInput* GetInput() { return &input; }
    void Quit() { isRunning = false; }
};