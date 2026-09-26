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

void gunstick_reset(void)
{
   memset(light, 0, sizeof(light));
}

#define GUNSTICK_NONE          0xff
#define GUNSTICK_HIT           0xfd
#define GUNSTICK_FIRE_MASK     0x10
#define GUNSTICK_TIMER         4

/**
 * TODO: 
 * I'm using the same code I used on Wii (Wiituka) &
 *  It could be unified with the pointer detection in PHASER code that Colin has done.
 **/

uint32_t _gunstick_get_screen(int x, int y)
{
   if (x < 0 || y < 0 || x >= EMULATION_SCREEN_WIDTH || y >= EMULATION_SCREEN_HEIGHT)
      return 0;
   if (retro_video.depth == DEPTH_16BPP)
      return ((uint16_t *)video_buffer)[y * EMULATION_SCREEN_WIDTH + x];
   if (retro_video.depth == DEPTH_8BPP)
      return ((uint8_t *)video_buffer)[y * EMULATION_SCREEN_WIDTH + x];
   return video_buffer[y * EMULATION_SCREEN_WIDTH + x];
}

/* The sensor responds to brightness, not an exact palette colour.
 * Decode the renderer's packed pixel before comparing with the existing
 * grey sensitivity level, so coloured flashes work in every pixel format. */
static unsigned gunstick_luminance(uint32_t pixel)
{
   unsigned r, g, b;
   if (retro_video.depth == DEPTH_16BPP) {
      r = ((pixel >> 11) & 31) * 255 / 31;
      g = ((pixel >> 5) & 63) * 255 / 63;
      b = (pixel & 31) * 255 / 31;
   } else if (retro_video.depth == DEPTH_8BPP) {
      r = ((pixel >> 5) & 7) * 255 / 7;
      g = ((pixel >> 2) & 7) * 255 / 7;
      b = (pixel & 3) * 255 / 3;
   } else {
      r = (pixel >> 16) & 255;
      g = (pixel >> 8) & 255;
      b = pixel & 255;
   }
   return 299*r + 587*g + 114*b;
}

bool _gunstick_check(unsigned port)
{
   uint32_t gcolor;
   unsigned threshold;

   if (gun[port].state == GUN_XYGET)
   {
      light[port].x = gun[port].x;
      light[port].y = gun[port].y;
      gun[port].state = GUN_SSEND;
   }

   gcolor = _gunstick_get_screen(light[port].x, light[port].y);
   #ifdef DEBUG_GUNSTICK
   printf("gunstick: 0x%X (%u,%u) [0x%X]\n", gcolor, light[port].x, light[port].y, lightgun_cfg.whitecolor);
   #endif

   /* Guillermo Tell uses a light-yellow flash. Keep grey targets (Solo)
    * and allow a black sample followed by a flash (Mike Gunner). */
   threshold = gunstick_luminance(lightgun_cfg.greycolor);
   if (threshold && gunstick_luminance(gcolor) >= threshold)
   {
      gun[port].state = GUN_SLEEP;
      return true;
   }

   return false;
}

void gunstick_emulator_update(void)
{
   unsigned port;
   for (port = 0; port < 2; port++) {
      if (light[port].timer)
         light[port].timer--;
      ev_lightgun(port);
      if (!lightgun_active(port)) {
         light[port].timer = 0;
         continue;
      }
      if (gun[port].pressed && gun[port].x >= 0)
         gun[port].state = GUN_SHOOT;
   }
}

unsigned char gunstick_emulator_IN()
{
   unsigned line = CPC.keyboard_line & 0x0f;
   unsigned port = line == 9 ? 0 : 1;
   unsigned char result;
   if ((line != 9 && line != 6) || !lightgun_active(port))
      return GUNSTICK_NONE;
   result = gun[port].pressed ? (GUNSTICK_NONE & ~GUNSTICK_FIRE_MASK) : GUNSTICK_NONE;
   if (gun[port].x < 0)
      return result;

   if (gun[port].state == GUN_SLEEP)
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
