/*////////////////////////////////////////////////////////////////////////////*/

PIXDEF const pixCHAR* pix_run(pixCHAR* program, lua_State* lua,
                              pixFLOAT dt, pixFLOAT t)
{
    // @Note: Is this okay to do every frame or do we need to pop these?
    lua_pushnumber(lua, dt);
    lua_setglobal(lua, "dt");
    lua_pushnumber(lua, t);
    lua_setglobal(lua, "t");
    lua_pushinteger(lua, PIXSCRW);
    lua_setglobal(lua, "scrw");
    lua_pushinteger(lua, PIXSCRH);
    lua_setglobal(lua, "scrh");

    // Make the program fully lowercase so that we're case-insensitive.
    for(pixCHAR* c=program; *c; ++c)
        *c = tolower(*c);
    pixINT ret = luaL_dostring(lua, program);
    if(ret != LUA_OK)
        return lua_tostring(lua,-1);
    return NULL;
}

/*////////////////////////////////////////////////////////////////////////////*/
