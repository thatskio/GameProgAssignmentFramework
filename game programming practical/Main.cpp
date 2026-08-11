#include "ClassEngine.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ClassEngine GameEngine;

    if (GameEngine.Initialize(hInstance)) {
        GameEngine.Run(); 
    }

    GameEngine.Shutdown();
    return 0;
}