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

// About unlock sequence ...
//
// testplus.cpr unlock with:
// AA 00 FF 77 B3 51 A8 D4 62 39 9C 46 2B 15 8A CD EE FF
// and lock with:
// AA 00 FF 77 B3 51 A8 D4 62 39 9C 46 2B 15 8A CD EE A5
//
// But Arnold5 specs says the unlock sequence ends at CD
// http://www.cpcwiki.eu/index.php/Arnold_V_Specs_Revised#Locking_of_enhanced_features
// and older specs says the unlock sequence ends at EE
// http://www.cpcwiki.eu/index.php/Arnold_V_specs#Locking_of_enhanced_features
// In all cases, the lock sequence is the same with the last byte having any value except the one it has in the unlock sequence
// All cartridges tested unlock with:
// FF 00 FF 77 B3 51 A8 D4 62 39 9C 46 2B 15 8A CD EE
// Klax alternates between:
// FF 00 FF 77 B3 51 A8 D4 62 39 9C 46 2B 15 8A CD EE
// And:
// FF 00 FF 77 B3 51 A8 D4 62 39 9C 46 2B 15 8A CD FF
// But making the latter unlock the asic make the game bug (loop on welcome screen)


#include "asic.h"
#include "crtc.h"

#include "libretro-core.h"

// TODO: remove log
#ifdef DEBUG_CART
#define ERR(x) fprintf(stderr, x "\n")
#define LOG(x, val) printf(x "\n", val)
#else
#define ERR(x)
#define LOG(x, val)
#endif

extern double colours_rgb[32][3];
extern uint32_t colours[32];

extern t_GateArray GateArray;
extern t_CRTC CRTC;
extern t_CPC CPC;
extern t_PSG PSG;
extern uint32_t dwXScale;
extern uint8_t *membank_config[8][4];
extern uint8_t *membank_write[4];

uint8_t *pbRegisterPage;

t_asic asic;
double asic_colours[32][3];

void asic_reset(){
   memset(&asic, 0, sizeof(asic));

   asic.locked = true;
   asic.raster_interrupt = false;
   asic.interrupt_vector = 1;
}

void asic_poke_lock_sequence(uint8_t val) {
   static const uint8_t lockSeq[] = { 0x00, 0x00, 0xff, 0x77, 0xb3, 0x51, 0xa8, 0xd4, 0x62, 0x39, 0x9c, 0x46, 0x2b, 0x15, 0x8a, 0xcd };
   static const int lockSeqLength = sizeof(lockSeq)/sizeof(lockSeq[0]);
   static int lockPos = 0;
   // Lock sequence can only start after a non zero value
   if (lockPos == 0) {
      if (val > 0) {
         lockPos = 1;
      }
   } else {
      if(lockPos < lockSeqLength) {
         if (val == lockSeq[lockPos]) {
            lockPos++;
         } else {
            lockPos++;
            // If the lock sequence is matched except for the last byte, it means lock
            if (lockPos == lockSeqLength) {
               LOG("ASIC locked: %u", lockPos);
               asic.locked = true;
            }
            if (val == 0) {
               if (lockPos == 3) {
                  // We had two 0 in a row, we still need a full sequence
                  lockPos = 0;
               } else {
                  // We had a non 0 then a 0, we're now waiting for 0xFF
                  lockPos = 2;
               }
            } else {
               // We had a non 0, we're now waiting for 0
               lockPos = 1;
            }
         }
      } else {
         // Full sequence matched and an additional value was written, it means unlock
         if (lockPos == lockSeqLength) {
            LOG("ASIC unlocked!! (%u)", lockPos);
            asic.locked = false;
            lockPos = (val == 0) ? -1 : 0;
         }
      }
   }
}

static INLINE uint16_t decode_magnification(uint8_t val) {
   uint8_t mag = (val & 0x3);
   if (mag == 3) mag = 4;
   return mag;
}

