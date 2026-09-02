#include "GameEngine.h"
#include "StateMainMenu.h" 
#include <iostream>

float FRAMES_PER_SECOND = 60.0f;

GameEngine::GameEngine() {
    isRunning = false;
}

bool GameEngine::Initialize(HINSTANCE applicationInstance) {
    window.CreateMyWindow(SCREEN_WIDTH, SCREEN_HEIGHT, IS_FULLSCREEN);
    std::cout << "Loaded Window!";

    if (!graphics.Initialize(window.GetWindowHandle(), SCREEN_WIDTH, SCREEN_HEIGHT, IS_FULLSCREEN)) {
        std::cout << "GraphicsManager failed to initialize!";
        return false;
    }
    if (!input.Initialize(window.GetWindowHandle())) {
        std::cout << "PlayerInput failed to initialize!";
        return false;
    }

    // Initialize global managers
    IDirect3DDevice9* direct3DDevice = graphics.GetDevice();
    lineManager.Initialize(direct3DDevice);
    spriteManager.Initialize(direct3DDevice);

    stateManager.PushState(new StateMainMenu(&stateManager, direct3DDevice, &input, &lineManager, &spriteManager, SCREEN_WIDTH, SCREEN_HEIGHT));

    gameTimer.Init(FRAMES_PER_SECOND);
    isRunning = true;
    return true;
}

void GameEngine::Run() {
    MSG windowMessage;
    ZeroMemory(&windowMessage, sizeof(windowMessage));

    while (isRunning) {
        if (!window.WindowIsRunning(windowMessage)) {
            isRunning = false;
            break;
        }

        //Global Input (Alt tab / esc key quitting)
        input.Update();
        if (input.IsKeyDown(DIK_ESCAPE)) {
            isRunning = false;
        }

        //Player Input
        stateManager.HandleInput();

        //AI / Logic Update
        int framesToProcess = gameTimer.FramesToUpdate();
        for (int i = 0; i < framesToProcess; i++) {
            stateManager.Update(1.0f / FRAMES_PER_SECOND);
        }

        //Rendering Loop
        graphics.ClearScreen(20, 20, 40); //Dark blue background
        graphics.BeginScene();
        stateManager.Render(); //The active game state draws everything here

        //Ending
        graphics.EndScene();

        //Sound
        audioManager.UpdateSound();
    }
}

void GameEngine::Shutdown() {
    window.CleanupWindow();
}