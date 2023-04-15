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
   CPC_KEY_JOY_FIRE3              = 0x96,
   CPC_KEY_DEL                    = 0x97,
   /* Joystick 2 using Y-CABLE */
   // http://www.cpcwiki.eu/index.php/Digital_Joysticks#Relevant_locations_in_Keyboard_Matrix
   CPC_KEY_JOY2_UP                = 0x60,
   CPC_KEY_JOY2_DOWN              = 0x61,
   CPC_KEY_JOY2_LEFT              = 0x62,
   CPC_KEY_JOY2_RIGHT             = 0x63,
   CPC_KEY_JOY2_FIRE1             = 0x64,
   CPC_KEY_JOY2_FIRE2             = 0x65,
   CPC_KEY_JOY2_FIRE3             = 0x66, // just DIY JOYSTICK
   /* no key press */
   CPC_KEY_NULL                   = 0xFF
};

enum retro_joystick_button
{
   JOY_PORT1_UP = 200,
   JOY_PORT1_DOWN = 201,
   JOY_PORT1_RIGHT = 202,
   JOY_PORT1_LEFT = 203,
   JOY_PORT1_FIRE1 = 204,
   JOY_PORT1_FIRE2 = 205,
   JOY_PORT2_UP = 206,
   JOY_PORT2_DOWN = 207,
   JOY_PORT2_RIGHT = 208,
   JOY_PORT2_LEFT = 209,
   JOY_PORT2_FIRE1 = 210,
   JOY_PORT2_FIRE2 = 211,
   JOY_PORT2_FIRE3 = 212,
   JOY_PORT1_FIRE3 = 213,
};

enum retro_events_action_type
{
   EVENT_NULL = 0,
   EVENT_WRITE,
   EVENT_VKEYB,
   EVENT_GUI,
   EVENT_TAPE_ON,
   EVENT_TAPE_OFF,
   EVENT_TAPE_REWIND,
   EVENT_CURSOR_JOY,
};

enum retro_event_call_type
{
   EV_NONE = 0,
   EV_JOY  = 1,
   EV_KBD  = 2,
   EV_AUTO = 3,
};

typedef struct {
   unsigned type;
   const char * kbd_buf;
   const char * message;
} retro_action_t;

typedef struct {
   unsigned id;
   retro_action_t action;
} retro_combo_event_t;

enum retro_cursor_status
{
   CURSOR_NONE        = 0,
   CURSOR_CLICKED     = 1,
   CURSOR_MOTION      = 2,
   CURSOR_CLICKED_JOY = 4,
};

typedef struct {
   int x, raw_x;
   int y, raw_y;
   int click;
   int click_joy;
   unsigned status;
} retro_mouse_t;

#define JOY_EVENT_ID_B 0
#define JOY_EVENT_ID_Y 1

void ev_press_key(unsigned char cpc_key);
void ev_release_key(unsigned char cpc_key);

extern bool (*process_events)(void);

void ev_combo_set(unsigned btn);
void ev_set(int type);
void ev_init();
uint8_t get_cpckey (unsigned int keysym);
void ev_autorun_prepare(char * kbd_buffer);

void Core_PollEvent(void);

void ev_process_cursor();

#endif
