module;

#include <Windows.h>
#include <iostream>

export module TestHelper;

import Windows;

export DWORD LeftMouseClick(int delay)
{
	return Windows::LeftMouseClick(delay);
}

export DWORD TypeTwo(int delay)
{
	INPUT input;
	memset(&input, 0, sizeof(input));
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_UNICODE;
	input.ki.wScan = L':';
	UINT n = SendInput(1, &input, sizeof(input));
	if (n != 1) return 1;
	Sleep(delay);
	input.ki.wScan = L'\n';
	//input.mi.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
	n = SendInput(1, &input, sizeof(input));
	if (n != 1) return 2;
	return 0;

}

export void PrintCursorPosition(int seconds)
{
	for (int i = 0; i < seconds; ++i)
	{
		Sleep(500);
		POINT p;
		GetCursorPos(&p);
		std::cout << "cursor is at " << p.x << ", " << p.y << std::endl;
	}
}
