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
   Base Plus implementation the from caprice fork (c) 2016-2019 Colin Pitrat.
   ASIC, DMA, PRI, and PPI accurate hardware rewrites (c) 2026 David Colmenero.
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
#ifdef DEBUG_ASIC
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
extern t_flags1 flags1;
extern t_CPC CPC;
extern t_PSG PSG;
extern t_z80regs z80;

extern uint32_t dwXScale;
extern uint8_t *membank_config[8][4];
extern uint8_t *membank_write[4];

uint8_t *pbRegisterPage;

uint8_t asic_ram[16384]; // asic full ram
t_asic asic;
double asic_colours[32][3];
const uint8_t asic_lock_data[ASIC_LOCK_SIZE] = { 0xff, 0x77, 0xb3, 0x51, 0xa8, 0xd4, 0x62, 0x39, 0x9c, 0x46, 0x2b, 0x15, 0x8a, 0xcd, 0xee };

/**
 * asic_reset:
 * power-on state: locked, no interrupt request, DMA channels disabled and
 * IVR bit 0 set, so DMA requests are not auto-cleared until software writes
 * a vector with bit 0 = 0 [A15 2.7].
 */
void asic_reset(){
   memset(&asic, 0, sizeof(asic));

   asic.locked = true;
   asic.interrupt_vector = ASIC_IVR_NO_AUTO_CLEAR;
   memset(asic_ram, 0, 16384);
}

void asic_poke_lock_sequence(uint8_t val) {
   if(val == 0 && asic.lock_prev_data != 0) {
      asic.lock_seq_pos = 0;
   }

   if(val == asic_lock_data[asic.lock_seq_pos]) {
      asic.lock_seq_pos++;
      if (asic.lock_seq_pos == (ASIC_LOCK_SIZE - 1) && !asic.locked ) {
         LOG("ASIC locked at %u", asic.lock_seq_pos);
         asic.locked = true;
      }
      if (asic.lock_seq_pos >= ASIC_LOCK_SIZE) {
         LOG("ASIC unlocked!! (%u)", asic.lock_seq_pos);
         asic.locked = false;
      }
   } else {
      // ASIC last byte can be any value
      if (asic.lock_seq_pos == (ASIC_LOCK_SIZE - 1)) {
         asic.lock_seq_pos++;
         LOG("ASIC unlocked at %u", asic.lock_seq_pos);
         asic.locked = false;
      }
   }
   asic.lock_prev_data = val;
}

/* Sprite positions are two's complement numbers of 10 bits for X (-256 to
 * +767, mode 2 pixels) and 9 bits for Y (-256 to +255, scan lines)
 * [A15 2.1]. X values &300-&3FF are therefore negative (-256 to -1); that is
 * also why reading back the X MSB returns &FF exactly when bits 1-0 are 11.
 * Negative positions let a sprite start left of / above the visible area and
 * be clipped smoothly, e.g. Shadow of the Beast places its leftmost masking
 * sprite at X = -16 (&FFF0). */
static INLINE int16_t decode_sprite_x(uint8_t lsb, uint8_t msb) {
   int x = ((msb & 0x03) << 8) | lsb;
   return x >= 0x300 ? x - 0x400 : x;
}

static INLINE int16_t decode_sprite_y(uint8_t lsb, uint8_t msb) {
   int y = ((msb & 0x01) << 8) | lsb;
   return y >= 0x100 ? y - 0x200 : y;
}

static INLINE uint16_t decode_magnification(uint8_t val) {
   uint8_t mag = (val & 0x3);
   if (mag == 3) mag = 4;
   return mag;
}

