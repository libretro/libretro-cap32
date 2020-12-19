/* Copyright (C) 2020
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>
#include <libretro-core.h>
#include <microui.h>

#include "retro_ui.h"
#include "gfx/software.h"
#include "retro_events.h"
#include "retro_render.h"
#include "retro_keyboard.h"
#include "assets/ui.h"

extern PIXEL_TYPE video_buffer[WINDOW_MAX_SIZE];
extern PIXEL_TYPE bmp[WINDOW_MAX_SIZE];
extern retro_mouse_t mouse;
extern computer_cfg_t retro_computer_cfg;

PIXEL_TYPE * keyboard_surface = NULL;
PIXEL_TYPE * ui_surface = NULL;

static unsigned char ui_status = 0;
static char ui_string[32];
static mu_Context *ctx;

// UI INTERNAL VALUES
#define INTERNAL_UI_KEYBOARD  2
#define INTERNAL_UI_MENU      4
#define INTERNAL_UI_STATUSBAR 8
#define INTERNAL_UI_LED       16

// UI STRING POSITION ON KEYBOARD
#define UI_STRING_X 70 * EMULATION_SCALE
#define UI_STRING_Y 2

void _render_window(mu_Context *ctx)
{
   if (
      mu_begin_window_ex(
         ctx,
         "Tape Control",
         mu_rect(2, EMULATION_SCREEN_HEIGHT - 180, EMULATION_SCREEN_WIDTH / 4, 120),
         //MU_OPT_NOINTERACT
         MU_OPT_NOCLOSE
      )
   ) {
      mu_layout_row(ctx, 6, NULL, 0);

      if (mu_button(ctx, "REWIND")) {
         printf("Button1 pressed\n");
      }

      if (mu_button(ctx, "PLAY")) {
         mu_open_popup(ctx, "My Popup");
      }

      if (mu_begin_popup(ctx, "My Popup")) {
         mu_label(ctx, "Hello!!");
         mu_end_popup(ctx);
      }

      mu_end_window(ctx);
   }
}

void _process_click(int clicked)
{
   if (ui_status & INTERNAL_UI_KEYBOARD)
      keyboard_mouse_click(clicked);
   if (clicked)
      mu_input_mousedown(ctx, mouse.x, mouse.y, MU_MOUSE_LEFT);
   else
      mu_input_mouseup(ctx, mouse.x, mouse.y, MU_MOUSE_LEFT);
}

void _process_input(mu_Context *ctx)
{
   if(mouse.status == CURSOR_NONE)
      return;

   if (mouse.status & CURSOR_MOTION)
   {
      mu_input_mousemove(ctx, mouse.x, mouse.y);
   }

   if (mouse.status & CURSOR_CLICKED)
      _process_click(mouse.click);
   else if (mouse.status & CURSOR_CLICKED_JOY)
      _process_click(mouse.click_joy);

   mouse.status = CURSOR_NONE;
}

void _process_frame(mu_Context *ctx)
{
   mu_begin(ctx);
   _render_window(ctx);
   mu_end(ctx);
}

void _process_commands(PIXEL_TYPE * buffer, mu_Context *ctx)
{
   mu_Command *cmd = NULL;
   while (mu_next_command(ctx, &cmd)) {
      switch (cmd->type) {
         case MU_COMMAND_TEXT: render_text(buffer, cmd->text.str, cmd->text.pos, cmd->text.color); break;
         case MU_COMMAND_RECT: render_rect(buffer, cmd->rect.rect, cmd->rect.color); break;
         case MU_COMMAND_ICON: render_icon(buffer, cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
         case MU_COMMAND_CLIP: break;
      }
   }
}

#define STATUSBAR_TIMER_UPDATE_MS 120
#define STATUSBAR_MAX_HEIGHT 11
static int statusbar_timer = 0;
static int statusbar_height = 0;

void _process_statusbar()
{
   draw_image_linear(
         video_buffer,
         keyboard_surface,
         0,
         EMULATION_SCREEN_HEIGHT - statusbar_height,
         IMG_KEYBOARD_WIDTH * statusbar_height
   );

   if (!statusbar_timer)
   {
      statusbar_height --;
      if (!statusbar_height)
      {
         BIT_CLEAR(ui_status, UI_STATUSBAR);
      }
   }
}

void retro_show_statusbar()
{
   if (
      (ui_status & INTERNAL_UI_KEYBOARD) ||
      (retro_computer_cfg.statusbar == STATUSBAR_OFF)
   )
      return;

   statusbar_timer = STATUSBAR_TIMER_UPDATE_MS;
   statusbar_height = STATUSBAR_MAX_HEIGHT;

   BIT_SET(ui_status, UI_STATUSBAR);
}

void retro_ui_update_text()
{
   char model[16];
   switch (retro_computer_cfg.model)
   {
      case 0: strncpy(model, "464", sizeof(model)); break;
      case 3: strncpy(model, "6128+", sizeof(model)); break;
      default: strncpy(model, "6128", sizeof(model)); break;
   }

   snprintf(
      ui_string,
      sizeof(ui_string),
      "%s %uk",
      model,
      retro_computer_cfg.ram
   );

   draw_rect(
      keyboard_surface,
      UI_STRING_X,
      UI_STRING_Y,
      80 * EMULATION_SCALE,
      8,
      RGB2COLOR(0x2c, 0x2c, 0x2c)
      );
   draw_text(
      keyboard_surface,
      UI_STRING_X,
      UI_STRING_Y,
      ui_string,
      RGB2COLOR(0x63, 0x63, 0x63)
   );


}

//////////////// PUBLIC

void retro_ui_init(void)
{
   // prepare pointer to surface data
   keyboard_surface = bmp;
   ui_surface = bmp + ui_keyboard_size;

   // convert KeyboardOnScreen to current video/color-depth
   convert_image(
      keyboard_surface,
      (const unsigned int *) ui_keyboard_data,
      IMG_KEYBOARD_HEIGHT * IMG_KEYBOARD_WIDTH
   );

   // init KoS
   keyboard_init();
   retro_ui_update_text();

   // Micro UI init
   ctx = malloc(sizeof(mu_Context));
   
   mu_init(ctx);
   ctx->text_width = text_width;
   ctx->text_height = text_height;
}

void retro_ui_free(void)
{
   mu_end(ctx);
}

void retro_ui_set_status(retro_commands_ui_t status, bool value)
{
   if (value)
      BIT_ADD(ui_status, status);
   else
      BIT_CLEAR(ui_status, status);
}

;
void retro_ui_toggle_status(retro_commands_ui_t status)
{
   BIT_TOGGLE(ui_status, status);
}

void retro_ui_set_led(bool value)
{
   retro_show_statusbar();

   draw_rect(
      keyboard_surface,
      EMULATION_SCREEN_WIDTH - (15 * EMULATION_SCALE),
      4,
      7 * EMULATION_SCALE,
      3,
      value ? RGB2COLOR(0xea, 0, 0x22) : RGB2COLOR(0x57, 0, 0x0d)
   );
}

void retro_ui_process()
{
   if (
      statusbar_timer &&
      retro_computer_cfg.statusbar == STATUSBAR_HIDE
   )
      statusbar_timer --;

   if(!ui_status)
      return;

   if (ui_status & INTERNAL_UI_KEYBOARD)
   {
      draw_image_linear(
         video_buffer,
         keyboard_surface,
         0,
         EMULATION_SCREEN_HEIGHT - IMG_KEYBOARD_HEIGHT,
         IMG_KEYBOARD_WIDTH * IMG_KEYBOARD_HEIGHT
      );
   }
   else if (statusbar_height)
   {
      _process_statusbar();
   }

   _process_input(ctx);

   if(ui_status & INTERNAL_UI_MENU)
   {
      _process_frame(ctx);
      _process_commands(video_buffer, ctx);
   }

   if(ui_status & (INTERNAL_UI_STATUSBAR ^ 0xFF))
   {
      draw_char(video_buffer, mouse.x, mouse.y, 126, CURSOR_COLOR);
      ev_process_cursor();
   }

}
