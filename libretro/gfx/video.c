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

/**
 * video_setup:
 * setup video on core boot
 **/
void video_setup(retro_video_depth_t video_depth)
{
    switch (video_depth)
    {
    case DEPTH_8BPP:
        break;

    case DEPTH_24BPP:
        retro_video.bytes = 2;
        retro_video.pitch = 4;
        retro_video.raw_density_byte = 0;
        retro_video.rgb2color = rgb2color_24bpp;
        retro_video.convert_image = convert_image_24bpp;
        retro_video.draw_line = draw_line_24bpp;
        retro_video.draw_char = draw_char_24bpp;
        retro_video.draw_pixel = draw_pixel_24bpp;
        retro_video.cursor_color = 0xCCCCCC;
        retro_video.blend_mask = 0x10101;
        retro_video.fmt = RETRO_PIXEL_FORMAT_XRGB8888;
        break;

    case DEPTH_16BPP:
        retro_video.bytes = 1;
        retro_video.pitch = 2;
        retro_video.raw_density_byte = 1;
        retro_video.rgb2color = rgb2color_16bpp;
        retro_video.convert_image = convert_image_16bpp;
        retro_video.draw_line = draw_line_16bpp;
        retro_video.draw_char = draw_char_16bpp;
        retro_video.draw_pixel = draw_pixel_16bpp;
        retro_video.cursor_color = 0xCE79;
        retro_video.blend_mask = 0x08210821;
        retro_video.fmt = RETRO_PIXEL_FORMAT_RGB565;
        break;
    }

    // cached values
    retro_video.bps = (EMULATION_SCREEN_WIDTH) >> retro_video.raw_density_byte;
    retro_video.char_size = (FNT_CHAR_WIDTH >> retro_video.raw_density_byte) * EMULATION_SCALE;
    retro_video.depth = video_depth;

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
}
