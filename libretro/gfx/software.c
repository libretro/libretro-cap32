/* Copyright (C) 2018
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>
#include <libretro-core.h>

#include "assets/ui.h"
#include "software.h"

// FIXME PIXEL_BYTES

#define DATA2BLUE5(colour)       (((colour>>3) & 0x1F))
#define DATA2GREEN6(colour)      (((colour>>2) & 0x3F) << 5)
#define DATA2RED5(colour)        (((colour>>3) & 0x1F) << 11)

#define DATA2BLUE(colour)       ((colour >> 16) & 0xFF)
#define DATA2GREEN(colour)      ((colour >> 8) & 0xFF)
#define DATA2RED(colour)        (colour & 0xFF)

#ifdef LOWRES
#define DRAW2BUFFER(buffer, img)      *(buffer++) = *(img++);
#else
#define DRAW2BUFFER(buffer, img) \
{ \
   *(buffer++) = *img; \
   *(buffer++) = *(img++); \
}
#endif

//#endif
PIXEL_TYPE convert_color (unsigned int colour)
{
   #ifdef M16B
   return (
      DATA2RED5(DATA2RED(colour))
      | DATA2GREEN6(DATA2GREEN(colour))
      | DATA2BLUE5(DATA2BLUE(colour))
   );
   #else
   return (
      ( DATA2RED(colour) << 16)
      | (colour & 0x00FF00)
      | DATA2BLUE(colour)
   );
   #endif
}

void draw_line(PIXEL_TYPE * buffer, int x, int y, int width, PIXEL_TYPE color)
{
   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   while (width--) {
      *(buffer++) = color;
   }
}

void draw_rect(PIXEL_TYPE * buffer, int x, int y, int width, int height, PIXEL_TYPE color)
{
   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   while (height--)
   {
      int loop_width = width;
      while (loop_width--)
         *(buffer++) = color;

      buffer += EMULATION_SCREEN_WIDTH - width;
   }
}

// M16B
static void _draw_char(PIXEL_TYPE * buffer, const unsigned char *font_data, PIXEL_TYPE color)
{
   int height = FNT_CHAR_HEIGHT;
   while (height--)
   {
      unsigned char data = *font_data; // get the bitmap information for one row
      int loop_width = FNT_CHAR_WIDTH;
      while (loop_width--)
      {
         if (data & 0x80) // is the bit set?
         {
            if( EMULATION_SCALE == 2)
               *(buffer++) = color;
            *(buffer++) = color; // draw the character pixel
         }
         else
         {
            buffer += EMULATION_SCALE;
         }
         data <<= 1; // advance to the next bit
      }
      font_data ++;
      buffer += EMULATION_SCREEN_WIDTH - (FNT_CHAR_WIDTH * EMULATION_SCALE);
   }
}

void draw_char(PIXEL_TYPE * buffer, int x, int y, char chr_idx, PIXEL_TYPE color)
{
   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   chr_idx -= FNT_MIN_CHAR; // zero base the index
   _draw_char(buffer, &font[chr_idx * BITS_IN_BYTE], color);
}

void draw_text(PIXEL_TYPE * buffer, int x, int y, const char *text, PIXEL_TYPE color)
{
   int len = strlen(text); // number of characters to process
   char *ptr_text = (char *) text;

   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   while(len--)
   {
      unsigned int chr_idx = *ptr_text;
      if ((chr_idx < FNT_MIN_CHAR) || (chr_idx > FNT_MAX_CHAR)) { // limit it to the range of chars in the font
         chr_idx = FNT_MIN_CHAR;
      }
      chr_idx -= FNT_MIN_CHAR; // zero base the index
      _draw_char(buffer, &font[chr_idx * BITS_IN_BYTE], color);
      ptr_text++;
      buffer += FNT_CHAR_WIDTH * EMULATION_SCALE;
   }
}


// base image in u_int32
void convert_image(PIXEL_TYPE * buffer, const unsigned int * img, unsigned int size)
{
   while (size--)
   {
#ifndef LOWRES
         *(buffer++) = convert_color(*img);
#endif
      *(buffer++) = convert_color(*(img++));
   }
}

// create linear version ??
void draw_image(PIXEL_TYPE * buffer, PIXEL_TYPE * img, int x, int y, int width, int height)
{
   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   while (height--)
   {
      int loop_width = width;
      while (loop_width--)
      {
         DRAW2BUFFER(buffer, img);
      }
      buffer += EMULATION_SCREEN_WIDTH  - (width * EMULATION_SCALE);
   }
}

void draw_image_linear(PIXEL_TYPE * buffer, PIXEL_TYPE * img, int x, int y, unsigned int size)
{
   uint32_t * buffer_ptr = (uint32_t *) (
      (PIXEL_TYPE *) (buffer + x) + (y * EMULATION_SCREEN_WIDTH)
   );
   uint32_t * img_ptr = (uint32_t *) img;
   unsigned int loop_counter = (EMULATION_SCALE * size) / PIXEL_RAW_DENSITY;

   while (loop_counter--)
   {
      *(buffer_ptr++) = *(img_ptr++);
   }
}
