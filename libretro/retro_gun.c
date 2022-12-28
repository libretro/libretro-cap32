/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2023)
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
#include "gfx/software.h"
#include "gfx/video.h"
#include "assets/assets.h"
#include "cap32.h"

#include "lightgun/lightgun.h"
#include "lightgun/gunstick.h"
#include "lightgun/phaser.h"
#include "retro_gun.h"

extern uint32_t * video_buffer;
extern retro_input_state_t input_state_cb;
extern retro_log_printf_t log_cb;

extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_VDU VDU;
extern uint32_t colours[32];

t_lightgun_cfg lightgun_cfg;

static uint32_t cursor_color = 0;

void lightgun_void(void){}
unsigned char lightgun_void_IN(void){ return 0xff; }

void lightgun_prepare(lightgun_type guntype)
{
   cursor_color = retro_video.cursor_color;

   switch (guntype)
   {
      case LIGHTGUN_TYPE_GUNSTICK:
         lightgun_cfg.gunconfigured = guntype;
         lightgun_cfg.gun_update = gunstick_emulator_update;
         CPC.gun_CRTC = gunstick_emulator_CRTC;
         CPC.gun_IN = gunstick_emulator_IN;
         CPC.gun_OUT = gunstick_emulator_OUT;
         break;

      case LIGHTGUN_TYPE_PHASER:
         lightgun_cfg.gunconfigured = guntype;
         lightgun_cfg.gun_update = phaser_emulator_update;
         CPC.gun_CRTC = phaser_emulator_CRTC;
         CPC.gun_IN = phaser_emulator_IN;
         CPC.gun_OUT = phaser_emulator_OUT;
         break;

      default:
         lightgun_cfg.gun_draw = lightgun_void;
         lightgun_cfg.gun_update = lightgun_void;
         CPC.gun_CRTC = lightgun_void;
         CPC.gun_IN = lightgun_void_IN;
         CPC.gun_OUT = lightgun_void;
         break;
   }

   if (lightgun_cfg.show && guntype != LIGHTGUN_TYPE_NONE)
   {
      lightgun_cfg.gun_draw = lightgun_draw;
   }

   switch (retro_video.depth)
   {
      case DEPTH_24BPP:
         lightgun_cfg.whitecolor = colours[11];
         break;
      
      case DEPTH_16BPP:
         lightgun_cfg.whitecolor = colours[11] + (colours[11] << 16);
         break;

      default:
         printf("[lightgun_prepare]: error invalid depth.\n");
         break;
   }

   LOGI("lightgun_prepare => %d \n", lightgun_cfg.gunconfigured);
}

void lightgun_draw(void)
{
   // check margins
   if (gun.x < 16 || gun.y < 16)
      return;

   draw_char(video_buffer, gun.x - (4 * EMULATION_SCALE), gun.y - 4, FNT_CROSS, cursor_color);
}
