//#warning Altered source only for use with RSDL_wrapper
//#warning RETRO HACK This is just an SDL wrapper for liretro core.
//original copyright follows:

/* 

RSDL_gfxPrimitives.h: graphics primitives for SDL

Copyright (C) 2001-2012  Andreas Schiffler

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.

Andreas Schiffler -- aschiffler at ferzkopp dot net

*/

#ifndef _RSDL_gfxPrimitives_h
#define _RSDL_gfxPrimitives_h

#include <math.h>
#ifndef M_PI
#define M_PI	3.1415926535897932384626433832795
#endif

#include "RSDL_wrapper.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

	/* ----- Versioning */

#define RSDL_GFXPRIMITIVES_MAJOR	2
#define RSDL_GFXPRIMITIVES_MINOR	0
#define RSDL_GFXPRIMITIVES_MICRO	25


	/* ---- Function Prototypes */

#ifdef _MSC_VER
#  if defined(DLL_EXPORT) && !defined(LIBRSDL_GFX_DLL_IMPORT)
#    define RSDL_GFXPRIMITIVES_SCOPE __declspec(dllexport)
#  else
#    ifdef LIBRSDL_GFX_DLL_IMPORT
#      define RSDL_GFXPRIMITIVES_SCOPE __declspec(dllimport)
#    endif
#  endif
#endif
#ifndef RSDL_GFXPRIMITIVES_SCOPE
#  define RSDL_GFXPRIMITIVES_SCOPE extern
#endif

	/* Note: all ___Color routines expect the color to be in format 0xRRGGBBAA */

	/* Pixel */

	RSDL_GFXPRIMITIVES_SCOPE int pixelColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int pixelRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Horizontal line */

	RSDL_GFXPRIMITIVES_SCOPE int hlineColor(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int hlineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Vertical line */

	RSDL_GFXPRIMITIVES_SCOPE int vlineColor(RSDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int vlineRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Rectangle */

	RSDL_GFXPRIMITIVES_SCOPE int rectangleColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int rectangleRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Rounded-Corner Rectangle */

	RSDL_GFXPRIMITIVES_SCOPE int roundedRectangleColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int roundedRectangleRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled rectangle (Box) */

	RSDL_GFXPRIMITIVES_SCOPE int boxColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int boxRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2,
		Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Rounded-Corner Filled rectangle (Box) */

	RSDL_GFXPRIMITIVES_SCOPE int roundedBoxColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int roundedBoxRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2,
		Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Line */

	RSDL_GFXPRIMITIVES_SCOPE int lineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int lineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA Line */

	RSDL_GFXPRIMITIVES_SCOPE int aalineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aalineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Thick Line */
	RSDL_GFXPRIMITIVES_SCOPE int thickLineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, 
		Uint8 width, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int thickLineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, 
		Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Circle */

	RSDL_GFXPRIMITIVES_SCOPE int circleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int circleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Arc */

	RSDL_GFXPRIMITIVES_SCOPE int arcColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int arcRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, 
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA Circle */

	RSDL_GFXPRIMITIVES_SCOPE int aacircleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aacircleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Circle */

	RSDL_GFXPRIMITIVES_SCOPE int filledCircleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledCircleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Ellipse */

	RSDL_GFXPRIMITIVES_SCOPE int ellipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int ellipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA Ellipse */

	RSDL_GFXPRIMITIVES_SCOPE int aaellipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aaellipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Ellipse */

	RSDL_GFXPRIMITIVES_SCOPE int filledEllipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledEllipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Pie */

	RSDL_GFXPRIMITIVES_SCOPE int pieColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int pieRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Pie */

	RSDL_GFXPRIMITIVES_SCOPE int filledPieColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledPieRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Trigon */

	RSDL_GFXPRIMITIVES_SCOPE int trigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int trigonRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA-Trigon */

	RSDL_GFXPRIMITIVES_SCOPE int aatrigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aatrigonRGBA(RSDL_Surface * dst,  Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Trigon */

	RSDL_GFXPRIMITIVES_SCOPE int filledTrigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledTrigonRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Polygon */

	RSDL_GFXPRIMITIVES_SCOPE int polygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int polygonRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy,
		int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA-Polygon */

	RSDL_GFXPRIMITIVES_SCOPE int aapolygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aapolygonRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy,
		int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Polygon */

	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonRGBA(RSDL_Surface * dst, const Sint16 * vx,
		const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	RSDL_GFXPRIMITIVES_SCOPE int texturedPolygon(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, RSDL_Surface * texture,int texture_dx,int texture_dy);

	/* (Note: These MT versions are required for multi-threaded operation.) */

	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonColorMT(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color, int **polyInts, int *polyAllocated);
	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonRGBAMT(RSDL_Surface * dst, const Sint16 * vx,
		const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
		int **polyInts, int *polyAllocated);
	RSDL_GFXPRIMITIVES_SCOPE int texturedPolygonMT(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, RSDL_Surface * texture,int texture_dx,int texture_dy, int **polyInts, int *polyAllocated);

	/* Bezier */

	RSDL_GFXPRIMITIVES_SCOPE int bezierColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, int s, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int bezierRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy,
		int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Characters/Strings */

	RSDL_GFXPRIMITIVES_SCOPE void gfxPrimitivesSetFont(const void *fontdata, Uint32 cw, Uint32 ch);
	RSDL_GFXPRIMITIVES_SCOPE void gfxPrimitivesSetFontRotation(Uint32 rotation);
	RSDL_GFXPRIMITIVES_SCOPE int characterColor(RSDL_Surface * dst, Sint16 x, Sint16 y, char c, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int characterRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, char c, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	RSDL_GFXPRIMITIVES_SCOPE int stringColor(RSDL_Surface * dst, Sint16 x, Sint16 y, const char *s, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int stringRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, const char *s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif				/* _RSDL_gfxPrimitives_h */
