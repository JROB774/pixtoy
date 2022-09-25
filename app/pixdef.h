#ifndef PIXDEF_H__ /*/////////////////////////////////////////////////////////*/
#define PIXDEF_H__

#define PIXEXTERNAL   extern
#define PIXINTERNAL   static
#define PIXGLOBAL     static
#define PIXPERSISTENT static
#define PIXDEF        static

#define PIXFALSE 0
#define PIXTRUE  1

#define PIXSCRW 128
#define PIXSCRH 128

typedef void     pixVOID;
typedef char     pixCHAR;
typedef unsigned pixUINT;
typedef int      pixINT;
typedef float    pixFLOAT;
typedef double   pixDOUBLE;
typedef int8_t   pixBOOL;
typedef uint8_t  pixU8;
typedef uint16_t pixU16;
typedef uint32_t pixU32;
typedef uint64_t pixU64;
typedef int8_t   pixS8;
typedef int16_t  pixS16;
typedef int32_t  pixS32;
typedef int64_t  pixS64;

#define PIXARRSIZE(arr) (sizeof(arr)/sizeof((arr)[0]))
#define PIXSWAP(t,x,y) do { t t_=x; x=y; y=t_; } while(0)
#define PIXCLAMP(x,l,h) (((x)>(h))?(h):(((x)<(l))?(l):(x)))
#define PIXCAST(t,x) ((t)(x))

#endif /* PIXDEF_H__ /////////////////////////////////////////////////////////*/
