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

#include <SDL.h>

#include <emscripten.h>

PIXINTERNAL struct pixCONTEXT
{
    SDL_Window*   window;
    SDL_Renderer* render;
    SDL_Surface*  screen;
    SDL_Texture*  target;
}
pixctx;

EM_JS(pixVOID, JSFUNC_display_error_msg, (const pixCHAR* err, pixU64 len),
{
    var errmsg = UTF8ToString(err,len);
    alert(errmsg);
});

EM_JS(pixVOID, JSFUNC_get_source, (const pixCHAR* src, pixU64 len),
{
    var text = ace.edit("editor").getValue();
    stringToUTF8(text, src, len);
});

EM_JS(pixVOID, JSFUNC_set_error_msg, (const pixCHAR* err, pixU64 len),
{
    var element = document.getElementById("error");
    if(len !== 0)
    {
        var errmsg = UTF8ToString(err,len);
        element.textContent = errmsg;
        element.style.display = "flex";
    }
    else
    {
        element.style.display = "none";
    }
});

PIXINTERNAL pixVOID sdl_fatal_error(const pixCHAR* message)
{
    pixCHAR buffer[512] = {0};
    snprintf(buffer, PIXARRSIZE(buffer), "%s\n(%s)", message, SDL_GetError());
    JSFUNC_display_error_msg(buffer, strlen(buffer));
    abort();
}

PIXINTERNAL pixVOID main_loop(pixVOID)
{
    PIXPERSISTENT pixU64 perf_frequency  = 0;
    PIXPERSISTENT pixU64 last_counter    = 0;
    PIXPERSISTENT pixU64 end_counter     = 0;
    PIXPERSISTENT pixU64 elapsed_counter = 0;

    PIXPERSISTENT pixFLOAT delta_time = 0.0f;

    perf_frequency = SDL_GetPerformanceFrequency();

    // @Incomplete: Make a dynamic buffer based on text size rather than fixed!
    // @Incomplete: Only build when the build button has been pressed...
    pixCHAR lua_buffer[4096] = {0};
    JSFUNC_get_source(lua_buffer, PIXARRSIZE(lua_buffer));
    const pixCHAR* err = pix_app_build(lua_buffer);
    if(!err) JSFUNC_set_error_msg(NULL, 0);
    else JSFUNC_set_error_msg(err, strlen(err));

    pix_app_tick(delta_time);

    end_counter = SDL_GetPerformanceCounter();
    elapsed_counter = end_counter - last_counter;
    last_counter = SDL_GetPerformanceCounter();

    delta_time = PIXCAST(pixFLOAT,elapsed_counter) /
                 PIXCAST(pixFLOAT,perf_frequency);

    pixU32* pixels = pixctx.screen->pixels;
    pixINT pitch = pixctx.screen->pitch;

    SDL_SetRenderDrawColor(pixctx.render, 0,0,0,255);
    SDL_RenderClear(pixctx.render);
    SDL_UpdateTexture(pixctx.target, NULL, pixels, pitch);
    SDL_RenderCopy(pixctx.render, pixctx.target, NULL, NULL);
    SDL_RenderPresent(pixctx.render);
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

    pixctx.window = SDL_CreateWindow("pixtoy", 0,0,ww,wh, SDL_WINDOW_SHOWN);
    if(!pixctx.window)
        sdl_fatal_error("Failed to create window.");
    pixctx.render = SDL_CreateRenderer(pixctx.window, -1, 0);
    if(!pixctx.render)
        sdl_fatal_error("Failed to create renderer.");

    // Setup the render target for drawing into.
    pixU32 pixel_format = SDL_GetWindowPixelFormat(pixctx.window);
    pixU32 r,g,b,a;
    pixINT bpp;
    if(!SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r,&g,&b,&a))
        sdl_fatal_error("Failed to convert format.");
    pixctx.screen = SDL_CreateRGBSurface(0, PIXSCRW,PIXSCRH, 32, r,g,b,a);
    if(!pixctx.screen)
        sdl_fatal_error("Failed to create screen.");
    pixctx.target = SDL_CreateTexture(pixctx.render, pixel_format,
        SDL_TEXTUREACCESS_STREAMING, PIXSCRW,PIXSCRH);
    if(!pixctx.target)
        sdl_fatal_error("Failed to create target.");

    pix_app_init(pixctx.screen->pixels);

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