// Use the DMA info to feed PSG from RAM:
// Read one 16bits instruction for each enabled channel X at each scan line and execute it.
// More precisely: after leading edge of HSYNC, one dead cycle followed by a fetch cycle for each active channel (enabled and not executing a pause) followed by an execution cycle for each active channel.
// All instructions last 1 cycle except LOAD that lasts 8 (up to 10 if CPU is also accessing the PSG).
//  - LOAD R,DD will write DD to PSG.RegisterAY.Index[R] - The ASIC should actually be blocking the CPU if it tries to access the PSG, to determine if it's important to emulate.
//  - PAUSE N set a counter to wait N*(chX.prescaler+1) cycles
//  - REPEAT NNN keep address of loop start (next instruction) and a counter of loops
//  - NOP does nothing
//  - LOOP jump to address of loop stat if counter of loops is >0 and decrement it (yes, code is actually executed NNN+1 times)
//  - INT generates an interruption for chX by setting chX.interrupt to true (code for CPU to detect it must also be done !)
//  - STOP set chX.enabled to false ? (still increment address for when processing will restart)
//  The last 4 can be OR-ed to be combined
void asic_dma_cycle() {
   int c;
   // The two first bits of the address give the page to read from
   uint8_t dcsr = 0;
   bool dcsr_changed = false;
   for(c = 0; c < NB_DMA_CHANNELS; c++) {
      t_DMA_channel *channel = &asic.dma.ch[c];
      if(!channel->enabled) continue;
      if(channel->pause_ticks > 0) { // PAUSE on-going
         if(channel->tick_cycles < channel->prescaler) {
           channel->tick_cycles++;
           continue;
         }
         channel->tick_cycles = 0;
         channel->pause_ticks--;
         continue;
      }
      uint16_t bank = ((channel->source_address & 0xC000) >> 14);
      uint16_t addr = (channel->source_address & 0x3FFF);
      uint16_t instruction = 0;
      instruction |= membank_config[GateArray.RAM_config & 7][bank][addr];
      instruction |= membank_config[GateArray.RAM_config & 7][bank][addr+1] << 8;
      //LOG_DEBUG("DMA [" << c << "] instruction " << std::hex << instruction << " from " << channel.source_address << std::dec);
      uint16_t opcode = ((instruction & 0x7000) >> 12);
      if (opcode == 0) { // LOAD
         int reg = ((instruction & 0x0F00) >> 8);
         uint8_t val = (instruction & 0x00FF);
         SetAYRegister(reg, val);
         //LOG_DEBUG("DMA [" << c << "] load " << std::hex << static_cast<int>(val) << " in register " << R << std::dec);
      }
      else {
         if (opcode & 0x01) { // PAUSE
            channel->pause_ticks = (instruction) & 0x0FFF;
            channel->tick_cycles = 0;
            //LOG_DEBUG("DMA [" << c << "] pause " << channel.pause_ticks << "*" << static_cast<int>(channel.tick_cycles) << " cycles");
         }
         if (opcode & 0x02) { // REPEAT
            channel->loops = (instruction) & 0x0FFF;
            channel->loop_address = channel->source_address;
            //LOG_DEBUG("DMA [" << c << "] repeat " << channel.loops);
         }
         if (opcode & 0x04) { // NOP, LOOP, INT, STOP
            if(instruction & 0x0001) { // LOOP
               if(channel->loops > 0) {
                  channel->source_address = channel->loop_address;
                  //LOG_DEBUG("DMA [" << c << "] loop");
               }
            }
            if(instruction & 0x0010) { // INT
               channel->interrupt = true;
               //LOG_DEBUG("DMA [" << c << "] interrupt");
            }
            if(instruction & 0x0020) { // STOP
               channel->enabled = false;
               //LOG_DEBUG("DMA [" << c << "] stop");
            }
         }
      }
      channel->source_address += 2;
      // TODO: cleaner way to modify back the register value here ...
      addr = 0x6C00 + (c << 2);
      *(membank_write[addr >> 14] + (addr & 0x3fff)) = (uint8_t) (channel->source_address & 0xFF);
      addr++;
      *(membank_write[addr >> 14] + (addr & 0x3fff)) = (uint8_t)((channel->source_address & 0xFF00) >> 8);
      /* Useless ?
      addr++;
      *(membank_write[addr >> 14] + (addr & 0x3fff)) = channel->prescaler;
      */
      if (channel->enabled) {
         dcsr |= (0x1 << c);
         dcsr_changed = true;
      }
      if (channel->interrupt) {
         dcsr |= (0x40 >> c);
         dcsr_changed = true;
      }

      // TODO: ... and here
      // Run RAM test of testplus.cpr when touching this (this is not a guarantee that this is correct but at least it's a guarantee that it's less wrong !)
      if (dcsr_changed) {
         addr = 0x6C0F;
         *(membank_write[addr >> 14] + (addr & 0x3fff)) = dcsr;
      }
   }
}

