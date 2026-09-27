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

#include "assets/assets.h"
#include "retro_events.h"
#include "gfx/video.h"
#include "retro_ui.h"
#include "lightgun/lightgun.h"
#include "retro_gun.h"
#include "retro_disk_control.h"

/**
 * TODO: input_state assignments just need it here,
               but i need to clean almost GUI code

retro_input_state_t input_cb;
static retro_input_poll_t poll_cb;

void retro_set_input_state(retro_input_state_t cb) { input_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { poll_cb = cb; }
*/

extern retro_input_poll_t input_poll_cb;
extern retro_input_state_t input_state_cb;
extern retro_environment_t environ_cb;
extern retro_mouse_t mouse;

extern bool kbd_runcmd;

extern void kbd_buf_feed(char *s);
extern bool kbd_buf_update();
extern void play_tape();
extern void stop_tape(void);
extern void Tape_Rewind(void);

extern uint8_t keyboard_matrix[16];
const uint8_t bit_values[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

#define MAX_CURSOR_X (EMULATION_SCREEN_WIDTH - (FNT_CHAR_WIDTH * retro_video.bytes))
#define MAX_CURSOR_Y (EMULATION_SCREEN_HEIGHT - FNT_CHAR_HEIGHT)

static uint8_t keyboard_translation[MAX_KEYSYMS];
unsigned int last_input[PORTS_NUMBER] = {0,0};
uint32_t padnum = 0;

bool (*ev_events)(void);
bool (*process_events)(void);
unsigned char (*process_ev_key)(int key, bool pressed);

static int autorun_delay = EMULATION_INIT_AUTORUNDELAY;
static int wait_computer = 1;

int event_call = EV_JOY;
void ev_toggle_call();

t_button_cfg btnPAD[MAX_PADPLAYERS];

const t_button_cfg cfgPAD[MAX_PADCFG] = {
   {{ // DEFAULT CFG ( AUTO )
   CPC_KEY_JOY_FIRE2,   // B
   CPC_KEY_SPACE,       // Y
   CPC_KEY_NULL,        // SELECT
   CPC_KEY_NULL,        // START
   CPC_KEY_JOY_UP,      // DUP
   CPC_KEY_JOY_DOWN,    // DDOWN
   CPC_KEY_JOY_LEFT,    // DLEFT
   CPC_KEY_JOY_RIGHT,   // DRIGHT
   CPC_KEY_JOY_FIRE1,   // A
   CPC_KEY_JOY_FIRE3,   // X
   //---------------------
   CPC_KEY_CONTROL,     // L
   CPC_KEY_INTRO,       // R
   CPC_KEY_F1,          // L2
   CPC_KEY_F2,          // R2
   }},
   {{ // KEYBOARD CFG ( QAOP )
   CPC_KEY_B,           // B
   CPC_KEY_Y,           // Y
   CPC_KEY_S,           // SELECT
   CPC_KEY_K,           // START
   CPC_KEY_Q,           // DUP
   CPC_KEY_A,           // DDOWN
   CPC_KEY_O,           // DLEFT
   CPC_KEY_P,           // DRIGHT
   CPC_KEY_SPACE,       // A
   CPC_KEY_N,           // X
   //---------------------
   CPC_KEY_CONTROL,     // L
   CPC_KEY_INTRO,       // R
   CPC_KEY_F1,          // L2
   CPC_KEY_F2,          // R2
   }},
   {{ // KEYBOARD CFG ( INCENTIVE )
   CPC_KEY_SPACE,       // B
   CPC_KEY_W,           // Y
   CPC_KEY_S,           // SELECT
   CPC_KEY_F,           // START
   CPC_KEY_CURSOR_UP,   // DUP
   CPC_KEY_CURSOR_DOWN, // DDOWN
   CPC_KEY_CURSOR_LEFT, // DLEFT
   CPC_KEY_CURSOR_RIGHT,// DRIGHT
   CPC_KEY_A,           // A
   CPC_KEY_C,           // X
   //---------------------
   CPC_KEY_P,           // L
   CPC_KEY_L,           // R
   CPC_KEY_R,           // L2
   CPC_KEY_U,           // R2
   }},
   {{ // JOYSTICK PLAYER 1 CFG
   CPC_KEY_JOY_FIRE2,   // B
   CPC_KEY_NULL,        // Y
   CPC_KEY_NULL,        // SELECT
   CPC_KEY_NULL,        // START
   CPC_KEY_JOY_UP,      // DUP
   CPC_KEY_JOY_DOWN,    // DDOWN
   CPC_KEY_JOY_LEFT,    // DLEFT
   CPC_KEY_JOY_RIGHT,   // DRIGHT
   CPC_KEY_JOY_FIRE1,   // A
   CPC_KEY_NULL,        // X
   //---------------------
   CPC_KEY_NULL,        // L
   CPC_KEY_NULL,        // R
   CPC_KEY_NULL,        // L2
   CPC_KEY_NULL,        // R2
   }},
   {{ // JOYSTICK PLAYER 2 CFG
   CPC_KEY_JOY2_FIRE2,  // B
   CPC_KEY_SPACE,       // Y
   CPC_KEY_S,           // SELECT
   CPC_KEY_J,           // START
   CPC_KEY_JOY2_UP,     // DUP
   CPC_KEY_JOY2_DOWN,   // DDOWN
   CPC_KEY_JOY2_LEFT,   // DLEFT
   CPC_KEY_JOY2_RIGHT,  // DRIGHT
   CPC_KEY_JOY2_FIRE1,  // A
   CPC_KEY_X,           // X
   //---------------------
   CPC_KEY_NULL,        // L
   CPC_KEY_NULL,        // R
   CPC_KEY_NULL,        // L2
   CPC_KEY_NULL,        // R2
   }},
};

// ---------------------------------------------

#define MAX_JOY_EVENT 11
static retro_combo_event_t events_combo[MAX_JOY_EVENT] =
{
   { RETRO_DEVICE_ID_JOYPAD_B,            // if you change this position, update JOY_EVENT_ID_B
      { EVENT_WRITE, "CAT\n", NULL } },
   { RETRO_DEVICE_ID_JOYPAD_Y,            // if you change this position, update JOY_EVENT_ID_Y
      { EVENT_WRITE, "|CPM\n", NULL } },
   { RETRO_DEVICE_ID_JOYPAD_A,
      { EVENT_WRITE, "RUN\"DISK\nRUN\"DISC\n", NULL } },
   { RETRO_DEVICE_ID_JOYPAD_X,
      { EVENT_WRITE, LOADER_TAPE_STR } },
   { RETRO_DEVICE_ID_JOYPAD_START,
      { EVENT_VKEYB, "VKEYB\n", NULL } },
   { RETRO_DEVICE_ID_JOYPAD_UP,
      { EVENT_WRITE, "1\nY\n", "PRESSED => 1/Y" } },
   { RETRO_DEVICE_ID_JOYPAD_DOWN,
      { EVENT_WRITE, "2\nN\n", "PRESSED => 2/N" } },
   { RETRO_DEVICE_ID_JOYPAD_LEFT,
      { EVENT_WRITE, "4\nS\n", "PRESSED => 4/S" } },
   { RETRO_DEVICE_ID_JOYPAD_RIGHT,
      { EVENT_WRITE, "3\nJ\n", "PRESSED => 3/J" } },
   { RETRO_DEVICE_ID_JOYPAD_L,
     { EVENT_DISK_PREV, "PREV", NULL} },
   { RETRO_DEVICE_ID_JOYPAD_R,
     { EVENT_DISK_NEXT, "NEXT", NULL} },
};

/**
 * ev_press_key:
 * using CPC keyboard matrix sets as pressed
 **/
void ev_press_key(uint8_t cpc_key) {
   keyboard_matrix[cpc_key >> 4] &= ~bit_values[cpc_key & 7]; // key is being held down
}

/**
 * ev_release_key:
 * using CPC keyboard matrix sets as released
 **/
void ev_release_key(uint8_t cpc_key) {
   keyboard_matrix[cpc_key >> 4] |= bit_values[cpc_key & 7]; // key has been released
}

/**
 * get_cpckey:
 * from RETROK_x converts to CPC_KEY_x
 **/
uint8_t get_cpckey (unsigned int keysym)
{
   if (keysym >= MAX_KEYSYMS) {
      return CPC_KEY_NULL;
   } else {
      return keyboard_translation[keysym];
   }
}

/**
 * ev_cursorjoy:
 * activate: if TRUE emulation on cursors is activated
 *            if FALSE emulation is disabled and cursors works normally
 *
 * Changes keyboard table to bind CPC joystick on cursors
 **/
void ev_cursorjoy() {
   static bool activate = false;
   activate ^= 1;
   if(activate) {
      keyboard_translation[RETROK_RCTRL] = CPC_KEY_JOY_FIRE1;
      keyboard_translation[RETROK_RSHIFT] = CPC_KEY_JOY_FIRE2;
      keyboard_translation[RETROK_UP] = CPC_KEY_JOY_UP;
      keyboard_translation[RETROK_DOWN] = CPC_KEY_JOY_DOWN;
      keyboard_translation[RETROK_LEFT] = CPC_KEY_JOY_LEFT;
      keyboard_translation[RETROK_RIGHT] = CPC_KEY_JOY_RIGHT;
   } else {
      keyboard_translation[RETROK_RCTRL] = CPC_KEY_CONTROL;
      keyboard_translation[RETROK_RSHIFT] = CPC_KEY_SHIFT;
      keyboard_translation[RETROK_UP] = CPC_KEY_CURSOR_UP;
      keyboard_translation[RETROK_DOWN] = CPC_KEY_CURSOR_DOWN;
      keyboard_translation[RETROK_LEFT] = CPC_KEY_CURSOR_LEFT;
      keyboard_translation[RETROK_RIGHT] = CPC_KEY_CURSOR_RIGHT;
   }
}

void ev_swap_media(int type) {
   unsigned int imagetotal = retro_get_num_images();
   unsigned int imageindex = retro_get_image_index();

   // need at least two disks inserted.
   if (imagetotal < 2)
      return;

   if ((type == EVENT_DISK_PREV) && imageindex == 0)
      return;
   if ((type == EVENT_DISK_NEXT) && imageindex == (imagetotal - 1))
      return;

   if (!retro_set_eject_state(true))
      return;

   imageindex += type == EVENT_DISK_PREV
      ? -1
      : +1;

   if (!retro_set_image_index(imageindex))
      return;

   retro_set_eject_state(false);

   char msg[64];
   snprintf(msg, 64, "DISK #%u", imageindex);
   retro_message(msg);
}

/**
 * do_action:
 * @return: the retro_events_action_type.
 * generates event actions
 **/
static unsigned do_action(const retro_action_t* action)
{
   switch(action->type) {
      case EVENT_WRITE:
         // TODO: generate an internal command for this behaivor
         kbd_buf_feed((char*) action->kbd_buf);
         ev_set(EV_AUTO);
         break;
      case EVENT_VKEYB:
         ev_toggle_call();
         retro_ui_toggle_status(UI_KEYBOARD);
         break;
      case EVENT_GUI:
         retro_ui_toggle_status(UI_MENU);
         break;
      case EVENT_TAPE_ON:
         play_tape();
         break;
      case EVENT_TAPE_OFF:
         stop_tape();
         break;
      case EVENT_TAPE_REWIND:
         Tape_Rewind();
         break;
      case EVENT_CURSOR_JOY:
         ev_cursorjoy();
         break;
      case EVENT_DISK_NEXT:
      case EVENT_DISK_PREV:
         ev_swap_media(action->type);
         break;
   }

   if(action->message)
      retro_message(action->message);

   return action->type;
}

static unsigned vkeyboard_key = RETROK_F9;
static bool vkeyboard_key_down, vkeyboard_button_down;
static retro_input_state_t vkeyboard_input_cb;
static const retro_action_t vkeyboard_action = { EVENT_VKEYB, "VKEYB", NULL };

void ev_vkeyboard_set_key(const char *value)
{
   unsigned key = RETROK_F9;
   if (value) {
      key = RETROK_UNKNOWN;
      if (strcmp(value, "F9") == 0) key = RETROK_F9;
      if (strcmp(value, "BACKSPACE") == 0) key = RETROK_BACKSPACE;
      if (strcmp(value, "TAB") == 0) key = RETROK_TAB;
      if (strcmp(value, "CLEAR") == 0) key = RETROK_CLEAR;
      if (strcmp(value, "RETURN") == 0) key = RETROK_RETURN;
      if (strcmp(value, "PAUSE") == 0) key = RETROK_PAUSE;
      if (strcmp(value, "ESCAPE") == 0) key = RETROK_ESCAPE;
      if (strcmp(value, "SPACE") == 0) key = RETROK_SPACE;
      if (strcmp(value, "QUOTE") == 0) key = RETROK_QUOTE;
      if (strcmp(value, "COMMA") == 0) key = RETROK_COMMA;
      if (strcmp(value, "MINUS") == 0) key = RETROK_MINUS;
      if (strcmp(value, "PERIOD") == 0) key = RETROK_PERIOD;
      if (strcmp(value, "SLASH") == 0) key = RETROK_SLASH;
      if (strcmp(value, "0") == 0) key = RETROK_0;
      if (strcmp(value, "1") == 0) key = RETROK_1;
      if (strcmp(value, "2") == 0) key = RETROK_2;
      if (strcmp(value, "3") == 0) key = RETROK_3;
      if (strcmp(value, "4") == 0) key = RETROK_4;
      if (strcmp(value, "5") == 0) key = RETROK_5;
      if (strcmp(value, "6") == 0) key = RETROK_6;
      if (strcmp(value, "7") == 0) key = RETROK_7;
      if (strcmp(value, "8") == 0) key = RETROK_8;
      if (strcmp(value, "9") == 0) key = RETROK_9;
      if (strcmp(value, "SEMICOLON") == 0) key = RETROK_SEMICOLON;
      if (strcmp(value, "EQUALS") == 0) key = RETROK_EQUALS;
      if (strcmp(value, "LEFTBRACKET") == 0) key = RETROK_LEFTBRACKET;
      if (strcmp(value, "BACKSLASH") == 0) key = RETROK_BACKSLASH;
      if (strcmp(value, "RIGHTBRACKET") == 0) key = RETROK_RIGHTBRACKET;
      if (strcmp(value, "BACKQUOTE") == 0) key = RETROK_BACKQUOTE;
      if (strcmp(value, "a") == 0) key = RETROK_a;
      if (strcmp(value, "b") == 0) key = RETROK_b;
      if (strcmp(value, "c") == 0) key = RETROK_c;
      if (strcmp(value, "d") == 0) key = RETROK_d;
      if (strcmp(value, "e") == 0) key = RETROK_e;
      if (strcmp(value, "f") == 0) key = RETROK_f;
      if (strcmp(value, "g") == 0) key = RETROK_g;
      if (strcmp(value, "h") == 0) key = RETROK_h;
      if (strcmp(value, "i") == 0) key = RETROK_i;
      if (strcmp(value, "j") == 0) key = RETROK_j;
      if (strcmp(value, "k") == 0) key = RETROK_k;
      if (strcmp(value, "l") == 0) key = RETROK_l;
      if (strcmp(value, "m") == 0) key = RETROK_m;
      if (strcmp(value, "n") == 0) key = RETROK_n;
      if (strcmp(value, "o") == 0) key = RETROK_o;
      if (strcmp(value, "p") == 0) key = RETROK_p;
      if (strcmp(value, "q") == 0) key = RETROK_q;
      if (strcmp(value, "r") == 0) key = RETROK_r;
      if (strcmp(value, "s") == 0) key = RETROK_s;
      if (strcmp(value, "t") == 0) key = RETROK_t;
      if (strcmp(value, "u") == 0) key = RETROK_u;
      if (strcmp(value, "v") == 0) key = RETROK_v;
      if (strcmp(value, "w") == 0) key = RETROK_w;
      if (strcmp(value, "x") == 0) key = RETROK_x;
      if (strcmp(value, "y") == 0) key = RETROK_y;
      if (strcmp(value, "z") == 0) key = RETROK_z;
      if (strcmp(value, "DELETE") == 0) key = RETROK_DELETE;
      if (strcmp(value, "KP0") == 0) key = RETROK_KP0;
      if (strcmp(value, "KP1") == 0) key = RETROK_KP1;
      if (strcmp(value, "KP2") == 0) key = RETROK_KP2;
      if (strcmp(value, "KP3") == 0) key = RETROK_KP3;
      if (strcmp(value, "KP4") == 0) key = RETROK_KP4;
      if (strcmp(value, "KP5") == 0) key = RETROK_KP5;
      if (strcmp(value, "KP6") == 0) key = RETROK_KP6;
      if (strcmp(value, "KP7") == 0) key = RETROK_KP7;
      if (strcmp(value, "KP8") == 0) key = RETROK_KP8;
      if (strcmp(value, "KP9") == 0) key = RETROK_KP9;
      if (strcmp(value, "KP_PERIOD") == 0) key = RETROK_KP_PERIOD;
      if (strcmp(value, "KP_DIVIDE") == 0) key = RETROK_KP_DIVIDE;
      if (strcmp(value, "KP_MULTIPLY") == 0) key = RETROK_KP_MULTIPLY;
      if (strcmp(value, "KP_MINUS") == 0) key = RETROK_KP_MINUS;
      if (strcmp(value, "KP_PLUS") == 0) key = RETROK_KP_PLUS;
      if (strcmp(value, "KP_ENTER") == 0) key = RETROK_KP_ENTER;
      if (strcmp(value, "KP_EQUALS") == 0) key = RETROK_KP_EQUALS;
      if (strcmp(value, "UP") == 0) key = RETROK_UP;
      if (strcmp(value, "DOWN") == 0) key = RETROK_DOWN;
      if (strcmp(value, "RIGHT") == 0) key = RETROK_RIGHT;
      if (strcmp(value, "LEFT") == 0) key = RETROK_LEFT;
      if (strcmp(value, "INSERT") == 0) key = RETROK_INSERT;
      if (strcmp(value, "HOME") == 0) key = RETROK_HOME;
      if (strcmp(value, "END") == 0) key = RETROK_END;
      if (strcmp(value, "PAGEUP") == 0) key = RETROK_PAGEUP;
      if (strcmp(value, "PAGEDOWN") == 0) key = RETROK_PAGEDOWN;
      if (strcmp(value, "F1") == 0) key = RETROK_F1;
      if (strcmp(value, "F2") == 0) key = RETROK_F2;
      if (strcmp(value, "F3") == 0) key = RETROK_F3;
      if (strcmp(value, "F4") == 0) key = RETROK_F4;
      if (strcmp(value, "F5") == 0) key = RETROK_F5;
      if (strcmp(value, "F6") == 0) key = RETROK_F6;
      if (strcmp(value, "F7") == 0) key = RETROK_F7;
      if (strcmp(value, "F8") == 0) key = RETROK_F8;
      if (strcmp(value, "F10") == 0) key = RETROK_F10;
      if (strcmp(value, "F11") == 0) key = RETROK_F11;
      if (strcmp(value, "F12") == 0) key = RETROK_F12;
      if (strcmp(value, "F13") == 0) key = RETROK_F13;
      if (strcmp(value, "F14") == 0) key = RETROK_F14;
      if (strcmp(value, "F15") == 0) key = RETROK_F15;
      if (strcmp(value, "NUMLOCK") == 0) key = RETROK_NUMLOCK;
      if (strcmp(value, "CAPSLOCK") == 0) key = RETROK_CAPSLOCK;
      if (strcmp(value, "SCROLLOCK") == 0) key = RETROK_SCROLLOCK;
      if (strcmp(value, "RSHIFT") == 0) key = RETROK_RSHIFT;
      if (strcmp(value, "LSHIFT") == 0) key = RETROK_LSHIFT;
      if (strcmp(value, "RCTRL") == 0) key = RETROK_RCTRL;
      if (strcmp(value, "LCTRL") == 0) key = RETROK_LCTRL;
      if (strcmp(value, "RALT") == 0) key = RETROK_RALT;
      if (strcmp(value, "LALT") == 0) key = RETROK_LALT;
      if (strcmp(value, "RMETA") == 0) key = RETROK_RMETA;
      if (strcmp(value, "LMETA") == 0) key = RETROK_LMETA;
      if (strcmp(value, "LSUPER") == 0) key = RETROK_LSUPER;
      if (strcmp(value, "RSUPER") == 0) key = RETROK_RSUPER;
      if (strcmp(value, "MODE") == 0) key = RETROK_MODE;
      if (strcmp(value, "COMPOSE") == 0) key = RETROK_COMPOSE;
      if (strcmp(value, "HELP") == 0) key = RETROK_HELP;
      if (strcmp(value, "PRINT") == 0) key = RETROK_PRINT;
      if (strcmp(value, "SYSREQ") == 0) key = RETROK_SYSREQ;
      if (strcmp(value, "BREAK") == 0) key = RETROK_BREAK;
      if (strcmp(value, "MENU") == 0) key = RETROK_MENU;
      if (strcmp(value, "POWER") == 0) key = RETROK_POWER;
      if (strcmp(value, "EURO") == 0) key = RETROK_EURO;
      if (strcmp(value, "UNDO") == 0) key = RETROK_UNDO;
      if (strcmp(value, "OEM_102") == 0) key = RETROK_OEM_102;
   }
   if (key != vkeyboard_key) {
      if (get_cpckey(key) != CPC_KEY_NULL)
         ev_release_key(get_cpckey(key));
      vkeyboard_key_down = false;
      vkeyboard_key = key;
   }
}

void ev_vkeyboard_input_init(retro_input_state_t cb)
{
   vkeyboard_input_cb = cb;
   vkeyboard_key_down = vkeyboard_button_down = false;
}

int16_t ev_vkeyboard_input(unsigned port, unsigned device, unsigned index, unsigned id)
{
   /* Reserve the action so it cannot also reach CPC or on-screen UI input. */
   if (port == 0 && device == RETRO_DEVICE_JOYPAD && id == RETRO_DEVICE_ID_JOYPAD_L3)
      return 0;
   return vkeyboard_input_cb(port, device, index, id);
}

void ev_vkeyboard_poll(void)
{
   bool down = vkeyboard_input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3);
   if (down && !vkeyboard_button_down)
      do_action(&vkeyboard_action);
   vkeyboard_button_down = down;
}

