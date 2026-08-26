#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class Windows {
private:
	HWND windowHandle;
	WNDCLASS windowClass;
	LPCSTR windowName;

	static LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT message, WPARAM wordParameter, LPARAM longParameter);


public:
	Windows();

	void CreateMyWindow(int screenWidth, int screenHeight, bool isFullscreen);
	bool WindowIsRunning(MSG& windowMessage);
	void CleanupWindow();
	HWND GetWindowHandle();
};