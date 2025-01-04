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

#include "libretro.h"
#include "libretro-core.h"
#include "video.h"
#include "software.h"
#include "assets/assets.h"
#include "cap32.h"
#include "crtc.h"

extern t_VDU VDU;
#ifdef RENDER_GSKIT_PS2
extern retro_environment_t environ_cb;
extern retro_log_printf_t log_cb;
#endif

/**
 * video_setup:
 * setup video on core boot
 **/
void video_setup(retro_video_depth_t video_depth)
{
    // crop screen borders
    retro_video.vertical_hold = retro_video.screen_crop
        ? MIN_VHOLD_CROP
        : MIN_VHOLD;
    retro_video.screen_render_width = retro_video.screen_crop
        ? EMULATION_SCREEN_WIDTH - (64 * EMULATION_SCALE)
        : EMULATION_SCREEN_WIDTH;
    retro_video.screen_render_height = retro_video.screen_crop
        ? CPC_SCREEN_HEIGHT - 32
        : CPC_SCREEN_HEIGHT;

    // per video depth configuration
    switch (video_depth)
    {
    case DEPTH_8BPP:
        retro_video.bytes = 1;
        retro_video.pitch = 1;
        retro_video.raw_density_byte = 2;
        retro_video.scr_off = 4;
        retro_video.rgb2color = rgb2color_8bpp;
        retro_video.convert_image = convert_image_8bpp;
        retro_video.draw_line = draw_line_8bpp;
        retro_video.draw_char = draw_char_8bpp;
        retro_video.draw_pixel = draw_pixel_8bpp;
        retro_video.screen_blit_crop = screen_blit_crop_8bpp;
        retro_video.screen_blit_full = screen_blit_full_8bpp;
        retro_video.cursor_color = 0xFFFF;
        retro_video.blend_mask = 0x08210821;
        retro_video.fmt = RETRO_PIXEL_FORMAT_RGB565;
        retro_video.char_size = 2;

        #ifdef RENDER_GSKIT_PS2
        if (!environ_cb(RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE, (void **)&retro_video.ps2) || !retro_video.ps2) {
            LOGE(" Failed to get HW rendering interface!\n");
            return;
        }

        if (retro_video.ps2->interface_version != RETRO_HW_RENDER_INTERFACE_GSKIT_PS2_VERSION) {
            LOGE(" HW render interface mismatch, expected %u, got %u!\n",
                    RETRO_HW_RENDER_INTERFACE_GSKIT_PS2_VERSION, retro_video.ps2->interface_version);
            return;
        }

        retro_video.ps2->coreTexture->Width = retro_video.screen_render_width;
        retro_video.ps2->coreTexture->Height = retro_video.screen_render_height;
        retro_video.ps2->coreTexture->PSM = GS_PSM_T8;
        retro_video.ps2->coreTexture->ClutPSM = GS_PSM_CT16;
        retro_video.ps2->coreTexture->Filter = GS_FILTER_LINEAR;
        retro_video.ps2->padding = (struct retro_hw_ps2_insets){ 0.0f, 0.0f, 0.0f, 0.0f};
        #endif

        video_ui_palette_8bpp();
        break;

    case DEPTH_24BPP:
        retro_video.bytes = 2;
        retro_video.pitch = 4;
        retro_video.raw_density_byte = 0;
        retro_video.scr_off = 1;
        retro_video.rgb2color = rgb2color_24bpp;
        retro_video.convert_image = convert_image_24bpp;
        retro_video.draw_line = draw_line_24bpp;
        retro_video.draw_char = draw_char_24bpp;
        retro_video.draw_pixel = draw_pixel_24bpp;
        retro_video.screen_blit_crop = screen_blit_crop;
        retro_video.screen_blit_full = screen_blit_full;
        retro_video.cursor_color = 0xCCCCCC;
        retro_video.blend_mask = 0x10101;
        retro_video.fmt = RETRO_PIXEL_FORMAT_XRGB8888;
        retro_video.char_size = (FNT_CHAR_WIDTH >> retro_video.raw_density_byte) * EMULATION_SCALE;
        break;

    case DEPTH_16BPP:
        retro_video.bytes = 1;
        retro_video.pitch = 2;
        retro_video.raw_density_byte = 1;
        retro_video.scr_off = 2;
        retro_video.rgb2color = rgb2color_16bpp;
        retro_video.convert_image = convert_image_16bpp;
        retro_video.draw_line = draw_line_16bpp;
        retro_video.draw_char = draw_char_16bpp;
        retro_video.draw_pixel = draw_pixel_16bpp;
        retro_video.screen_blit_crop = screen_blit_crop;
        retro_video.screen_blit_full = screen_blit_full;
        retro_video.cursor_color = 0xCE79;
        retro_video.blend_mask = 0x08210821;
        retro_video.fmt = RETRO_PIXEL_FORMAT_RGB565;
        retro_video.char_size = (FNT_CHAR_WIDTH >> retro_video.raw_density_byte) * EMULATION_SCALE;
        break;
    }

    // cached values
    retro_video.bps = (EMULATION_SCREEN_WIDTH) >> retro_video.raw_density_byte;
    retro_video.depth = video_depth;

   // by default no blending
   retro_video.draw_keyboard_func = draw_image_linear;
}

/**
 * screen_blit_crop:
 * Blits video when screen is cropped. Works for 16/24bits.
 **/
void screen_blit_crop(uint32_t * src, uint32_t * dest, const uint16_t render_width, uint16_t render_height)
{
   int width;
   int x_max = render_width >> retro_video.raw_density_byte;

   while(render_height--)
   {
      src += EMULATION_CROP >> retro_video.raw_density_byte;
      width = x_max;

      do
      {
         *(dest++) = *(src++);
      } while(--width);

      src += EMULATION_CROP >> retro_video.raw_density_byte;
   }
}

void screen_blit_full(uint32_t * _src, uint32_t * _dest)
{}