/**
 * ev_events_joy:
 * generate the SELECT + JOYPAD_x result in screen/emulation
 *
 * TODO: add an help-screen in emulation screen?
 **/
bool _events_joy()
{
   static unsigned event = EVENT_NULL;
   unsigned n;

   if(!input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, retro_computer_cfg.combokey))
   {
      if(event)
      {
         event = EVENT_NULL;
         return true;
      }
      return false;
   }

   for(n = 0; n < MAX_JOY_EVENT; n++) {
      if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, events_combo[n].id)){
         if(event == EVENT_NULL){
            event = do_action(&events_combo[n].action);
         }
      }
   }
   return true;
}


/**
 * ev_events_null:
 * simple disabled event combokey
 **/
bool _events_null()
{
   return false;
}


/**
 * ev_process_joy:
 * @playerID: the player id (see DEVICE AMSTRAD)
 *
 * process joystick using input_state_cb to CPC/Keyboard
 **/
static void _process_joy(int playerID){

// evercade do not call to retro_set_controller_port_device avoid this check
#ifndef EVERCADE
   // is disabled?
   if ( (amstrad_devices[playerID]&RETRO_DEVICE_MASK) == RETRO_DEVICE_NONE)
      return;
#endif

   uint8_t * pad = (uint8_t*) &btnPAD[playerID].buttons;

   int i;
   for (i = 0; i < MAX_BUTTONS; i++) {
      if (input_state_cb(playerID, RETRO_DEVICE_JOYPAD, 0, i)) {
         if(!(BIT_CHECK(last_input[playerID], i))) {
            ev_press_key(*(pad+i));
            BIT_ADD(last_input[playerID], i);
         }
      }
      else if (BIT_CHECK(last_input[playerID], i)) {
         ev_release_key(*(pad+i));
         BIT_CLEAR(last_input[playerID], i);
      }
   }
}


