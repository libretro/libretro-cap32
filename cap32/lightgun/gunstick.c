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
extern uint8_t keyboard_matrix[16];
extern t_CPC CPC;

typedef struct{
   int x, y;
   unsigned int timer;
} t_light;
t_light light = {0,0,0};

void ev_lightgun();

#define GUNSTICK_NONE          0xff
#define GUNSTICK_HIT           0xfd
#define GUNSTICK_PREPARE_COLOR 0x0
#define GUNSTICK_FIRE_KEYCODE  0x94
#define GUNSTICK_FIRE_MASK     0x10
#define GUNSTICK_TIMER         4

/**
 * TODO: 
 * I'm using the same code I used on Wii (Wiituka) &
 *  It could be unified with the pointer detection in PHASER code that Colin has done.
 **/

uint32_t _gunstick_get_screen(int x, int y)
{
   return *(
      (video_buffer + (x >> retro_video.raw_density_byte)) +
      (y * retro_video.bps)
   );
}

bool _gunstick_check(void)
{
   uint32_t gcolor;

   if (gun.state == GUN_XYGET)
   {
      light.x = gun.x;
      light.y = gun.y;
      gun.state = GUN_SSEND;
   }

   gcolor = _gunstick_get_screen(light.x, light.y);
   #ifdef DEBUG_GUNSTICK
   printf("gunstick: 0x%X (%u,%u) [0x%X]\n", gcolor, light.x, light.y, lightgun_cfg.whitecolor);
   #endif

   if (gcolor == GUNSTICK_PREPARE_COLOR)
      gun.state = GUN_PREPARE;

   if ((gun.state != GUN_PREPARE) && (gcolor == lightgun_cfg.whitecolor))
   {
      gun.state = GUN_SLEEP;
      return true;
   }

   return false;
}

void gunstick_emulator_update(void)
{
   if (light.timer)
     light.timer --;

   ev_lightgun();

   // send joy fire on gunstick pressed and update state
   if (gun.pressed)
   {
      keyboard_matrix[GUNSTICK_FIRE_KEYCODE >> 4] &= ~GUNSTICK_FIRE_MASK;
      gun.state = GUN_SHOOT;
   } else {
      keyboard_matrix[GUNSTICK_FIRE_KEYCODE >> 4] |= GUNSTICK_FIRE_MASK;
   }
}

unsigned char gunstick_emulator_IN()
{
   if (gun.state == GUN_SLEEP)
      return GUNSTICK_NONE;

   // on shoot update current light X/Y
   // some games prove that the light is not always on target.
   if (gun.state == GUN_SHOOT)
   {
      light.timer = GUNSTICK_TIMER;
      gun.state = GUN_XYGET;
   }

   // wait timer finished
   // TODO: after shoot maybe we need another timer (better dettection if user press long)
   if (!light.timer)
      gun.state = GUN_SLEEP;
   else if (_gunstick_check())
      return GUNSTICK_HIT;

   // need a fail answer when you have missed or in any other case
   return GUNSTICK_NONE;
}

void gunstick_emulator_OUT(){}
void gunstick_emulator_CRTC(){}
