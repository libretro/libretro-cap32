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

#define RGB2COLOR(r, g, b)    ((b>>3) | ((g>>2)<<5) | ((r>>3)<<11))
#define RGB2RED(colour)       (((colour>>11)<<3) & 0xFF)
#define RGB2GREEN(colour)     (((colour>>5)<<2) & 0xFF)
#define RGB2BLUE(colour)      ((colour<<3) & 0xFF)

#define DATA2BLUE5(colour)       (((colour>>3) & 0x1F))
#define DATA2GREEN6(colour)      (((colour>>2) & 0x3F) << 5)
#define DATA2RED5(colour)        (((colour>>3) & 0x1F) << 11)

/**
 * generate antialias values using 16bpp macros
 *
 * RGB[10] 00CE60 || CC CC 00
 * 00CE60 (10)    || C8 CC 00
 */
void video_set_palette_antialias_16bpp(void)
{
   uint8_t r2,g2,b2;
   r2=RGB2RED(colours[GateArray.ink_values[0]]) + RGB2RED(colours[GateArray.ink_values[1]]);
   g2=RGB2GREEN(colours[GateArray.ink_values[0]]) + RGB2GREEN(colours[GateArray.ink_values[1]]);
   b2=RGB2BLUE(colours[GateArray.ink_values[0]]) + RGB2BLUE(colours[GateArray.ink_values[1]]);
   GateArray.palette[33] = (unsigned short) RGB2COLOR(r2/2, g2/2, b2/2);
}

/**
 * rgb2color_16bpp:
 * convert rgb to 16bpp color (see macros)
 **/
unsigned int rgb2color_16bpp(unsigned int r, unsigned int g, unsigned int b)
{
   return RGB2COLOR(r, g, b);
}

/**
 * convert_color:
 * convert raw color to 16bpp color (see macros)
 **/
static INLINE unsigned int convert_color(unsigned int colour)
{
   pixel.colour = colour;
   return (
      DATA2RED5(pixel.r)
      | DATA2GREEN6(pixel.g)
      | DATA2BLUE5(pixel.b)
   );
}

/**
 * draw_pixel_16bpp:
 * simple method to emulate a transparency, almost slow but is only used on init
 **/
void draw_pixel_16bpp(unsigned int * dest, const unsigned int * img)
{
   uint16_t * buffer_ptr = (uint16_t *) dest;
   uint16_t * img_ptr = (uint16_t *) img;

   if (*(img_ptr) != PIXEL_TRANSPARENT)
   {
      *(buffer_ptr) = *(img_ptr);
   }
   if (*(img_ptr+1) != PIXEL_TRANSPARENT)
   {
      *(buffer_ptr+1) = *(img_ptr+1);
   }
}

/**
 * convert_image_16bpp:
 * convert raw image to 16bpp, used on init
 **/
void convert_image_16bpp(unsigned int * dest, const unsigned int * img, int size)
{
   uint16_t * buffer_ptr = (uint16_t *) dest;
   while (size--)
   {
      #ifndef LOWRES
      *(buffer_ptr++) = convert_color(*img);
      #endif
      *(buffer_ptr++) = convert_color(*(img++));
   }
}

/**
 * draw_line_16bpp:
 * copy a 16bpp color to your dest, optimized
 **/
void draw_line_16bpp(unsigned int * dest, int width, unsigned int colour)
{
   // prepare copy using 32bits
   colour += colour << 16;
   width >>= 1;

   while (width--)
      *(dest++) = colour;
}

/**
 * draw_char_16bpp:
 * draw a 16bpp char to your dest, optimized
 **/
void draw_char_16bpp(unsigned int * dest, const unsigned char *font_data, unsigned int colour)
{
   #ifdef LOWRES
   unsigned short *buffer_ptr = (unsigned short *) dest;
   #else
   unsigned int *buffer_ptr = dest;
   #endif

   int height = FNT_CHAR_HEIGHT;
   while (height--)
   {
      unsigned char data = *font_data; // get the bitmap information for one row
      int loop_width = FNT_CHAR_WIDTH;
      while (loop_width--)
      {
         if (data & 0x80) // is the bit set?
         {
            #ifdef LOWRES
            *(buffer_ptr++) = colour; // draw the character pixel
            #else
            *(buffer_ptr++) = colour + (colour << 16);
            #endif
         }
         else
         {
            buffer_ptr ++;
         }
         data <<= 1; // advance to the next bit
      }
      font_data ++;
      buffer_ptr += CPC_SCREEN_WIDTH - (FNT_CHAR_WIDTH);
   }
}
