#include "Windows.h"

Windows::Windows() {
    windowHandle = NULL;
    windowName = "Progaming Framework";
}

HWND Windows::GetWindowHandle() {
    return windowHandle;
}

LRESULT CALLBACK Windows::WindowProcedure(HWND windowHandle, UINT message, WPARAM wordParameter, LPARAM longParameter) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(windowHandle, message, wordParameter, longParameter);
    }
    return 0;
}

void Windows::CreateMyWindow(int screenWidth, int screenHeight, bool isFullscreen) {
    ZeroMemory(&windowClass, sizeof(windowClass));

    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.lpfnWndProc = Windows::WindowProcedure;
    windowClass.lpszClassName = windowName;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&windowClass);

    //Toggle window border style based on fullscreen request
    DWORD windowStyle = isFullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;

    //Window size
    RECT windowSize = { 0, 0, screenWidth, screenHeight };
    AdjustWindowRect(&windowSize, windowStyle, FALSE);
    int exactWidth = windowSize.right - windowSize.left;
    int exactHeight = windowSize.bottom - windowSize.top;

    //Apply the exactWidth and exactHeight instead of hardcoding 800 and 600
    windowHandle = CreateWindowEx(0, windowClass.lpszClassName, windowName,
        windowStyle, 0, 0, exactWidth, exactHeight,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(windowHandle, 1);
    ShowCursor(true);
}

bool Windows::WindowIsRunning(MSG& windowMessage) {
    while (PeekMessage(&windowMessage, NULL, 0, 0, PM_REMOVE)) {
        if (windowMessage.message == WM_QUIT) { return false; }
        TranslateMessage(&windowMessage);
        DispatchMessage(&windowMessage);
    }
    return true;
}

void Windows::CleanupWindow() {
    UnregisterClass(windowClass.lpszClassName, GetModuleHandle(NULL));
}