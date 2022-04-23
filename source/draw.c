typedef union Color
{
    struct { u8 b,g,r,a; }; // @Incomplete: Handle big endian?
    u32 raw;
}
Color;

// Colors can be passed in using a number of different formats, this function
// handles the logic for parsing the Lua function arguments into a final color.
static Color get_lua_color_arg(lua_State* lua, int offs)
{
    int comps = (lua_gettop(lua)+1) - offs;
    Color col = {0};
    switch(comps)
    {
        case 1: // RRR1
            col.r = luaL_checkinteger(lua, offs+0);
            col.g = luaL_checkinteger(lua, offs+0);
            col.b = luaL_checkinteger(lua, offs+0);
            col.a = 1;
        break;
        case 3: // RGB1
            col.r = luaL_checkinteger(lua, offs+0);
            col.g = luaL_checkinteger(lua, offs+1);
            col.b = luaL_checkinteger(lua, offs+2);
            col.a = 1;
        break;
        case 4: // RGBA
            col.r = luaL_checkinteger(lua, offs+0);
            col.g = luaL_checkinteger(lua, offs+1);
            col.b = luaL_checkinteger(lua, offs+2);
            col.a = luaL_checkinteger(lua, offs+3);
        break;
    }
    return col;
}

LUA_FUNCTION(cls)
{
    Color c = get_lua_color_arg(lua, 1);
    for(u32 i=0; i<SCREEN_W*SCREEN_H; ++i)
        pixels[i] = c.raw;
    return 0;
}

LUA_FUNCTION(px)
{
    int x = luaL_checkinteger(lua, 1);
    int y = luaL_checkinteger(lua, 2);
    Color c = get_lua_color_arg(lua, 3);
    pixels[y*SCREEN_W+x] = c.raw;
    return 0;
}

LUA_FUNCTION(line)
{
    return 0;
}

LUA_FUNCTION(rect)
{
    return 0;
}

LUA_FUNCTION(circ)
{
    return 0;
}
