/*////////////////////////////////////////////////////////////////////////////*/

#include "pixapp.h"

#include <emscripten.h>

#include <SDL.h>

#define SCREEN_W 256
#define SCREEN_H 256

static SDL_Window*   window;
static SDL_Renderer* renderer;
static SDL_Surface*  screen;
static SDL_Texture*  target;
static pixU32*       pixels;
static lua_State*    luastate;

#include "pixlua.c"
#include "pixapi.c"

// Function call into JS to retrieve text edit string for parsing into Lua code.
EM_JS(void, JS_get_lua_string, (const char* out_str, size_t max_bytes),
{
    get_lua_string(out_str, max_bytes);
});

// Function call into JS to display a Lua error message on the webpage.
EM_JS(void, JS_set_error_message, (const char* err, size_t len),
{
    set_error_message(err, len);
});

static void main_loop()
{
    static pixU64 perf_frequency = 0;
    static pixU64 last_counter = 0;
    static pixU64 end_counter = 0;
    static pixU64 elapsed_counter = 0;

    static float delta_time = 0.0f;
    static float total_time = 0.0f;

    perf_frequency = SDL_GetPerformanceFrequency();

    // @Incomplete: Make a dynamic buffer based on text size rather than fixed size!
    char lua_buffer[4096] = {0};
    JS_get_lua_string(lua_buffer, 4096);

    // @Note: Is this okay to do every fra,e or do we need to pop these?
    lua_pushnumber(luastate, delta_time);
    lua_setglobal(luastate, "dt");
    lua_pushnumber(luastate, total_time);
    lua_setglobal(luastate, "t");
    lua_pushinteger(luastate, SCREEN_W);
    lua_setglobal(luastate, "scrw");
    lua_pushinteger(luastate, SCREEN_H);
    lua_setglobal(luastate, "scrh");

    int ret = luaL_dostring(luastate, lua_buffer);
    if(ret != LUA_OK)
    {
        const char* err = lua_tostring(luastate,-1);
        JS_set_error_message(err, strlen(err));
    }
    else
    {
        JS_set_error_message(NULL,0);
    }

    end_counter = SDL_GetPerformanceCounter();
    elapsed_counter = end_counter - last_counter;
    last_counter = SDL_GetPerformanceCounter();

    delta_time = (float)elapsed_counter / (float)perf_frequency;
    total_time += delta_time;

    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(target, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, target, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    // Stop SDL from eating all of our inputs!
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_DISABLE);

    window = SDL_CreateWindow("pixtoy", 0,0,SCREEN_W*2,SCREEN_H*2, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Setup the render target for drawing into.
    pixU32 pixel_format = SDL_GetWindowPixelFormat(window);
    pixU32 r,g,b,a;
    int bpp;
    SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a);
    screen = SDL_CreateRGBSurface(0, SCREEN_W,SCREEN_H, 32, r,g,b,a);
    target = SDL_CreateTexture(renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, SCREEN_W,SCREEN_H);
    pixels = (pixU32*)screen->pixels;

    // Expose functions to Lua.
    luastate = luaL_newstate();
    pix_register_api(luastate);

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
