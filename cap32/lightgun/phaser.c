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

extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_VDU VDU;

// libretro update lightgun position
void ev_lightgun();

#define PHASER_SCREEN_SHIFT 4

void phaser_emulator_update(void)
{
   ev_lightgun();

   if(gun.pressed)
   {
      gun.state = GUN_SHOOT;
   } else {
      gun.state = GUN_PREPARE;
   }

}

// When the phazer is not pressed, the CRTC is constantly refreshing R16 & R17:
// https://www.cpcwiki.eu/index.php/Amstrad_Magnum_Phaser
void phaser_emulator_OUT()
{
   if (gun.state != GUN_PREPARE)
      return;

   CRTC.registers[17] += 1;
}

void phaser_emulator_CRTC()
{
   // If the trigger is pressed, it only updates it when the phazer receives light from the screen.
   if (gun.state != GUN_SHOOT)
      return;

   unsigned int x = CPC.scr_pos - CPC.scr_base;
   unsigned int y = VDU.scrln;

   unsigned int address = CRTC.addr + CRTC.char_count + PHASER_SCREEN_SHIFT;

   if (gun.x >= x && gun.x < x + 16 && gun.y >= y && gun.y < y + 2)
   {
      CRTC.registers[16] = address >> 8;
      CRTC.registers[17] = address & 0xff;
   }
}

unsigned char phaser_emulator_IN(){ return 0xff; }
