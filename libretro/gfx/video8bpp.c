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

#define VIDEO_8BPP_PALETTE_SIZE 32

extern t_GateArray GateArray;
extern uint32_t colours[32];
extern uint16_t retro_palette[256];
retro_palette_t ui_palette[VIDEO_8BPP_PALETTE_SIZE];
uint8_t palette_last = 1;


static union TPixel pixel;

#define RGB2COLOR(r, g, b)    ((((int)(r) >> 5) << 5) | (((int)(g) >> 5) << 2) | ((int)(b) >> 6))
#define RGB2RED(colour)       (((colour>>5)<<5) & 0xFF)
#define RGB2GREEN(colour)     (((colour>>2)<<5) & 0xFF)
#define RGB2BLUE(colour)      ((colour<<6) & 0xFF)

#define DATA2BLUE5(colour)       (((colour>>3) & 0x1F))
#define DATA2GREEN6(colour)      (((colour>>2) & 0x3F) << 5)
#define DATA2RED5(colour)        (((colour>>3) & 0x1F) << 11)

void video_retro_palette_prepare()
{
   uint8_t index;
   uint16_t color;

   // Create RGB8 to RGB565 table
   for (int i = 0; i < 256; i++)
   {
      index = i;
      color = (((i >> 5) * 31 / 7) << 11) |
                     ((((i & 0x1C) >> 2) * 63 / 7) << 5) |
                     ((i & 0x3) * 31 / 3);

#if defined(RENDER_GSKIT_PS2)
      /* Index correction for PS2 GS */
      int modi = i & 63;
      if ((modi >= 8 && modi < 16) || (modi >= 40 && modi < 48)) {
         index += 8;
      } else if ((modi >= 16 && modi < 24) || (modi >= 48 && modi < 56)) {
         index -= 8;
      }

      // Transfrom from RGB565 to ABGR1555
      color = ((color & 0xF800) >> 11) | ((color & 0x7C0) >> 1)  | ((color & 0x1F) << 10);
#endif

      retro_palette[index] = color;
   }
}

unsigned char video_ui_palette_get(unsigned short colour)
{
   for(int p = 1; p < VIDEO_8BPP_PALETTE_SIZE; p++)
   {
      if (ui_palette[p].colour == colour)
         return ui_palette[p].idx;
   }

   printf("!!! >> video_ui_palette_get ?? 0x%04x\n", colour);
   return 0;
}

void video_ui_palette_8bpp()
{
   memset(ui_palette, 0, sizeof(ui_palette));

   ui_palette[ 1].pal = 0x2104;
   ui_palette[ 2].pal = 0x2965;
   ui_palette[ 3].pal = 0x630c;
   ui_palette[ 4].pal = 0xffff;
   ui_palette[ 5].pal = 0xf004;
   ui_palette[ 6].pal = 0x06a5;
   ui_palette[ 7].pal = 0x39b6;
   ui_palette[ 8].pal = 0x5001;
   ui_palette[ 9].pal = 0x0423;
   ui_palette[10].pal = 0x2103;
   ui_palette[11].pal = 0x8800;
   ui_palette[12].pal = 0x3125;
   ui_palette[13].pal = 0xf38f;
   ui_palette[14].pal = 0x66ce;
   ui_palette[15].pal = 0x39e7;
   ui_palette[16].pal = 0x06a6;
   ui_palette[17].pal = 0xf085;
   ui_palette[18].pal = 0xf147;
   ui_palette[19].pal = 0xe804;
   ui_palette[20].pal = 0x290b;
   ui_palette[21].pal = 0x6336;
   ui_palette[22].pal = 0x0000;
   ui_palette[23].pal = 0x0024;
   ui_palette[24].pal = 0x00db;
   ui_palette[25].pal = 0x0049;
   ui_palette[26].pal = 0x006d;
   ui_palette[27].pal = 0x0040;
   ui_palette[28].pal = 0x00e0;

   ui_palette[ 1].idx = 32;
   ui_palette[ 2].idx = 36;
   ui_palette[ 3].idx = 146;
   ui_palette[ 4].idx = 255;
   ui_palette[ 5].idx = 192;
   ui_palette[ 6].idx = 52;
   ui_palette[ 7].idx = 47;
   ui_palette[ 8].idx = 64;
   ui_palette[ 9].idx = 8;
   ui_palette[10].idx = 72;
   ui_palette[11].idx = 64;
   ui_palette[13].idx = 242;
   ui_palette[14].idx = 25;
   ui_palette[15].idx = 73;
   ui_palette[16].idx = 52; // fix crop keyb_bg
   ui_palette[20].idx = 38;
   ui_palette[21].idx = 51;
   ui_palette[22].idx = 0;
   ui_palette[23].idx = 36;
   ui_palette[25].idx = 110;
   ui_palette[26].idx = 50;
   ui_palette[27].idx = 64; // led off
   ui_palette[28].idx = 192; // led on
}

/**
 * generate antialias values using 8bpp macros
 *
 */
void video_set_palette_antialias_8bpp(void)
{
   uint8_t r2,g2,b2;
   r2=RGB2RED(colours[GateArray.ink_values[0]]) + RGB2RED(colours[GateArray.ink_values[1]]);
   g2=RGB2GREEN(colours[GateArray.ink_values[0]]) + RGB2GREEN(colours[GateArray.ink_values[1]]);
   b2=RGB2BLUE(colours[GateArray.ink_values[0]]) + RGB2BLUE(colours[GateArray.ink_values[1]]);
   GateArray.palette[33] = (unsigned short) RGB2COLOR(r2/2, g2/2, b2/2);
}

