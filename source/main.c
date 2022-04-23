#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int main(int argc, char** argv)
{
    lua_State* lua = luaL_newstate();
    luaL_dostring(lua, "return 'Hello PixToy!'");
    const char* ret = lua_tostring(lua, -1);
    lua_close(lua);
    printf("Lua Says: %s\n", ret);
    return 0;
}
