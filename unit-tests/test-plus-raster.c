/* ROM-free Plus raster/scroll regressions.
 * cc -Ilibretro-common/include -Icap32 unit-tests/test-plus-raster.c -o test-plus-raster
 * ./test-plus-raster ./cap32_libretro.so   (add -ldl on Linux)
 */
#include <assert.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cap32.h"
#include "z80.h"
#include "asic.h"
#include "crtc.h"

int main(int argc, char **argv)
{
   assert(argc == 2);
   void *lib = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
   assert(lib);
#define GET(type, name) type *name = dlsym(lib, #name); assert(name)
   GET(t_CPC, CPC);
   GET(t_CRTC, CRTC);
   GET(t_VDU, VDU);
   GET(t_GateArray, GateArray);
   GET(t_z80regs, z80);
   GET(t_asic, asic);
   GET(t_flags1, flags1);
   void (*init)(void) = dlsym(lib, "crtc_init");
   void (*reset)(void) = dlsym(lib, "crtc_reset");
   void (*cycle)(int) = dlsym(lib, "crtc_cycle");
   bool (*poke)(uint16_t, uint8_t) = dlsym(lib, "asic_register_page_write");
   assert(init && reset && cycle && poke);
   init();
   reset();
   CPC->model = CPC_MODEL_PLUS;
   asic->locked = false;
   asic->irq_cause = 6;
   asic->dma.dcsr = 0x87;
   CRTC->interrupt_sl = 7;
   z80->int_pending = 1;
   CRTC->raster_interrupt_delay = 3;
   poke(0x6800, 11);
   assert(!z80->int_pending && !CRTC->raster_interrupt_delay);
   assert(asic->dma.dcsr == 0x87); /* PRI writes do not acknowledge DCSR. */
   z80->int_pending = 1;
   poke(0x6800, 11);
   assert(z80->int_pending); /* Rewriting the same value is not an acknowledge. */
   poke(0x6800, 0);
   assert(z80->int_pending); /* Switching back to classic IRQ preserves pending. */
   for (unsigned source = 0; source < 6; source += 2) {
      asic->irq_cause = source;
      z80->int_pending = 1;
      poke(0x6800, 20 + source);
      assert(z80->int_pending); /* A DMA source must survive a PRI change. */
   }
   asic->irq_cause = 6;
   CRTC->line_count = 1;
   CRTC->raster_count = 6;
   flags1->inHSYNC = 0xff;
   poke(0x6800, 14);
   assert(z80->int_pending);
   poke(0x6800, 13);
   assert(!z80->int_pending);
   CRTC->line_count = 33;
   poke(0x6800, 14);
   assert(!z80->int_pending); /* PRI does not wrap at line 256. */

   /* A fixed scroll of two rasters advances a 49-character row at RA=5,
    * before the CRTC's own RA=7 boundary. Reprogramming SSCR afterwards
    * must not retroactively change that latched row address. */
   memset(asic, 0, sizeof(*asic));
   memset(flags1, 0, sizeof(*flags1));
   memset(GateArray, 0, sizeof(*GateArray));
   memset(VDU, 0, sizeof(*VDU));
   reset();
   VDU->flag_drawing = 0;
   CRTC->registers[0] = 63;
   CRTC->registers[1] = 49;
   CRTC->registers[2] = 55;
   CRTC->registers[9] = 7;
   CRTC->line_count = 3;
   CRTC->raster_count = 5;
   CRTC->addr = CRTC->next_addr = 0x1234;
   CRTC->char_count = 48;
   asic->vscroll = 2;
   cycle(1);
   assert(CRTC->next_addr == 0x1234 + 49);
   poke(0x6804, 0x60);
   assert(CRTC->next_addr == 0x1234 + 49);

   /* A split at that same R1 boundary takes priority, including its second
    * comparison after the 8-bit split line wraps at 256. */
   for (unsigned row = 3; row <= 35; row += 32) {
      CRTC->line_count = row;
      CRTC->raster_count = 5;
      CRTC->char_count = 48;
      CRTC->addr = CRTC->next_addr = 0x1234;
      CRTC->split_sl = 29;
      CRTC->split_addr = 0x2345;
      asic->vscroll = 2;
      cycle(1);
      assert(CRTC->next_addr == 0x2345);
   }
   /* Classic CPC row advancement is independent of stale Plus registers. */
   CPC->model = CPC_MODEL_6128;
   CRTC->char_count = 48;
   CRTC->raster_count = 5;
   CRTC->addr = CRTC->next_addr = 0x1234;
   cycle(1);
   assert(CRTC->next_addr == 0x1234);
   CRTC->char_count = 48;
   CRTC->raster_count = 7;
   cycle(1);
   assert(CRTC->next_addr == 0x1234 + 49);
   puts("PASS: PRI reprogramming, DMA preservation, SSCR row latch, split priority/wrap, classic isolation");
   dlclose(lib);
   return 0;
}
