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

extern int showkeyb;
extern void kbd_buf_feed(char *s);

extern uint8_t keyboard_matrix[16];
const uint8_t bit_values[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

// --- events code
#define MAX_KEYSYMS 324
#define MAX_BUTTONS 14
#define MAX_PADCFG 3

static uint8_t keyboard_translation[MAX_KEYSYMS];
unsigned int last_input[PORTS_NUMBER] = {0,0};
uint32_t padnum = 0;

const uint8_t btnPAD[MAX_PADCFG][MAX_BUTTONS] = {
   {
   CPC_KEY_JOY_FIRE2,   // B
   CPC_KEY_SPACE,       // Y
   CPC_KEY_NULL,        // SELECT
   CPC_KEY_J,           // START
   CPC_KEY_JOY_UP,      // DUP
   CPC_KEY_JOY_DOWN,    // DDOWN
   CPC_KEY_JOY_LEFT,    // DLEFT
   CPC_KEY_JOY_RIGHT,   // DRIGHT
   CPC_KEY_JOY_FIRE1,   // A
   CPC_KEY_S,           // X
   //---------------------
   CPC_KEY_RETURN,      // L
   CPC_KEY_SHIFT,       // R
   CPC_KEY_CONTROL,     // L2
   CPC_KEY_COPY,        // R2
   },
   {
   CPC_KEY_F1,          // B
   CPC_KEY_F2,          // Y
   CPC_KEY_NULL,        // SELECT
   CPC_KEY_K,           // START
   CPC_KEY_Q,           // DUP
   CPC_KEY_A,           // DDOWN
   CPC_KEY_O,           // DLEFT
   CPC_KEY_P,           // DRIGHT
   CPC_KEY_SPACE,       // A
   CPC_KEY_H,           // X
   //---------------------
   CPC_KEY_RETURN,      // L
   CPC_KEY_SHIFT,       // R
   CPC_KEY_CONTROL,     // L2
   CPC_KEY_COPY,        // R2
   },
   {
   CPC_KEY_SPACE,       // B
   CPC_KEY_W,           // Y
   CPC_KEY_NULL,        // SELECT
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
   }
};

/**
 * press_emulated_key:
 * using CPC keyboard matrix sets as pressed
 **/
static void press_emulated_key(uint8_t cpc_key) {
   keyboard_matrix[cpc_key >> 4] &= ~bit_values[cpc_key & 7]; // key is being held down
}

/**
 * release_emulated_key:
 * using CPC keyboard matrix sets as released
 **/
static void release_emulated_key(uint8_t cpc_key) {
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
 * ev_special_combos:
 * generate the SELECT + JOYPAD_x result in screen/emulation
 *
 * TODO: add an help-screen in emulation screen?
 **/
static void ev_special_combos()
{
   static uint32_t last_event = 0;
   uint32_t pressed = 0;

   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_B);
      if(pressed != last_event)
            kbd_buf_feed("CAT\n");
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_Y);
      if(pressed != last_event)
            kbd_buf_feed("|CPM\n");
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_A);
      if(pressed != last_event)
           kbd_buf_feed("RUN\"DISK\nRUN\"DISC\n");
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_X);
      if(pressed != last_event)
           kbd_buf_feed("|TAPE\nRUN\"\n^");
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_START);
      if(pressed != last_event) {
            showkeyb=-showkeyb;
            memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix)); // clear CPC keyboard matrix
      }
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_UP);
      if(pressed != last_event){
           kbd_buf_feed("1\nY\n");
           retro_message("PRESSED => 1/Y");
        }
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_DOWN);
      if(pressed != last_event) {
           kbd_buf_feed("2\nN\n");
           retro_message("PRESSED => 2/N");
        }
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_LEFT);
      if(pressed != last_event){
         kbd_buf_feed("4\nS\n");
         retro_message("PRESSED => 4/S");
      }
   } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)) {
      BIT_SET(pressed, RETRO_DEVICE_ID_JOYPAD_RIGHT);
      if(pressed != last_event){
         kbd_buf_feed("3\nJ\n");
         retro_message("PRESSED => 3/J");
      }
   }

   last_event = pressed;
}

