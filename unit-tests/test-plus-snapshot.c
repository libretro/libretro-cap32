/* ROM-free SNA round-trip test. Compile with -Ilibretro-common/include
 * -Icap32, then run with a native shared core path (-ldl on Linux). */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <dlfcn.h>
#include "libretro.h"
#include "cap32.h"
#include "z80.h"
#include "asic.h"
#include "slots.h"
static struct retro_variable opts[100];
static int nopts;
static const char *model, *depth, *out;
static void logger(enum retro_log_level l, const char *s, ...) {
   (void)l;
   (void)s;
}
static bool env(unsigned cmd, void *p) {
   switch (cmd) {
   case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
      ((struct retro_log_callback *)p)->log = logger;
      return true;
   case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
   case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
   case RETRO_ENVIRONMENT_GET_CONTENT_DIRECTORY:
      *(const char **)p = out;
      return true;
   case RETRO_ENVIRONMENT_SET_VARIABLES: {
      struct retro_variable *v = p;
      for (; v->key; v++) {
         assert(nopts < 100);
         opts[nopts].key = v->key;
         char *s = strdup(strchr(v->value, ';') + 2);
         char *bar = strchr(s, '|');
         if (bar)
            *bar = 0;
         opts[nopts++].value = s;
      }
      return true;
   }
   case RETRO_ENVIRONMENT_GET_VARIABLE: {
      struct retro_variable *v = p;
      const char *s = NULL;
      for (int i = 0; i < nopts; i++)
         if (!strcmp(opts[i].key, v->key))
            s = opts[i].value;
      if (!strcmp(v->key, "cap32_model"))
         s = model;
      if (!strcmp(v->key, "cap32_gfx_colors"))
         s = depth;
      if (!strcmp(v->key, "cap32_statusbar"))
         s = "disabled";
      if (!strcmp(v->key, "cap32_retrojoy0"))
         s = "joystick_port1";
      v->value = s;
      return s != NULL;
   }
   case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
      *(bool *)p = false;
      return true;
   case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
      return true;
   default:
      return false;
   }
}

