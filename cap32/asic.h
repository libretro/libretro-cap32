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

/* CPC Plus ASIC ("Arnold V" enhancements of the 6128+, 464+ and GX4000)

   References used throughout asic.c and crtc.c:
     [A15]   Arnold V specification 1.5            - docs/ArnoldV15.txt
     [AVR]   Arnold V Specs Revised (CPCWiki)      - docs/Arnold V Specs Revised - CPCWiki.txt
     [CPC+]  Measured with custom tools while running "Sonic GX (2025)" and SotB Demo.

   Locking
   -------
   All enhanced features stay hidden until the unlock sequence is written to the
   CRTC register-select port &BCxx (see asic_poke_lock_sequence). While the ASIC
   is locked the register page cannot be mapped, and an interrupt acknowledge
   reads &FF from the data bus, exactly as on a classic CPC.

   Register page (&4000-&7FFF)
   ---------------------------
   Mapped over RAM at &4000-&7FFF by writing RMR2 (port &7Fxx, value %101xxxxx)
   with bits 4-3 = %11, and only while the ASIC is unlocked:
     &4000-&4FFF  sprite pixels, 16 sprites of 16x16, one 4-bit pen per byte
     &6000-&607F  per sprite (8 bytes): X (16 bit), Y (16 bit), magnification
     &6400-&643F  12-bit palette: pens 0-15, border, sprite pens 1-15
     &6800        PRI   programmable raster interrupt scan line   (write only)
     &6801        SPLT  split screen scan line                    (write only)
     &6802-&6803  SSA   split screen start address, high byte first (write only)
     &6804        SSCR  soft scroll control                       (write only)
     &6805        IVR   interrupt vector                          (write only)
     &6808-&680F  analogue inputs                                 (read only)
     &6C00-&6C0B  DMA channels 0-2: source address (2), prescaler (1), unused (1)
     &6C0F        DCSR  DMA control and status
   The read-back value of each area is described in asic_register_page_read().

   Raster features (evaluated by crtc_cycle() in crtc.c, once per 1us character)
   -------------------------------------------------------------------------------
   PRI (&6800) [A15 2.4]
     0 (power-on value): the classic Gate Array interrupt is used, one request
       every 52 HSYNCs (GateArray.sl_count), resynchronised by VSYNC.
     1-255: the Gate Array interrupt is suppressed and a raster request is raised
       on the scan line PRI instead, which software reprograms to obtain several
       interrupts per frame. The scan line is matched as VCC == PRI >> 3 and
       VLC == PRI & 7 when the CRTC HSYNC starts (R2 match); VCC is the full 7-bit
       character row, so PRI never matches a second time after line 255. The
       request is raised on the trailing edge of the HSYNC sent to the monitor
       [AVR 2.4]: the Gate Array ends that HSYNC 6us after the CRTC HSYNC starts,
       so the delay is min(R3 width, 6)us, 6us with the usual R3 = &8E ("if
       Horizontal Sync width was set to 14, raster interrupt would trigger on
       cycle 6 not 14"). The classic 52-line interrupt instead follows the CRTC
       HSYNC trailing edge, so R3 widths above 6 still move it.

     Rewriting PRI with a different non-zero value withdraws a raster request
       that is pending but not yet acknowledged, and cancels one that is still
       inside its 10us delay; DMA requests are not affected. If the new value
       matches the current scan line while HSYNC is active, the request is raised
       at once. Writing 0 never withdraws a request.

   SPLT (&6801) and SSA (&6802-&6803) [A15 2.3]
     SPLT is the scan line after which the screen splits; 0 disables the split.
     When the CRTC reaches the end of the displayed line (character counter ==
     R1) on scan line SPLT, the address of the next line is taken from SSA
     instead of continuing the current screen. SSA is a CRTC address (MA),
     written high byte first like R12/R13. The comparison uses an 8-bit line
     number ((VCC & 31) << 3) | (VLC & 7), so it wraps at 256 lines.

   SSCR (&6804) [A15 2.5]
     bits 3-0  horizontal delay of 0-15 mode 2 pixels; the picture moves right.
     bits 6-4  added to the low 3 bits of the raster address: the picture moves
               up by that number of scan lines. It applies immediately to the
               fetched raster address, while the decision to advance to the next
               character row is sampled at R1 (see crtc_cycle).
     bit 7     the border also covers the first 2 bytes (16 mode 2 pixels) of
               each line, hiding the garbage revealed by the horizontal delay.
     Soft scroll affects the whole screen including the split part, never sprites.

   Hardware sprites (&4000-&4FFF, &6000-&607F) [A15 2.1]
   -----------------------------------------------------
   16 sprites of 16x16 pixels, 4 bits per pixel (0 = transparent, 1-15 = the
   15 sprite pens at &6422-&643F), independent of the screen mode: one
   unmagnified sprite pixel is one mode 2 pixel wide and one scan line high.
     X, Y   two's complement positions of the top left corner: X is 10 bits
            (-256 to +767, &300-&3FF are negative), Y is 9 bits (-256 to
            +255). X = 0 is the first displayed pixel (after the 16 pixels
            of border added by SSCR bit 7 when it is set, Y = 0 the first
            displayed line. Negative values clip the sprite smoothly at the
            left/top edge.
     M      bits 3-2 X and bits 1-0 Y magnification: 0 = sprite off, 1 = x1,
            2 = x2, 3 = x4 (so a sprite covers 16, 32 or 64 pixels/lines).

   Priority: border > sprite 0 > ... > sprite 15 > screen. Sprites are behind
   the border, including the 16 pixels added by SSCR bit 7.
   Changing a position mid-frame takes effect from the current beam position,
   which is how software multiplexes the 16 sprites to cover a full screen.

   Integrated 8255 PPI (&F4xx-&F7xx) [AVR "8255"]
   ----------------------------------------------------
   The keyboard, the joysticks and the AY-3-8912 are reached through the PPI:
   the PSG data bus is wired to PPI port A, its function lines (BDIR/BC1) to
   port C bits 7-6, and the keyboard is read through the PSG's own I/O port
   (register 14). On the Plus the 8255 is not a separate chip: the ASIC
   imitates it, incompletely. The emulation (z80_OUT_handler/z80_IN_handler in
   cap32.c) has two levels; a classic CPC always keeps the real 8255 logic.

   Default behaviour:
     - only mode 0 and the direction of port A exist: port B is always an
       input and port C always an output, whatever the control byte says;
     - a mode set does NOT clear the port latches (a real 8255 resets A, B
       and C to 0);
     - reading the control register (&F7xx) returns &FF while port A is an
       input and &00 while it is an output;
     - after reset port A is an input, B an input, C an output.
     The PSG receives the port A latch whenever port C selects or writes, as
     on a classic machine, so classic games keep working.

   Strict option (core option cap32_strict_asic_ppi, CPC.asic_ppi_strict):
     additionally reproduces the hardware fault documented in [AVR]: while
     port A is an input the ASIC drives &FF on its outputs. Because the
     function lines are not cleared by a mode set, switching port A to input
     while port C still selects a PSG register makes the PSG latch register
     &FF; register reads then return &FF and keyboard scanning fails. Code
     that sets port C to "inactive" before changing the direction (as
     recommended in [AVR]) is unaffected; classic games that rely on the
     8255 clearing its ports may misread the keyboard (some affected games are
     Comando Quatro and Antiriad). Measured on this core with the option
     enabled: Thing Bounces Back briefly changes its music and Comando
     Quatro's gameplay diverges.

   DMA sound channels (&6C00-&6C0F) [A15 2.6]
   -------------------------------------------
   See asic_dma_channel() for the instruction set and timing.

   Interrupt controller [A15 2.7, AVR, CPC+]
   ------------------------------------------
   Four request sources, kept in asic.int_pending with the same bit layout as
   DCSR bits 7-4 (ASIC_INT_*):
     raster   PRI match, or the classic 52-line interrupt while PRI = 0
     DMA 0-2  an INT instruction (&4010) executed by that channel
   The Z80 INT line is level triggered: it stays asserted while any request bit
   is set (z80.int_pending mirrors asic.int_pending != 0).

   An interrupt acknowledge cycle (any interrupt mode; in IM1 the vector is just
   ignored) is handled by asic_int_acknowledge():
     1. The source is chosen by fixed priority Raster > DMA 2 > DMA 1 > DMA 0,
        and the vector is (IVR & &F8) | source bits:
            raster   %110 (6)
            DMA 2    %000 (0)
            DMA 1    %010 (2)
            DMA 0    %100 (4)
        Bit 0 of the supplied vector is always 0.
     2. The raster request is always cleared by the acknowledge.
     3. IVR bit 0 = 0 enables auto-clear: the acknowledged DMA request is
        cleared by the acknowledge cycle itself, so vectored software never has
        to touch DCSR. IVR bit 0 = 1 (the reset value) keeps DMA requests set
        until software writes 1 to the channel's DCSR bit; until then the
        channel keeps interrupting [AVR].
     4. DCSR bit 7 becomes 1 if this acknowledge was for the raster request,
        0 otherwise.
     5. Requests that were not acknowledged stay pending and interrupt again as
        soon as the CPU re-enables interrupts.
   Requests are also cleared by: setting bit 4 of RMR (port &7Fxx, value
   %10xxxxxx; on an unlocked Plus %101xxxxx selects RMR2 instead) for the raster
   request [A15 2.7]; a PRI rewrite for the raster request (see above); writing
   1 to DCSR bits 6-4 for DMA requests.

   [CPC+] measurements (IVR = &00, 40 seconds): every one of 10,813 DMA 2 INT
   instructions was acknowledged once with vector &00; raster+DMA 2 pending gave
   &06 first and left DMA 2 pending; DMA 1+DMA 0 pending gave &02 first and left
   DMA 0 pending; 52-line interrupts with PRI = 0 were acknowledged as &06.

   Not emulated / not verified:
     - the IVR bit 0 = 1 path follows [A15]/[AVR], but was not exercised by the
       measured software;
     - the "Plus vectored interrupt bug" mentioned by [AVR] is not documented in
       docs/ and is not emulated;
     - with the ASIC locked, an acknowledge returns &FF, clears the raster
       request and leaves DMA requests untouched (I checked other emulators and
       does the same).
*/

