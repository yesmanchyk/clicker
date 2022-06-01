module;

#include <functional>
#include <Windows.h>

export module Windows;

BOOL CALLBACK EnumWindowsToLambdaCallback(HWND hwnd, LPARAM lambda);

export namespace Windows {
	using EnumLambda = std::function<bool(HWND)>;

	export bool EnumToLambda(EnumLambda lambda)
	{
		return EnumWindows(EnumWindowsToLambdaCallback, (LPARAM)&lambda);
	}

	using Descriptors = std::vector< std::pair<HWND, std::string> >;

	Descriptors Titled(std::string subs)
	{
		Descriptors notepads;
		EnumLambda lambda = [&subs, &notepads](HWND hwnd) -> bool {
			char text[128];
			GetWindowTextA(hwnd, text, sizeof(text));
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
		INPUT input;
		memset(&input, 0, sizeof(input));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		UINT n = SendInput(1, &input, sizeof(input));
		if (n != 1) return input.mi.dwFlags;
		Sleep(delay);
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		n = SendInput(1, &input, sizeof(input));
		if (n != 1) return input.mi.dwFlags;
		return 0;
	}

	void LeftMouseClickOverTheWindow(int x, int y, std::string hwnd)
	{
		SetCursorPos(x, y);
		int delay = 250;
		Sleep(delay);
		LeftMouseClick(delay);
	}
}

BOOL CALLBACK EnumWindowsToLambdaCallback(HWND hwnd, LPARAM lambda)
{
	Windows::EnumLambda* fp = (Windows::EnumLambda*)lambda;
	Windows::EnumLambda f = *fp;
	return f(hwnd);
}
