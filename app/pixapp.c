/*////////////////////////////////////////////////////////////////////////////*/

#include "pixapp.h"

#include "pixlua.c"
#include "pixapi.c"

#include <emscripten.h>

#include <SDL.h>

typedef struct pixCONTEXT
{
    lua_State*    luastate;
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Surface*  screen;
    SDL_Texture*  target;
}
pixCONTEXT;

PIXINTERNAL pixCONTEXT pix_context;

// Function call into JS to retrieve text edit string for parsing into Lua code.
EM_JS(pixVOID, JS_get_lua_string, (const pixCHAR* out_str, pixU64 max_bytes),
{
    get_lua_string(out_str, max_bytes);
});

// Function call into JS to display a Lua error message on the webpage.
EM_JS(pixVOID, JS_set_error_message, (const pixCHAR* err, pixU64 len),
{
    set_error_message(err, len);
});

PIXINTERNAL pixVOID main_loop(pixVOID)
{
    PIXPERSISTENT pixU64 perf_frequency = 0;
    PIXPERSISTENT pixU64 last_counter = 0;
    PIXPERSISTENT pixU64 end_counter = 0;
    PIXPERSISTENT pixU64 elapsed_counter = 0;

    PIXPERSISTENT pixFLOAT delta_time = 0.0f;
    PIXPERSISTENT pixFLOAT total_time = 0.0f;

    perf_frequency = SDL_GetPerformanceFrequency();

    // @Incomplete: Make a dynamic buffer based on text size rather than fixed!
    pixCHAR lua_buffer[4096] = {0};
    JS_get_lua_string(lua_buffer, 4096);

    // @Note: Is this okay to do every fra,e or do we need to pop these?
    lua_pushnumber(pix_context.luastate, delta_time);
    lua_setglobal(pix_context.luastate, "dt");
    lua_pushnumber(pix_context.luastate, total_time);
    lua_setglobal(pix_context.luastate, "t");
    lua_pushinteger(pix_context.luastate, PIXSCRW);
    lua_setglobal(pix_context.luastate, "scrw");
    lua_pushinteger(pix_context.luastate, PIXSCRH);
    lua_setglobal(pix_context.luastate, "scrh");

    pixINT ret = luaL_dostring(pix_context.luastate, lua_buffer);
    if(ret != LUA_OK)
    {
        const pixCHAR* err = lua_tostring(pix_context.luastate,-1);
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

    pixU32* pixels = pix_context.screen->pixels;
    pixINT pitch = pix_context.screen->pitch;

    SDL_SetRenderDrawColor(pix_context.renderer, 0,0,0,255);
    SDL_RenderClear(pix_context.renderer);
    SDL_UpdateTexture(pix_context.target, NULL, pixels, pitch);
    SDL_RenderCopy(pix_context.renderer, pix_context.target, NULL, NULL);
    SDL_RenderPresent(pix_context.renderer);
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

    pixINT ww = PIXSCRW*2;
    pixINT wh = PIXSCRH*2;

    pix_context.window = SDL_CreateWindow("pixtoy",
        0,0,ww,wh, SDL_WINDOW_SHOWN);
    pix_context.renderer = SDL_CreateRenderer(pix_context.window,
        -1, SDL_RENDERER_ACCELERATED);

    // Setup the render target for drawing into.
    pixU32 pixel_format = SDL_GetWindowPixelFormat(pix_context.window);
    pixU32 r,g,b,a;
    pixINT bpp;
    SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a);
    pix_context.screen = SDL_CreateRGBSurface(0, PIXSCRW,PIXSCRH, 32, r,g,b,a);
    pix_context.target = SDL_CreateTexture(pix_context.renderer, pixel_format,
        SDL_TEXTUREACCESS_STREAMING, PIXSCRW,PIXSCRH);

    // Expose functions to Lua.
    pix_context.luastate = luaL_newstate();
    pix_set_screen(pix_context.screen->pixels);
    pix_register_api(pix_context.luastate);

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
