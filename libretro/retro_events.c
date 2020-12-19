/* Copyright (C) 2018
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
#include "retro_ui.h"

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
extern void save_bkg();
extern void play_tape();
extern void stop_tape(void);
extern void Tape_Rewind(void);

extern uint8_t keyboard_matrix[16];
const uint8_t bit_values[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

// --- events code
#define MAX_KEYSYMS 324
#define MAX_BUTTONS 14
#define MAX_PADCFG 4

static uint8_t keyboard_translation[MAX_KEYSYMS];
unsigned int last_input[PORTS_NUMBER] = {0,0};
uint32_t padnum = 0;

bool (*ev_events)(void);
bool (*process_events)(void);
unsigned char (*process_ev_key)(int key, bool pressed);

enum retro_event_call
{
   EV_NONE = 0,
   EV_JOY  = 1,
   EV_KBD  = 2
};
int event_call = EV_JOY;
void ev_toggle_call();

const uint8_t btnPAD[MAX_PADCFG][MAX_BUTTONS] = {
   { // JOYSTICK CFG
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
   CPC_KEY_CONTROL,     // L
   CPC_KEY_INTRO,       // R
   CPC_KEY_F1,          // L2
   CPC_KEY_F2,          // R2
   },
   { // KEYBOARD CFG ( QAOP )
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
   },
   { // KEYBOARD CFG ( INCENTIVE )
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
   },
   { // JOYSTICK PLAYER 2 CFG
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
   },
};

// ---------------------------------------------

#define MAX_JOY_EVENT 9
static retro_combo_event_t events_combo[MAX_JOY_EVENT] =
{
   { RETRO_DEVICE_ID_JOYPAD_B,            // if you change this position, update JOY_EVENT_ID_B
      { EVENT_WRITE, "CAT\n", NULL } },
   { RETRO_DEVICE_ID_JOYPAD_Y,            // if you change this position, update JOY_EVENT_ID_Y
      { EVENT_WRITE, "|CPM\n", NULL } },
   { RETRO_DEVICE_ID_JOYPAD_A,
      { EVENT_WRITE, "RUN\"DISK\nRUN\"DISC\n", NULL } },
   { RETRO_DEVICE_ID_JOYPAD_X,
      { EVENT_WRITE, "|TAPE\nRUN\"\n^" } },
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
static uint8_t get_cpckey (unsigned int keysym)
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

/**
 * do_action:
 * @return: the retro_events_action_type.
 * generates event actions
 **/
static unsigned do_action(const retro_action_t* action)
{
   switch(action->type) {
      case EVENT_WRITE:
         kbd_buf_feed((char*) action->kbd_buf);
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
   }

   if(action->message)
      retro_message(action->message);

   return action->type;
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

   // is disabled?
   if(((amstrad_devices[playerID])&RETRO_DEVICE_MASK)==RETRO_DEVICE_NONE)
      return;

   uint8_t * pad = (uint8_t*) &btnPAD[retro_computer_cfg.padcfg[playerID]];

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
bool ev_joysticks() {
   // you cannot use key-remap on player two, force read it
   _process_joy(ID_PLAYER2);

   // exit on controllers config to RETRO_DEVICE_AMSTRAD_KEYBOARD
   // but allows legacy keyboard-remap + joystick-simple combo, issue #63
   if(amstrad_devices[0] == RETRO_DEVICE_AMSTRAD_KEYBOARD &&
      retro_computer_cfg.padcfg[ID_PLAYER1] != PADCFG_JOYSTICK)
         return false;

   if(!ev_events())
      _process_joy(ID_PLAYER1);

   return true;
}

//-----------------------------------------------------

#define MAX_KEY_EVENT 6
const retro_combo_event_t keyb_events[MAX_KEY_EVENT] =
{
   { RETROK_F9,
      { EVENT_VKEYB, "VKEYB", NULL } },
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
 * _ev_key:
 * emulator keyboard handler
 * BUG: ra call keyboard_cb incorrectly with release events
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
 * FIXME: to try to avoid ghosting bug we set ev_key as null on KoS.
 **/
unsigned char _ev_key_null(int key, bool pressed) {
   return CPC_KEY_NULL;
}

/**
 * keyboard_cb:
 * Is called by main retro_callback when user press/release a key
 * just convert this event to an emulated event
 * BUG: generates ghosting release events when "amstrad keyboard" is selected on controls
 **/
static void keyboard_cb(bool down, unsigned keycode, uint32_t character, uint16_t mod)
{
   //printf( "Down: %s, Code: %d, Char: %u, Mod: %u.\n",
   //       down ? "yes" : "no", keycode, character, mod);

   if(kbd_runcmd) // FIXME -- send bug to libretro
      return;

   if(process_ev_key(keycode, down) != CPC_KEY_NULL)
      return;

   ev_events_key(keycode, down);
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

/**
 * ev_init:
 * prepare events interface, keyboard tables and sets retro environment input data
 * TODO: patch keyboard with user selected LANGUAGE/LAYOUT
 **/
void ev_init(){

   struct retro_input_descriptor inputDescriptors[] = {
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2, "R2" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2, "L2" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3, "R3" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3, "L3" },

      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2, "R2" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2, "L2" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3, "R3" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3, "L3" },

		{ 0 }
	};
	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, &inputDescriptors);

   init_keyboard_table();

   struct retro_keyboard_callback cb = { keyboard_cb };
   environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cb);

   ev_events = _events_null;
   process_events = ev_joysticks;
   process_ev_key = _ev_key;
}

void ev_toggle_call()
{
   if (event_call == EV_JOY)
   {
      process_ev_key = _ev_key_null;
      process_events = *ev_events;
      event_call = EV_KBD;
   } else {
      process_ev_key = _ev_key;
      process_events = ev_joysticks;
      event_call = EV_JOY;
   }
}

void ev_combo_set(unsigned btn)
{
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

// TODO: check if focused because dont work correctly there...
void ev_mouse_motion_absolute()
{
   int mouse_x = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
   int mouse_y = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

   // I think DS comes to this part if no event on screen
   if( !mouse_x || !mouse_y ) {
      return;
   }

   if ((mouse.raw_x - mouse_x) == 0 && (mouse.raw_y - mouse_y) == 0)
      return;

   int px=(int) ((mouse_x + 0x7fff) * EMULATION_SCREEN_WIDTH / 0xffff);
   int py=(int) ((mouse_y + 0x7fff) * EMULATION_SCREEN_HEIGHT / 0xffff);

   mouse.raw_x = mouse_x;
   mouse.raw_y = mouse_y;
   mouse.x = px;
   mouse.y = py;
   mouse.status |= CURSOR_MOTION;
}

bool ev_cursor_click(unsigned int device, unsigned int event, int * ref_ptr, int value)
{
   int clicked = input_state_cb(0, device, 0, event);
   if (clicked == *ref_ptr)
      return false;

   *ref_ptr = clicked;
   mouse.status |= value;
   return true;
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
      RETRO_DEVICE_MOUSE,
      RETRO_DEVICE_ID_MOUSE_LEFT,
      &mouse.click,
      CURSOR_CLICKED
   );
}