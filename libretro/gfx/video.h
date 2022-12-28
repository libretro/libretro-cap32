
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
   uint32_t bps;
   uint32_t cursor_color;
   uint32_t char_size;
   uint32_t blend_mask;
   void (*video_set_palette_antialias)(void);
   unsigned int (*rgb2color)(unsigned int r, unsigned int g, unsigned int b);
   void (*convert_image)(unsigned int * dest, const unsigned int * img, int size);
   void (*draw_line)(unsigned int * dest, int width, unsigned int color);
   void (*draw_char)(unsigned int * dest, const unsigned char *font_data, unsigned int color);
   void (*draw_pixel)(unsigned int * dest, const unsigned int * img);
   void (*draw_keyboard_func)(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size);
} retro_video_t;
extern retro_video_t retro_video;

void video_setup(retro_video_depth_t video_depth);

void video_set_palette_antialias_16bpp(void);
void video_set_palette_antialias_24bpp(void);

unsigned int rgb2color_16bpp(unsigned int r, unsigned int g, unsigned int b);
unsigned int rgb2color_24bpp(unsigned int r, unsigned int g, unsigned int b);

void convert_image_16bpp(unsigned int * dest, const unsigned int * img, int size);
void convert_image_24bpp(unsigned int * dest, const unsigned int * img, int size);

void draw_line_16bpp(unsigned int * dest, int width, unsigned int color);
void draw_line_24bpp(unsigned int * dest, int width, unsigned int color);

void draw_char_16bpp(unsigned int * dest, const unsigned char *font_data, unsigned int color);
void draw_char_24bpp(unsigned int * dest, const unsigned char *font_data, unsigned int color);

void draw_pixel_16bpp(unsigned int * dest, const unsigned int * img);
void draw_pixel_24bpp(unsigned int * dest, const unsigned int * img);

#endif