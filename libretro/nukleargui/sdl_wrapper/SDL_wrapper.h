
#ifndef NUKLEAR_RSDL_H
#define NUKLEAR_RSDL_H

#include "libretro-core.h"

//RETRO HACK
//#warning just an SDL wrapper for use SDL surface/maprgba in the core.

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* The number of elements in a table */
#define RSDL_TABLESIZE(table)	(sizeof(table)/sizeof(table[0]))

typedef unsigned char	Uint8;
typedef signed char	Sint8;
typedef unsigned short	Uint16;
typedef signed short	Sint16;
typedef unsigned int	Uint32;
typedef signed int	Sint32;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int   int32;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int  uint32;

/* Figure out how to support 64-bit datatypes */
#if !defined(__STRICT_ANSI__)
#if defined(__GNUC__) || defined(__MWERKS__) || defined(__SUNPRO_C)
#define RSDL_HAS_64BIT_TYPE	 long
#elif defined(_MSC_VER) /* VC++ */
#define RSDL_HAS_64BIT_TYPE	__int64
#endif
#endif /* !__STRICT_ANSI__ */


#define Uint64 unsigned long long 
#define Sint64 signed long long 

#undef RSDL_HAS_64BIT_TYPE

/* General keyboard/mouse state definitions */
enum { RSDL_PRESSED = 0x01, RSDL_RELEASED = 0x00 };


typedef struct{
     Sint16 x, y;
     Uint16 w, h;
} RSDL_Rect;


typedef struct RSDL_Color
{
 Uint8 r;
 Uint8 g;
 Uint8 b;
 Uint8 a;
} RSDL_Color;

#define RSDL_Colour RSDL_Color
typedef struct RSDL_Palette
{
  int ncolors;
  RSDL_Color *colors;
  Uint32 version;
  int refcount;
} RSDL_Palette;
 
typedef struct {
      RSDL_Palette *palette;
      Uint8  BitsPerPixel;
      Uint8  BytesPerPixel;
      Uint8  Rloss, Gloss, Bloss, Aloss;
      Uint8  Rshift, Gshift, Bshift, Ashift;
      Uint32 Rmask, Gmask, Bmask, Amask;
      Uint32 colorkey;
      Uint8  alpha;
} RSDL_PixelFormat;

typedef struct RSDL_Surface {
        Uint32 flags;                           /* Read-only */
        RSDL_PixelFormat *format;                /* Read-only */
        int w, h;                               /* Read-only */
        Uint16 pitch;                           /* Read-only */
        void *pixels;                           /* Read-write */

        /* clipping information */
        RSDL_Rect clip_rect;                     /* Read-only */

        /* Reference count -- used when freeing surface */
        int refcount;                           /* Read-mostly */

	/* This structure also contains private fields not shown here */
} RSDL_Surface;


static __inline__ unsigned short RSDL_Swap16(unsigned short x){
	unsigned short result= ((x<<8)|(x>>8)); 
return result;
}
static __inline__ unsigned RSDL_Swap32(unsigned x){
	unsigned result= ((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
 return result;
}


#ifdef LSB_FIRST

#define RSDL_SwapLE16(X)	(X)
#define RSDL_SwapLE32(X) (X)

#define RSDL_SwapBE16(X) RSDL_Swap16(X)
#define RSDL_SwapBE32(X) RSDL_Swap32(X)

#else

#define RSDL_SwapLE16(X)	RSDL_Swap16(X)
#define RSDL_SwapLE32(X) RSDL_Swap32(X)

#define RSDL_SwapBE16(X) (X)
#define RSDL_SwapBE32(X) (X)

#endif

#define RSDL_LIL_ENDIAN	1234
#define RSDL_BIG_ENDIAN	4321

//RETRO HACK
//FIXME: TODO LSB as default
#ifdef LSB_FIRST
#define RSDL_BYTEORDER RSDL_LIL_ENDIAN         
#else    
#define RSDL_BYTEORDER RSDL_BIG_ENDIAN
#endif

#ifdef __cplusplus
typedef bool RSDL_bool; 
#define RSDL_FALSE false
#define RSDL_TRUE  true
#else
typedef enum {
	RSDL_FALSE = 0,
	RSDL_TRUE  = 1
} RSDL_bool;
#endif

#define RSDL_MUSTLOCK(a) 0
#define RSDL_LockSurface(a) 0
#define RSDL_UnlockSurface(a) 
#define RSDL_MapRGB(fmt, r, g, b) Retro_MapRGB(fmt, r, g, b)
#define RSDL_MapRGBA(fmt, r, g, b,a) Retro_MapRGBA(fmt, r, g, b,a)

extern void RSDL_GetClipRect(RSDL_Surface *surface, RSDL_Rect *rect);
extern RSDL_bool RSDL_SetClipRect(RSDL_Surface *surface, const RSDL_Rect *rect);
#ifdef M16B
extern void Retro_Draw_char(RSDL_Surface *surface, signed short int x, signed short int y,  char string,unsigned short xscale, unsigned short yscale, unsigned short fg, unsigned short bg);
#else
extern void Retro_Draw_char(RSDL_Surface *surface, signed short int x, signed short int y,  char string,unsigned short xscale, unsigned short yscale, unsigned  fg, unsigned  bg);
#endif
//extern void Retro_Draw_char(RSDL_Surface *surface, signed short int x, signed short int y,  char string,unsigned short xscale, unsigned short yscale, unsigned  fg, unsigned  bg);
extern void Retro_FreeSurface(RSDL_Surface *surf );
extern RSDL_Surface *Retro_CreateRGBSurface32( int w,int h, int d, int rm,int rg,int rb,int ra);
extern RSDL_Surface *Retro_CreateRGBSurface16( int w,int h, int d, int rm,int rg,int rb,int ra);
extern unsigned int Retro_MapRGB(RSDL_PixelFormat *a, int r, int g, int b);
extern  unsigned int Retro_MapRGBA(RSDL_PixelFormat *a, int r, int g, int b,int alpha);
#endif
