module;

#include "LuaInc.h"
#include <vector>
#include <sstream>
#include <iostream>

export module Lua;

import Windows;

static int LuaSleep(lua_State* L)
{
	auto n = luaL_checkinteger(L, 1);
	Windows::Sleep(n);
	return 0;
}

Windows::WindowHandle GetWindowHandle(lua_State* L)
{
	if (!lua_istable(L, 1)) return NULL;
	lua_pushvalue(L, 1);
	lua_pushstring(L, "hwnd");
	lua_gettable(L, -2);
	if (!lua_isstring(L, -1)) return NULL;
	std::string s = lua_tostring(L, -1);
	lua_pop(L, 1);  /* remove lua_gettable result */
	if (s == "") return NULL;
	std::istringstream ss(s);
	Windows::WindowHandle hwnd;
	ss >> hwnd;
	return hwnd;
}

static int PrintWindowCursor(lua_State* L)
{
	auto hwnd = GetWindowHandle(L);
	if (hwnd != NULL) Windows::PrintWindowCursor(hwnd);
	return 0;
}

static int Resize(lua_State* L)
{
	auto hwnd = GetWindowHandle(L);
	int w = luaL_checkinteger(L, 2);
	int h = luaL_checkinteger(L, 3);
	if (hwnd != NULL) Windows::ResizeWindow(hwnd, w, h);
	return 0;
}

static int LeftMouseClick(lua_State* L)
{
	auto hwnd = GetWindowHandle(L);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	if (hwnd != NULL)
	{
		Windows::LeftMouseClickOverTheWindow(x, y, hwnd);
	}
	//++hwnd;
	//lua_pushstring(L, "hwnd");
	//lua_pushinteger(L, hwnd);
	//lua_settable(L, -3);
	return 0;
}

static int RightMouseClick(lua_State* L)
{
	auto hwnd = GetWindowHandle(L);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	if (hwnd != NULL)
	{
		Windows::RightMouseClickOverTheWindow(x, y, hwnd);
	}
	return 0;
}

static int TypeText(lua_State* L)
{
	auto* text = luaL_checkstring(L, 1);
	Windows::Type(text, 10);
	return 0;
}

static int FindWindow(lua_State* L)
{
	auto* title = luaL_checkstring(L, 1);
	auto windows = Windows::Titled(title);
	std::string hwnd;
	std::stringstream ss;
	int n = windows.size();
	if (n > 0)
	{
		int i = 0;
		if (n > 1) while(true)
		{
			i = 0;
			for (auto& w : windows)
			{
				std::cout << i++ << ": " << w.first << " " << w.second << "\n";
			}
			std::cout << "Please enter the window number: ";
			std::cin >> i;
			if (i < 0 || n <= i)
			{
				std::cout << "Invalid number, please enter between 0 and " << n - 1 << "\n";
			}
			else break;
		}
		ss << windows[i].first;
		hwnd = ss.str();
	}
	lua_newtable(L);
	// if (!lua_istable(L, -1)) return 0;
	lua_pushstring(L, "hwnd");
	lua_pushstring(L, hwnd.c_str());
	lua_settable(L, -3);
	//
	auto pushf = [L](const char* name, int (*f)(lua_State* L)) {
		lua_pushstring(L, name);
		lua_pushcfunction(L, f);
		lua_settable(L, -3);
	};
	pushf("left", LeftMouseClick);
	pushf("right", RightMouseClick);
	pushf("cursor", PrintWindowCursor);
	pushf("resize", Resize);
	return 1;
}

export class Lua
{
	lua_State* L;
public:
	Lua()
	{
		L = luaL_newstate();
		auto l = L;
		auto pushg = [l](const char* name, int (*f)(lua_State* L)) {
			lua_pushcfunction(l, f);
			lua_setglobal(l, name);
		};
		pushg("find", FindWindow);
		pushg("sleep", LuaSleep);
		pushg("type", TypeText);
	}

	~Lua()
	{
		lua_close(L);
	}

	int dos(const char* s)
	{
		return luaL_dostring(L, s);
	}

	const char* tops()
	{
		return lua_tostring(L, -1);
	}

	int topn()
	{
		return lua_tointeger(L, -1);
	}

private:
	int setcfunction(const char* key, int (*f)(lua_State* L))
	{
		lua_pushstring(L, key);
		lua_pushcfunction(L, f);
		lua_settable(L, -3);
		return 0;
	}
};