/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2021)
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

extern uint32_t * video_buffer;
extern retro_input_state_t input_state_cb;
extern uint8_t keyboard_matrix[16];
extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_VDU VDU;

#define GUN_FAILED        0xff
#define GUN_HIT           0xfd
#define GUN_HIT_COLOR     0xffffff
#define GUN_PREPARE_COLOR 0x0
#define GUN_FIRE_KEYCODE  0x94
#define GUN_FIRE_MASK     0x10
#define GUN_TIMER         4
#define GUN_SCREEN_SHIFT  4

typedef enum {
   GUNSTICK_SLEEP   = 0,
   GUNSTICK_SHOOT   = 1,
   GUNSTICK_XYGET   = 2,
   GUNSTICK_SSEND   = 3,
   GUNSTICK_PREPARE = 4,
} gun_state;

typedef struct{
  int cursor_x, cursor_y;
  int x, y;
  gun_state state;
  unsigned int timer;
  unsigned int hcolor;
} gunstick;

static gunstick gun = { 0, 0, GUNSTICK_SLEEP, 0, 0 };
static uint32_t cursor_color = 0;

void gunstick_prepare(void)
{
   cursor_color = retro_video.cursor_color;
}

uint32_t _gunstick_get_screen(int x, int y)
{
   return *((video_buffer + x) + (y * retro_video.bps));
}

void gunstick_update(void)
{
   if (gun.timer)
     gun.timer --;

   if ( input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN) )
   {
      gun.cursor_x = 0xfff;
      gun.cursor_y = 0xfff;
      return;
   }

   gun.cursor_x = ((input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X) + 0x7fff) * EMULATION_SCREEN_WIDTH) / 0xfffe;
   gun.cursor_y = ((input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y) + 0x7fff) * EMULATION_SCREEN_HEIGHT) / 0xfffe;

   if (input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER)
      || (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT)))
   {
      keyboard_matrix[GUN_FIRE_KEYCODE >> 4] &= ~GUN_FIRE_MASK;
      gun.state = GUNSTICK_SHOOT;
   } else {
      keyboard_matrix[GUN_FIRE_KEYCODE >> 4] |= GUN_FIRE_MASK;
   }

   printf("gun: %ux%u [s:%u] (t:%u)\n", gun.cursor_x, gun.cursor_y, gun.state, gun.timer);
}

bool _gunstick_check(void)
{
   uint32_t gcolor;

   if(gun.state == GUNSTICK_XYGET)
   {
      gun.x = gun.cursor_x;
      gun.y = gun.cursor_y;
      gun.state = GUNSTICK_SSEND;
      //_gunstick_get_input();
   }

   gcolor = _gunstick_get_screen(gun.x, gun.y);

   if(gcolor == GUN_PREPARE_COLOR)
      gun.state = GUNSTICK_PREPARE;

   if((gun.state != GUNSTICK_PREPARE) && (gcolor == GUN_HIT_COLOR))
   {
      printf("bang! %u\n", gcolor);
      gun.state = GUNSTICK_SLEEP;
      return true;
   }

   return false;
}

unsigned char gunstick_emulator_IN()
{
   //printf("!IN!!\n");
   if(gun.state == GUNSTICK_SLEEP)
      return GUN_FAILED;
    
   if(gun.state == GUNSTICK_SHOOT)
   {
      gun.timer = GUN_TIMER;
      gun.state = GUNSTICK_XYGET;
   }

   if(!gun.timer)
      gun.state = GUNSTICK_SLEEP;
   else if(_gunstick_check())
      return GUN_HIT; //joy arriba phaser fe / gunstick fd

   return GUN_FAILED;
}

void gunstick_draw(void)
{
   draw_char(video_buffer, gun.x, gun.y, FNT_CROSS, cursor_color);
}

void gunstick_void(void)
{}

void gunstick_emulator_OUT()
{}

void gunstick_emulator_CRTC()
{}
