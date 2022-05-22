module;

#include "LuaInc.h"

export module Lua;

export class Lua
{
	lua_State* l;
public:
	Lua()
	{
		l = luaL_newstate();
	}

	~Lua()
	{
		lua_close(l);
	}

	int dos(const char* s)
	{
		return luaL_dostring(l, s);
	}
};