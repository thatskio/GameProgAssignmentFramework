#pragma once
#include "Windows.h"
#include "GraphicsManager.h" 
#include "PlayerInput.h"
#include "FrameTimer.h"
#include "GameStateManager.h" 
#include "AudioManager.h" 

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const bool IS_FULLSCREEN = false;

class GameEngine {
private:
    Windows window;
    GraphicsManager graphics;
    PlayerInput input;
    FrameTimer gameTimer;
    GameStateManager stateManager;
    AudioManager audioManager;

    bool isRunning;

public:
    GameEngine();

    bool Initialize(HINSTANCE applicationInstance);
    void Run();
    void Shutdown();

    //Allows individual game states to access global tools via game engine
    GraphicsManager* GetGraphics() { return &graphics; }
    PlayerInput* GetInput() { return &input; }
    void Quit() { isRunning = false; }
};