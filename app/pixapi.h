#ifndef PIXAPI_H__ /*/////////////////////////////////////////////////////////*/
#define PIXAPI_H__

#define PIXAPI(name) PIXDEF pixINT PIXAPI_##name(lua_State* lua)

PIXDEF pixVOID pix_register_api(lua_State* lua);
PIXDEF pixVOID pix_set_screen(pixU32* screen);

// UTIL //////
PIXAPI(chr  ); // x       -> c   :: converts ascii code x to a character
// BITS //////
PIXAPI(band ); // a,b     -> x   :: the bitwise AND operation of a and b
PIXAPI(bor  ); // a,b     -> x   :: the bitwise OR operation of a and b
PIXAPI(bxor ); // a,b     -> x   :: the bitwise XOR operation of a and b
PIXAPI(bnot ); // x       -> x   :: the bitwise NOT operation of a
// MATH //////
PIXAPI(abs  ); // x       -> x   :: returns absolute value of x
PIXAPI(acos ); // x       -> x   :: returns arc cosine of x
PIXAPI(asin ); // x       -> x   :: returns arc sine of x
PIXAPI(atan ); // y,[x]   -> x   :: returns arc tangent of either y or y/x
PIXAPI(round); // x       -> x   :: rounds x to the nearest whole number
PIXAPI(ceil ); // x       -> x   :: rounds x up to the nearest whole number
PIXAPI(floor); // x       -> x   :: rounds x down to the nearest while number
PIXAPI(deg  ); // rad     -> x   :: converts rad from radians to degress
PIXAPI(rad  ); // deg     -> x   :: converts deg from degrees to radians
PIXAPI(exp  ); // x       -> x   :: returns base-e exponential of x
PIXAPI(fmod ); // n,d     -> x   :: returns remainder of n/d
PIXAPI(modf ); // x       -> i,f :: returns the integral and fractional of x
PIXAPI(log  ); // x       -> x   :: returns base-e logarithm of x
PIXAPI(min  ); // a,b     -> x   :: returns the smallest of a and b
PIXAPI(max  ); // a,b     -> x   :: returns the largest of a and b
PIXAPI(sqrt ); // x       -> x   :: returns the square root of x
PIXAPI(cos  ); // x       -> x   :: returns cosine of x
PIXAPI(sin  ); // x       -> x   :: returns sine of x
PIXAPI(tan  ); // x       -> x   :: returns tan of x
PIXAPI(clamp); // x,l,h   -> x   :: returns x clamped between l and h
PIXAPI(seed ); // [x]     -> x   :: uses current time or x as new random seed
PIXAPI(rand ); // [x,[y]] -> x   :: generate a random number within a range
// DRAW //////
PIXAPI(clrs ); // c              :: clears the screen with color c
PIXAPI(pset ); // x,y,c          :: sets screen pixel at x,y to color c
PIXAPI(pget ); // x,y     -> c   :: returns screen pixel color at x,y
PIXAPI(line ); // x,y,z,w,c      :: draws line from point x,y to z,w
PIXAPI(recto); // x,y,w,h,c      :: draws a rect outline with bounds x,y,w,h
PIXAPI(rectf); // x,y,w,h,c      :: draws a rect fill with bounds x,y,w,h
PIXAPI(circo); // x,y,r,t,c      :: draws a circle outline with radius r at x,y
PIXAPI(circf); // x,y,r,c        :: draws a circle fill with raiuds r at x,y
PIXAPI(text ); // x,y,t,c        :: draws text string t at position x,y

#endif /* PIXAPI_H__ /////////////////////////////////////////////////////////*/
