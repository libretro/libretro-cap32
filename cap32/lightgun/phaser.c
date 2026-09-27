/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


/* PHASER Logic
   David Skywalker - libretro port Phaser code
   based on the new cpc caprice core by Colin Pitrat - https://github.com/ColinPitrat/caprice32
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>
#include <libretro-core.h>

#include "cap32.h"
#include "lightgun.h"
#include "gunstick.h"
#include "retro_gun.h"

extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_VDU VDU;

#define PHASER_SCREEN_SHIFT 4

void phaser_emulator_update(void)
{
   ev_lightgun(0);

   if(gun[0].pressed)
   {
      gun[0].state = GUN_SHOOT;
   } else {
      gun[0].state = GUN_PREPARE;
   }

}

// When the phazer is not pressed, the CRTC is constantly refreshing R16 & R17:
// https://www.cpcwiki.eu/index.php/Amstrad_Magnum_Phaser
void phaser_emulator_OUT()
{
   if (gun[0].state != GUN_PREPARE)
      return;

   CRTC.registers[17] += 1;
}

static void phaser_latch(unsigned shift)
{
   // If the trigger is pressed, it only updates it when the phazer receives light from the screen.
   if (gun[0].state != GUN_SHOOT)
      return;

   /* scr_pos is uint32_t *, even for 8/16-bit framebuffers. */
   unsigned int x = ((uint8_t *)CPC.scr_pos - (uint8_t *)CPC.scr_base)
      / (CPC.scr_bpp / 8);
   unsigned int y = VDU.scrln;

   unsigned int address = CRTC.addr + CRTC.char_count + shift;

   if (gun[0].x >= x && gun[0].x < x + 16 && gun[0].y >= y && gun[0].y < y + 2)
   {
      CRTC.registers[16] = address >> 8;
      CRTC.registers[17] = address & 0xff;
   }
}

unsigned char phaser_emulator_IN(){ return 0xff; }

/* The Plus AUX gun has a separate active-low trigger on joystick 1 fire 2.
 * Unlike the expansion-port Magnum, it does not use writes to port FBFE. */
unsigned char trojan_emulator_IN(void)
{
   if (CPC.model != CPC_MODEL_PLUS || (CPC.keyboard_line & 15) != 9 ||
       !lightgun_active(0))
      return 0xff;
   return gun[0].pressed ? 0xef : 0xff;
}

void phaser_emulator_CRTC(void)
{
   phaser_latch(PHASER_SCREEN_SHIFT);
}

void trojan_emulator_CRTC(void)
{
   if (CPC.model == CPC_MODEL_PLUS && lightgun_active(0))
      phaser_latch(0);
}

/* Loriciel connects the trigger to joystick fire 1 and the optical sensor
 * to joystick up. The sensor remains available with the trigger released.
 * See the West Phaser circuit documented by Jose Leandro on CPCWiki. */
unsigned char westphaser_emulator_IN(void)
{
   unsigned char value = 0xff;
   int x, y;
   if ((CPC.keyboard_line & 15) != 9 || !lightgun_active(0))
      return value;
   if (gun[0].pressed)
      value &= ~0x20;
   if (gun[0].x < 0 || gun[0].y < 0 || CPC.scr_bpp < 8)
      return value;
   x = ((uint8_t *)CPC.scr_pos - (uint8_t *)CPC.scr_base) / (CPC.scr_bpp / 8);
   y = VDU.scrln;
   /* West software times the start of the sensor response, then samples
    * subsequent scanlines. Keep the response for the rest of each visible
    * line after the aim point; blanking and black pixels produce no light.
    * Read the current scanline: games change the palette during acquisition. */
   if (VDU.flag_drawing && y > gun[0].y &&
       x > gun[0].x && x < EMULATION_SCREEN_WIDTH &&
       _gunstick_get_screen(gun[0].x, y))
      value &= ~0x01;
   return value;
}