/**
 * ev_process_joy:
 * @playerID: the player id (see DEVICE AMSTRAD)
 *
 * process joystick using input_state_cb to CPC/Keyboard
 **/
static void ev_process_joy(int playerID){

   /*disabled?*/
   if(((amstrad_devices[playerID])&RETRO_DEVICE_MASK)==RETRO_DEVICE_NONE)
      return;

   if( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT)) {
      ev_special_combos();
      return;
   }

   uint8_t * pad = (uint8_t*) &btnPAD[retro_computer_cfg.padcfg[playerID]];

   int i;
   for (i = 0; i < MAX_BUTTONS; i++) {
      if (input_state_cb(playerID, RETRO_DEVICE_JOYPAD, 0, i)) {
         if(!(BIT_CHECK(last_input[playerID], i))) {
            press_emulated_key(*(pad+i));
            BIT_ADD(last_input[playerID], i);
         }
      }
      else if (BIT_CHECK(last_input[playerID], i)) {
         release_emulated_key(*(pad+i));
         BIT_CLEAR(last_input[playerID], i);
      }
   }
}

/**
 * ev_joysticks:
 * function to unify event code, call joy events and get user pad data
 **/
void ev_joysticks() {
   // exit on controllers config to RETRO_DEVICE_AMSTRAD_KEYBOARD
   if(amstrad_devices[0] == RETRO_DEVICE_AMSTRAD_KEYBOARD)
         return;

   ev_process_joy(ID_PLAYER1);
   ev_process_joy(ID_PLAYER2);
}

/**
 * ev_gui_keyboard:
 * GUI_MENU / GUI_VIRTUAL_KEYBOARD
 * WIP: preliminar code - just remove GUI_VIRTUAL_KEYBOARD atm
 **/
void ev_vkeyboard(){
   static uint32_t last_event = 0;
   uint32_t pressed = 0;

   if ( (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT))
        && (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START)) ) {
      BIT_SET(pressed, 3);
      if(pressed != last_event) {
         showkeyb=-showkeyb;
         last_event = pressed;
      }
   } else {
      last_event = 0;
   }
}

void ev_key(int key, bool pressed) {
   uint8_t cpc_key = get_cpckey(key);
   if (cpc_key != CPC_KEY_NULL) {
      if (pressed)
         press_emulated_key(cpc_key);
      else
         release_emulated_key(cpc_key);
   }
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

   ev_key(keycode, down);

}

/**
 * ev_cursorjoy:
 * @activate: if TRUE emulation on cursors is activated
 *            if FALSE emulation is disabled and cursors works normally
 *
 * Changes keyboard table to bind CPC joystick on cursors
 * TODO: activate using GUI
 **/
void ev_cursorjoy(bool activate) {
   if(activate) {
      keyboard_translation[RETROK_LCTRL] = CPC_KEY_JOY_FIRE1;
      keyboard_translation[RETROK_UP] = CPC_KEY_JOY_UP;
      keyboard_translation[RETROK_DOWN] = CPC_KEY_JOY_DOWN;
   	keyboard_translation[RETROK_LEFT] = CPC_KEY_JOY_LEFT;
   	keyboard_translation[RETROK_RIGHT] = CPC_KEY_JOY_RIGHT;
   } else {
      keyboard_translation[RETROK_LCTRL] = CPC_KEY_NULL;
      keyboard_translation[RETROK_UP] = CPC_KEY_CURSOR_UP;
   	keyboard_translation[RETROK_DOWN] = CPC_KEY_CURSOR_DOWN;
   	keyboard_translation[RETROK_LEFT] = CPC_KEY_CURSOR_LEFT;
   	keyboard_translation[RETROK_RIGHT] = CPC_KEY_CURSOR_RIGHT;
   }
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
		{ }
	};
	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, &inputDescriptors);

   init_keyboard_table();

   struct retro_keyboard_callback cb = { keyboard_cb };
   environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cb);
}
