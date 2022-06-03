#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <thread>
#include <Windows.h>

import Windows;
import TestHelper;

/**/
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

TEST(WinTestCase, EnumWindows)
{
	//std::vector< std::pair<HWND, std::string> > notepads;
	Windows::Descriptors notepads;
	auto result = WithNotepad([&notepads](auto& pi) {
		Sleep(1000);
		notepads = Windows::Titled("Notepad");
		CloseNotepad();
		Sleep(1000);
		});
	EXPECT_EQ(1, notepads.size());
	EXPECT_EQ("Untitled - Notepad", notepads[0].second);
}

TEST(WinTestCase, SendKeyboardInput)
{
	auto result = WithNotepad([](auto& pi) {
		//WaitForSingleObject(pi.hProcess, INFINITE);
		Sleep(1000);
		SetCursorPos(50, 80);
		PrintCursorPosition(2);
		LeftMouseClick(250);
		Windows::Type("http://www.example.com/?a=1&b=2", 10);
		CloseNotepad();
		Sleep(1000);
		SetCursorPos(664, 385);
		PrintCursorPosition(2);
		LeftMouseClick(250);
		Sleep(1000);
		});
	EXPECT_TRUE(result);
}
/**/
