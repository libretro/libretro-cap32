/* ROM-free CPU interrupt acknowledge regression.
 * cc -Ilibretro-common/include -Icap32 unit-tests/test-plus-irq.c -o test-plus-irq
 * ./test-plus-irq ./cap32_libretro.so    (add -ldl on Linux)
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
   t_CPC *cpc = dlsym(lib, "CPC");
   t_CRTC *crtc = dlsym(lib, "CRTC");
   t_VDU *vdu = dlsym(lib, "VDU");
   t_GateArray *ga = dlsym(lib, "GateArray");
   t_z80regs *cpu = dlsym(lib, "z80");
   t_asic *asic = dlsym(lib, "asic");
   uint8_t **reads = dlsym(lib, "membank_read");
   uint8_t **writes = dlsym(lib, "membank_write");
   void (*init_tables)(void) = dlsym(lib, "z80_init_tables");
   void (*init_crtc)(void) = dlsym(lib, "crtc_init");
   void (*reset_crtc)(void) = dlsym(lib, "crtc_reset");
   int (*execute)(void) = dlsym(lib, "z80_execute");
   assert(cpc && crtc && vdu && ga && cpu && asic && reads && writes);
   assert(init_tables && init_crtc && reset_crtc && execute);
   uint8_t ram[65536];
   init_tables();
   init_crtc();
   for (int locked = 0; locked < 2; locked++) {
      for (int enabled = 0; enabled < 2; enabled++) {
         for (int im = 0; im <= 2; im++) {
            memset(ram, 0, sizeof(ram));
            memset(cpu, 0, sizeof(*cpu));
            memset(asic, 0, sizeof(*asic));
            memset(ga, 0, sizeof(*ga));
            memset(vdu, 0, sizeof(*vdu));
            reset_crtc();
            for (int bank = 0; bank < 4; bank++)
               reads[bank] = writes[bank] = ram + bank * 16384;
            cpc->snd_enabled = 0;
            cpc->cycle_count = 10000;
            cpu->PC.w.l = 0x1000; /* NOP, then acknowledge the pending interrupt. */
            cpu->SP.w.l = 0x8000;
            cpu->IFF1 = enabled ? Pflag : 0;
            cpu->IM = im;
            cpu->I = 0x40;
            cpu->int_pending = 1;
            asic->locked = locked;
            asic->irq_cause = 6;
            asic->irq_vector = 0xe0;
            asic->dma.dcsr = 0x25;
            unsigned vector = locked ? 0xff : 0xe6;
            ram[0x4000 + vector] = 0x34;
            ram[0x4001 + vector] = 0x12;
            cpu->break_point = !enabled ? 0x1001 : im == 2 ? 0x1234 : 0x38;
            assert(execute() == EC_BREAKPOINT);
            assert(asic->dma.dcsr == (enabled && !locked ? 0xa5 : 0x25));
            assert(cpu->PC.w.l == cpu->break_point);
            assert(cpu->SP.w.l == (enabled ? 0x7ffe : 0x8000));
            if (enabled) {
               assert(ram[0x7ffe] == 1 && ram[0x7fff] == 0x10);
               assert(!cpu->int_pending && !cpu->IFF1);
            }
         }
      }
   }
   puts("PASS: ASIC raster status acknowledged in IM0/IM1/IM2; masked and locked cases unchanged");
   dlclose(lib);
   return 0;
}
