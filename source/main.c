#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <emscripten.h>

#include <SDL.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static SDL_Window* window;
static SDL_Renderer* renderer;

static lua_State* lua;

int test_function(lua_State* lua)
{
    int a = luaL_checkinteger(lua, 1);
    int b = luaL_checkinteger(lua, 2);
    int c = a + b;
    printf("result: %d\n", c);
    return 0;
}

void main_loop()
{
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("pixtoy", 0,0, 256,256, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    lua = luaL_newstate();

    lua_pushcfunction(lua, test_function);
    lua_setglobal(lua, "test");

    luaL_dostring(lua, "test(30,40)");
    luaL_dostring(lua, "test(20,10)");
    luaL_dostring(lua, "test(50,50)");

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}
