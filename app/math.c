LUA_FUNCTION(abs)
{
    float x = luaL_checknumber(lua, 1);
    x = fabsf(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(acos)
{
    float x = luaL_checknumber(lua, 1);
    x = acos(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(asin)
{
    float x = luaL_checknumber(lua, 1);
    x = asin(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(atan)
{
    float y = luaL_checknumber(lua, 1);
    float x = 1.0f;
    if(lua_gettop(lua) == 2)
        x = luaL_checknumber(lua, 2);
    float r = atan2(y,x);
    lua_pushnumber(lua, r);
    return 1;
}

LUA_FUNCTION(round)
{
    float x = luaL_checknumber(lua, 1);
    x = round(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(ceil)
{
    float x = luaL_checknumber(lua, 1);
    x = ceil(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(floor)
{
    float x = luaL_checknumber(lua, 1);
    x = floor(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(deg)
{
    float rad = luaL_checknumber(lua, 1);
    float deg = (rad * 180.0f) / (float)M_PI;
    lua_pushnumber(lua, deg);
    return 1;
}

LUA_FUNCTION(rad)
{
    float deg = luaL_checknumber(lua, 1);
    float rad = (deg * (float)M_PI) / 180.0f;
    lua_pushnumber(lua, rad);
    return 1;
}

LUA_FUNCTION(exp)
{
    float x = luaL_checknumber(lua, 1);
    x = exp(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(fmod)
{
    float x = luaL_checknumber(lua, 1);
    float y = luaL_checknumber(lua, 2);
    float r = fmod(x,y);
    lua_pushnumber(lua, r);
    return 1;
}

LUA_FUNCTION(modf)
{
    float x = luaL_checknumber(lua, 1);
    float y;
    x = modff(x, &y);
    lua_pushnumber(lua, x);
    lua_pushnumber(lua, y);
    return 2;
}

LUA_FUNCTION(log)
{
    float x = luaL_checknumber(lua, 1);
    x = log(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(min)
{
    float x = luaL_checknumber(lua, 1);
    float y = luaL_checknumber(lua, 2);
    float r = (((x)<(y))?(x):(y));
    lua_pushnumber(lua, r);
    return 1;
}

LUA_FUNCTION(max)
{
    float x = luaL_checknumber(lua, 1);
    float y = luaL_checknumber(lua, 2);
    float r = (((x)>(y))?(x):(y));
    lua_pushnumber(lua, r);
    return 1;
}

LUA_FUNCTION(sqrt)
{
    float x = luaL_checknumber(lua, 1);
    x = sqrt(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(cos)
{
    float x = luaL_checknumber(lua, 1);
    x = cos(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(sin)
{
    float x = luaL_checknumber(lua, 1);
    x = sin(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(tan)
{
    float x = luaL_checknumber(lua, 1);
    x = tan(x);
    lua_pushnumber(lua, x);
    return 1;
}

LUA_FUNCTION(seed)
{
    float x = time(NULL);
    if(lua_gettop(lua) >= 1)
        x = luaL_checknumber(lua, 1);
    srand(x);
    return 0;
}

// @Incomplete: Arguments aren't really handled well, if we have one that should be max! If we have two then they are min,max.
LUA_FUNCTION(rand)
{
    float x = 0.0f;
    float y = (float)RAND_MAX;
    int args = lua_gettop(lua);
    if(args >= 1) x = luaL_checknumber(lua, 1);
    if(args >= 2) y = luaL_checknumber(lua, 2);
    float r = (x + ((float)rand())) / (((float)RAND_MAX)/(y-x));
    lua_pushnumber(lua, r);
    return 1;
}
