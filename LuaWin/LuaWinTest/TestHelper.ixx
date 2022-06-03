module;

#include <Windows.h>
#include <functional>
#include <iostream>

export module TestHelper;

import Windows;

export
{
	DWORD LeftMouseClick(int delay)
	{
		return Windows::LeftMouseClick(delay);
	}

	void PrintCursorPosition(int seconds)
	{
		for (int i = 0; i < seconds; ++i)
		{
			Sleep(500);
			POINT p;
			GetCursorPos(&p);
			std::cout << "cursor is at " << p.x << ", " << p.y << std::endl;
		}
	}

	bool WithNotepad(std::function<void(const PROCESS_INFORMATION&)> body)
	{
		char path[] = R"(c:\windows\system32\notepad.exe)";
		char cmd[] = "";
		STARTUPINFOA info = { sizeof(info) };
		PROCESS_INFORMATION pi;
		auto result = CreateProcessA(
			path, cmd, NULL, NULL, TRUE, 0,
			NULL, NULL, &info, &pi
		);
		if (!result) return result;
		body(pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return result;
	}

	void CloseNotepad()
	{
		SetCursorPos(760, 25);
		LeftMouseClick(250);
	}
}
