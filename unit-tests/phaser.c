/* Standalone regression test; no ROMs or frontend required.
 * From the repository root:
 * cc -Ilibretro -Ilibretro-common/include -Icap32 -Icap32/lightgun unit-tests/phaser.c cap32/lightgun/phaser.c -o /tmp/cap32-phaser-test
 * /tmp/cap32-phaser-test
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cap32.h"
#include "lightgun.h"

t_CPC CPC;
t_CRTC CRTC;
t_VDU VDU;
t_lightgun gun;

void ev_lightgun(void) {}
void phaser_emulator_CRTC(void);
void phaser_emulator_OUT(void);

static void check_hit(unsigned depth, int x, int y, gun_state state, int hit)
{
   uint32_t pixels[1024];
   memset(&CRTC, 0, sizeof(CRTC));
   CPC.scr_bpp = depth;
   CPC.scr_base = pixels;
   CPC.scr_pos = (uint32_t *)((uint8_t *)pixels + 500 * (depth / 8));
   VDU.scrln = 100;
   CRTC.addr = 0x1000;
   CRTC.char_count = 31;
   gun.x = x;
   gun.y = y;
   gun.state = state;
   phaser_emulator_CRTC();
   unsigned address = (CRTC.registers[16] << 8) | CRTC.registers[17];
   if (address != (hit ? 0x1023 : 0)) {
      fprintf(stderr, "%u bpp at (%d,%d), state %d: got %04x, expected %04x\n",
         depth, x, y, state, address, hit ? 0x1023 : 0);
      exit(EXIT_FAILURE);
   }
}

int main(void)
{
   const unsigned depths[] = {16, 8, 32};
   for (unsigned i = 0; i < sizeof(depths) / sizeof(depths[0]); ++i) {
      unsigned depth = depths[i];
      check_hit(depth, 504, 100, GUN_SHOOT, 1);
      check_hit(depth, 500, 100, GUN_SHOOT, 1);
      check_hit(depth, 515, 101, GUN_SHOOT, 1);
      check_hit(depth, 499, 100, GUN_SHOOT, 0);
      check_hit(depth, 516, 100, GUN_SHOOT, 0);
      check_hit(depth, 504, 99, GUN_SHOOT, 0);
      check_hit(depth, 504, 102, GUN_SHOOT, 0);
      check_hit(depth, -1, -1, GUN_SHOOT, 0);
      check_hit(depth, 504, 100, GUN_PREPARE, 0);
      check_hit(depth, 504, 100, GUN_SLEEP, 0);
   }
   gun.state = GUN_PREPARE;
   CRTC.registers[17] = 10;
   phaser_emulator_OUT();
   if (CRTC.registers[17] != 11) return EXIT_FAILURE;
   gun.state = GUN_SHOOT;
   phaser_emulator_OUT();
   if (CRTC.registers[17] != 11) return EXIT_FAILURE;
   puts("PASS: Phaser hits, boundaries, offscreen and trigger state at 8/16/32 bpp");
   return EXIT_SUCCESS;
}