/**
 * rgb2color_8bpp:
 * convert rgb to 8bpp color (see macros)
 **/
unsigned int rgb2color_8bpp(unsigned int r, unsigned int g, unsigned int b)
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
   return 
   /*(
      ((pixel.r >> RED_EXPAND) << RED_SHIFT)
      | ((pixel.g >> GREEN_EXPAND) << GREEN_SHIFT)
      | ((pixel.b >> BLUE_EXPAND) << BLUE_SHIFT)
   );*/

   (
      DATA2RED5(pixel.r)
      | DATA2GREEN6(pixel.g)
      | DATA2BLUE5(pixel.b)
   );
}

/**
 * draw_pixel_8bpp:
 * simple method to emulate a transparency, almost slow but is only used on init
 **/
void draw_pixel_8bpp(uint32_t * dest, const uint32_t * img)
{
   uint8_t * buffer_ptr = (uint8_t *) dest;
   uint8_t * img_ptr = (uint8_t *) img;

   if (*(img_ptr) == 0xFF)
   {
      *(buffer_ptr) = *(img_ptr);
   }
   if (*(img_ptr+1) == 0xFF)
   {
      *(buffer_ptr+1) = *(img_ptr+1);
   }
   if (*(img_ptr+2) == 0xFF)
   {
      *(buffer_ptr+2) = *(img_ptr+2);
   }
   if (*(img_ptr+3) == 0xFF)
   {
      *(buffer_ptr+3) = *(img_ptr+3);
   }
}

/**
 * convert_image_8bpp:
 * convert raw image to 8bpp, used on init
 **/
void convert_image_8bpp(uint32_t * dest, const uint32_t * img, int size)
{
   uint8_t * buffer_ptr = (uint8_t *) dest;
   while (size--)
   {
      #ifndef LOWRES
      *(buffer_ptr++) = video_ui_palette_get(convert_color(*img));
      #endif
      *(buffer_ptr++) = video_ui_palette_get(convert_color(*(img++)));
   }
}

/**
 * draw_line_8bpp:
 * copy a 8bpp color to your dest, optimized
 **/
void draw_line_8bpp(uint32_t * dest, int width, unsigned int colour)
{
   // prepare copy using 32bits
   //printf("line: base: 0x%04x [%u]", colour, width);
   uint8_t idx = video_ui_palette_get(colour);
   colour = idx + (idx<<8) + (idx<<16) + (idx<<24) ;
   //printf(" => 0x%02x 0x%08x", idx, colour);
   width >>= 2;
   if (width == 0)
      width = 1;

   //printf(" loop[%u]\n", width);

   while (width--)
      *(dest++) = colour;
}

/**
 * draw_char_8bpp:
 * draw a 8bpp char to your dest, optimized
 **/
void draw_char_8bpp(uint32_t * dest, const unsigned char *font_data, unsigned int colour)
{
   #ifdef LOWRES
   unsigned char *buffer_ptr = (unsigned char *) dest;
   #else
   unsigned short *buffer_ptr = (uint16_t *) dest;
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
            *(buffer_ptr++) = 0xdf; // draw the character pixel
            #else
            *(buffer_ptr++) = 0xffff;
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



#ifndef RENDER_GSKIT_PS2
/**
 * screen_blit_full_8bpp:
 * crop a 8bpp screen to your dest render, optimized
 **/
 __attribute__((optimize("unroll-loops"))) void screen_blit_full_8bpp(uint32_t * video_buffer, uint32_t * dest_buffer)
{
   uint8_t *src_row = (uint8_t *) video_buffer;
   uint16_t *dest_row = (uint16_t *) dest_buffer;
   int size = EMULATION_SCREEN_WIDTH * EMULATION_SCREEN_HEIGHT;

   while(size--)
   {
      *(dest_row++) = retro_palette[*(src_row++)];
   }
}

/**
 * screen_blit_crop_8bpp:
 * crop a 8bpp screen to your dest render, optimized
 **/
__attribute__((optimize("unroll-loops"))) void screen_blit_crop_8bpp(uint32_t * video_buffer, uint32_t * dest_buffer, const uint16_t render_width, uint16_t render_height)
{
   int width;
   int x_max = EMULATION_SCREEN_WIDTH - (EMULATION_CROP * 2);
   int y_max = EMULATION_SCREEN_HEIGHT - (EMULATION_CROP / EMULATION_SCALE);

   uint8_t *src = (uint8_t *) video_buffer;
   uint16_t *dest = (uint16_t *) dest_buffer;

   while(y_max--)
   {
      src += EMULATION_CROP;
      width = x_max;

      do
      {
         *(dest++) = retro_palette[*(src++)];
      } while(--width);

      src += EMULATION_CROP;
   }
}

#else // RENDER_GSKIT_PS2
inline void screen_blit_full_8bpp(uint32_t * video_buffer, uint32_t * _dest_buffer)
{
   retro_video.ps2->coreTexture->Clut = (u32 *) retro_palette; // Even being both `uint32_t` the types are defined as `u32` in the PS2 gsKit
   retro_video.ps2->coreTexture->Mem = (u32 *) video_buffer; // Even being both `uint32_t` the types are defined as `u32` in the PS2 gsKit
}

// TODO
inline void screen_blit_crop_8bpp(uint32_t * video_buffer, uint32_t * dest_buffer, const uint16_t _width, uint16_t _height)
{}

#endif