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

TEST(LuaTestCase, WindowTable) {
	// open standard-sized notepad and move to the top left corner
	Lua lua;
	int n = lua.dos(R"(
			t = find_window("Notepad")
			t:left(920, 20)
		)");
	EXPECT_EQ(0, n);
	//EXPECT_EQ(std::string(""), lua.tops());
}