/**
 * ev_joysticks:
 * function to unify event code, call joy events and get user pad data
 **/
bool ev_joysticks()
{
   // you cannot use key-remap on player two, force read it
   _process_joy(ID_PLAYER2);

   // exit on controllers config to RETRO_DEVICE_AMSTRAD_KEYBOARD
   // but allows legacy keyboard-remap + joystick-simple combo, issue #63
   if(amstrad_devices[0] == RETRO_DEVICE_AMSTRAD_KEYBOARD &&
      retro_computer_cfg.padcfg[ID_PLAYER1] != PADCFG_AUTO)
         return false;

   if(!ev_events())
      _process_joy(ID_PLAYER1);

   return true;
}

void ev_autorun_prepare(char * kbd_buffer)
{
   Tape_Rewind();

   kbd_buf_feed(kbd_buffer);
   ev_set(EV_AUTO);

   autorun_delay = EMULATION_INIT_AUTORUNDELAY;
   wait_computer = 1;
}

bool ev_autorun()
{
   if(autorun_delay)
   {
      autorun_delay --;
      return false;
   }
   
   // wait one loop for the key be pressed
   wait_computer ^= 1;
   if(wait_computer)
      return false;

   if(kbd_buf_update())
   {
      ev_set(EV_JOY);

      // prepare next autorun
      autorun_delay = EMULATION_INIT_AUTORUNDELAY;
      wait_computer = 1;
   }

   return true;
}

