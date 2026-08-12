#include "GameEngine.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    GameEngine gameEngine;

    if (gameEngine.Initialize(hInstance)) {
        gameEngine.Run();
    }

    gameEngine.Shutdown();
    return 0;
}