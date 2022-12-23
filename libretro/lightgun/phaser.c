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
extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_VDU VDU;

#define GUN_FAILED        0xff
#define GUN_HIT           0xfd
#define GUN_TIMER         500
#define GUN_SCREEN_SHIFT  4

typedef enum {
   GUNSTICK_SLEEP   = 0,
   GUNSTICK_SHOOT   = 1,
   GUNSTICK_XYGET   = 2,
   GUNSTICK_SSEND   = 3,
   GUNSTICK_PREPARE = 4,
} gun_state;

typedef struct{
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

void gunstick_update(void)
{
   if ( input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN) )
   {
      gun.state = GUNSTICK_SLEEP;
      gun.x = 0xfff;
      gun.y = 0xfff;
      return;
   }

   gun.x = ((input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X) + 0x7fff) * EMULATION_SCREEN_WIDTH) / 0xfffe;
   gun.y = ((input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y) + 0x7fff) * EMULATION_SCREEN_HEIGHT) / 0xfffe;

   if (input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER)
      || (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT)))
   {
      gun.state = GUNSTICK_SHOOT;
   } else {
      gun.state = GUNSTICK_PREPARE;
   }

   printf("gun: %ux%u [%u]\n", gun.x, gun.y, gun.state);
}

// When the phazer is not pressed, the CRTC is constantly refreshing R16 & R17:
// https://www.cpcwiki.eu/index.php/Amstrad_Magnum_Phaser
void gunstick_emulator_OUT()
{
   if (gun.state == GUNSTICK_SLEEP)
      return;

   if (gun.state != GUNSTICK_SHOOT)
      CRTC.registers[17] += 1;
}

void gunstick_emulator_CRTC()
{
   // If the trigger is pressed, it only updates it when the phazer receives light from the screen.
   if (gun.state != GUNSTICK_SHOOT)
      return;

   unsigned int x = ((CPC.scr_pos - CPC.scr_base) * 8) / retro_video.pitch;
   unsigned int y = VDU.scrln * retro_video.bps;

   unsigned int address = CRTC.addr + CRTC.char_count + GUN_SCREEN_SHIFT;

   if (gun.x >= x && gun.x < x + 16 && gun.y >= y && gun.y < y + 2)
   {
      CRTC.registers[16] = address >> 8;
      CRTC.registers[17] = address & 0xFF;
      printf("bang: %ux%u <=> %ux%u\n", gun.x, gun.y, x, y);
   }
}


/*
unsigned char gunstick_emulator_PPI()
{
   if(gun.state == GUNSTICK_SLEEP)
      return GUN_FAILED; //nada
    
   if(gun.state == GUNSTICK_SHOOT)
   {
      gun.timer = GetTicks(); //cogemos el timer
      gun.state = GUNSTICK_XYGET;
   }
   else if((GetTicks() - gun.timer) > GUN_TIMER) //si han pasado...
      gun.state = GUNSTICK_SLEEP;
   else if(_gunstick_checkHit())
      return GUN_HIT; //joy arriba phaser fe / gunstick fd

   return GUN_FAILED; //nada
}
*/

void gunstick_draw(void)
{
   draw_char(video_buffer, gun.x, gun.y, FNT_CROSS, cursor_color);
}

void gunstick_void(void)
{}