/**
 * asic_dma_channel:
 * @c: DMA channel (0-2)
 *
 * Why the Plus has "DMA" sound channels
 * -------------------------------------
 * On every CPC the AY-3-8912 sound chip (PSG) is not on the Z80 bus: the CPU
 * reaches it through the 8255 PPI, with several OUT instructions per register
 * write (select the register, then write the value, each through the PPI
 * data and control ports). Music players therefore
 * steal CPU time every frame, and sampled sound needs the CPU almost entirely.
 * The Plus ASIC adds three independent instruction streams that feed the PSG
 * without any CPU work. Each one is a tiny program in RAM; the ASIC fetches and
 * executes one 16-bit instruction per channel on every scan line, i.e. at the
 * horizontal frequency of 15.625 kHz. "DMA" is Amstrad's name: the channels do
 * read RAM on their own, but they only ever write to the PSG. Because the CPU
 * and the channels share the PPI, the ASIC holds the CPU off (wait states)
 * while a LOAD instruction is talking to the PSG.
 *
 * Besides sound, the INT instruction makes each list a programmable line
 * timer: a list of PAUSE/INT pairs interrupts the CPU on chosen scan lines
 * with no CPU involvement, a technique Sonic GX (2025) uses to drive its
 * raster splits.
 *
 * Instruction set [A15 2.6]
 * -------------------------
 *   &0RDD  LOAD R,DD  write DD to PSG register R (0-15)
 *   &1NNN  PAUSE N    wait N ticks of (prescaler + 1) scan lines; 0 = NOP
 *   &2NNN  REPEAT N   loop counter = N, the next instruction starts the loop;
 *                     the loop body therefore runs N + 1 times; 0 = NOP
 *   &3xxx  reserved
 *   &4000  NOP        no operation (one scan line, 64us)
 *   &4001  LOOP       if the loop counter is not 0, decrement it and jump back
 *                     to the instruction after REPEAT
 *   &4010  INT        request a CPU interrupt for this channel (ASIC_INT_DMA(c)),
 *                     see asic_int_acknowledge() for vectors and priority
 *   &4020  STOP       clear the channel enable bit; the source address still
 *                     moves on, so the list can be resumed after CPU intervention
 *   &4xxx  LOOP, INT and STOP can be ORed together, e.g. &4030 = INT + STOP.
 * Only one REPEAT loop can be active per channel (no nesting).
 *
 * Registers: SAR (&6C00/1, &6C04/5, &6C08/9) holds the address of the next
 * instruction; its bit 0 is ignored. PPR (&6C02, &6C06, &6C0A) is the pause
 * prescaler: a PAUSE tick lasts PPR + 1 scan lines, from 64us to 16.384ms, so
 * PAUSE alone spans 64us to about 67s. DCSR (&6C0F) enables the channels
 * (bits 2-0) and reports their pending interrupts (bits 6-4).
 * Instructions are always fetched from RAM (pages 0-3 by address bits 15-14),
 * never from ROM, whatever the ROM/RAM mapping registers select.
 *
 * Timing
 * ------
 * On hardware, the HSYNC leading edge is followed by one dead 1us cycle, one
 * fetch cycle per active channel (enabled and not pausing) and then one
 * execute cycle per channel; LOAD takes at least 8 cycles and the CPU can be
 * held off the PPI for up to 8us meanwhile [A15]. Fetch and execution of every
 * channel at once when the HSYNC width counter reaches 3 (crtc.c match_hsw).
 * A PAUSE M fetched on line L lets the next instruction be fetched on line
 * L + M * (PPR + 1) exactly. CPU wait states caused by LOAD are not emulated.
 */
void asic_dma_channel(int c)
{
   t_DMA_channel *channel = &asic.dma.ch[c];

   /* PAUSE M with prescaler P: the instruction after the PAUSE is fetched
    * exactly M * (P + 1) scan lines after the PAUSE itself [A15 2.6], i.e. on
    * the very line where the last tick ends, not on the line after it. Each
    * tick lasts P + 1 lines; tick_cycles counts the lines left in the current tick.
    */
   if(channel->pause_ticks > 0) { // PAUSE on-going
      if(channel->tick_cycles > 0) { // inside the current tick
        channel->tick_cycles--;
        return;
      }
      channel->tick_cycles = channel->prescaler; // a tick ended, start the next one
      if(--channel->pause_ticks > 0)
        return;
      // last tick ended: fetch and execute the next instruction on this line
   }

   if( channel->source_address & 0x01)
		channel->source_address++;  // align address

   uint16_t bank = ((channel->source_address & 0xC000) >> 14);
   uint16_t addr = (channel->source_address & 0x3FFF);
   uint16_t instruction = 0;

   instruction |= membank_config[GateArray.RAM_config & 7][bank][addr];
   instruction |= membank_config[GateArray.RAM_config & 7][bank][addr+1] << 8;

   switch (instruction & 0xf000)
   {
      case 0x0000: // LOAD PSG Register
         SetAYRegister(((instruction & 0x0F00) >> 8), // reg
                        (instruction & 0x00FF) );     // val
         break;
      case 0x1000: // PAUSE N: wait N ticks of (prescaler + 1) scan lines; PAUSE 0 = NOP
         channel->pause_ticks = (instruction & 0x0FFF);
         channel->tick_cycles = channel->prescaler;
         break;
      case 0x2000: // REPEAT loop
         channel->loops = (instruction) & 0x0FFF;
         channel->loop_address = channel->source_address;
         break;
      case 0x4000: // NOP, LOOP, INT, STOP
         if(instruction & 0x0001) // LOOP
         {
            if(channel->loops > 0)
            {
               channel->source_address = channel->loop_address;
               //LOG_DEBUG("DMA [" << c << "] loop");
               channel->loops--;
            }
         }
         if(instruction & 0x0010) // INT
         {
            // the request stays set until acknowledged (auto-clear) or cleared through DCSR
            asic_raise_interrupt(ASIC_INT_DMA(c));
         }
         if(instruction & 0x0020) // STOP
         {
            // also clears the DCSR enable bit, see asic_dcsr()
            channel->enabled = false;
         }
         break;
   }
   channel->source_address += 2;
}

