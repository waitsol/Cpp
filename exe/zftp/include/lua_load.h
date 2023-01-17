#ifndef __LUA_LOAD
#define __LUA_LOAD
#include <string>
#include <unordered_map>
using namespace std;

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "zlk_lua.h"

/* 遍历一维表 */
static void table_traverse(string fileName, unordered_map<string, string> m)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadfile(L, fileName.data());
    lua_pushnil(L); /* table 里第一对 key-value 的前面没有数据，所以先用 lua_pushnil() 压入一个 nil 充当初始 key */

    while (lua_next(L, -2))
    {
        // std::cout << "---Loop start: Stack size = " << lua_gettop(L) << std::endl;
        //   printluaTopType(L);
        /* 输出key */
        string key, val;
        if (lua_isnumber(L, -2))
        {
            key = to_string(lua_tonumber(L, -2));
        }
        else if (lua_isstring(L, -2))
        {
            key = lua_tostring(L, -2);
        }
        else
        {
            std::cout << "Error key type!" << std::endl;
        }

        /* 输出value */
        if (lua_isnumber(L, -1))
        {
            val = to_string(lua_tonumber(L, -1));
            lua_pop(L, 1);
        }
        else if (lua_isstring(L, -1))
        {
            val = lua_tostring(L, -1);
            lua_pop(L, 1);
        }
        else
        {
            std::cout << "Error val type!" << std::endl;
        }
        m[key] = val;
    }

    lua_pop(L, 1);
    lua_close(L);
}

#endif