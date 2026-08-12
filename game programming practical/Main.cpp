#include "GameEngine.h"

// Linker directives for DirectX and DirectInput
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    // Create the new engine instance
    GameEngine gameEngine;

    // Initialize the window, input, and graphics device
    if (gameEngine.Initialize(hInstance)) {

        // Hand over control to the engine's main loop and State Manager
        gameEngine.Run();
    }

    // Safely clean up the window and OS resources upon exit
    gameEngine.Shutdown();

    return 0;
}