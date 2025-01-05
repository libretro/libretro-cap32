
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

#ifndef GFX_VIDEO_H__
#define GFX_VIDEO_H__

#ifdef RENDER_GSKIT_PS2
#include "libretro-common/include/libretro_gskit_ps2.h"
#endif

typedef enum
{
   DEPTH_8BPP  = 1,
   DEPTH_16BPP = 2,
   DEPTH_24BPP = 3,
} retro_video_depth_t;

typedef struct {
   retro_video_depth_t depth;
   uint8_t bytes;
   uint8_t pitch;
   uint8_t fmt;
   uint8_t raw_density_byte;
   uint16_t scr_off;
   uint32_t bps;
   uint32_t cursor_color;
   uint32_t char_size;
   uint32_t blend_mask;

   // emulation screen/blitter
   uint16_t vertical_hold;
   uint16_t screen_crop;
   uint16_t screen_render_width;
   uint16_t screen_render_height;

   void (*video_set_palette_antialias)(void);
   unsigned int (*rgb2color)(unsigned int r, unsigned int g, unsigned int b);
   void (*convert_image)(uint32_t * dest, const uint32_t * img, int size);
   void (*draw_line)(uint32_t * dest, int width, unsigned int color);
   void (*draw_char)(uint32_t * dest, const unsigned char *font_data, unsigned int color);
   void (*draw_pixel)(uint32_t * dest, const uint32_t * img);
   void (*draw_keyboard_func)(uint32_t * buffer, const uint32_t * img, int x, int y, unsigned int size);
   void (*draw_screen)(void);

   void (*screen_blit)(uint32_t * video_buffer, uint32_t * dest_buffer, uint16_t render_width, uint16_t render_height);

   #ifdef RENDER_GSKIT_PS2
   RETRO_HW_RENDER_INTEFACE_GSKIT_PS2* ps2;
   #endif

} retro_video_t;
extern retro_video_t retro_video;

typedef union{
struct {
   uint16_t colour;
   uint16_t idx;
   };
   uint32_t pal;
} retro_palette_t;

void video_setup(retro_video_depth_t video_depth);
void video_ui_palette_8bpp(void);
void video_retro_palette_prepare(void);

void video_set_palette_antialias_8bpp(void);
void video_set_palette_antialias_16bpp(void);
void video_set_palette_antialias_24bpp(void);

unsigned int rgb2color_8bpp(unsigned int r, unsigned int g, unsigned int b);
unsigned int rgb2color_16bpp(unsigned int r, unsigned int g, unsigned int b);
unsigned int rgb2color_24bpp(unsigned int r, unsigned int g, unsigned int b);

void convert_image_8bpp(uint32_t * dest, const uint32_t * img, int size);
void convert_image_16bpp(uint32_t * dest, const uint32_t * img, int size);
void convert_image_24bpp(uint32_t * dest, const uint32_t * img, int size);

void draw_line_8bpp(uint32_t * dest, int width, unsigned int color);
void draw_line_16bpp(uint32_t * dest, int width, unsigned int color);
void draw_line_24bpp(uint32_t * dest, int width, unsigned int color);

void draw_char_8bpp(uint32_t * dest, const unsigned char *font_data, unsigned int color);
void draw_char_16bpp(uint32_t * dest, const unsigned char *font_data, unsigned int color);
void draw_char_24bpp(uint32_t * dest, const unsigned char *font_data, unsigned int color);

void draw_pixel_8bpp(uint32_t * dest, const uint32_t * img);
void draw_pixel_16bpp(uint32_t * dest, const uint32_t * img);
void draw_pixel_24bpp(uint32_t * dest, const uint32_t * img);

void screen_blit_crop_8bpp(uint32_t * video_buffer, uint32_t * dest_buffer, uint16_t render_width, uint16_t render_height);
void screen_blit_full_8bpp(uint32_t * video_buffer, uint32_t * dest_buffer, uint16_t render_width, uint16_t render_height);
void screen_blit_crop(uint32_t * video_buffer, uint32_t * dest_buffer, uint16_t render_width, uint16_t render_height);

#if defined (FRONTEND_SUPPORTS_ABGR1555)
#define EXPAND_RED(c) ((c << 2) | (c >> 1)) // From 3 to 5 bits
#define EXPAND_GREEN(c) ((c << 2) | (c >> 1)) // From 3 to 5 bits
#define EXPAND_BLUE(c) ((c << 3) | (c << 1) | (c >> 1)) // From 2 to 5 bits

#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 10

#else // RGB565

#define EXPAND_RED(c) ((c << 2) | (c >> 1)) // From 3 to 5 bits
#define EXPAND_GREEN(c) ((c << 3) | (c << 1) | (c >> 1)) // From 3 to 6 bits
#define EXPAND_BLUE(c) ((c << 3) | (c << 1) | (c >> 1)) // From 2 to 5 bits

#define RED_SHIFT 11
#define GREEN_SHIFT 5
#define BLUE_SHIFT 0

#endif

#if defined (RENDER_GSKIT_PS2)
void init_ps2_hw_render(uint32_t * video_buffer, uint32_t * dest_buffer, uint16_t width, uint16_t height);

// Index color CLUT config RRRGGGBB
#define EXTRACT_RED(i) (((i) >> 5) & 0x111)
#define EXTRACT_GREEN(i) (((i) >> 2) & 0x111)
#define EXTRACT_BLUE(i) (((i) >> 0) & 0x11)

#define BUILD_CLUT_COLOR_FOR_INDEX(i) \
   (EXPAND_RED(EXTRACT_RED(i)) << RED_SHIFT) | \
   (EXPAND_GREEN(EXTRACT_GREEN(i)) << GREEN_SHIFT) | \
   (EXPAND_BLUE(EXTRACT_BLUE(i)) << BLUE_SHIFT)
#endif // RENDER_GSKIT_PS2

#endif // GFX_VIDEO_H__