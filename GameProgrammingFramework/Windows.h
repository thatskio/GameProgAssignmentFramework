#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class Windows {
private:
	HWND hWnd;
	WNDCLASS wndClass;
	LPCSTR windowName;

	static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


public:
	Windows();

	void CreateMyWindow();
	bool WindowIsRunning(MSG& msg);
	void CleanupWindow();
	HWND GetWindowHandle();
};