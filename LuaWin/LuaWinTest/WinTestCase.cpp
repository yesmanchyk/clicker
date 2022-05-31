#include "gtest/gtest.h"
#include <string>
#include <thread>
#include <Windows.h>

import TestHelper;

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
	char path[] = R"(c:\windows\system32\notepad.exe)";
	char cmd[] = "";
	STARTUPINFOA info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	auto result = CreateProcessA(
		path, cmd, NULL, NULL, TRUE, 0,
		NULL, NULL, &info, &processInfo
	);
	if (result)
	{
		//WaitForSingleObject(processInfo.hProcess, INFINITE);
		Sleep(1000);
		SetCursorPos(50, 80);
		PrintCursorPosition(2);
		LeftMouseClick(250);
		TypeTwo(1000);
		SetCursorPos(920, 20);
		LeftMouseClick(250);
		SetCursorPos(671, 387);
		PrintCursorPosition(2);
		LeftMouseClick(250);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	EXPECT_TRUE(result);
}