//-----------------------------------------------------

#define MAX_KEY_EVENT 5
const retro_combo_event_t keyb_events[MAX_KEY_EVENT] =
{
   { RETROK_F10,
      { EVENT_GUI, "GUI", NULL} },
   { RETROK_HOME,
      { EVENT_TAPE_ON, "PLAY TAPE", "TAPE => PLAY PRESSED" } },
   { RETROK_END,
      { EVENT_TAPE_OFF, "STOP TAPE", "TAPE => STOP PRESSED" } },
   { RETROK_PAGEUP,
      { EVENT_TAPE_REWIND, "REW TAPE", "TAPE => REWIND PRESSED" } },
   { RETROK_INSERT,
      { EVENT_CURSOR_JOY, "SWITCH CURSOR", "SWITCHED CURSOR/JOY" } },
};


/**
 * ev_events_key:
 * @keycode: bind code
 * @down: is key pressed?
 *
 * check special keyboard events and generates his event/actions
 **/
static void ev_events_key(unsigned keycode, bool down)
{
   static unsigned event_last = 0;
   int n;

   if(!down)
   {
      event_last = EVENT_NULL;
      return;
   }

   for(n = 0; n < MAX_KEY_EVENT; n++) {
      if (keyb_events[n].id == keycode){
         if(!event_last)
            event_last = do_action(&keyb_events[n].action);
         return;
      }
   }
}


