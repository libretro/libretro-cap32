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

#include "retro_events.h"
#include "retro_keyboard.h"
#include "assets/ui.h"
#include "gfx/software.h"

extern PIXEL_TYPE video_buffer[WINDOW_MAX_SIZE];
extern PIXEL_TYPE bmp[WINDOW_MAX_SIZE];
extern retro_mouse_t mouse;
extern PIXEL_TYPE * keyboard_surface;

#define COMPOSE_CLEAN_COLOR RGB2COLOR(0, 0xd5, 0x2e)
#define COMPOSE_YELLOW_COLOR RGB2COLOR(0xe7, 0xea, 0)
#define COMPOSE_PRESS_COLOR RGB2COLOR(0xea, 0, 0x22)

#define BASE_X 3
#define BASE_Y 12 + ( EMULATION_SCREEN_HEIGHT - IMG_KEYBOARD_HEIGHT )
#define KEY_HEIGHT 16
#define KEY_WIDTH 18
#define KEY_GAP 3
#define KEYBOARD_LED_SIZE 2

#define KEYBOARD_MAX_KEYS 79

typedef struct {
   // calc keyboard position on screen
   unsigned short x, y;
   unsigned short w, h;
   // keyboard internal value
   unsigned char value;
} retro_keyboard;

typedef struct {
   unsigned char value;
   unsigned char width;
} base_keyboard_t;

static retro_keyboard kbd_data [KEYBOARD_MAX_KEYS];
static base_keyboard_t keyboard_positions [KEYBOARD_MAX_KEYS] = {
   { CPC_KEY_ESC  , 20 },
   { CPC_KEY_1    , KEY_WIDTH },
   { CPC_KEY_2    , KEY_WIDTH },
   { CPC_KEY_3    , KEY_WIDTH },
   { CPC_KEY_4    , KEY_WIDTH },
   { CPC_KEY_5    , KEY_WIDTH },
   { CPC_KEY_6    , KEY_WIDTH },
   { CPC_KEY_7    , KEY_WIDTH },
   { CPC_KEY_8    , KEY_WIDTH },
   { CPC_KEY_9    , KEY_WIDTH },
   { CPC_KEY_ZERO , KEY_WIDTH },
   { CPC_KEY_MINUS, KEY_WIDTH },
   { CPC_KEY_HAT  , KEY_WIDTH },
   { CPC_KEY_CLR  , KEY_WIDTH },
   { CPC_KEY_DEL  , 19 },
   { CPC_KEY_F7   , KEY_WIDTH },
   { CPC_KEY_F8   , KEY_WIDTH },
   { CPC_KEY_F9   , KEY_WIDTH },
   { CPC_KEY_NULL , 0 }, // next line

   { CPC_KEY_TAB  , 31},
   { CPC_KEY_Q    , KEY_WIDTH },
   { CPC_KEY_W    , KEY_WIDTH },
   { CPC_KEY_E    , KEY_WIDTH },
   { CPC_KEY_R    , KEY_WIDTH },
   { CPC_KEY_T    , KEY_WIDTH },
   { CPC_KEY_Y    , KEY_WIDTH },
   { CPC_KEY_U    , KEY_WIDTH },
   { CPC_KEY_I    , KEY_WIDTH },
   { CPC_KEY_O    , KEY_WIDTH },
   { CPC_KEY_P    , KEY_WIDTH },
   { CPC_KEY_AT   , KEY_WIDTH },
   { CPC_KEY_OPEN_SQUARE_BRACKET , KEY_WIDTH },
   { CPC_KEY_RETURN, 29 },
   { CPC_KEY_F4   , KEY_WIDTH },
   { CPC_KEY_F5   , KEY_WIDTH },
   { CPC_KEY_F6   , KEY_WIDTH },
   { CPC_KEY_NULL , 0 }, // next line

   { CPC_KEY_CAPS_LOCK, 37 },
   { CPC_KEY_A    , KEY_WIDTH },
   { CPC_KEY_S    , KEY_WIDTH },
   { CPC_KEY_D    , KEY_WIDTH },
   { CPC_KEY_F    , KEY_WIDTH },
   { CPC_KEY_G    , KEY_WIDTH },
   { CPC_KEY_H    , KEY_WIDTH },
   { CPC_KEY_J    , KEY_WIDTH },
   { CPC_KEY_K    , KEY_WIDTH },
   { CPC_KEY_L    , KEY_WIDTH },
   { CPC_KEY_COLON, KEY_WIDTH },
   { CPC_KEY_SEMICOLON, KEY_WIDTH },
   { CPC_KEY_CLOSE_SQUARE_BRACKET, KEY_WIDTH },
   { CPC_KEY_RETURN, 23 },
   { CPC_KEY_F1   , KEY_WIDTH },
   { CPC_KEY_F2   , KEY_WIDTH },
   { CPC_KEY_F3   , KEY_WIDTH },
   { CPC_KEY_NULL , 0 }, // next line

   { CPC_KEY_SHIFT, 48 },
   { CPC_KEY_Z    , KEY_WIDTH },
   { CPC_KEY_X    , KEY_WIDTH },
   { CPC_KEY_C    , KEY_WIDTH },
   { CPC_KEY_V    , KEY_WIDTH },
   { CPC_KEY_B    , KEY_WIDTH },
   { CPC_KEY_N    , KEY_WIDTH },
   { CPC_KEY_M    , KEY_WIDTH },
   { CPC_KEY_COMMA, KEY_WIDTH },
   { CPC_KEY_DOT  , KEY_WIDTH },
   { CPC_KEY_BACKSLASH     , KEY_WIDTH },
   { CPC_KEY_FORWARD_SLASH , KEY_WIDTH },
   { CPC_KEY_SHIFT, 33 },
   { CPC_KEY_F0        , KEY_WIDTH },
   { CPC_KEY_CURSOR_UP , KEY_WIDTH },
   { CPC_KEY_FDOT      , KEY_WIDTH },
   { CPC_KEY_NULL , 0 }, // next line

   { CPC_KEY_CONTROL, 37 },
   { CPC_KEY_COPY, 36 },
   { CPC_KEY_SPACE, 168 },
   { CPC_KEY_INTRO, 65 },
   { CPC_KEY_CURSOR_LEFT , KEY_WIDTH },
   { CPC_KEY_CURSOR_DOWN , KEY_WIDTH },
   { CPC_KEY_CURSOR_RIGHT, KEY_WIDTH },
};

