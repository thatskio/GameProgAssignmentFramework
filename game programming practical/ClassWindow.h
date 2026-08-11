#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class ClassWindow {
private:
    HWND hWnd;
    WNDCLASS wndClass;
    LPCSTR windowName;

    // This MUST be static so the Windows OS can call it!
    static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ClassWindow();

    void CreateMyWindow();
    bool WindowIsRunning(MSG& msg);
    void CleanupWindow();

    // Allows Engine and Graphics to get the window ID without making it global
    HWND GetHandle();
};