/**
 * process_ev_key:
 * emulator keyboard handler
 **/
unsigned char _ev_key(int key, bool pressed) {
   uint8_t cpc_key = get_cpckey(key);
   if (cpc_key != CPC_KEY_NULL) {
      if (pressed)
         ev_press_key(cpc_key);
      else
         ev_release_key(cpc_key);
      return cpc_key;
   }
   return CPC_KEY_NULL;
}

/**
 * _ev_key_null:
 * Here we to try to avoid ghosting bug on old retroarch versions
 * issue: https://github.com/libretro/RetroArch/issues/8838
 **/
unsigned char _ev_key_null(int key, bool pressed) {
   return CPC_KEY_NULL;
}

/**
 * keyboard_cb:
 * Is called by main retro_callback when user press/release a key
 * just convert this event to an emulated event
 **/
static void keyboard_cb(bool down, unsigned keycode, uint32_t character, uint16_t mod)
{
   //printf( "Down: %s, Code: %d, Char: %u, Mod: %u.\n",
   //       down ? "yes" : "no", keycode, character, mod);

   if (vkeyboard_key != RETROK_UNKNOWN && keycode == vkeyboard_key) {
      if (down && !vkeyboard_key_down)
         do_action(&vkeyboard_action);
      vkeyboard_key_down = down;
      return;
   }

   if(process_ev_key(keycode, down) != CPC_KEY_NULL)
      return;

   ev_events_key(keycode, down);
}

/**
 * init_joystick_table:
 * prepare joystick table using internal JOY enums
 * allows remaps using joy events
 **/
void init_joystick_table() {
   keyboard_translation[JOY_PORT1_UP] = CPC_KEY_JOY_UP;
   keyboard_translation[JOY_PORT1_DOWN] = CPC_KEY_JOY_DOWN;
   keyboard_translation[JOY_PORT1_RIGHT] = CPC_KEY_JOY_RIGHT;
   keyboard_translation[JOY_PORT1_LEFT] = CPC_KEY_JOY_LEFT;
   keyboard_translation[JOY_PORT1_FIRE1] = CPC_KEY_JOY_FIRE1;
   keyboard_translation[JOY_PORT1_FIRE2] = CPC_KEY_JOY_FIRE2;
   keyboard_translation[JOY_PORT2_UP] = CPC_KEY_JOY2_UP;
   keyboard_translation[JOY_PORT2_DOWN] = CPC_KEY_JOY2_DOWN;
   keyboard_translation[JOY_PORT2_RIGHT] = CPC_KEY_JOY2_RIGHT;
   keyboard_translation[JOY_PORT2_LEFT] = CPC_KEY_JOY2_LEFT;
   keyboard_translation[JOY_PORT2_FIRE1] = CPC_KEY_JOY2_FIRE1;
   keyboard_translation[JOY_PORT2_FIRE2] = CPC_KEY_JOY2_FIRE2;
}

/**
 * init_keyboard_table:
 * prepare keyboard table using RETROK_x enums
 * assignments F1/F2/... are emulated only on KEYPAD, add to Fx?
 **/
