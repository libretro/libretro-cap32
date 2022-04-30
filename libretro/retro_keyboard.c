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

#include "cap32.h"
#include "retro_events.h"
#include "retro_keyboard.h"
#include "assets/assets.h"
#include "gfx/software.h"
#include "gfx/video.h"

extern t_CPC CPC;
extern retro_mouse_t mouse;
extern uint32_t * keyboard_surface;

#define COMPOSE_CLEAN_COLOR retro_video.rgb2color(0, 0xd5, 0x2e)
#define COMPOSE_YELLOW_COLOR retro_video.rgb2color(0xe7, 0xea, 0)
#define COMPOSE_PRESS_COLOR retro_video.rgb2color(0xea, 0, 0x22)

#define BASE_X 2
#define BASE_Y (12 + ( EMULATION_SCREEN_HEIGHT - IMG_KEYBOARD_HEIGHT ))
#define KEY_HEIGHT 16
#define KEY_WIDTH 18
#define KEY_GAP 3
#define KEYBOARD_LED_SIZE 2

#define KEYBOARD_MAX_KEYS 79

typedef struct {
   // calc keyboard position on screen
   unsigned short top, left, bottom, right;
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
   { CPC_KEY_ESC  , 21 },
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

   { CPC_KEY_TAB  , 32},
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

   { CPC_KEY_CAPS_LOCK, 38 },
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

   { CPC_KEY_CONTROL, 38 },
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
      kbd_data[key].w = keyboard_positions[key].width * EMULATION_SCALE;
      kbd_data[key].h = KEY_HEIGHT;

      kbd_data[key].top = y;
      kbd_data[key].left = x;
      kbd_data[key].right = x + kbd_data[key].w;
      kbd_data[key].bottom = y + kbd_data[key].h;

      x = kbd_data[key].right + (KEY_GAP * EMULATION_SCALE);
   }
}

static unsigned char _get_key()
{

   for (int key = 0; key < KEYBOARD_MAX_KEYS; key++)
   {
      if (
         mouse.x > kbd_data[key].left && mouse.x < kbd_data[key].right &&
         mouse.y > kbd_data[key].top && mouse.y < kbd_data[key].bottom)
      {
         #ifdef DEBUG_KOS
         int y = ((kbd_data[key].top) - BASE_Y) + 14;
         printf("mouse: (%u,%u %u,%u) [%i=%u]\n", 
            kbd_data[key].left, y, kbd_data[key].w, kbd_data[key].h,
            key, kbd_data[key].value
         );
         
         draw_rect(
            keyboard_surface,
            kbd_data[key].left + 4,
            y,
            kbd_data[key].w,
            kbd_data[key].h,
            0xaaaaaa
         );
         #endif

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
         kbd_data[keys].left + (2 * EMULATION_SCALE),
         kbd_data[keys].top - ( EMULATION_SCREEN_HEIGHT - IMG_KEYBOARD_HEIGHT ) + 3,
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
