#include <iostream>
#include <string>

import Lua;
int main()
{
    Lua lua;
    std::string line;
    while(true)
    {
        std::getline(std::cin, line);
        if (line == "") break;
        auto n = lua.dos(line.c_str());
        if (n) std::cerr << lua.tops() << "\n";
    }
}