void init_keyboard_table() {
   memset(keyboard_translation, CPC_KEY_NULL, MAX_KEYSYMS );

   keyboard_translation[RETROK_0] = CPC_KEY_ZERO;
   keyboard_translation[RETROK_1] = CPC_KEY_1;
   keyboard_translation[RETROK_2] = CPC_KEY_2;
   keyboard_translation[RETROK_3] = CPC_KEY_3;
   keyboard_translation[RETROK_4] = CPC_KEY_4;
   keyboard_translation[RETROK_5] = CPC_KEY_5;
   keyboard_translation[RETROK_6] = CPC_KEY_6;
   keyboard_translation[RETROK_7] = CPC_KEY_7;
   keyboard_translation[RETROK_8] = CPC_KEY_8;
   keyboard_translation[RETROK_9] = CPC_KEY_9;
   keyboard_translation[RETROK_a] = CPC_KEY_A;
   keyboard_translation[RETROK_b] = CPC_KEY_B;
   keyboard_translation[RETROK_c] = CPC_KEY_C;
   keyboard_translation[RETROK_d] = CPC_KEY_D;
   keyboard_translation[RETROK_e] = CPC_KEY_E;
   keyboard_translation[RETROK_f] = CPC_KEY_F;
   keyboard_translation[RETROK_g] = CPC_KEY_G;
   keyboard_translation[RETROK_h] = CPC_KEY_H;
   keyboard_translation[RETROK_i] = CPC_KEY_I;
   keyboard_translation[RETROK_j] = CPC_KEY_J;
   keyboard_translation[RETROK_k] = CPC_KEY_K;
   keyboard_translation[RETROK_l] = CPC_KEY_L;
   keyboard_translation[RETROK_m] = CPC_KEY_M;
   keyboard_translation[RETROK_n] = CPC_KEY_N;
   keyboard_translation[RETROK_o] = CPC_KEY_O;
   keyboard_translation[RETROK_p] = CPC_KEY_P;
   keyboard_translation[RETROK_q] = CPC_KEY_Q;
   keyboard_translation[RETROK_r] = CPC_KEY_R;
   keyboard_translation[RETROK_s] = CPC_KEY_S;
   keyboard_translation[RETROK_t] = CPC_KEY_T;
   keyboard_translation[RETROK_u] = CPC_KEY_U;
   keyboard_translation[RETROK_v] = CPC_KEY_V;
   keyboard_translation[RETROK_w] = CPC_KEY_W;
   keyboard_translation[RETROK_x] = CPC_KEY_X;
   keyboard_translation[RETROK_y] = CPC_KEY_Y;
   keyboard_translation[RETROK_z] = CPC_KEY_Z;
   keyboard_translation[RETROK_SPACE] = CPC_KEY_SPACE;
   keyboard_translation[RETROK_COMMA] = CPC_KEY_COMMA;
   keyboard_translation[RETROK_PERIOD] = CPC_KEY_DOT;
   keyboard_translation[RETROK_SEMICOLON] = CPC_KEY_COLON;
   keyboard_translation[RETROK_MINUS] = CPC_KEY_MINUS;
   keyboard_translation[RETROK_EQUALS] = CPC_KEY_HAT;
   keyboard_translation[RETROK_LEFTBRACKET] = CPC_KEY_AT;
   keyboard_translation[RETROK_RIGHTBRACKET] =CPC_KEY_OPEN_SQUARE_BRACKET;

   keyboard_translation[RETROK_TAB] = CPC_KEY_TAB;
   keyboard_translation[RETROK_RETURN] = CPC_KEY_RETURN;
   keyboard_translation[RETROK_BACKSPACE] = CPC_KEY_DEL;
   keyboard_translation[RETROK_ESCAPE] = CPC_KEY_ESC;

   keyboard_translation[RETROK_UP] = CPC_KEY_CURSOR_UP;
   keyboard_translation[RETROK_DOWN] = CPC_KEY_CURSOR_DOWN;
   keyboard_translation[RETROK_LEFT] = CPC_KEY_CURSOR_LEFT;
   keyboard_translation[RETROK_RIGHT] = CPC_KEY_CURSOR_RIGHT;

   keyboard_translation[RETROK_KP0] = CPC_KEY_F0;
   keyboard_translation[RETROK_KP1] = CPC_KEY_F1;
   keyboard_translation[RETROK_KP2] = CPC_KEY_F2;
   keyboard_translation[RETROK_KP3] = CPC_KEY_F3;
   keyboard_translation[RETROK_KP4] = CPC_KEY_F4;
   keyboard_translation[RETROK_KP5] = CPC_KEY_F5;
   keyboard_translation[RETROK_KP6] = CPC_KEY_F6;
   keyboard_translation[RETROK_KP7] = CPC_KEY_F7;
   keyboard_translation[RETROK_KP8] = CPC_KEY_F8;
   keyboard_translation[RETROK_KP9] = CPC_KEY_F9;

   keyboard_translation[RETROK_KP_ENTER] = CPC_KEY_INTRO;
   keyboard_translation[RETROK_KP_PERIOD] = CPC_KEY_FDOT;

   keyboard_translation[RETROK_DELETE] = CPC_KEY_CLR;

   keyboard_translation[RETROK_LSHIFT] = CPC_KEY_SHIFT;
   keyboard_translation[RETROK_RSHIFT] = CPC_KEY_SHIFT;
   keyboard_translation[RETROK_LCTRL] = CPC_KEY_CONTROL;
   keyboard_translation[RETROK_RCTRL] = CPC_KEY_CONTROL;
   keyboard_translation[RETROK_CAPSLOCK] = CPC_KEY_CAPS_LOCK;

   keyboard_translation[RETROK_LALT] = CPC_KEY_COPY;
   keyboard_translation[RETROK_RALT] = CPC_KEY_COPY;
   keyboard_translation[RETROK_BACKQUOTE] = CPC_KEY_FORWARD_SLASH;
   keyboard_translation[RETROK_SLASH] = CPC_KEY_BACKSLASH;
   keyboard_translation[RETROK_QUOTE] = CPC_KEY_SEMICOLON;
   keyboard_translation[RETROK_BACKSLASH] = CPC_KEY_CLOSE_SQUARE_BRACKET;
   keyboard_translation[RETROK_OEM_102] = CPC_KEY_FORWARD_SLASH;

}

