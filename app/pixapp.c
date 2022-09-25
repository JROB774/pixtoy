/*////////////////////////////////////////////////////////////////////////////*/

PIXINTERNAL struct pixAPP
{
    lua_State* lua;
    pixU32*    screen;
    pixFLOAT   t;
    pixBOOL    playing;
    pixBOOL    built;
}
pixapp;

PIXDEF pixVOID pix_app_init(pixU32* screen)
{
    // @Improve: Add some error reporting if this fails...
    pixapp.lua = luaL_newstate();
    pixapp.screen = screen;

    pix_api_register(pixapp.lua);
}

PIXDEF pixVOID pix_app_quit(pixVOID)
{
    lua_close(pixapp.lua);
    pixapp.lua = NULL;
    pixapp.screen = NULL;
}

PIXDEF pixVOID pix_app_tick(pixFLOAT dt)
{
    if(!pixapp.playing || !pixapp.built) return;

    pixapp.t += dt;

    lua_pushnumber (pixapp.lua, dt      );
    lua_setglobal  (pixapp.lua, "dt"    );
    lua_pushnumber (pixapp.lua, pixapp.t);
    lua_setglobal  (pixapp.lua, "t"     );
    lua_pushinteger(pixapp.lua, PIXSCRW );
    lua_setglobal  (pixapp.lua, "scrw"  );
    lua_pushinteger(pixapp.lua, PIXSCRH );
    lua_setglobal  (pixapp.lua, "scrh"  );

    // @Improve; Add some error reporting if this fails...
    lua_getglobal(pixapp.lua, "draw");
    lua_pcall(pixapp.lua, 0,0,0);
}

PIXDEF const pixCHAR* pix_app_build(pixCHAR* src)
{
    pixapp.built = PIXFALSE;
    pixapp.t = 0.0f;

    // Make the program fully lowercase so that we're case-insensitive.
    for(pixCHAR* c=src; *c; ++c)
        *c = tolower(*c);
    pixINT ret = luaL_dostring(pixapp.lua, src);
    if(ret != LUA_OK)
        return lua_tostring(pixapp.lua,-1);
    pixapp.built = PIXTRUE;
    return NULL;
}

PIXDEF pixVOID pix_app_start(pixVOID)
{
    if(!pixapp.built) return;
    pixapp.playing = PIXTRUE;
}

PIXDEF pixVOID pix_app_pause(pixVOID)
{
    if(!pixapp.built) return;
    pixapp.playing = PIXFALSE;
}

PIXDEF pixBOOL pix_app_is_playing(pixVOID)
{
    return (pixapp.playing && pixapp.built);
}

PIXDEF pixBOOL pix_app_is_paused(pixVOID)
{
    return (!pixapp.playing && pixapp.built);
}

PIXDEF pixU32* pix_app_get_screen(pixVOID)
{
    return pixapp.screen;
}

/*////////////////////////////////////////////////////////////////////////////*/
