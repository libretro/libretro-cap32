/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2021)
 *  Copyright Daniel De Matteis (2012-2021)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>
#include <libretro-core.h>

#include "assets/assets.h"
#include "software.h"
#include "video.h"

/**
 * draw_rect:
 * draw a rectangle, used by UI and KoS
 **/
void draw_rect(uint32_t * buffer, int x, int y, int width, int height, uint32_t color)
{
   buffer = (buffer + (x >> retro_video.raw_density_byte)) + (y * retro_video.bps);
   register uint32_t screen_width = retro_video.bps;
   while (height--)
   {
      retro_video.draw_line(buffer, width, color);
      buffer += screen_width;
   }
}

/**
 * draw_char:
 * draw a char, used by mouse cursor and text
 **/
void draw_char(uint32_t * buffer, int x, int y, char chr_idx, uint32_t color)
{
   buffer = (buffer + (x >> retro_video.raw_density_byte)) + (y * retro_video.bps);
   chr_idx -= FNT_MIN_CHAR; // zero base the index
   retro_video.draw_char(buffer, &font[chr_idx * BITS_IN_BYTE], color);
}

/**
 * draw_text:
 * draw a text, used by UI and KoS
 **/
void draw_text(uint32_t * buffer, int x, int y, const char *text, uint32_t color)
{
   int len = strlen(text); // number of characters to process
   char *ptr_text = (char *) text;
   unsigned int chr_idx = *ptr_text;

   buffer = (buffer + (x >> retro_video.raw_density_byte) ) + (y * retro_video.bps);
   while(len--)
   {
      // limit it to the range of chars in the font
      if ((chr_idx < FNT_MIN_CHAR) || (chr_idx > FNT_MAX_CHAR))
      {
         chr_idx = FNT_MIN_CHAR;
      }
      chr_idx -= FNT_MIN_CHAR; // zero base the index

      retro_video.draw_char(buffer, &font[chr_idx * BITS_IN_BYTE], color);

      // prepare next char
      ptr_text++;
      buffer += retro_video.char_size;
      chr_idx = *ptr_text;
   }
}

/**
 * draw_image_linear:
 * simple function to draw an image that occupies the entire screen
 * does not need to recalculate the position after each line.
 **/
void draw_image_linear(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size)
{
   buffer = (buffer + x) + (y * retro_video.bps);

   register uint32_t loop = size * retro_video.bytes;
   while (loop--)
   {
      *(buffer++) = *(img++);
   }
}

// TODO: need more work with 16bits blending 'cause fg/bg are 32bits pointers
#define BLEND_COLOR(fg, bg, out)                    \
{                                                       \
   (*(out)) = ((fg + bg + ((fg ^ bg) & retro_video.blend_mask)) >> 1); \
}

/**
 * draw_image_linear_blend:
 * function to draw an image with blending that occupies the entire screen
 * does not need to recalculate the position after each line.
 **/
void draw_image_linear_blend(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size)
{
   buffer = (buffer + x) + (y * retro_video.bps);

   register uint32_t loop = size * retro_video.bytes;
   while (loop--)
   {
      BLEND_COLOR(*img, *buffer, buffer);
      img++; buffer++;
   }
}

/*
 * Prepare UI functions
 * images are in uint32_t mode
 */

/**
 * convert_image:
 * convert intenal UI images to current depth mode (using video depth driver)
 **/
void convert_image(unsigned int * buffer, const unsigned int * img, unsigned int size)
{
   retro_video.convert_image(buffer, img, size);
}

/**
 * draw_image_transparent:
 * draw a image with full transparency or not (no blend)
 * used by KoS letters
 **/
void draw_image_transparent(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size)
{
   uint32_t * buffer_ptr = (
      (uint32_t *) (buffer + x) + (y * retro_video.bps)
   );
   uint32_t * img_ptr = (uint32_t *) img;
   unsigned int loop_counter = (EMULATION_SCALE * size);

   while (loop_counter--)
   {
      retro_video.draw_pixel(buffer_ptr++, img_ptr++);
   }

}

#ifdef UNUSED

#ifdef LOWRES
#define DRAW2BUFFER(buffer, img)      *(buffer++) = *(img++);
#else
#define DRAW2BUFFER(buffer, img) \
{ \
   *(buffer++) = *img; \
   *(buffer++) = *(img++); \
}
#endif

void draw_image(uint32_t * buffer, uint32_t * img, int x, int y, int width, int height)
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
void draw_line(uint32_t * buffer, int x, int y, int width, uint32_t color)
{
   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   while (width--) {
      *(buffer++) = color;
   }
}
#endif
