/* Compile with -Ilibretro-common/include -Ilibretro -Icap32 (and -ldl on
 * Linux). Run against a shared core exporting its internal test symbols. */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <dlfcn.h>
#include "libretro.h"
#include "libretro-core.h"
#include "retro_ui.h"
#include "gfx/video.h"
#include "retro_gun.h"
#include "lightgun/lightgun.h"

static void logger(enum retro_log_level level, const char *format, ...) {}

static int16_t input(unsigned port, unsigned device, unsigned index, unsigned id)
{
   if (device == RETRO_DEVICE_MOUSE)
      return id == RETRO_DEVICE_ID_MOUSE_LEFT ? 1 : 0;
   if (device != RETRO_DEVICE_LIGHTGUN)
      return 0;
   return id == RETRO_DEVICE_ID_LIGHTGUN_TRIGGER ? 1 : 0;
}

int main(int argc, char **argv)
{
   assert(argc == 2);
   void *core = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
   assert(core);
   t_lightgun *guns = dlsym(core, "gun");
   t_lightgun_cfg *cfg = dlsym(core, "lightgun_cfg");
   void (*set_ui)(retro_commands_ui_t, bool) = dlsym(core, "retro_ui_set_status");
   void (*update)(void) = dlsym(core, "ev_lightgun");
   void (*set_device)(unsigned, unsigned) = dlsym(core, "retro_set_controller_port_device");
   retro_input_state_t *callback = dlsym(core, "input_state_cb");
   assert(guns && cfg && set_ui && update && set_device && callback);
   *callback = input;
   *(retro_log_printf_t *)dlsym(core, "log_cb") = logger;
   retro_video_t *video = dlsym(core, "retro_video");
   assert(video);
   video->depth = DEPTH_16BPP;
   video->screen_render_width = 768;
   video->screen_render_height = 272;
   cfg->guntype = LIGHTGUN_TYPE_GUNSTICK;
   set_device(0, RETRO_DEVICE_AMSTRAD_LIGHTGUN);
   set_ui(UI_STATUSBAR, true);
   update();
   assert(guns[0].pressed);
   set_ui(UI_KEYBOARD, true);
   update();
   assert(!guns[0].pressed && guns[0].x == 0xfff && guns[0].y == 0xfff);
   set_ui(UI_MENU, true);
   set_ui(UI_KEYBOARD, false);
   update();
   assert(!guns[0].pressed);
   set_ui(UI_MENU, false);
   update();
   assert(guns[0].pressed);
   puts("PASS: OSK/menu consumes gun/mouse input; status bar and closed UI allow shooting");
   return 0;
}