/**
 * asic_raise_interrupt:
 * @sources: ASIC_INT_* request bits
 *
 * Sets interrupt requests and asserts the Z80 INT line. The line is level
 * triggered: it stays asserted until every request has been cleared, so a
 * request that loses the priority arbitration is serviced on the next
 * acknowledge instead of being lost. Used for every model; on a classic CPC
 * only the raster bit (the Gate Array 52-line interrupt) is ever raised.
 */
void asic_raise_interrupt(uint8_t sources)
{
   asic.int_pending |= sources;
   z80.int_pending = 1;
}

/**
 * asic_clear_raster_interrupt:
 * withdraws the raster request without an acknowledge cycle (RMR bit 4 or a
 * PRI rewrite). DMA requests are independent and keep INT asserted.
 */
void asic_clear_raster_interrupt(void)
{
   asic.int_pending &= ~ASIC_INT_RASTER;
   z80.int_pending = asic.int_pending != 0;
}

/**
 * asic_int_acknowledge:
 * Z80 interrupt acknowledge cycle (called in IM0, IM1 and IM2).
 *
 * Returns the byte the ASIC drives on the data bus: (IVR & &F8) | source,
 * with the source chosen by the fixed priority Raster (6) > DMA 2 (0) >
 * DMA 1 (2) > DMA 0 (4). The raster request is always cleared; the DMA request
 * that was acknowledged is cleared too when IVR bit 0 is 0 (auto-clear).
 * DCSR bit 7 records whether this acknowledge was for the raster request.
 * Requests left pending keep the INT line asserted. If no request is pending
 * the DMA 0 vector is supplied. A locked ASIC supplies &FF and leaves DMA
 * requests alone. Full description and measurements in asic.h.
 */
uint8_t asic_int_acknowledge(void)
{
   uint8_t pending = asic.int_pending;
   uint8_t vector = 0xff;

   asic.int_pending &= ~ASIC_INT_RASTER;
   asic.raster_acknowledged = (pending & ASIC_INT_RASTER) != 0;

   if (!asic.locked) {
      uint8_t source, request;

      if (pending & ASIC_INT_RASTER) {
         source = ASIC_VECTOR_RASTER;
         request = ASIC_INT_RASTER;
      } else if (pending & ASIC_INT_DMA2) {
         source = ASIC_VECTOR_DMA2;
         request = ASIC_INT_DMA2;
      } else if (pending & ASIC_INT_DMA1) {
         source = ASIC_VECTOR_DMA1;
         request = ASIC_INT_DMA1;
      } else {
         source = ASIC_VECTOR_DMA0;
         request = ASIC_INT_DMA0;
      }

      if (!(asic.interrupt_vector & ASIC_IVR_NO_AUTO_CLEAR))
         asic.int_pending &= ~request;

      vector = (asic.interrupt_vector & 0xf8) | source;
   }

   z80.int_pending = asic.int_pending != 0;
   return vector;
}

/**
 * asic_dcsr:
 * DMA control and status register as read by the CPU [A15 2.6]:
 *   bit 7     the last interrupt acknowledge was for the raster request
 *   bits 6-4  DMA channel 0, 1, 2 interrupt requests still pending (with
 *             auto-clear they are cleared before software can read them)
 *   bit 3     unused, reads 0
 *   bits 2-0  DMA channel 2, 1, 0 enables (also cleared by STOP)
 * We refreshes its copy only during an acknowledge; agree whenever software
 * reads DCSR from its interrupt handler, which is how it is used.
 */
