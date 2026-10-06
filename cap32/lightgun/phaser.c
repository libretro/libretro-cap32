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


/* Magnum Phaser Logic
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
#include "retro_gun.h"

extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_VDU VDU;


#define PHASER_NONE              0xff
#define PHASER_SCREEN_SHIFT      4
#define WESTPHASER_FIRE_MASK     0x20  /* Bit 5 down (Joystick Fire 1) */
#define WESTPHASER_HIT           0x01
#define TROJAN_FIRE_MASK         0x10  /* Bit 4 down (Joystick Fire 2) */

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

   unsigned int x = (CPC.scr_pos - CPC.scr_base) << CPC.scr_density;
   unsigned int y = VDU.scrln;
   
   unsigned int address = CRTC.addr + CRTC.char_count + shift;
   
   if (gun[0].x >= x && gun[0].x < x + 16 && gun[0].y >= y && gun[0].y < y + 2)
   {
      CRTC.registers[16] = address >> 8;
      CRTC.registers[17] = address & 0xff;
   }
}

void phaser_emulator_CRTC()
{
   phaser_latch(PHASER_SCREEN_SHIFT);
}

unsigned char phaser_emulator_IN(){ return PHASER_NONE; }

/* The Plus AUX gun has a separate active-low trigger on joystick 1 fire 2.
 * Unlike the expansion-port Magnum, it does not use writes to port FBFE. */
unsigned char trojan_emulator_IN(void)
{
   if ((CPC.keyboard_line & 15) != 9)
      return PHASER_NONE;

   return gun[0].pressed ? (PHASER_NONE & ~TROJAN_FIRE_MASK) : PHASER_NONE;
}

void trojan_emulator_CRTC(void)
{
   phaser_latch(0);
}


/* Loriciel West Phaser - Implementación basada en rastreo del haz de electrones */
unsigned char westphaser_emulator_IN(void)
{
   unsigned char value = PHASER_NONE;
   int x, y;

   /* The West Phaser connects to joystick port 1 (matrix line 9) */
   if ((CPC.keyboard_line & 15) != 9)
      return value;

   /* On Fire we inject Bit 5 low (Joystick Fire 1) */
   if (gun[0].pressed)
      value &= ~WESTPHASER_FIRE_MASK;

   /* If the gun is pointing off-screen, abort */
   if (gun[0].x < 0 || gun[0].y < 0)
      return value;

   /* Exact electron beam tracking (Raster tracking)
    * Get the X coordinate by subtracting the line base from the current memory position,
    * adjusting by screen density. Y is provided directly by VDU.scrln. */
   x = (CPC.scr_pos - CPC.scr_base) << CPC.scr_density;
   y = VDU.scrln;

   /* The Light sensor: Inject Bit 0 low (Joystick Up)
    * The game times the start of the sensor response and then samples the
    * subsequent scanlines. To simulate persistence, keep the signal active
    * for the rest of each visible line AFTER the aim point (x > gun.x and y > gun.y),
    * provided that the screen coordinate has a bright color. */
   if (
      VDU.flag_drawing &&
      y > gun[0].y &&
       x > gun[0].x &&
       x < EMULATION_SCREEN_WIDTH
   )
   {
      uint32_t color = lightgun_get_screen(gun[0].x, y);
      if (lightgun_luminance(color) > lightgun_cfg.threshold_color)
      {
         value &= ~WESTPHASER_HIT;
      }
   }

   return value;
}
