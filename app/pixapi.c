/*////////////////////////////////////////////////////////////////////////////*/

// Expose all of the PIXAPI functions to Lua so that they can be called.
PIXDEF pixVOID pix_register_api(lua_State* lua)
{
    #define PIXAPI_REGISTER(name)          \
    lua_pushcfunction(lua, PIXAPI_##name); \
    lua_setglobal(lua, #name)

    PIXAPI_REGISTER(band );
    PIXAPI_REGISTER(bor  );
    PIXAPI_REGISTER(bxor );
    PIXAPI_REGISTER(bnot );
    PIXAPI_REGISTER(abs  );
    PIXAPI_REGISTER(acos );
    PIXAPI_REGISTER(asin );
    PIXAPI_REGISTER(atan );
    PIXAPI_REGISTER(round);
    PIXAPI_REGISTER(ceil );
    PIXAPI_REGISTER(floor);
    PIXAPI_REGISTER(deg  );
    PIXAPI_REGISTER(rad  );
    PIXAPI_REGISTER(exp  );
    PIXAPI_REGISTER(fmod );
    PIXAPI_REGISTER(modf );
    PIXAPI_REGISTER(log  );
    PIXAPI_REGISTER(min  );
    PIXAPI_REGISTER(max  );
    PIXAPI_REGISTER(sqrt );
    PIXAPI_REGISTER(cos  );
    PIXAPI_REGISTER(sin  );
    PIXAPI_REGISTER(tan  );
    PIXAPI_REGISTER(seed );
    PIXAPI_REGISTER(rand );
    PIXAPI_REGISTER(clrs );
    PIXAPI_REGISTER(pset );
    PIXAPI_REGISTER(pget );
    PIXAPI_REGISTER(line );
    PIXAPI_REGISTER(rect );
    PIXAPI_REGISTER(circ );

    #undef PIXAPI_REGISTER
}

/*////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////// API IMPLEMENTATION ////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/

//
// BITS
//

PIXAPI(band)
{
    int x = luaL_checkinteger(lua, 1);
    int y = luaL_checkinteger(lua, 2);
    int z = x & y;
    lua_pushinteger(lua, z);
    return 1;
}

PIXAPI(bor)
{
    int x = luaL_checkinteger(lua, 1);
    int y = luaL_checkinteger(lua, 2);
    int z = x | y;
    lua_pushinteger(lua, z);
    return 1;
}

PIXAPI(bxor)
{
    int x = luaL_checkinteger(lua, 1);
    int y = luaL_checkinteger(lua, 2);
    int z = x ^ y;
    lua_pushinteger(lua, z);
    return 1;
}

PIXAPI(bnot)
{
    int x = luaL_checkinteger(lua, 1);
    int y = ~x;
    lua_pushinteger(lua, y);
    return 1;
}

//
// MATH
//

PIXAPI(abs)
{
    float x = luaL_checknumber(lua, 1);
    x = fabsf(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(acos)
{
    float x = luaL_checknumber(lua, 1);
    x = acos(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(asin)
{
    float x = luaL_checknumber(lua, 1);
    x = asin(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(atan)
{
    float y = luaL_checknumber(lua, 1);
    float x = 1.0f;
    if(lua_gettop(lua) == 2)
        x = luaL_checknumber(lua, 2);
    float r = atan2(y,x);
    lua_pushnumber(lua, r);
    return 1;
}

PIXAPI(round)
{
    float x = luaL_checknumber(lua, 1);
    x = round(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(ceil)
{
    float x = luaL_checknumber(lua, 1);
    x = ceil(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(floor)
{
    float x = luaL_checknumber(lua, 1);
    x = floor(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(deg)
{
    float rad = luaL_checknumber(lua, 1);
    float deg = (rad * 180.0f) / (float)M_PI;
    lua_pushnumber(lua, deg);
    return 1;
}

PIXAPI(rad)
{
    float deg = luaL_checknumber(lua, 1);
    float rad = (deg * (float)M_PI) / 180.0f;
    lua_pushnumber(lua, rad);
    return 1;
}

PIXAPI(exp)
{
    float x = luaL_checknumber(lua, 1);
    x = exp(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(fmod)
{
    float x = luaL_checknumber(lua, 1);
    float y = luaL_checknumber(lua, 2);
    float r = fmod(x,y);
    lua_pushnumber(lua, r);
    return 1;
}

PIXAPI(modf)
{
    float x = luaL_checknumber(lua, 1);
    float y;
    x = modff(x, &y);
    lua_pushnumber(lua, x);
    lua_pushnumber(lua, y);
    return 2;
}

PIXAPI(log)
{
    float x = luaL_checknumber(lua, 1);
    x = log(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(min)
{
    float x = luaL_checknumber(lua, 1);
    float y = luaL_checknumber(lua, 2);
    float r = (((x)<(y))?(x):(y));
    lua_pushnumber(lua, r);
    return 1;
}

PIXAPI(max)
{
    float x = luaL_checknumber(lua, 1);
    float y = luaL_checknumber(lua, 2);
    float r = (((x)>(y))?(x):(y));
    lua_pushnumber(lua, r);
    return 1;
}

PIXAPI(sqrt)
{
    float x = luaL_checknumber(lua, 1);
    x = sqrt(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(cos)
{
    float x = luaL_checknumber(lua, 1);
    x = cos(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(sin)
{
    float x = luaL_checknumber(lua, 1);
    x = sin(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(tan)
{
    float x = luaL_checknumber(lua, 1);
    x = tan(x);
    lua_pushnumber(lua, x);
    return 1;
}

PIXAPI(seed)
{
    float x = time(NULL);
    if(lua_gettop(lua) >= 1)
        x = luaL_checknumber(lua, 1);
    srand(x);
    return 0;
}

// @Incomplete: Arguments aren't really handled well, if we have
// one that should be max! If we have two then they are min,max.
PIXAPI(rand)
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

//
// DRAW
//

// Colors can be passed in using a number of different formats, this function
// handles the logic for parsing the Lua function arguments into a final color.
PIXINTERNAL pixCOLOR get_lua_color_arg(lua_State* lua, int offs)
{
    pixCOLOR col = {0};
    int comps = (lua_gettop(lua)+1) - offs;
    switch(comps)
    {
        case 1: // RRR1
            col.r = luaL_checknumber(lua, offs+0);
            col.g = luaL_checknumber(lua, offs+0);
            col.b = luaL_checknumber(lua, offs+0);
            col.a = 255;
        break;
        case 3: // RGB1
            col.r = luaL_checknumber(lua, offs+0);
            col.g = luaL_checknumber(lua, offs+1);
            col.b = luaL_checknumber(lua, offs+2);
            col.a = 255;
        break;
        case 4: // RGBA
            col.r = luaL_checknumber(lua, offs+0);
            col.g = luaL_checknumber(lua, offs+1);
            col.b = luaL_checknumber(lua, offs+2);
            col.a = luaL_checknumber(lua, offs+3);
        break;
    }
    return col;
}

// Safe function for setting pixels with bounds checking on edges.
PIXINTERNAL pixVOID set_pixel(int x, int y, pixCOLOR c)
{
    if(x < 0 || x >= SCREEN_W) return;
    if(y < 0 || y >= SCREEN_H) return;
    pixels[y*SCREEN_W+x] = c.raw;
}

PIXINTERNAL pixVOID draw_line(int x0, int y0, int x1, int y1, pixCOLOR c)
{
    // Clamp the bounds to avoid overflows.
    x0 = PIXCLAMP(x0, 0, SCREEN_W-1);
    y0 = PIXCLAMP(y0, 0, SCREEN_H-1);
    x1 = PIXCLAMP(x1, 0, SCREEN_W-1);
    y1 = PIXCLAMP(y1, 0, SCREEN_H-1);

    bool steep = false;
    if(abs(x0-x1)<abs(y0-y1))
    {
        PIXSWAP(int, x0, y0);
        PIXSWAP(int, x1, y1);
        steep = true;
    }
    if(x0>x1)
    {
        PIXSWAP(int, x0, x1);
        PIXSWAP(int, y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = abs(dy)*2;
    int error2 = 0;
    int iy = y0;

    for(int ix=x0; ix<=x1; ++ix)
    {
        if(steep) set_pixel(iy,ix,c);
        else set_pixel(ix,iy,c);
        error2 += derror2;
        if(error2 > dx)
        {
            iy += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

PIXAPI(clrs)
{
    pixCOLOR c = get_lua_color_arg(lua, 1);
    for(pixU32 i=0; i<SCREEN_W*SCREEN_H; ++i)
        pixels[i] = c.raw;
    return 0;
}

PIXAPI(pset)
{
    int x = luaL_checknumber(lua, 1);
    int y = luaL_checknumber(lua, 2);
    pixCOLOR c = get_lua_color_arg(lua, 3);
      set_pixel(x,y,c);
    return 0;
}

PIXAPI(pget)
{
    int x = luaL_checknumber(lua, 1);
    int y = luaL_checknumber(lua, 2);

    pixCOLOR c = {0};
    if(x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
        c.raw = pixels[y*SCREEN_W+x];
    lua_pushnumber(lua, c.r);
    lua_pushnumber(lua, c.g);
    lua_pushnumber(lua, c.b);
    lua_pushnumber(lua, c.a);

    return 4;
}
PIXAPI(line)
{
    int  x0 = luaL_checknumber(lua, 1);
    int  y0 = luaL_checknumber(lua, 2);
    int  x1 = luaL_checknumber(lua, 3);
    int  y1 = luaL_checknumber(lua, 4);
    pixCOLOR c = get_lua_color_arg(lua, 5);

    draw_line(x0,y0,x1,y1, c);

    return 0;
}

PIXAPI(rect)
{
    int mode = luaL_checknumber(lua, 1);
    int    x = luaL_checknumber(lua, 2);
    int    y = luaL_checknumber(lua, 3);
    int    w = luaL_checknumber(lua, 4);
    int    h = luaL_checknumber(lua, 5);
    pixCOLOR  c = get_lua_color_arg(lua, 6);

    // Don't even bother rendering if we're offscreen.
    if(x >= SCREEN_W) return 0;
    if(y >= SCREEN_H) return 0;

    int x0 = x;
    int y0 = y;
    int x1 = x+w-1;
    int y1 = y+h-1;

    if(mode == 0) // Outline
    {
        draw_line(x1,y0,x1,y1, c); // Right
        draw_line(x0,y0,x0,y1, c); // Left
        draw_line(x0,y0,x1,y0, c); // Top
        draw_line(x0,y1,x1,y1, c); // Bottom
    }
    if(mode == 1) // Fill
    {
        // Clamp the bounds to avoid overflows.
        x0 = PIXCLAMP(x0, 0, SCREEN_W-1);
        y0 = PIXCLAMP(y0, 0, SCREEN_H-1);
        x1 = PIXCLAMP(x1, 0, SCREEN_W-1);
        y1 = PIXCLAMP(y1, 0, SCREEN_H-1);

        for(int iy=y0; iy<=y1; ++iy)
        {
            for(int ix=x0; ix<=x1; ++ix)
            {
                set_pixel(ix,iy,c);
            }
        }
    }

    return 0;
}

PIXAPI(circ)
{
    int mode = luaL_checknumber(lua, 1);
    int    x = luaL_checknumber(lua, 2);
    int    y = luaL_checknumber(lua, 3);
    int    r = luaL_checknumber(lua, 4);
    int    t = luaL_checknumber(lua, 5);
    pixCOLOR  c = get_lua_color_arg(lua, 6);

    // If the user wants fill mode we just set the thickness to the radius and that will make a filled circle.
    if(mode == 1)
    {
        t = r+1;
    }

    t = PIXCLAMP(t,0,r+1);

    int outer = r;
    int inner = outer-t+1;

    int xo   = outer;
    int xi   = inner;
    int yy   = 0;
    int erro = 1-xo;
    int erri = 1-xi;

    while (xo >= yy)
    {
        draw_line(x+xi, y+yy, x+xo, y+yy, c);
        draw_line(x+yy, y+xi, x+yy, y+xo, c);
        draw_line(x-xo, y+yy, x-xi, y+yy, c);
        draw_line(x-yy, y+xi, x-yy, y+xo, c);
        draw_line(x-xo, y-yy, x-xi, y-yy, c);
        draw_line(x-yy, y-xo, x-yy, y-xi, c);
        draw_line(x+xi, y-yy, x+xo, y-yy, c);
        draw_line(x+yy, y-xo, x+yy, y-xi, c);

        yy++;

        if(erro < 0)
        {
            erro += 2*yy+1;
        }
        else
        {
            xo--;
            erro += 2*(yy-xo+1);
        }

        if(yy > inner)
        {
            xi = yy;
        }
        else
        {
            if(erri < 0)
            {
                erri += 2*yy+1;
            }
            else
            {
                xi--;
                erri += 2*(yy-xi+1);
            }
        }
    }

    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
