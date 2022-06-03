module;

#include <iostream>
#include <functional>
#include <Windows.h>

export module Windows;

BOOL CALLBACK EnumWindowsToLambdaCallback(HWND hwnd, LPARAM lambda);

int SendTwoMouseInputs(int flags1, int delay, int flags2)
{
	INPUT input;
	memset(&input, 0, sizeof(input));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = flags1;
	UINT n = SendInput(1, &input, sizeof(input));
	if (n != 1) return input.mi.dwFlags;
	Sleep(delay);
	input.mi.dwFlags = flags2;
	n = SendInput(1, &input, sizeof(input));
	if (n != 1) return input.mi.dwFlags;
	return 0;
}

bool SetCursorOverTheWindow(int x, int y, void* w)
{
	POINT p = { x, y };
	return ClientToScreen((HWND)w, &p) && SetCursorPos(p.x, p.y); // potentially left x, y here
}

export namespace Windows {
	using WindowHandle = void*;

	using EnumLambda = std::function<bool(WindowHandle)>;

	export bool EnumToLambda(EnumLambda lambda)
	{
		return EnumWindows(EnumWindowsToLambdaCallback, (LPARAM)&lambda);
	}

	using Descriptors = std::vector< std::pair<WindowHandle, std::string> >;

	Descriptors Titled(std::string subs)
	{
		Descriptors notepads;
		EnumLambda lambda = [&subs, &notepads](WindowHandle hwnd) -> bool {
			char text[128];
			GetWindowTextA((HWND)hwnd, text, sizeof(text));
			if (strstr(text, subs.c_str()) != NULL)
			{
				std::string title = text;
				notepads.push_back(std::make_pair(hwnd, title));
			}
			return true;
		};
		EnumToLambda(lambda);
		return notepads;
	}

	int LeftMouseClick(int delay)
	{
		return SendTwoMouseInputs(MOUSEEVENTF_LEFTDOWN, delay, MOUSEEVENTF_LEFTUP);
	}

	int Type(std::string s, int delay)
	{
		for (int i = 0; i < s.length(); ++i)
		{
			INPUT input;
			memset(&input, 0, sizeof(input));
			input.type = INPUT_KEYBOARD;
			input.ki.dwFlags = KEYEVENTF_UNICODE;
			input.ki.wScan =(wchar_t)s[i];
			UINT n = SendInput(1, &input, sizeof(input));
			if (n != 1) return i;
			Sleep(delay);
		}
		return s.length();
	}

	void LeftMouseClickOverTheWindow(int x, int y, WindowHandle hwnd)
	{
		SetCursorOverTheWindow(x, y, hwnd);
		int delay = 250;
		Sleep(delay);
		LeftMouseClick(delay);
	}

	void RightMouseClickOverTheWindow(int x, int y, WindowHandle hwnd)
	{
		SetCursorOverTheWindow(x, y, hwnd);
		int delay = 250;
		Sleep(delay);
		SendTwoMouseInputs(MOUSEEVENTF_RIGHTDOWN, delay, MOUSEEVENTF_RIGHTUP);
	}

	bool ResizeWindow(WindowHandle wh, int w, int h)
	{
		HWND hwnd = (HWND)wh;
		RECT r = {0, 0, 0, 0};
		if (!GetWindowRect(hwnd, &r)) return false;
		return SetWindowPos(hwnd, NULL, r.left, r.top, w, h, 0);
	}

	void Sleep(int n)
	{
		::Sleep(n);
	}

	void PrintWindowCursor(WindowHandle handle)
	{
		POINT p;
		GetCursorPos(&p);
		HWND hwnd = (HWND)handle;
		if (ScreenToClient(hwnd, &p))
			std::cout << "cursor is at " << p.x << ", " << p.y << "\n";
	}
}

BOOL CALLBACK EnumWindowsToLambdaCallback(HWND hwnd, LPARAM lambda)
{
	Windows::EnumLambda* fp = (Windows::EnumLambda*)lambda;
	Windows::EnumLambda f = *fp;
	return f(hwnd);
}
