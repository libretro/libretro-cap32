/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2021)
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

#include "cap32.h"
#include "libretro-core.h"
#include "video.h"
#include "software.h"
#include "assets/assets.h"

extern t_GateArray GateArray;
extern uint32_t colours[32];

static union TPixel pixel;

#define RGB2COLOR(r, g, b)    (b | ((g << 8) | (r << 16)))
#define RGB2RED(colour)       ((colour>>16) & 0xFF)
#define RGB2GREEN(colour)     ((colour>>8) & 0xFF)
#define RGB2BLUE(colour)      (colour & 0xFF)

/**
 * generate antialias values using 24bpp macros
 *
 * RGB[10] 00CE60 || CC CC 00
 * 00CE60 (10)    || C8 CC 00
 */
void video_set_palette_antialias_24bpp(void)
{
   uint8_t r2,g2,b2;
   r2=RGB2RED(colours[GateArray.ink_values[0]]) + RGB2RED(colours[GateArray.ink_values[1]]);
   g2=RGB2GREEN(colours[GateArray.ink_values[0]]) + RGB2GREEN(colours[GateArray.ink_values[1]]);
   b2=RGB2BLUE(colours[GateArray.ink_values[0]]) + RGB2BLUE(colours[GateArray.ink_values[1]]);
   GateArray.palette[33] = (unsigned short) RGB2COLOR(r2/2, g2/2, b2/2);
}

/**
 * rgb2color_24bpp:
 * convert rgb to 24bpp color (see macros)
 **/
unsigned int rgb2color_24bpp(unsigned int r, unsigned int g, unsigned int b)
{
   return RGB2COLOR(r, g, b);
}

/**
 * convert_color:
 * convert raw color to 24bpp color (see macros)
 **/
static INLINE unsigned int convert_color(unsigned int colour)
{
   pixel.colour = colour;
   return (
      (pixel.r << 16)
      | (pixel.g << 8)
      | pixel.b
   );
}

/**
 * draw_pixel_24bpp:
 * simple method to emulate a transparency
 **/
void draw_pixel_24bpp(unsigned int * dest, const unsigned int * img)
{
   if (*(img) != PIXEL_TRANSPARENT)
   {
      *(dest) = *(img);
   }
}

/**
 * convert_image_24bpp:
 * convert raw image to 24bpp, used on init
 **/
void convert_image_24bpp(unsigned int * dest, const unsigned int * img, int size)
{
   while (size--)
   {
      #ifndef LOWRES
      *(dest++) = convert_color(*img);
      #endif
      *(dest++) = convert_color(*(img++));
   }
}

/**
 * draw_line_24bpp:
 * copy a 24bpp color to your dest
 **/
void draw_line_24bpp(unsigned int * dest, int width, unsigned int color)
{
   while (width--)
      *(dest++) = color;
}

/**
 * draw_char_24bpp:
 * draw a 24bpp char to your dest
 **/
void draw_char_24bpp(unsigned int * buffer_ptr, const unsigned char *font_data, unsigned int color)
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
            #ifndef LOWRES
            *(buffer_ptr++) = color;
            #endif
            *(buffer_ptr++) = color; // draw the character pixel
         }
         else
         {
            buffer_ptr += EMULATION_SCALE;
         }
         data <<= 1; // advance to the next bit
      }
      font_data ++;
      buffer_ptr += EMULATION_SCREEN_WIDTH - (FNT_CHAR_WIDTH * EMULATION_SCALE);
   }
}
