#include <iostream>

import Lua;
int main()
{
    Lua lua;
    auto lua_result = lua.dos(R"(
        three = 1 + 2
    )" );
    std::cout << lua_result << " is returned from luaL_dostring\n";
}