uint8_t asic_dcsr(void)
{
   uint8_t dcsr = (asic.raster_acknowledged ? 0x80 : 0) | (asic.int_pending & ASIC_INT_DMA_MASK);
   int c;

   for (c = 0; c < NB_DMA_CHANNELS; c++)
      if (asic.dma.ch[c].enabled)
         dcsr |= 1 << c;
   return dcsr;
}

/**
 * asic_dma_cycle:
 * runs every enabled DMA channel for the current scan line, in channel order
 * (see asic_dma_channel for the timing).
 */
void asic_dma_cycle()
{
   int c;
   for(c = 0; c < NB_DMA_CHANNELS; c++)
   {
      if (asic.dma.ch[c].enabled)
      {
         asic_dma_channel(c);
      }
   }
}

/**
 * asic_register_page_read:
 * @addr: CPU address
 * @val: receives the byte the ASIC drives on the data bus
 *
 * What the register page is
 * -------------------------
 * The Plus keeps all its new hardware state (sprite images, sprite positions,
 * the 4096-colour palette, raster, scroll and DMA registers) inside the ASIC.
 * Rather than adding I/O ports, Amstrad mapped this state into the CPU's
 * memory map: once the ASIC is unlocked, writing RMR2 (&7Fxx, %10111xxx)
 * pages a 16KB "register page" over RAM at &4000-&7FFF. Ordinary memory
 * instructions then program the hardware, e.g. LD (&6400),HL sets a colour,
 * and the 64K of RAM underneath is untouched until the page is unmapped. On
 * real hardware these are ASIC registers, not memory, so reading them back
 * does not always return what was written. This function reproduces that.
 *
 * Returns true when @addr is outside the register page, i.e. the caller must
 * read RAM as usual; false when @val holds the ASIC's answer.
 */
bool asic_register_page_read(uint16_t addr, uint8_t* val) {
   if (addr < ASIC_RAM_INIT || addr > ASIC_RAM_END)
      return true;

   // Sprite data
   //   &4000-&4FFF  sprite pixels: only 4 bits are stored per pixel, so the high
   //                nibble reads as 0.
   if (addr >= 0x4000 && addr <= 0x4FFF) {
      *val = asic_ram[addr - ASIC_RAM_INIT] & 0x0F;
   }

   // Sprite position and magnification information
   //   &6000-&607F  sprite X/Y read back sign extended from 10 (X) and 9 (Y)
   //                bits: the MSB reads &FF for negative positions. The
   //                magnification registers are write only [A15 2.1].
   else if (addr >= 0x6000 && addr <= 0x607F) {
      *val = asic_ram[addr - ASIC_RAM_INIT];
      switch (addr & 0x03) {
         case 0x03:
            if ((*val & 0x01) == 0x01) {
               *val = 0x0ff;
            } else {
               *val &= 0x01;
            }
            break;
         case 0x01:
            if ((*val & 0x03) == 0x03) {
               *val = 0x0ff;
            } else {
               *val &= 0x03;
            }
            break;
      }
   }
   // Palette data
   //   &6400-&643F  palette: the green byte (odd address) only has 4 bits.
   else if (addr >= 0x6400 && addr <= 0x643F) {
      *val = asic_ram[addr - ASIC_RAM_INIT];
      if (addr & 0x01)
          *val &= 0x0F;
   }
   // Misc registers
   //   &6800-&6807  write only (PRI, SPLT, SSA, SSCR, IVR): the value read is
   //                undefined on hardware; a fixed pattern is returned.
   else if (addr >= 0x06800 && addr <= 0x06807) {
      *val = 0x0B0 + (addr & 0x1); // random - invalid data area
   }
   // Analog input stuff
   //   &6808-&680F  analogue joystick inputs; no analogue device is emulated, so
   //                the idle values are returned.
   else if (addr >= 0x6808 && addr <= 0x680C) {
      *val = 0x3F;
   } else if (addr == 0x680E) {
      *val = 0x3F;
   } else if (addr == 0x680D || addr == 0x680F) {
      *val = 0x00;
   }
   // DMA registers are write only; the whole &6C00-&6C0F range reads DCSR
   //   &6C00-&6C0F  the DMA registers are write only and every address in the
   //                range reads DCSR (see asic_dcsr()).
   else if (addr >= 0x6C00 && addr <= 0x06C0F) {
      *val = asic_dcsr();
   }
   // Elsewhere - unused register space reads back the last byte written.
   else {
      *val = asic_ram[addr - ASIC_RAM_INIT];
   }

   return false;
}

