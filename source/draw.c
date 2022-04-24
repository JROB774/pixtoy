// Union for colors as it makes accessing individual color components easier without bitwise ops.
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
            col.r = CLAMP(luaL_checkinteger(lua, offs+0),0,255);
            col.g = CLAMP(luaL_checkinteger(lua, offs+0),0,255);
            col.b = CLAMP(luaL_checkinteger(lua, offs+0),0,255);
            col.a = 255;
        break;
        case 3: // RGB1
            col.r = CLAMP(luaL_checkinteger(lua, offs+0),0,255);
            col.g = CLAMP(luaL_checkinteger(lua, offs+1),0,255);
            col.b = CLAMP(luaL_checkinteger(lua, offs+2),0,255);
            col.a = 255;
        break;
        case 4: // RGBA
            col.r = CLAMP(luaL_checkinteger(lua, offs+0),0,255);
            col.g = CLAMP(luaL_checkinteger(lua, offs+1),0,255);
            col.b = CLAMP(luaL_checkinteger(lua, offs+2),0,255);
            col.a = CLAMP(luaL_checkinteger(lua, offs+3),0,255);
        break;
    }
    return col;
}

// Safe function for setting pixels with bounds checking on edges.
static void set_pixel(int x, int y, Color c)
{
    if(x < 0 || x >= SCREEN_W) return;
    if(y < 0 || y >= SCREEN_H) return;
    pixels[y*SCREEN_W+x] = c.raw;
}

static void draw_line(int x0, int y0, int x1, int y1, Color c)
{
    // Clamp the bounds to avoid overflows.
    x0 = CLAMP(x0, 0, SCREEN_W-1);
    y0 = CLAMP(y0, 0, SCREEN_H-1);
    x1 = CLAMP(x1, 0, SCREEN_W-1);
    y1 = CLAMP(y1, 0, SCREEN_H-1);

    bool steep = false;
    if(abs(x0-x1)<abs(y0-y1))
    {
        SWAP(int, x0, y0);
        SWAP(int, x1, y1);
        steep = true;
    }
    if(x0>x1)
    {
        SWAP(int, x0, x1);
        SWAP(int, y0, y1);
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
      set_pixel(x,y,c);
    return 0;
}

LUA_FUNCTION(line)
{
    int  x0 = luaL_checkinteger(lua, 1);
    int  y0 = luaL_checkinteger(lua, 2);
    int  x1 = luaL_checkinteger(lua, 3);
    int  y1 = luaL_checkinteger(lua, 4);
    Color c = get_lua_color_arg(lua, 5);

    draw_line(x0,y0,x1,y1, c);

    return 0;
}

LUA_FUNCTION(rect)
{
    int mode = luaL_checkinteger(lua, 1);
    int    x = luaL_checkinteger(lua, 2);
    int    y = luaL_checkinteger(lua, 3);
    int    w = luaL_checkinteger(lua, 4);
    int    h = luaL_checkinteger(lua, 5);
    Color  c = get_lua_color_arg(lua, 6);

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
        x0 = CLAMP(x0, 0, SCREEN_W-1);
        y0 = CLAMP(y0, 0, SCREEN_H-1);
        x1 = CLAMP(x1, 0, SCREEN_W-1);
        y1 = CLAMP(y1, 0, SCREEN_H-1);

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

LUA_FUNCTION(circ)
{
    int mode = luaL_checkinteger(lua, 1);
    int    x = luaL_checkinteger(lua, 2);
    int    y = luaL_checkinteger(lua, 3);
    int    r = luaL_checkinteger(lua, 4);
    int    t = luaL_checkinteger(lua, 5);
    Color  c = get_lua_color_arg(lua, 6);

    // If the user wants fill mode we just set the thickness to the radius and that will make a filled circle.
    if(mode == 1)
    {
        t = r+1;
    }

    t = CLAMP(t,0,r+1);

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

LUA_FUNCTION(get)
{
    int x = luaL_checkinteger(lua, 1);
    int y = luaL_checkinteger(lua, 2);

    Color c = {0};
    if(x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
        c.raw = pixels[y*SCREEN_W+x];
    lua_pushnumber(lua, c.r);
    lua_pushnumber(lua, c.g);
    lua_pushnumber(lua, c.b);
    lua_pushnumber(lua, c.a);

    return 4;
}
