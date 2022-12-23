/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2021)
 *  Copyright Daniel De Matteis (2012-2021)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>
#include <libretro-core.h>
#include <microui.h>

#include "cart.h"
#include "retro_ui.h"
#include "gfx/software.h"
#include "gfx/video.h"
#include "retro_events.h"
#include "retro_render.h"
#include "retro_keyboard.h"
#include "assets/assets.h"

extern t_CPC CPC; 

extern uint32_t * video_buffer;
extern uint32_t * temp_buffer;

extern retro_mouse_t mouse;
extern computer_cfg_t retro_computer_cfg;

uint32_t * keyboard_surface = NULL;
uint32_t * keyboard_lang = NULL;
uint32_t * ui_surface = NULL;

static unsigned char ui_status = 0;
static char ui_string[32];
static mu_Context *ctx;

// VIDEO draw keyboard cached function
static void (*draw_keyboard)(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size);

// UI INTERNAL VALUES
#define INTERNAL_UI_KEYBOARD  2
#define INTERNAL_UI_MENU      4
#define INTERNAL_UI_STATUSBAR 8
#define INTERNAL_UI_LED       16

// UI STRING POSITION ON KEYBOARD
#define UI_STRING_X 75 * EMULATION_SCALE
#define UI_STRING_Y 2

void _render_window()
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

void _process_input()
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

void _process_frame()
{
   mu_begin(ctx);
   _render_window();
   mu_end(ctx);
}

void _process_commands(uint32_t * buffer)
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
static uint32_t cursor_color = 0;

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

void retro_ui_prepare(void)
{
   cursor_color = retro_video.cursor_color;
   draw_keyboard = retro_video.draw_keyboard_func;
   // convert KeyboardOnScreen to current video/color-depth
   convert_image(
      keyboard_surface,
      (const unsigned int *) ui_keyboard_bg,
      IMG_KEYBOARD_HEIGHT * IMG_KEYBOARD_WIDTH
   );

   unsigned int * ui_keyboard_lang = (unsigned int *) ui_keyboard_en;
   if (retro_computer_cfg.lang == 2) {
      ui_keyboard_lang = (unsigned int *) ui_keyboard_es;
   } else if(retro_computer_cfg.lang == 1) {
      ui_keyboard_lang = (unsigned int *) ui_keyboard_fr;
   }

   // convert raw keyboard to current video/color-depth 
   // and blit to keyboard_surface to optimize draw KoS
   convert_image(
      keyboard_lang,
      (const unsigned int *) ui_keyboard_lang,
      IMG_KEYBOARD_HEIGHT * IMG_KEYBOARD_WIDTH
   );
   draw_image_transparent(keyboard_surface, keyboard_lang, 0, 0, IMG_KEYBOARD_HEIGHT * IMG_KEYBOARD_WIDTH);
}

//////////////// PUBLIC
void retro_ui_update_text()
{
   retro_ui_prepare();

   char model[16];
   switch (retro_computer_cfg.model)
   {
      case 0: strncpy(model, "464", sizeof(model)); break;
      case 1: strncpy(model, "664", sizeof(model)); break;
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
      retro_video.rgb2color(0x2c, 0x2c, 0x2c)
      );
   draw_text(
      keyboard_surface,
      UI_STRING_X,
      UI_STRING_Y,
      ui_string,
      retro_video.rgb2color(0x63, 0x63, 0x63)
   );
}

void retro_ui_draw_db(void)
{
   // darker DSK if hash is unknown
   if (game_configuration.is_clean)
      return;

   draw_rect(
      keyboard_surface,
      8,
      UI_STRING_Y + 4,
      5 * EMULATION_SCALE,
      3,
      retro_video.rgb2color(0x3f, 0x3f, 0x3f)
      );

   draw_rect(
      keyboard_surface,
      10,
      UI_STRING_Y + 1,
      3 * EMULATION_SCALE,
      1,
      retro_video.rgb2color(0xcc, 0xcc, 0xcc)
      );
}

void retro_ui_init(void)
{
   // prepare pointer to surface data
   keyboard_surface = temp_buffer;
   keyboard_lang = temp_buffer + ((IMG_KEYBOARD_HEIGHT * IMG_KEYBOARD_WIDTH) * EMULATION_SCALE);
   ui_surface = temp_buffer + ((IMG_KEYBOARD_HEIGHT * IMG_KEYBOARD_WIDTH) * EMULATION_SCALE * 2);

   // init KoS
   keyboard_init();
   retro_ui_prepare();
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
      value
         ? retro_video.rgb2color(0xea, 0, 0x22)
         : retro_video.rgb2color(0x57, 0, 0x0d)
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
      draw_keyboard(
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

   _process_input();

   if(ui_status & INTERNAL_UI_MENU)
   {
      _process_frame();
      _process_commands(video_buffer);
   }

   if(ui_status & (INTERNAL_UI_STATUSBAR ^ 0xFF))
   {
      draw_char(video_buffer, mouse.x, mouse.y, FNT_MOUSE, cursor_color);
      ev_process_cursor();
   }
}