/**
 * asic_register_page_write:
 * @addr: CPU address
 * @val: byte written by the CPU
 *
 * Programs the Plus hardware through the register page (see
 * asic_register_page_read for how the page is mapped). Each area controls one
 * feature, and a write takes effect immediately, at the current beam
 * position: changing a colour, a sprite position or SSCR halfway through a
 * line changes the picture from that point on. Plus software relies on this
 * all the time, combining it with the raster interrupt (PRI) to change the
 * hardware several times per frame: palette bars, parallax splits, sprite
 * multiplexing.
 *
 * Timing note: the emulated Z80 performs its memory accesses before the CRTC
 * is advanced for the instruction, so a write becomes visible from the
 * character at which the instruction starts.
 *
 * Returns true when @addr is outside the register page (the caller must write
 * RAM as usual), false when the ASIC consumed the write.
 */
bool asic_register_page_write(uint16_t addr, uint8_t val) {
   if (addr < ASIC_RAM_INIT || addr > ASIC_RAM_END)
      return true;

   asic_ram[addr - ASIC_RAM_INIT] = val; // force write values in asic ram - help to test/debug

   //   &4000-&4FFF  sprite pixels, one 4-bit pen per byte (256 bytes per sprite)
   if (addr >= 0x4000 && addr <= 0x4FFF) {
      int id = ((addr & 0xF00) >> 8);
      int y = ((addr & 0xF0) >> 4);
      int x = (addr & 0xF);
      uint8_t color = (val & 0xF);
      pbRegisterPage[(addr & 0x3FFF)] = color;
      if(color)
         color += 16;
      asic.sprites[id][x][y] = color;

      return false;
   // 0x5000 --- unused
   } else if (addr >= 0x5000 && addr < 0x6000) {
   }
   //   &6000-&607F  sprite X (LSB, MSB), Y (LSB, MSB) and magnification, 8 bytes
   //                per sprite. Positions are two's complement (asic.h) and are
   //                decoded here so that the renderer gets signed values.
   else if (addr >= 0x6000 && addr <= 0x607F) {
      // 6000h    2  N  R/W   X0    Sprite 0 X position
      // 6002h    2  N  R/W   Y0    Sprite 0 Y position
      // 6004h    1  Y  W     M0    Sprite 0 magnification
      // 6005/6/7 3                 Sprite 0 magnification
      // 6008h    2  N  R/W   X0    Sprite 0 X position
      // 600Ah    2  N  R/W   Y0    Sprite 0 Y position
      // 600Ch    1  Y  W     M0    Sprite 0 magnification
      // 600D/E/F 3                 Sprite 0 magnification
      // ...
      int id = ((addr - 0x6000) >> 3);
      int type = (addr & 0x7);
      // raw register bytes of this sprite (asic_ram already holds the new value)
      const uint8_t *regs = asic_ram + (0x2000 + id * 8);
      switch (type) {
         case 0: // X position LSB
            asic.sprites_x[id] = decode_sprite_x(regs[0], regs[1]);
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 1: // X position MSB, only bits 1-0 exist
            val = val & 0x3;
            asic.sprites_x[id] = decode_sprite_x(regs[0], regs[1]);
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 2: // Y position LSB
            asic.sprites_y[id] = decode_sprite_y(regs[2], regs[3]);
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 3: // Y position MSB, only bit 0 exists
            val = val & 0x1;
            asic.sprites_y[id] = decode_sprite_y(regs[2], regs[3]);
            // Mirrored in RAM image 4 bytes after
            pbRegisterPage[(addr & 0x3FFF) + 4] = val;
            break;
         case 4: // Magnification | if val == 0 -- sprite dissabled
         case 5: // Magnification - http://cpctech.cpc-live.com/docs/cpcplus.html
         case 6: // Magnification
         case 7: // Magnification
            asic.sprites_mag_x[id] = decode_magnification(val >> 2);
            asic.sprites_mag_y[id] = decode_magnification(val);
            // Write-only: does not affect pbRegisterPage
      }
   }
   // 0x6080 --- unused
   // ASIC -- palette operation, from 6400h to 6440h
   //   &6400-&643F  palette: even byte = red (bits 7-4) and blue (bits 3-0), odd
   //                byte = green (bits 3-0); 16 screen pens, border, 15 sprite
   //                pens. It replaces the classic 27-colour Gate Array inks.
   else if (addr >= 0x6400 && addr <= 0x643F) {
      int colour = (addr & 0x3F) >> 1;
      asic.legacy_palette &= ~(1u << colour);
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
      GateArray.palette[colour] = CPC.video_monitor(
         asic_colours[colour][0],
         asic_colours[colour][1],
         asic_colours[colour][2]
      );
   }
   // 0x6440 --- unused

   // ASIC - raster registers, from 6800h to 6805h (timing in asic.h and crtc.c)
   // *   &6800-&6805  PRI, SPLT, SSA, SSCR, IVR: raster interrupt line, split
   // *                screen, soft scroll and interrupt vector (asic.h has the
   // *                exact semantics and timing).
   else if (addr >= 0x6800 && addr <= 0x6805) {
      switch (addr) {
         case 0x6800: // PRI - programmable raster interrupt scan line
            if (val != CRTC.interrupt_sl) {
               /* A changed PRI withdraws the previous raster request and
                * cancels one still in its 10us delay. DMA requests and DCSR's
                * last-acknowledge bit are independent. Zero returns to the
                * classic interrupt without withdrawing an asserted request. */
               CRTC.raster_interrupt_delay = 0;
               if (val && (asic.int_pending & ASIC_INT_RASTER)) {
                  asic_clear_raster_interrupt();
               }
               // the new line is the current one and its HSYNC already began
               if (val && flags1.inHSYNC &&
                   CRTC.line_count == (val >> 3) &&
                   (CRTC.raster_count & 7) == (val & 7)) {
                  asic_raise_interrupt(ASIC_INT_RASTER);
               }
            }
            CRTC.interrupt_sl = val;
            break;
         case 0x6801: CRTC.split_sl = val; break; // SPLT - split after this scan line, 0 = off
         // SSA - split start address, high byte first like CRTC R12/R13
         case 0x6802: CRTC.split_addr &= 0x00FF; CRTC.split_addr |= (val << 8); break;
         case 0x6803: CRTC.split_addr &= 0x3F00; CRTC.split_addr |= val; break;

         case 0x6804: // SSCR - soft scroll control
            asic.hscroll = (val & 0xf);           // bits 3-0: delay in mode 2 pixels
            asic.vscroll = ((val >> 4) & 0x7);    // bits 6-4: raster address offset
            asic.extend_border = (val >> 7);      // bit 7: border over the first 2 bytes
            update_skew(); // update CRTC
            break;
         case 0x6805: // IVR - vector bits 7-3; bit 0 = 0 enables DMA auto-clear
            asic.interrupt_vector = val;
            LOG("Received interrupt vector write - data = &%02x", asic.interrupt_vector);
      }
   }
   // 0x6806 --- unused
   // 0x6807 --- unused
   else if (addr >= 0x6808 && addr < 0x6810) {
      LOG("Received analog input stuff %x", addr);
   }
   // 0x6810 --- unused

   //   &6C00-&6C0B  DMA channel addresses, prescalers and DCSR.
   else if (addr >= 0x6C00 && addr < 0x6C0B) {
      // 0x6C00 --- DMA channel 0 address pointer
      // 0x6C02 --- DMA channel 0 pause prescaler
      // 0x6C03 --- unused
      // 0x6C04 --- DMA channel 1 address pointer
      // 0x6C06 --- DMA channel 1 pause prescaler
      // 0x6C07 --- unused
      // 0x6C08 --- DMA channel 2 address pointer
      // 0x6C0A --- DMA channel 2 pause prescaler
      // 0x6C0B --- unused
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
   // 0x6C0F --- DMA control/status register (DCSR), read back by asic_dcsr()
   //  bit 7     - read only: last acknowledge was for the raster request
   //  bits 6-4  - write 1 to clear the DMA channel 0, 1, 2 interrupt request
   //  bit 3     - unused (write 0)
   //  bits 2-0  - DMA channel 2, 1, 0 enable
   else if(addr == 0x6C0F) {
      for (int c = 0; c < NB_DMA_CHANNELS; c++)
         asic.dma.ch[c].enabled = (val >> c) & 1;

      // acknowledging DMA requests by software (required while IVR bit 0 = 1)
      if (val & ASIC_INT_DMA_MASK) {
         asic.int_pending &= ~(val & ASIC_INT_DMA_MASK);
         z80.int_pending = asic.int_pending != 0;
      }
   }

   return false;
}
