/*////////////////////////////////////////////////////////////////////////////*/

#include <stdint.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "pixdef.h"
#include "pixchr.h"
#include "pixlua.h"
#include "pixapi.h"
#include "pixapp.h"

#include "pixlua.c"
#include "pixapi.c"
#include "pixapp.c"

#include <emscripten.h>

#include <SDL.h>

typedef struct pixCONTEXT
{
    lua_State*    lstate; // @Incomplete: Pull out into pixlua as this is not
    SDL_Window*   window; // platform-specific code so it can be more generic.
    SDL_Renderer* render;
    SDL_Surface*  screen;
    SDL_Texture*  target;
}
pixCONTEXT;

PIXINTERNAL pixCONTEXT pix_context;

// Function call into JS to display an error message in a webpage alert box.
EM_JS(pixVOID, JS_display_error, (const pixCHAR* err, pixU64 len),
{
    display_error(err, len);
});
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

PIXINTERNAL pixVOID sdl_fatal_error(const pixCHAR* message)
{
    pixCHAR buffer[512] = {0};
    snprintf(buffer, PIXARRSIZE(buffer), "%s\n(%s)", message, SDL_GetError());
    JS_display_error(buffer, strlen(buffer));
    abort();
}

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
    JS_get_lua_string(lua_buffer, PIXARRSIZE(lua_buffer));
    const pixCHAR* err = pix_run(lua_buffer, pix_context.lstate,
        delta_time, total_time);
    if(!err) JS_set_error_message(NULL, 0);
    else JS_set_error_message(err, strlen(err));

    end_counter = SDL_GetPerformanceCounter();
    elapsed_counter = end_counter - last_counter;
    last_counter = SDL_GetPerformanceCounter();

    delta_time = PIXCAST(pixFLOAT,elapsed_counter) /
                 PIXCAST(pixFLOAT,perf_frequency);
    total_time += delta_time;

    pixU32* pixels = pix_context.screen->pixels;
    pixINT pitch = pix_context.screen->pitch;

    SDL_SetRenderDrawColor(pix_context.render, 0,0,0,255);
    SDL_RenderClear(pix_context.render);
    SDL_UpdateTexture(pix_context.target, NULL, pixels, pitch);
    SDL_RenderCopy(pix_context.render, pix_context.target, NULL, NULL);
    SDL_RenderPresent(pix_context.render);
}

pixINT main(pixINT argc, pixCHAR** argv)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        sdl_fatal_error("Failed to initialize SDL video system.");
    }

    // Disable all of these events to stop SDL from eating all of our inputs.
    // The program itself doesn't need any user input but the webpage does.
    SDL_EventState(SDL_TEXTINPUT,       SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN,         SDL_DISABLE);
    SDL_EventState(SDL_KEYUP,           SDL_DISABLE);
    SDL_EventState(SDL_MOUSEMOTION,     SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONUP,   SDL_DISABLE);

    pixINT ww = PIXSCRW*6;
    pixINT wh = PIXSCRH*6;

    pix_context.window = SDL_CreateWindow("pixtoy",
        0,0,ww,wh, SDL_WINDOW_SHOWN);
    if(!pix_context.window)
        sdl_fatal_error("Failed to create window.");
    pix_context.render = SDL_CreateRenderer(pix_context.window,
        -1, SDL_RENDERER_ACCELERATED);
    if(!pix_context.render)
        sdl_fatal_error("Failed to create renderer.");

    // Setup the render target for drawing into.
    pixU32 pixel_format = SDL_GetWindowPixelFormat(pix_context.window);
    pixU32 r,g,b,a;
    pixINT bpp;
    if(!SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a))
        sdl_fatal_error("Failed to convert format.");
    pix_context.screen = SDL_CreateRGBSurface(0, PIXSCRW,PIXSCRH, 32, r,g,b,a);
    if(!pix_context.screen)
        sdl_fatal_error("Failed to create screen.");
    pix_context.target = SDL_CreateTexture(pix_context.render, pixel_format,
        SDL_TEXTUREACCESS_STREAMING, PIXSCRW,PIXSCRH);
    if(!pix_context.target)
        sdl_fatal_error("Failed to create target.");

    // @Improve: Don't really want to call SDL_GetError on this.
    pix_context.lstate = luaL_newstate();
    if(!pix_context.lstate)
    {
        sdl_fatal_error("Failed to create Lua state.");
    }

    // Setup the API for Lua to use.
    pix_set_screen(pix_context.screen->pixels);
    pix_register_api(pix_context.lstate);

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
