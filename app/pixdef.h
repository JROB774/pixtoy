#ifndef PIXDEF_H__ /*/////////////////////////////////////////////////////////*/
#define PIXDEF_H__

#define PIXINTERNAL   static
#define PIXGLOBAL     static
#define PIXPERSISTENT static
#define PIXDEF        static

#define PIXFALSE 0
#define PIXTRUE  1
#define PIXTRUE  1

#define PIXSCRW 256
#define PIXSCRH 256

typedef void     pixVOID;
typedef char     pixCHAR;
typedef unsigned pixUINT;
typedef int      pixINT;
typedef float    pixFLOAT;
typedef double   pixDOUBLE;
typedef bool     pixBOOL;
typedef uint8_t  pixU8;
typedef uint16_t pixU16;
typedef uint32_t pixU32;
typedef uint64_t pixU64;
typedef int8_t   pixS8;
typedef int16_t  pixS16;
typedef int32_t  pixS32;
typedef int64_t  pixS64;

// @Incomplete: Handle big endian?
typedef union pixCOLOR
{
    struct { pixU8 b,g,r,a; };
    pixU32 raw;
}
pixCOLOR;

#define PIXSWAP(t,x,y) do { t t_ = x; x = y; y = t_; } while(0)
#define PIXCLAMP(x,l,h) (((x)>(h))?(h):(((x)<(l))?(l):(x)))

#endif /* PIXDEF_H__ /////////////////////////////////////////////////////////*/
