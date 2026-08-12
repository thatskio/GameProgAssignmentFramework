#include "GameEngine.h"
#include "StateMainMenu.h" 

GameEngine::GameEngine() {
    isRunning = false;
}

bool GameEngine::Initialize(HINSTANCE hInstance) {
    Window.CreateMyWindow();

    if (!Graphics.Initialize(Window.GetHandle())) return false;
    if (!Input.Initialize(Window.GetHandle())) return false;

    StateManager.ChangeState(new StateMainMenu(&StateManager, Graphics.GetDevice(), &Input, 800, 600));

    gameTimer.Init(60);
    isRunning = true;
    return true;
}

void GameEngine::Run() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (isRunning) {
        if (!Window.WindowIsRunning(msg)) {
            isRunning = false;
            break;
        }

        // Global Input (Window scaling & quitting)
        Input.Update();
        if (Input.IsKeyDown(DIK_ESCAPE) || Input.IsKeyDown(DIK_Q)) isRunning = false;

        // Route Input to the current state
        StateManager.HandleInput();

        // Frame and Physics Updates (Locked Step)
        int framesToProcess = gameTimer.FramesToUpdate();
        for (int i = 0; i < framesToProcess; i++) {
            StateManager.Update(1.0f / 60.0f);
        }

        // Rendering Loop
        Graphics.ClearScreen(20, 20, 40); // Dark ocean blue background
        Graphics.BeginScene();

        StateManager.Render(); // The active state draws everything here

        Graphics.EndScene();
    }
}

void GameEngine::Shutdown() {
    Window.CleanupWindow();
}