void keyboard_init()
{
   int x = BASE_X * EMULATION_SCALE, y = BASE_Y;
   for (int key = 0; key < KEYBOARD_MAX_KEYS; key++)
   {
      if( keyboard_positions[key].value == CPC_KEY_NULL)
      {
         x = BASE_X * EMULATION_SCALE;
         y = y + KEY_HEIGHT + KEY_GAP;
         continue;
      }

      kbd_data[key].value = keyboard_positions[key].value;
      kbd_data[key].x = x;
      kbd_data[key].y = y;
      kbd_data[key].w = x + (keyboard_positions[key].width * EMULATION_SCALE);
      kbd_data[key].h = y + KEY_HEIGHT;

      x = kbd_data[key].w + (KEY_GAP * EMULATION_SCALE);
   }
}

static unsigned char _get_key()
{
   for (int key = 0; key < KEYBOARD_MAX_KEYS; key++)
   {
      if (
         mouse.x > kbd_data[key].x && mouse.x < kbd_data[key].w &&
         mouse.y > kbd_data[key].y && mouse.y < kbd_data[key].h)
      {
         //printf("mouse: (%u,%u) (%i=%u)\n", mouse.x, mouse.y, key, kbd_data[key].value);
         return kbd_data[key].value;
      }
   }

   return CPC_KEY_NULL;
}

static void _draw_compose_led (unsigned char key, bool clicked)
{
   int keys = KEYBOARD_MAX_KEYS;
   while(keys--)
   {
      if (kbd_data[keys].value != key)
         continue;

      draw_rect(
         keyboard_surface,
         kbd_data[keys].x + (2 * EMULATION_SCALE),
         kbd_data[keys].y - ( EMULATION_SCREEN_HEIGHT - IMG_KEYBOARD_HEIGHT ) + 3,
         KEYBOARD_LED_SIZE * EMULATION_SCALE,
         KEYBOARD_LED_SIZE,
         clicked ? COMPOSE_PRESS_COLOR : COMPOSE_CLEAN_COLOR
      );
   }
}

void keyboard_mouse_click(bool clicked)
{
   static unsigned char last_key = CPC_KEY_NULL;
   static unsigned char composed_key = CPC_KEY_NULL;

   if (clicked)
   {
      last_key = _get_key();

      if (last_key == CPC_KEY_NULL)
         return;
      
      if (
         composed_key == CPC_KEY_NULL && 
         (
            last_key == CPC_KEY_SHIFT ||
            last_key == CPC_KEY_CONTROL
         ))
      {
         composed_key = last_key;
         _draw_compose_led(composed_key, true);
      }
      // composed keys are released on the next click
      else if (composed_key != CPC_KEY_NULL)
      {
         ev_release_key(composed_key);
         _draw_compose_led(composed_key, false);
         composed_key = CPC_KEY_NULL;
      }

      ev_press_key(last_key);
   }
   else
   {
      if (last_key == CPC_KEY_NULL)
         return;
      
      if (composed_key != CPC_KEY_NULL)
         return;

      ev_release_key(last_key);
      last_key = CPC_KEY_NULL;
   }
}
