#include "pch.h"
#include <string>
#include <thread>

import Lua;
import TestHelper;
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
			t = find("Notepad")
			t:left(920, -36)
		)");
	EXPECT_EQ(0, n);
	//EXPECT_EQ(std::string(""), lua.tops());
}

TEST(LuaTestCase, ClickOverTheWindow) {
	WithNotepad([](auto& pi) {
		Lua lua;
		int n = lua.dos(R"(
			sleep(500)
			t = find("Notepad")
			t:resize(800, 400)
			sleep(500)
			type("http://example.com")
			t:right(10, 10)
			for i = 1,2 do t:cursor() sleep(500) end
			t:left(760, -35)
			for i = 1,2 do t:cursor() sleep(500) end
			t:left(665, 335)
		)");
		//EXPECT_EQ(std::string(""), lua.tops());
		EXPECT_EQ(0, n);
	});
}
