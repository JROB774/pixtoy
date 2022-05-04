#ifndef PIXAPI_H__ /*/////////////////////////////////////////////////////////*/
#define PIXAPI_H__

#define PIXAPI(name) PIXDEF pixINT PIXAPI_##name(lua_State* lua)

PIXDEF pixVOID pix_api_register(lua_State* lua);

// UTIL //////
PIXAPI(chr  ); // x       -> c   :: converts ascii code x to a character
// BITS //////
PIXAPI(band ); // a,b     -> c   :: the bitwise AND operation of a and b
PIXAPI(bor  ); // a,b     -> c   :: the bitwise OR operation of a and b
PIXAPI(bxor ); // a,b     -> c   :: the bitwise XOR operation of a and b
PIXAPI(bnot ); // x       -> y   :: the bitwise NOT operation of a
// MATH //////
PIXAPI(abs  ); // x       -> y   :: returns absolute value of x
PIXAPI(acos ); // x       -> y   :: returns arc cosine of x
PIXAPI(asin ); // x       -> y   :: returns arc sine of x
PIXAPI(atan ); // y,[x]   -> z   :: returns arc tangent of either y or y/x
PIXAPI(round); // x       -> y   :: rounds x to the nearest whole number
PIXAPI(ceil ); // x       -> y   :: rounds x up to the nearest whole number
PIXAPI(floor); // x       -> y   :: rounds x down to the nearest while number
PIXAPI(deg  ); // r       -> d   :: converts rad from radians to degress
PIXAPI(rad  ); // d       -> r   :: converts deg from degrees to radians
PIXAPI(exp  ); // x       -> y   :: returns base-e exponential of x
PIXAPI(fmod ); // n,d     -> r   :: returns remainder of n/d
PIXAPI(modf ); // x       -> i,f :: returns the integral and fractional of x
PIXAPI(log  ); // x       -> y   :: returns base-e logarithm of x
PIXAPI(min  ); // a,b     -> c   :: returns the smallest of a and b
PIXAPI(max  ); // a,b     -> c   :: returns the largest of a and b
PIXAPI(sqrt ); // x       -> y   :: returns the square root of x
PIXAPI(cos  ); // x       -> y   :: returns cosine of x
PIXAPI(sin  ); // x       -> y   :: returns sine of x
PIXAPI(tan  ); // x       -> y   :: returns tan of x
PIXAPI(clamp); // x,l,h   -> y   :: returns x clamped between l and h
PIXAPI(seed ); // [x]            :: uses current time or x as new random seed
PIXAPI(rand ); // [x,[y]] -> z   :: generate a random number within a range
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
