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

TEST(WinTestCase, SendMouseInput)
{
	std::thread child([]()
		{
			int delay = 1000;
			Sleep(delay);
			bool result = SetCursorPos(610, 450);
			std::cout << "SetCursorPos() -> " << result << std::endl;
			for (int i = 0; i < 3; ++i)
			{
				POINT p;
				GetCursorPos(&p);
				std::cout << "cursor is at " << p.x << ", " << p.y << std::endl;
				Sleep(1000);
			}
			INPUT input;
			memset(&input, 0, sizeof(input));
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			UINT n = SendInput(1, &input, sizeof(input));
			std::cout << "SendInput() -> " << n << std::endl;
			Sleep(delay);
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			n = SendInput(1, &input, sizeof(input));
			std::cout << "SendInput() -> " << n << std::endl;
		});
	std::cout << "MessageBoxA()" << std::endl;
	int choice = MessageBoxA(NULL, "Pass test?", "Test", MB_YESNO);
	std::cout << "MessageBoxA() -> " << choice << std::endl;
	child.join();
	std::cout << "child thread joined" << std::endl;
	EXPECT_EQ(IDYES, choice);
}