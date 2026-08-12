#include "GameEngine.h"
#include "StateMainMenu.h" 
#include <iostream>

float FRAMES_PER_SECOND = 60.0f;

GameEngine::GameEngine() {
    isRunning = false;
}

bool GameEngine::Initialize(HINSTANCE hInstance) {
    window.CreateMyWindow();
    std::cout << "Loaded window!";

    if (!graphics.Initialize(window.GetWindowHandle())) { 
        std::cout << "Graphics failed to load!";
        return false; 
    }
    if (!input.Initialize(window.GetWindowHandle())) {
        std::cout << "Player input failed to load!";
        return false;
    }

    stateManager.ChangeState(new StateMainMenu(&stateManager, graphics.GetDevice(), &input, 800, 600));

    gameTimer.Init(FRAMES_PER_SECOND);
    isRunning = true;
    return true;
}

void GameEngine::Run() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (isRunning) {
        if (!window.WindowIsRunning(msg)) {
            isRunning = false;
            break;
        }

        //Global Input (Alt tab / esc key quitting)
        input.Update();
        if (input.IsKeyDown(DIK_ESCAPE) || input.IsKeyDown(DIK_Q)) {
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
    }
}

void GameEngine::Shutdown() {
    window.CleanupWindow();
}