/* Matrix aliases for joystick 2 are also real CPC keyboard keys. */
static const char *input_key_label(unsigned key)
{
   switch (key) {
      case CPC_KEY_CURSOR_UP: return "CPC Cursor Up";
      case CPC_KEY_CURSOR_RIGHT: return "CPC Cursor Right";
      case CPC_KEY_CURSOR_DOWN: return "CPC Cursor Down";
      case CPC_KEY_F9: return "CPC F9";
      case CPC_KEY_F6: return "CPC F6";
      case CPC_KEY_F3: return "CPC F3";
      case CPC_KEY_INTRO: return "CPC Keypad Enter";
      case CPC_KEY_FDOT: return "CPC Keypad .";
      case CPC_KEY_CURSOR_LEFT: return "CPC Cursor Left";
      case CPC_KEY_COPY: return "CPC Copy";
      case CPC_KEY_F7: return "CPC F7";
      case CPC_KEY_F8: return "CPC F8";
      case CPC_KEY_F5: return "CPC F5";
      case CPC_KEY_F1: return "CPC F1";
      case CPC_KEY_F2: return "CPC F2";
      case CPC_KEY_F0: return "CPC F0";
      case CPC_KEY_CLR: return "CPC Clr";
      case CPC_KEY_OPEN_SQUARE_BRACKET: return "CPC [";
      case CPC_KEY_RETURN: return "CPC Return";
      case CPC_KEY_CLOSE_SQUARE_BRACKET: return "CPC ]";
      case CPC_KEY_F4: return "CPC F4";
      case CPC_KEY_SHIFT: return "CPC Shift";
      case CPC_KEY_FORWARD_SLASH: return "CPC /";
      case CPC_KEY_CONTROL: return "CPC Control";
      case CPC_KEY_HAT: return "CPC ^";
      case CPC_KEY_MINUS: return "CPC Minus";
      case CPC_KEY_AT: return "CPC At";
      case CPC_KEY_P: return "CPC P";
      case CPC_KEY_SEMICOLON: return "CPC Semicolon";
      case CPC_KEY_COLON: return "CPC Colon";
      case CPC_KEY_BACKSLASH: return "CPC Backslash";
      case CPC_KEY_DOT: return "CPC .";
      case CPC_KEY_ZERO: return "CPC 0";
      case CPC_KEY_9: return "CPC 9";
      case CPC_KEY_O: return "CPC O";
      case CPC_KEY_I: return "CPC I";
      case CPC_KEY_L: return "CPC L";
      case CPC_KEY_K: return "CPC K";
      case CPC_KEY_M: return "CPC M";
      case CPC_KEY_COMMA: return "CPC Comma";
      case CPC_KEY_8: return "CPC 8";
      case CPC_KEY_7: return "CPC 7";
      case CPC_KEY_U: return "CPC U";
      case CPC_KEY_Y: return "CPC Y";
      case CPC_KEY_H: return "CPC H";
      case CPC_KEY_J: return "CPC J";
      case CPC_KEY_N: return "CPC N";
      case CPC_KEY_SPACE: return "CPC Space";
      case CPC_KEY_6: return "CPC 6 / Joystick 2 Up";
      case CPC_KEY_5: return "CPC 5 / Joystick 2 Down";
      case CPC_KEY_R: return "CPC R / Joystick 2 Left";
      case CPC_KEY_T: return "CPC T / Joystick 2 Right";
      case CPC_KEY_G: return "CPC G / Joystick 2 Fire 1";
      case CPC_KEY_F: return "CPC F / Joystick 2 Fire 2";
      case CPC_KEY_B: return "CPC B / Joystick 2 Fire 3";
      case CPC_KEY_V: return "CPC V";
      case CPC_KEY_4: return "CPC 4";
      case CPC_KEY_3: return "CPC 3";
      case CPC_KEY_E: return "CPC E";
      case CPC_KEY_W: return "CPC W";
      case CPC_KEY_S: return "CPC S";
      case CPC_KEY_D: return "CPC D";
      case CPC_KEY_C: return "CPC C";
      case CPC_KEY_X: return "CPC X";
      case CPC_KEY_1: return "CPC 1";
      case CPC_KEY_2: return "CPC 2";
      case CPC_KEY_ESC: return "CPC Esc";
      case CPC_KEY_Q: return "CPC Q";
      case CPC_KEY_TAB: return "CPC Tab";
      case CPC_KEY_A: return "CPC A";
      case CPC_KEY_CAPS_LOCK: return "CPC Caps Lock";
      case CPC_KEY_Z: return "CPC Z";
      case CPC_KEY_JOY_UP: return "Joystick 1 Up";
      case CPC_KEY_JOY_DOWN: return "Joystick 1 Down";
      case CPC_KEY_JOY_LEFT: return "Joystick 1 Left";
      case CPC_KEY_JOY_RIGHT: return "Joystick 1 Right";
      case CPC_KEY_JOY_FIRE1: return "Joystick 1 Fire 1";
      case CPC_KEY_JOY_FIRE2: return "Joystick 1 Fire 2";
      case CPC_KEY_JOY_FIRE3: return "Joystick 1 Fire 3";
      case CPC_KEY_DEL: return "CPC Del";
      default: return NULL;
   }
}

void ev_update_input_descriptors(void)
{
   static struct retro_input_descriptor descriptors[35];
   static char labels[2][16][160];
   static const char *combo_labels[MAX_JOY_EVENT] = {
      "Catalogue", "Boot CP/M", "Run disk", "Run tape", "Toggle on-screen keyboard",
      "Type 1/Y", "Type 2/N", "Type 4/S", "Type 3/J", "Previous disk", "Next disk"
   };
   unsigned port, id, n, count = 0;
   memset(descriptors, 0, sizeof(descriptors));
   for (port = 0; port < 2; port++) {
      for (id = 0; id < 16; id++) {
         const char *label = id < MAX_BUTTONS ? input_key_label(btnPAD[port].buttons[id]) : NULL;
         char *text = labels[port][id];
         snprintf(text, sizeof(labels[port][id]), "%s", label ? label : "");
         if (port == 0 && ev_events == _events_joy) {
            if (id == retro_computer_cfg.combokey)
               snprintf(text, sizeof(labels[port][id]), "Core shortcut modifier");
            else for (n = 0; n < MAX_JOY_EVENT; n++) {
               if (events_combo[n].id == id) {
                  size_t used = strlen(text);
                  snprintf(text + used, sizeof(labels[port][id]) - used, "%sCombo: %s",
                     used ? " / " : "", combo_labels[n]);
                  break;
               }
            }
         }
         if (port == 0 && id == RETRO_DEVICE_ID_JOYPAD_L3)
            snprintf(text, sizeof(labels[port][id]), "Toggle on-screen keyboard");
         if (!text[0])
            continue;
         descriptors[count++] = (struct retro_input_descriptor){port, RETRO_DEVICE_JOYPAD, 0, id, text};
      }
      descriptors[count++] = (struct retro_input_descriptor){port, RETRO_DEVICE_LIGHTGUN, 0,
         RETRO_DEVICE_ID_LIGHTGUN_TRIGGER, "Gun Trigger"};
   }
   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, descriptors);
}

/**
 * ev_init:
 * prepare events interface, keyboard tables and sets retro environment input data
 * TODO: patch keyboard with user selected LANGUAGE/LAYOUT
 **/
void ev_init(){

   init_keyboard_table();
   init_joystick_table();

   struct retro_keyboard_callback cb = { keyboard_cb };
   environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cb);

   ev_events = _events_null;
   process_events = ev_joysticks;
   process_ev_key = _ev_key_null;
}

