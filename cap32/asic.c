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


/* ASIC Logic
   David Skywalker - libretro port CPC+ code
   based on the new cpc caprice core by Colin Pitrat - https://github.com/ColinPitrat/caprice32

*/


#include "asic.h"

// TODO: remove log
#ifdef DEBUG_CART
#define ERR(x) fprintf(stderr, x "\n")
#define LOG(x, val) printf(x "\n", val)
#else
#define ERR(x)
#define LOG(x, val)
#endif


uint8_t *pbRegisterPage;
bool asic_locked = true;

void asic_poke_lock_sequence(uint8_t val) {
   static const uint8_t lockSeq[] = { 0x00, 0xff, 0x77, 0xb3, 0x51, 0xa8, 0xd4, 0x62, 0x39, 0x9c, 0x46, 0x2b, 0x15, 0x8a, 0xcd };
   static int lockPos = -1;
   // Lock sequence can only start after a non zero value
   if (lockPos == -1) {
      if (val > 0) {
         lockPos = 0;
      }
   } else {
      if (val == lockSeq[lockPos]) {
         LOG("ASIC: Received %u", (int) val);
         lockPos++;
         if (lockPos == sizeof(lockSeq)/sizeof(lockSeq[0])) {
            LOG("ASIC unlocked %u", lockPos);
            asic_locked = false;
            lockPos = 0;
         }
      } else {
         // If the lock sequence is matched except for the last byte, it means lock
         if (lockPos == sizeof(lockSeq)/sizeof(lockSeq[0])) {
            LOG("ASIC locked!!! %u", lockPos);
            asic_locked = true;
         }
         lockPos = 0;
      }
   }
}

void asic_register_page_write(uint16_t addr, uint8_t val) {
   #ifdef DEBUG_ASIC
   if (addr >= 0x4000 && addr < 0x5000) {
      LOG("Received sprite data %x", addr);
   } else if (addr >= 0x6000 && addr < 0x607D) {
      LOG("Received sprite operation %x", addr);
   } else if (addr >= 0x6400 && addr < 0x6440) {
      LOG("Received color operation %x", addr);
   } else if (addr >= 0x6800 && addr < 0x6806) {
      printf("Received raster interrupt stuff: %x - val: %u\n" , addr , (int) val);
   } else if (addr >= 0x6808 && addr < 0x6810) {
      LOG("Received analog input stuff %x", addr);
   } else if (addr >= 0x6C00 && addr < 0x6C10) {
      LOG("Received DMA stuff %x", addr);
   } else {
      printf("Received unused write at %x - val: %u\n", addr, (int) val);
   }
   #endif
}