#define FN(name)                                                                                   \
   __typeof__(&name) name = dlsym(h, #name);                                                       \
   assert(name)
int main(int argc, char **argv) {
   assert(argc == 2);
   model = "6128+ (experimental)";
   depth = "24bit";
   out = ".";
   void *h = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
   assert(h);
   FN(retro_init);
   FN(retro_set_environment);
   FN(retro_load_game);
   FN(retro_deinit);
   FN(retro_serialize_size);
   FN(retro_serialize);
   FN(retro_unserialize);
   retro_set_environment(env);
   retro_init();
   assert(retro_load_game(NULL));
   t_CPC *c = dlsym(h, "CPC");
   t_CRTC *crtc = dlsym(h, "CRTC");
   t_asic *a = dlsym(h, "asic");
   t_GateArray *ga = dlsym(h, "GateArray");
   uint8_t **ram = dlsym(h, "pbRAM");
   uint8_t **page = dlsym(h, "pbRegisterPage");
   bool (*write_reg)(uint16_t, uint8_t) = dlsym(h, "asic_register_page_write");
   void (*portout)(reg_pair, uint8_t) = dlsym(h, "z80_OUT_handler");
   void (*reset)(bool) = dlsym(h, "emulator_reset");
   assert(c && crtc && a && ga && ram && page && write_reg && portout && reset);
   for (int mode = 0; mode < 4; mode++) {
      c->model = CPC_MODEL_PLUS;
      c->ram_size = 128;
      reset(false);
      a->locked = false;
      for (unsigned i = 0; i < 4096; i++)
         write_reg(0x4000 + i, (i * 7 + i / 16) & 15);
      for (unsigned i = 0; i < 16; i++) {
         write_reg(0x6000 + i * 8, 31 + i);
         write_reg(0x6001 + i * 8, i & 3);
         write_reg(0x6002 + i * 8, 57 + i);
         write_reg(0x6003 + i * 8, i & 1);
         write_reg(0x6004 + i * 8, i);
      }
      for (unsigned i = 0; i < 64; i++)
         write_reg(0x6400 + i, i * 3);
      for (unsigned i = 0; i < 6; i++)
         write_reg(0x6800 + i, 17 + i * 3);
      reg_pair port;
      port.w.l = 0x7f00;
      portout(port, 0xa0 + mode * 8 + 3);
      a->locked = mode & 1;
      a->lock_seq_pos = mode * 5;
      a->lock_prev_data = 0x77;
      a->raster_interrupt = true;
      a->irq_cause = 4;
      a->irq_vector = 0xec;
      a->dma.dcsr = 0xa5; /* STOP/acknowledge can leave different latched bits. */
      for (int i = 0; i < 3; i++) {
         a->dma.ch[i].source_address = 0x3100 + i * 0x102;
         a->dma.ch[i].loop_address = 0x2900 + i * 2;
         a->dma.ch[i].prescaler = 17 + i;
         a->dma.ch[i].pause_ticks = 4095 - i;
         a->dma.ch[i].tick_cycles = 11 + i;
         a->dma.ch[i].loops = 2039 + i;
         a->dma.ch[i].enabled = i != 1;
         a->dma.ch[i].interrupt = i == 1;
      }
      crtc->sl_count = 287;
      crtc->raster_interrupt_delay = 9;
      for (unsigned i = 0; i < 128 * 1024; i++)
         (*ram)[i] = (i * 3 + i / 256) & 255;
      t_asic before = *a;
      uint32_t palette[32];
      memcpy(palette, ga->palette, sizeof(palette));
      unsigned bank = ga->lower_ROM_bank, page_on = ga->registerPageOn;
      uint8_t mirrors[128];
      memcpy(mirrors, *page + 0x2000, 128);
      size_t size = retro_serialize_size();
      uint8_t *s = malloc(size + 16);
      assert(s);
      memset(s, 0xcc, size + 16);
      assert(!retro_serialize(s, size - 1));
      assert(retro_serialize(s, size));
      for (unsigned i = 0; i < 16; i++)
         assert(s[size + i] == 0xcc);
      assert(s[0x6d] == 4);
      assert(!memcmp(s + 256 + 128 * 1024, "CPC+", 4));
      assert(retro_unserialize(s, size));
      assert(!memcmp(a, &before, sizeof(before)));
      assert(!memcmp(ga->palette, palette, sizeof(palette)));
      assert(ga->lower_ROM_bank == bank && ga->registerPageOn == page_on);
      assert(crtc->sl_count == 287 && crtc->raster_interrupt_delay == 9);
      assert(crtc->interrupt_sl == 17 && crtc->split_sl == 20 && crtc->split_addr == 0x171a);
      assert(!memcmp(mirrors, *page + 0x2000, 128));
      for (unsigned i = 0; i < 128 * 1024; i++)
         assert((*ram)[i] == ((i * 3 + i / 256) & 255));
      /* Truncated/oversized chunks must fail without changing the running ASIC. */
      assert(!retro_unserialize(s, size - 1));
      assert(!memcmp(a, &before, sizeof(before)));
      unsigned off = 256 + 128 * 1024;
      s[off + 7] = 255;
      assert(!retro_unserialize(s, size));
      assert(!memcmp(a, &before, sizeof(before)));
      s[off + 7] = 0;
      /* Standard CPC+ chunks also work without our continuation extension. */
      assert(retro_unserialize(s, size - 24));
      assert(a->sprites_x[7] == before.sprites_x[7]);
      assert(a->dma.ch[1].interrupt && !a->dma.ch[1].enabled);
      memcpy(s + size, "TEST", 4);
      memset(s + size + 4, 0, 4);
      assert(retro_unserialize(s, size + 8));
      s[0x6d] = 3;
      assert(retro_unserialize(s, 256 + 128 * 1024));
      assert(c->model == CPC_MODEL_PLUS);
      free(s);
   }
   puts("PASS: Plus sprites/palette/split/scroll/DMA/IRQs/ROM banks/lock state, portable chunks, "
        "bounds");
   for (unsigned model_id = 0; model_id < 3; model_id++)
      for (int k = 0; k < 3; k++) {
         unsigned sizes[] = {64, 128, 576};
         c->model = model_id;
         c->ram_size = sizes[k];
         reset(false);
         size_t size = retro_serialize_size();
         assert(size == 256 + sizes[k] * 1024);
         uint8_t *s = malloc(size);
         assert(retro_serialize(s, size));
         c->model = CPC_MODEL_PLUS;
         assert(retro_unserialize(s, size));
         assert(c->model == model_id);
         assert(c->ram_size == sizes[k]);
         s[0x6b] = 0xff;
         s[0x6c] = 0xff;
         assert(!retro_unserialize(s, size));
         free(s);
      }
   puts("PASS: classic CPC464/664/6128 states at 64/128/576K, model switching, RAM bounds");
   retro_deinit();
   for (int i = 0; i < nopts; i++)
      free((void *)opts[i].value);
   dlclose(h);
   return 0;
}