#ifndef ASIC_H
#define ASIC_H

#include "cap32.h"

#define ASIC_SPRITES 16
#define ASIC_SPRITE_SIZE 16
#define ASIC_LOCK_SIZE 15
#define NB_DMA_CHANNELS 3
#define ASIC_RAM_INIT 0x4000
#define ASIC_RAM_END 0x7FFF

/* Interrupt request bits, laid out like DCSR bits 7-4. */
#define ASIC_INT_RASTER   0x80
#define ASIC_INT_DMA0     0x40
#define ASIC_INT_DMA1     0x20
#define ASIC_INT_DMA2     0x10
#define ASIC_INT_DMA(c)   (ASIC_INT_DMA0 >> (c))
#define ASIC_INT_DMA_MASK (ASIC_INT_DMA0 | ASIC_INT_DMA1 | ASIC_INT_DMA2)

/* Low 3 bits of the IM2 vector supplied for each source. */
#define ASIC_VECTOR_RASTER 6
#define ASIC_VECTOR_DMA0   4
#define ASIC_VECTOR_DMA1   2
#define ASIC_VECTOR_DMA2   0

/* IVR bit 0: 1 = DMA requests must be cleared through DCSR (reset value),
 * 0 = the acknowledge cycle clears the acknowledged DMA request. */