void ev_set(int type)
{
   switch (type)
   {
      case EV_AUTO:
         process_ev_key = _ev_key_null;
         process_events = ev_autorun;
         break;
      
      case EV_JOY:
         process_ev_key = _ev_key;
         process_events = ev_joysticks;
         break;

      case EV_KBD:
         process_ev_key = _ev_key_null;
         process_events = *ev_events;
         break;
   }

   //printf("[CPC] [EV] type %u\n", type);
   event_call = type;
}

void ev_toggle_call()
{
   if (event_call == EV_JOY)
      ev_set(EV_KBD);
   else
      ev_set(EV_JOY);
}

void ev_combo_set(unsigned btn)
{
   events_combo[JOY_EVENT_ID_Y].id = RETRO_DEVICE_ID_JOYPAD_Y;
   events_combo[JOY_EVENT_ID_B].id = RETRO_DEVICE_ID_JOYPAD_B;
   if (btn >= 16) {
      ev_events = _events_null;
      if (event_call == EV_KBD)
         process_events = ev_events;
      return;
   }
   retro_computer_cfg.combokey = btn;
   if (retro_computer_cfg.combokey == RETRO_DEVICE_ID_JOYPAD_Y)
   {
      events_combo[JOY_EVENT_ID_Y].id = RETRO_DEVICE_ID_JOYPAD_SELECT;
   }
   else if (retro_computer_cfg.combokey == RETRO_DEVICE_ID_JOYPAD_B)
   {
      events_combo[JOY_EVENT_ID_B].id = RETRO_DEVICE_ID_JOYPAD_SELECT;
   }

   ev_events = _events_joy;
   if (event_call == EV_KBD)
      process_events = ev_events;
}

static bool cursor_movement(int *axis, int value, int max_value, int sum)
{
   int abs_value = abs(value);
   if (abs_value < 6)
      sum = sum / 2;
   else if (abs_value > 24)
      sum = sum * 3;
   else if (abs_value > 12)
      sum = sum * 2;

   int new_value = *axis;

   new_value += (value > 0) ? sum : sum * -1;

   if ( new_value < 0)
   {
      new_value = 0;
   }
   else if(new_value > max_value)
   {
      new_value = max_value;
   }

   if((*axis - new_value) == 0)
      return false;

   *axis = new_value;
   return true;
}

#ifndef MOUSE_RELATIVE
// absolute mouse movement, currently default mode
void ev_mouse_motion()
{
   int mouse_x = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
   int mouse_y = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

   // I think DS comes to this part if no event on screen
   if( !mouse_x || !mouse_y ) {
      return;
   }

   if ((mouse.raw_x - mouse_x) == 0 && (mouse.raw_y - mouse_y) == 0)
      return;

   int px=(int) ((mouse_x + 0x7fff) * retro_video.screen_render_width / 0xfffe) + (64 * retro_video.screen_crop);
   int py=(int) ((mouse_y + 0x7fff) * retro_video.screen_render_height / 0xfffe);

   mouse.raw_x = mouse_x;
   mouse.raw_y = mouse_y;
   mouse.x = px < MAX_CURSOR_X? px : MAX_CURSOR_X;
   mouse.y = py < MAX_CURSOR_Y? py : MAX_CURSOR_Y;
   mouse.status |= CURSOR_MOTION;
}
#else
// relative mouse movement unused atm
void ev_mouse_motion()
{
   int mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
   int mouse_y = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);

   // I think DS comes to this part if no event on screen
   if( !mouse_x && !mouse_y )
      return;

   if (
      mouse_x &&
      cursor_movement(
         &mouse.x,
         mouse_x,
         EMULATION_SCREEN_WIDTH - (6 * EMULATION_SCALE),
         CURSOR_MOVEMENT_X
      ))
   {
      mouse.status |= CURSOR_MOTION;
   }

   if (
      mouse_y &&
      cursor_movement(
         &mouse.y,
         mouse_y,
         EMULATION_SCREEN_HEIGHT - 8,
         CURSOR_MOVEMENT_Y
      ))
   {
      mouse.status |= CURSOR_MOTION;
   }

}
#endif

void ev_cursor_click(unsigned int device, unsigned int event, int * ref_ptr, int value)
{
   int clicked = input_state_cb(0, device, 0, event);
   if (clicked == *ref_ptr)
      return;

   *ref_ptr = clicked;
   mouse.status |= value;
}

void ev_joy_motion()
{
   int cursor_x = 0;
   int cursor_y = 0;

   if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))
      cursor_y -= 8;

   if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))
      cursor_y += 8;

   if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))
      cursor_x -= 8 * EMULATION_SCALE;

   if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))
      cursor_x += 8 * EMULATION_SCALE;

   if (
      cursor_x &&
      cursor_movement(
         &mouse.x,
         cursor_x,
         EMULATION_SCREEN_WIDTH - (6 * EMULATION_SCALE),
         CURSOR_MOVEMENT_X
      ))
   {
      mouse.status |= CURSOR_MOTION;
   }

   if (
      cursor_y &&
      cursor_movement(
         &mouse.y,
         cursor_y,
         EMULATION_SCREEN_HEIGHT - 8,
         CURSOR_MOVEMENT_Y
      ))
   {
      mouse.status |= CURSOR_MOTION;
   }
}

void ev_process_cursor()
{
   ev_joy_motion();
   ev_mouse_motion();

   ev_cursor_click(
      RETRO_DEVICE_JOYPAD,
      RETRO_DEVICE_ID_JOYPAD_A,
      &mouse.click_joy,
      CURSOR_CLICKED_JOY
   );
   ev_cursor_click(
      RETRO_DEVICE_POINTER,
      RETRO_DEVICE_ID_POINTER_PRESSED,
      &mouse.click,
      CURSOR_CLICKED
   );
}

void ev_lightgun()
{
   if(input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN))
   {
      gun.state = GUN_PREPARE;
      gun.x = 0xfff;
      gun.y = 0xfff;
      return;
   }

   gun.x = ((input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X) + 0x7fff) * EMULATION_SCREEN_WIDTH) / 0xfffe;
   gun.y = ((input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y) + 0x7fff) * EMULATION_SCREEN_HEIGHT) / 0xfffe;

   if(input_state_cb(0, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER)
      || (input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT)))
   {
      gun.pressed = true;
   } else {
      gun.pressed = false;
   }
}