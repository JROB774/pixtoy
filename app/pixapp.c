/*////////////////////////////////////////////////////////////////////////////*/

#include "pixapp.h"

#include <emscripten.h>

#include <SDL.h>

static SDL_Window*   window;
static SDL_Renderer* renderer;
static SDL_Surface*  screen;
static SDL_Texture*  target;
static pixU32*       pixels;
static lua_State*    luastate;

#include "pixlua.c"
#include "pixapi.c"

// Function call into JS to retrieve text edit string for parsing into Lua code.
EM_JS(void, JS_get_lua_string, (const pixCHAR* out_str, pixU64 max_bytes),
{
    get_lua_string(out_str, max_bytes);
});

// Function call into JS to display a Lua error message on the webpage.
EM_JS(void, JS_set_error_message, (const pixCHAR* err, pixU64 len),
{
    set_error_message(err, len);
});

static void main_loop()
{
    static pixU64 perf_frequency = 0;
    static pixU64 last_counter = 0;
    static pixU64 end_counter = 0;
    static pixU64 elapsed_counter = 0;

    static pixFLOAT delta_time = 0.0f;
    static pixFLOAT total_time = 0.0f;

    perf_frequency = SDL_GetPerformanceFrequency();

    // @Incomplete: Make a dynamic buffer based on text size rather than fixed size!
    pixCHAR lua_buffer[4096] = {0};
    JS_get_lua_string(lua_buffer, 4096);

    // @Note: Is this okay to do every fra,e or do we need to pop these?
    lua_pushnumber(luastate, delta_time);
    lua_setglobal(luastate, "dt");
    lua_pushnumber(luastate, total_time);
    lua_setglobal(luastate, "t");
    lua_pushinteger(luastate, PIXSCRW);
    lua_setglobal(luastate, "scrw");
    lua_pushinteger(luastate, PIXSCRH);
    lua_setglobal(luastate, "scrh");

    pixINT ret = luaL_dostring(luastate, lua_buffer);
    if(ret != LUA_OK)
    {
        const pixCHAR* err = lua_tostring(luastate,-1);
        JS_set_error_message(err, strlen(err));
    }
    else
    {
        JS_set_error_message(NULL,0);
    }

    end_counter = SDL_GetPerformanceCounter();
    elapsed_counter = end_counter - last_counter;
    last_counter = SDL_GetPerformanceCounter();

    delta_time = (pixFLOAT)elapsed_counter / (pixFLOAT)perf_frequency;
    total_time += delta_time;

    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(target, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, target, NULL, NULL);
    SDL_RenderPresent(renderer);
}

pixINT main(pixINT argc, pixCHAR** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    // Stop SDL from eating all of our inputs!
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_DISABLE);

    window = SDL_CreateWindow("pixtoy", 0,0,PIXSCRW*2,PIXSCRH*2, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Setup the render target for drawing into.
    pixU32 pixel_format = SDL_GetWindowPixelFormat(window);
    pixU32 r,g,b,a;
    pixINT bpp;
    SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a);
    screen = SDL_CreateRGBSurface(0, PIXSCRW,PIXSCRH, 32, r,g,b,a);
    target = SDL_CreateTexture(renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, PIXSCRW,PIXSCRH);
    pixels = (pixU32*)screen->pixels;

    // Expose functions to Lua.
    luastate = luaL_newstate();
    pix_register_api(luastate);

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