// Return true if byte should be written in memory
// ASIC register page, from 4000h to 7FFFh is used
bool asic_register_page_write(uint16_t addr, uint8_t val) {
   if (addr < 0x4000 || addr > 0x7FFF)
      return true;

   // TODO:double check the writes (more cases with mirroring / write only ?)
   if (addr >= 0x4000 && addr < 0x5000) {
      int id = ((addr & 0xF00) >> 8);
      int y = ((addr & 0xF0) >> 4);
      int x = (addr & 0xF);
      uint8_t color = (val & 0xF);
      if(color)
         color += 16;
      asic.sprites[id][x][y] = color;
      //LOG("Received sprite %u data", id);
   }
   // 0x5000 --- unused
   // ASIC -- sprite operation, from 6000h to 6080h
   else if (addr >= 0x6000 && addr < 0x6080) {
      // 6000h 	2 	N 	R/W 	X0 	Sprite 0 X position
      // 6002h 	2 	N 	R/W 	Y0 	Sprite 0 Y position
      // 6004h 	1 	Y 	W 	M0 	Sprite 0 magnification
      // 6005/6/7 					(unused)
      // 6008h 	2 	N 	R/W 	X0 	Sprite 0 X position
      // 600Ah 	2 	N 	R/W 	Y0 	Sprite 0 Y position
      // 600Ch 	1 	Y 	W 	M0 	Sprite 0 magnification
      // 600D/E/F 	3 	  	  	  	(unused)
      // ...
      int id = ((addr - 0x6000) >> 3);
      int type = (addr & 0x7);
      switch (type) {
         case 0: // X position HI
            asic.sprites_x[id] = (asic.sprites_x[id] & 0xFF00) | val;
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 1: // X position LO
            asic.sprites_x[id] = (asic.sprites_x[id] & 0x00FF) | (val << 8);
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 2: // Y position HI
            asic.sprites_y[id] = ((asic.sprites_y[id] & 0xFF00) | val);
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 3: // Y position LO
            asic.sprites_y[id] = ((asic.sprites_y[id] & 0x00FF) | (val << 8));
            // Affect RAM image
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 4: // Magnification | if val == 0 -- sprite dissabled
            asic.sprites_mag_x[id] = decode_magnification(val >> 2);
            asic.sprites_mag_y[id] = decode_magnification(val);
            // Write-only: does not affect pbRegisterPage
            return false;
         default:
            // ignore sprite operation of unsupported ASIC types: 5/6/7 or d/e/f
            return true;
      }
   }
   // 0x6080 --- unused
   // ASIC -- palette operation, from 6400h to 6440h
   else if (addr >= 0x6400 && addr < 0x6440) {
      int colour = (addr & 0x3F) >> 1;
      // TODO: CLEAN THIS green/color
      if ((addr % 2) == 1) {
         double green = (double) (val & 0x0F)/16;
         asic_colours[colour][1] = green;
         pbRegisterPage[(addr & 0x3FFF)] = (val & 0x0F);
      } else {
         double red   = (double) ((val & 0xF0) >> 4)/16;
         double blue  = (double) (val & 0x0F)/16;
         asic_colours[colour][0] = red;
         asic_colours[colour][2] = blue;
         pbRegisterPage[(addr & 0x3FFF)] = val;
      }
      GateArray.palette[colour] = CPC.video_monitor( asic_colours[colour][0],
                                                asic_colours[colour][1],
                                                asic_colours[colour][2]);
      return false;
   }
   // 0x6440 --- unused
   // ASIC - Programmable raster, from 6800h to 6806h
   else if (addr >= 0x6800 && addr < 0x6806) {
      switch (addr) {
         case 0x6800: CRTC.interrupt_sl = val; break;
         case 0x6801: CRTC.split_sl = val; break;
         case 0x6802: CRTC.split_addr &= 0x00FF; CRTC.split_addr |= (val << 8); break;
         case 0x6803: CRTC.split_addr &= 0x3F00; CRTC.split_addr |= val; break;

         // Soft scroll control register
         case 0x6804:
            asic.hscroll = (val & 0xf);
            asic.vscroll = ((val >> 4) & 0x7);
            asic.extend_border = (val >> 7);
            update_skew(); // update CRTC
            break;
         case 0x6805:
            // TODO: Write this part !!! (Interrupt service part from http://www.cpcwiki.eu/index.php/Arnold_V_Specs_Revised)
            LOG("Received interrupt vector: %u", (int) val);
      }
   }
   // 0x6806 --- unused
   else if (addr >= 0x6808 && addr < 0x6810) {
      LOG("Received analog input stuff %x", addr);
   }
   // 0x6810 --- unused
   // ...
   // 0x6C00 --- DMA channel 0 address pointer
   // 0x6C02 --- DMA channel 0 pause prescaler
   // 0x6C03 --- unused
   // 0x6C04 --- DMA channel 1 address pointer
   // 0x6C06 --- DMA channel 1 pause prescaler
   // 0x6C07 --- unused
   // 0x6C08 --- DMA channel 2 address pointer
   // 0x6C0A --- DMA channel 2 pause prescaler
   // 0x6C0B --- unused
   else if (addr >= 0x6C00 && addr < 0x6C0B) {
      int c = ((addr & 0xc) >> 2);
      //LOG("Received cmd DMA channel %x", c);
      t_DMA_channel *channel = NULL;
      channel = &asic.dma.ch[c];
      switch(addr & 0x3) {
         case 0:
            channel->source_address &= 0xFF00;
            // least significant bit is ignored (address are word-aligned)
            channel->source_address |= (val & 0xFE);
            break;
        case 1:
            channel->source_address &= 0x00FF;
            channel->source_address |= (val << 8);
            break;
        case 2:
            channel->prescaler = val;
            break;
      }
   }
   // 0x6C0F --- DMA control/status register (DCSR)
   else if(addr == 0x6C0F) {
      for (int c = 0; c < NB_DMA_CHANNELS; c++) {
         asic.dma.ch[c].enabled = (val & (0x1 << c));
      }
   } else {
      //printf("Received unused write at %x - val: %u\n", addr, (int) val);
   }
   return true;
}
