#ifndef __ZLK_LUA_
#define __Zlk_LUA_
#include "iostream"
#include "fstream"
#include "string.h"
using namespace std;

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
static string luastring[] = {"LUA_TNIL", "LUA_TBOOLEAN", "LUA_TLIGHTUSERDATA", "LUA_TNUMBER", "LUA_TSTRING", "LUA_TTABLE", "LUA_TFUNCTION", "LUA_TUSERDATA", "LUA_TTHREAD", "LUA_NUMTYPES"};

static void zlk_lua_top_type(lua_State *L)
{
    printf("zlk_lua_top_type begin--------------------------\n");
    int n = lua_gettop(L);
    for (int i = 1; i <= n; i++)
    {
        printf("i = %d type = %s\n", -i, luastring[lua_type(L, -i)].c_str());
    }
    printf("zlk_lua_top_type end----------------------------\n");
}
static string zlk_lua_t2s(int n)
{
    return luastring[n];
}
#endif