#define ASIC_IVR_NO_AUTO_CLEAR 0x01

typedef struct {
   uint32_t source_address;  /* SAR: next instruction (always even, RAM only) */
   uint32_t loop_address;    /* address of the REPEAT instruction (LOOP returns after it) */
   uint8_t prescaler;        /* PPR: a pause tick lasts prescaler + 1 scan lines */
   bool enabled;             /* DCSR enable bit; cleared by the CPU, STOP or reset */
   int pause_ticks;          /* remaining PAUSE ticks, 0 = executing instructions */
   uint8_t tick_cycles;      /* scan lines left in the current pause tick (counts down) */
   int loops;                /* REPEAT counter */
} t_DMA_channel;

typedef struct {
   t_DMA_channel ch[NB_DMA_CHANNELS];
} t_dma;

typedef struct {
   bool locked;
   int lock_seq_pos;
   int lock_prev_data;

   uint8_t rmr2;
   bool extend_border;       /* SSCR bit 7 */
   int hscroll;              /* SSCR bits 3-0 */
   int vscroll;              /* SSCR bits 6-4 */
   uint8_t sprites[ASIC_SPRITES][ASIC_SPRITE_SIZE][ASIC_SPRITE_SIZE];
   int16_t sprites_x[ASIC_SPRITES];
   int16_t sprites_y[ASIC_SPRITES];
   int16_t sprites_mag_x[ASIC_SPRITES];
   int16_t sprites_mag_y[ASIC_SPRITES];

   t_dma dma;

   uint8_t interrupt_vector; /* IVR as last written; bit 0 is set at reset */
   uint8_t int_pending;      /* ASIC_INT_* requests not yet cleared */
   bool raster_acknowledged; /* DCSR bit 7: the last acknowledge was for the raster request */

   /* Palette entries last written through the classic Gate Array ports. */
   uint32_t legacy_palette;
} t_asic;

extern t_asic asic;
extern uint8_t *pbRegisterPage;

void asic_reset();
void asic_poke_lock_sequence(uint8_t val);
void asic_dma_cycle();
bool asic_register_page_read(uint16_t addr, uint8_t* val);
bool asic_register_page_write(uint16_t addr, uint8_t val);

void asic_raise_interrupt(uint8_t sources);
void asic_clear_raster_interrupt(void);
uint8_t asic_int_acknowledge(void);
uint8_t asic_dcsr(void);

#endif
