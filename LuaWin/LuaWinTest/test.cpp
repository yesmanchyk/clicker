#include "pch.h"
#include <string>
#include <thread>
#include <Windows.h>

import Lua;
TEST(LuaTestCase, DosReturnsZero) {
	Lua lua;
	const char* s = R"(
        three = 1 + 2
    )";
	EXPECT_EQ(0, lua.dos(s));
}

TEST(LuaTestCase, TopsReturnsSyntaxError) {
	Lua lua;
	const char* s = R"(
		a = 0
		for i = 1, 3 -- do
			a = a + i
		end
    )";
	EXPECT_EQ(1, lua.dos(s));
	std::string a = lua.tops();
	std::string e = R"([string "..."]:4: 'do' expected near 'a')";
	EXPECT_EQ(e, a);
}

DWORD LeftMouseClick(int delay)
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

DWORD TypeTwo(int delay)
{
	INPUT input;
	memset(&input, 0, sizeof(input));
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_UNICODE;
	input.ki.wScan = L'1';
	UINT n = SendInput(1, &input, sizeof(input));
	if (n != 1) return 1;
	Sleep(delay);
	input.ki.wScan = L'2';
	//input.mi.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
	n = SendInput(1, &input, sizeof(input));
	if (n != 1) return 2;
	return 0;

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

TEST(WinTestCase, SendMouseInput)
{
	std::thread child([]()
		{
			int delay = 1000;
			Sleep(delay);
			bool result = SetCursorPos(610, 450);
			std::cout << "SetCursorPos() -> " << result << std::endl;
			PrintCursorPosition(1);
			LeftMouseClick(delay);
		});
	std::cout << "MessageBoxA()" << std::endl;
	int choice = MessageBoxA(NULL, "Pass test?", "Test", MB_YESNO);
	std::cout << "MessageBoxA() -> " << choice << std::endl;
	child.join();
	std::cout << "child thread joined" << std::endl;
	EXPECT_EQ(IDYES, choice);
}

TEST(WinTestCase, SendKeyboardInput)
{
	char path[] = R"(c:\windows\system32\calc.exe)";
	char cmd[] = "";
	STARTUPINFOA info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	auto result = CreateProcessA(
		path, cmd, NULL, NULL, TRUE, 0, 
		NULL, NULL, &info, &processInfo
	);
	if (result)
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		Sleep(1000);
		SetCursorPos(100, 80);
		PrintCursorPosition(4);
		LeftMouseClick(250);
		TypeTwo(1000);
		SetCursorPos(200, 15);
		PrintCursorPosition(4);
		LeftMouseClick(250);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	EXPECT_TRUE(result);
}