/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright not6 - r-type (2015-2018)
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

#ifndef LIBRETRO_CORE_H
#define LIBRETRO_CORE_H 1

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define VERSION_STRING "4.5.3"

// DEVICE AMSTRAD
#define RETRO_DEVICE_AMSTRAD_KEYBOARD RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_KEYBOARD, 0)
#define RETRO_DEVICE_AMSTRAD_JOYSTICK RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD,   1)
#define RETRO_DEVICE_AMSTRAD_LIGHTGUN RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_LIGHTGUN, 0)

#define PORTS_NUMBER 2
#define ID_PLAYER1 0
#define ID_PLAYER2 1

#define PADCFG_AUTO 0
#define PADCFG_QAOP 1
#define PADCFG_INCENTIVE 2
#define PADCFG_JOYSTICK_1 3
#define PADCFG_JOYSTICK_2 4

extern unsigned amstrad_devices[ PORTS_NUMBER ];

#define CPC_SCREEN_WIDTH 384
#define CPC_SCREEN_HEIGHT 272
#define CPC_TIMING 838977920.0 / 16777215.0

#define TEX_MAX_WIDTH CPC_SCREEN_WIDTH * 2
#define TEX_MAX_HEIGHT CPC_SCREEN_HEIGHT

// --- loader
#define LOADER_MAX_SIZE 256
#define LOADER_TAPE_STR "|TAPE\nRUN\"\n^        "

// --- events
#define MAX_KEYSYMS 324
#define MAX_BUTTONS 14
#define MAX_PADCFG 5


// compile flags
//#define NO_FLOPPY_SND
//#define NO_BORDER
//#define LOWRES
//#define M16B
//#define MOUSE_RELATIVE // mouse relative movement

#define PIXEL_TRANSPARENT 0x0000
#define PIXEL_DEPTH_DEFAULT_SIZE 4

#define WINDOW_MAX_SIZE (TEX_MAX_WIDTH * TEX_MAX_HEIGHT)
#define EMULATION_INIT_AUTORUNDELAY 50

#ifdef LOWRES
#define SCREENMODE_STR " LO"
#define EMULATION_SCALE 1
#define EMULATION_SCREEN_WIDTH CPC_SCREEN_WIDTH
#define EMULATION_SCREEN_HEIGHT CPC_SCREEN_HEIGHT
#define CURSOR_MOVEMENT_X 2
#define CURSOR_MOVEMENT_Y 2
#else
#define SCREENMODE_STR " HI"
#define EMULATION_SCALE 2
#define EMULATION_SCREEN_WIDTH CPC_SCREEN_WIDTH * 2
#define EMULATION_SCREEN_HEIGHT CPC_SCREEN_HEIGHT
#define CURSOR_MOVEMENT_X 4
#define CURSOR_MOVEMENT_Y 2
#endif

//AUDIO
#define FRAME_PERIOD_MS        20.0 // check cap32.h
#define AUDIO_BYTES 2
#define AUDIO_CHANNELS 2
#define AUDIO_SAMPLE_SIZE 44100
#define AUDIO_BUFSIZE (AUDIO_SAMPLE_SIZE * FRAME_PERIOD_MS / 1000)

//LOG
#if  defined(__ANDROID__) || defined(ANDROID)
#include <android/log.h>
#define LOG_TAG "RetroArch.Frodo"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#else
#define LOGI(...) log_cb(RETRO_LOG_INFO, __VA_ARGS__)
#endif
#define LOGE(...) log_cb(RETRO_LOG_ERROR, __VA_ARGS__)
#define LOGD(...) log_cb(RETRO_LOG_DEBUG, __VA_ARGS__)

// BIT OPERATIONS - MACROS
#define BIT_SET(var, bit)    var  = 1 << bit
#define BIT_ADD(var, bit)    var |= 1 << bit
#define BIT_CLEAR(var, bit)  var &= ~(1 << bit)
#define BIT_CHECK(var, bit)  ((var >> bit) & 1)
#define BIT_TOGGLE(var, bit) var ^= 1 << bit

// COMPUTER/EMU STATUS
#define COMPUTER_OFF     0
#define COMPUTER_BOOTING 1
#define COMPUTER_READY   2
extern int emu_status;

#define STATUSBAR_HIDE    0
#define STATUSBAR_SHOW    1
#define STATUSBAR_OFF     2

#define SLOT_UNK 0
#define SLOT_SNA 1
#define SLOT_DSK 2
#define SLOT_TAP 3
#define SLOT_M3U 4

typedef struct {
   int model;
   int ram; /*request only! beware: 6128 enforces minimum!*/
   int lang;
   int slot;
   uint32_t combokey;
   uint32_t statusbar;
   uint32_t padcfg[PORTS_NUMBER];
   bool is_dirty;
   bool floppy_snd;
   bool autorun;
   bool use_internal_remap;
} computer_cfg_t;
extern computer_cfg_t retro_computer_cfg;

typedef enum {
   BTN_B      = 0,
   BTN_Y      = 1,
   BTN_SELECT = 2,
   BTN_START  = 3,
   BTN_DUP    = 4,
   BTN_DDOWN  = 5,
   BTN_DLEFT  = 6,
   BTN_DRIGHT = 7,
   BTN_A      = 8,
   BTN_X      = 9,
   BTN_L      = 10,
   BTN_R      = 11,
   BTN_L2     = 12,
   BTN_R2     = 13,
} t_buttons;

typedef struct {
   unsigned char buttons[MAX_BUTTONS];
} t_button_cfg;


typedef struct {
   uint32_t hash;
   t_button_cfg btn_config;
   char loader_command[LOADER_MAX_SIZE];
   bool has_command;
   bool has_btn;
   bool is_fail;
   bool is_clean;
   bool is_cpm;
} game_cfg_t;
extern game_cfg_t game_configuration;

void retro_message(const char *text);
void computer_reset();

//SCREEN FUNCTIONS
extern int retro_getStyle();
extern int retro_getGfxBpp();
extern int retro_getGfxBps();
extern int retro_getAudioBuffer();
extern unsigned int * retro_getScreenPtr();

// allowed file types
#define EXT_FILE_CDT "cdt"
#define EXT_FILE_DSK "dsk"
#define EXT_FILE_M3U "m3u"
#define EXT_FILE_SNA "sna"
#define EXT_FILE_CPR "cpr"

// allowed file flags - BIOS
#define FLAG_BIOS_464 "[464]"
#define FLAG_BIOS_CPM "[CPM]"
#define FLAG_BIOS_664 "[664]"
#define FLAG_BIOS_B10 "Basic 1.0"
#define FLAG_BIOS_B11 "Basic 1.1"

// allowed file flags - memory
#define FLAG_MEMORY_576 "[576K]"
#define FLAG_MEMORY_128 "[128K]"
#define FLAG_MEMORY_064 "[064K]"

#endif
