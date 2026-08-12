#include "Windows.h"

Windows::Windows() {
    hWnd = NULL;
    windowName = "Progaming Framework";
}

HWND Windows::GetWindowHandle() {
    return hWnd;
}

LRESULT CALLBACK Windows::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Windows::CreateMyWindow() {
    ZeroMemory(&wndClass, sizeof(wndClass));

    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpfnWndProc = Windows::WindowProcedure;
    wndClass.lpszClassName = windowName;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndClass);

    //Window size
    RECT windowRect = { 0, 0, 800, 600 };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    int exactWidth = windowRect.right - windowRect.left;
    int exactHeight = windowRect.bottom - windowRect.top;

    //Apply the exactWidth and exactHeight instead of hardcoding 800 and 600
    hWnd = CreateWindowEx(0, wndClass.lpszClassName, windowName,
        WS_OVERLAPPEDWINDOW, 0, 100, exactWidth, exactHeight,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hWnd, 1);
    ShowCursor(true);
}

bool Windows::WindowIsRunning(MSG& msg) {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) { return false; }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Windows::CleanupWindow() {
    UnregisterClass(wndClass.lpszClassName, GetModuleHandle(NULL));
}