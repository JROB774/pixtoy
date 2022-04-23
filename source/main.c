#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <emscripten.h>

#include <SDL.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define LUA_FUNCTION(func) int LUA_##func(lua_State* lua)
#define LUA_REGISTER(func) lua_pushcfunction(luastate, LUA_##func); lua_setglobal(luastate, #func)

#define SCREEN_W 256
#define SCREEN_H 256

typedef uint32_t u32;

static lua_State*    luastate;
static SDL_Window*   window;
static SDL_Renderer* renderer;
static SDL_Surface*  screen;
static SDL_Texture*  target;

LUA_FUNCTION(clear)
{
    int c = luaL_checkinteger(lua, 1);
    memset(screen->pixels, c, SCREEN_W*SCREEN_H*4);
    return 0;
}

LUA_FUNCTION(pixel)
{
    int x = luaL_checkinteger(lua, 1);
    int y = luaL_checkinteger(lua, 2);
    int c = luaL_checkinteger(lua, 3);
    u32*pixels = (u32*)screen->pixels;
    memset(&pixels[y*SCREEN_W+x], c, sizeof(u32));
    return 0;
}

void main_loop()
{
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    const char* luacode = "clear(40);pixel(10,10,255);pixel(11,10,255);pixel(12,10,255);";
    luaL_dostring(luastate, luacode);

    SDL_UpdateTexture(target, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, target, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("pixtoy", 0,0,SCREEN_W*3,SCREEN_H*3, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Setup a render target for drawing into.
    u32 pixel_format = SDL_GetWindowPixelFormat(window);
    u32 r,g,b,a;
    int bpp;
    SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a);
    screen = SDL_CreateRGBSurface(0, SCREEN_W,SCREEN_H, 32, r,g,b,a);
    target = SDL_CreateTexture(renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, SCREEN_W,SCREEN_H);

    // Expose drawing functions to Lua.
    luastate = luaL_newstate();
    LUA_REGISTER(clear);
    LUA_REGISTER(pixel);

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}
