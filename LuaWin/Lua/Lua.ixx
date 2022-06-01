module;

#include "LuaInc.h"
#include <vector>
#include <sstream>
#include <iostream>

export module Lua;

import Windows;

static int LeftMouseClick(lua_State* L)
{
	if (!lua_istable(L, 1)) return 0;
	lua_pushvalue(L, 1);
	lua_pushstring(L, "hwnd");
	lua_gettable(L, -2);
	if (!lua_isstring(L, -1)) return 0;
	std::string hwnd = lua_tostring(L, -1);
	lua_pop(L, 1);  /* remove lua_gettable result */
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	if (hwnd != "")
	{
		std::cout << hwnd << " (" << x << ", " << y << ") clicked\n";
		Windows::LeftMouseClickOverTheWindow(x, y, hwnd);
	}
	//++hwnd;
	//lua_pushstring(L, "hwnd");
	//lua_pushinteger(L, hwnd);
	//lua_settable(L, -3);
	return 0;
}

static int FindWindow(lua_State* L)
{
	auto* title = luaL_checkstring(L, 1);
	auto windows = Windows::Titled(title);
	std::string hwnd;
	std::stringstream ss;
	if (windows.size() > 0) {
		ss << windows[0].first;
		hwnd = ss.str();
	}
	lua_newtable(L);
	if (!lua_istable(L, -1)) return 10;
	lua_pushstring(L, "hwnd");
	lua_pushstring(L, hwnd.c_str());
	lua_settable(L, -3);
	//
	lua_pushstring(L, "left");
	lua_pushcfunction(L, LeftMouseClick);
	lua_settable(L, -3);
	return 1;
}

export class Lua
{
	lua_State* L;
public:
	Lua()
	{
		L = luaL_newstate();
		lua_pushcfunction(L, FindWindow);
		lua_setglobal(L, "find_window");
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