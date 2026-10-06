/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2023)
 *  - original header -
 *  Pituka - Nintendo Wii/Gamecube Port
 *  (c) Copyright 2008-2009 David Colmenero (aka D_Skywalk)
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
#include "lightgun.h"
#include "retro_gun.h"

extern uint32_t * video_buffer;
extern t_CPC CPC;

typedef struct{
   int x, y;
   unsigned int timer;
} t_light;
static t_light light[2];

#define GUNSTICK_NONE          0xff
#define GUNSTICK_HIT           0xfd
#define GUNSTICK_PREPARE_COLOR 0x0
#define GUNSTICK_FIRE_KEYCODE  0x94
#define GUNSTICK_FIRE_MASK     0x10
#define GUNSTICK_TIMER         4


static unsigned gunstick_luminance(uint32_t pixel)
{
   unsigned r, g, b;

   switch (retro_video.depth) {
      case DEPTH_16BPP:
         r = (pixel >> 11) & 0x1F; r = (r << 3) | (r >> 2);
         g = (pixel >> 5)  & 0x3F; g = (g << 2) | (g >> 4);
         b = pixel         & 0x1F; b = (b << 3) | (b >> 2);
         break;

      case DEPTH_8BPP:
         r = ((pixel >> 5) & 0x07) * 36;
         g = ((pixel >> 2) & 0x07) * 36;
         b = (pixel & 0x03) * 85;
         break;

      default:
         r = (pixel >> 16) & 0xFF;
         g = (pixel >> 8)  & 0xFF;
         b = pixel         & 0xFF;
         break;
   }

   return (299 * r) + (587 * g) + (114 * b);
}

void gunstick_reset(void)
{
   memset(light, 0, sizeof(light));

   /* Guillermo Tell uses a light-yellow flash. Keep grey targets (Solo)
    * and allow a black sample followed by a flash (Mike Gunner). */
   lightgun_cfg.threshold_color = gunstick_luminance(lightgun_cfg.luminance_color);
}

uint32_t _gunstick_get_screen(int x, int y)
{
   if ((unsigned)x >= (unsigned)EMULATION_SCREEN_WIDTH || 
       (unsigned)y >= (unsigned)EMULATION_SCREEN_HEIGHT)
      return 0;

   int index = y * EMULATION_SCREEN_WIDTH + x - 1;

   switch (retro_video.depth) {
      case DEPTH_8BPP:
         return ((uint8_t *)video_buffer)[index];
      case DEPTH_16BPP:
         return ((uint16_t *)video_buffer)[index];
      default:
         return video_buffer[index];
   }
}

bool _gunstick_check(unsigned port)
{
   uint32_t gcolor, luminance;

   if (gun[port].state == GUN_XYGET)
   {
      light[port].x = gun[port].x;
      light[port].y = gun[port].y;
      gun[port].state = GUN_SSEND;
   }

   gcolor = _gunstick_get_screen(light[port].x, light[port].y);
   luminance = gunstick_luminance(gcolor);
   #ifdef DEBUG_GUNSTICK
   printf("gunstick: 0x%X[0x%X] (%u,%u) [0x%X]\n", gcolor, luminance, light[port].x, light[port].y, lightgun_cfg.threshold_color);
   #endif

   if ( luminance > lightgun_cfg.threshold_color)
   {
      gun[port].state = GUN_SLEEP;
      return true;
   }

   return false;
}

void gunstick_emulator_update(void)
{
   for (unsigned port = 0; port < 2; port++) {
      if (light[port].timer)
         light[port].timer--;
      
      ev_lightgun(port);
      
      if (gun[port].pressed && gun[port].x >= 0)
         gun[port].state = GUN_SHOOT;
   }
}

unsigned char gunstick_emulator_IN()
{
   unsigned line = CPC.keyboard_line & 0x0f;
   unsigned port;

   if (line == 9) port = 0;
   else if (line == 6) port = 1;
   else return GUNSTICK_NONE;

   unsigned char result = gun[port].pressed ? (GUNSTICK_NONE & ~GUNSTICK_FIRE_MASK) : GUNSTICK_NONE;

   if (gun[port].x < 0 || gun[port].state == GUN_SLEEP)
      return result;

   // on shoot update current light X/Y
   // some games prove that the light is not always on target.
   if (gun[port].state == GUN_SHOOT)
   {
      light[port].timer = GUNSTICK_TIMER;
      gun[port].state = GUN_XYGET;
   }

   // wait timer finished
   // TODO: after shoot maybe we need another timer (better dettection if user press long)
   if (!light[port].timer)
      gun[port].state = GUN_SLEEP;
   else if (_gunstick_check(port))
      return result & GUNSTICK_HIT;

   // need a fail answer when you have missed or in any other case
   return result;
}

void gunstick_emulator_OUT(){}
void gunstick_emulator_CRTC(){}
