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


#ifndef RETRO_EVENTS_H__
#define RETRO_EVENTS_H__

// TAKEN FORM ARNOLD, adapted to caprice using u8
enum emu_key_id {
   /* line 0, bit 0..bit 7 */
   CPC_KEY_CURSOR_UP              = 0x00,
   CPC_KEY_CURSOR_RIGHT           = 0x01,
   CPC_KEY_CURSOR_DOWN            = 0x02,
   CPC_KEY_F9                     = 0x03,
   CPC_KEY_F6                     = 0x04,
   CPC_KEY_F3                     = 0x05,
   CPC_KEY_INTRO                  = 0x06,
   CPC_KEY_FDOT                   = 0x07,
   /* line 1, bit 0..bit 7 */ //8=
   CPC_KEY_CURSOR_LEFT            = 0x10,
   CPC_KEY_COPY                   = 0x11,
   CPC_KEY_F7                     = 0x12,
   CPC_KEY_F8                     = 0x13,
   CPC_KEY_F5                     = 0x14,
   CPC_KEY_F1                     = 0x15,
   CPC_KEY_F2                     = 0x16,
   CPC_KEY_F0                     = 0x17,
   /* line 2, bit 0..bit 7 */  //16=
   CPC_KEY_CLR                    = 0x20,
   CPC_KEY_OPEN_SQUARE_BRACKET    = 0x21,
   CPC_KEY_RETURN                 = 0x22,
   CPC_KEY_CLOSE_SQUARE_BRACKET   = 0x23,
   CPC_KEY_F4                     = 0x24,
   CPC_KEY_SHIFT                  = 0x25,
   CPC_KEY_FORWARD_SLASH          = 0x26,
   CPC_KEY_CONTROL                = 0x27,
   /* line 3, bit 0.. bit 7 */ //24=
   CPC_KEY_HAT                    = 0x30,
   CPC_KEY_MINUS                  = 0x31,
   CPC_KEY_AT                     = 0x32,
   CPC_KEY_P                      = 0x33,
   CPC_KEY_SEMICOLON              = 0x34,
   CPC_KEY_COLON                  = 0x35,
   CPC_KEY_BACKSLASH              = 0x36,
   CPC_KEY_DOT                    = 0x37,
   /* line 4, bit 0..bit 7 */ //32=
   CPC_KEY_ZERO                   = 0x40,
   CPC_KEY_9                      = 0x41,
   CPC_KEY_O                      = 0x42,
   CPC_KEY_I                      = 0x43,
   CPC_KEY_L                      = 0x44,
   CPC_KEY_K                      = 0x45,
   CPC_KEY_M                      = 0x46,
   CPC_KEY_COMMA                  = 0x47,
   /* line 5, bit 0..bit 7 */ //40=
   CPC_KEY_8                      = 0x50,
   CPC_KEY_7                      = 0x51,
   CPC_KEY_U                      = 0x52,
   CPC_KEY_Y                      = 0x53,
   CPC_KEY_H                      = 0x54,
   CPC_KEY_J                      = 0x55,
   CPC_KEY_N                      = 0x56,
   CPC_KEY_SPACE                  = 0x57,
   /* line 6, bit 0..bit 7 */ //48=
   CPC_KEY_6                      = 0x60,
   CPC_KEY_5                      = 0x61,
   CPC_KEY_R                      = 0x62,
   CPC_KEY_T                      = 0x63,
   CPC_KEY_G                      = 0x64,
   CPC_KEY_F                      = 0x65,
   CPC_KEY_B                      = 0x66,
   CPC_KEY_V                      = 0x67,
   /* line 7, bit 0.. bit 7 */ //56=
   CPC_KEY_4                      = 0x70,
   CPC_KEY_3                      = 0x71,
   CPC_KEY_E                      = 0x72,
   CPC_KEY_W                      = 0x73,
   CPC_KEY_S                      = 0x74,
   CPC_KEY_D                      = 0x75,
   CPC_KEY_C                      = 0x76,
   CPC_KEY_X                      = 0x77,
   /* line 8, bit 0.. bit 7 */ //64=
   CPC_KEY_1                      = 0x80,
   CPC_KEY_2                      = 0x81,
   CPC_KEY_ESC                    = 0x82,
   CPC_KEY_Q                      = 0x83,
   CPC_KEY_TAB                    = 0x84,
   CPC_KEY_A                      = 0x85,
   CPC_KEY_CAPS_LOCK              = 0x86,
   CPC_KEY_Z                      = 0x87,
   /* line 9, bit 7..bit 0 */  //72=
   CPC_KEY_JOY_UP                 = 0x90,
   CPC_KEY_JOY_DOWN               = 0x91,
   CPC_KEY_JOY_LEFT               = 0x92,
   CPC_KEY_JOY_RIGHT              = 0x93,
   CPC_KEY_JOY_FIRE1              = 0x94,
   CPC_KEY_JOY_FIRE2              = 0x95,
   CPC_KEY_SPARE                  = 0x96,
   CPC_KEY_DEL                    = 0x97,
   /* no key press */
   CPC_KEY_NULL                   = 0xFF
};

void ev_joysticks();
void ev_vkeyboard();
void ev_cursorjoy(bool activate);
void ev_init();

#endif
