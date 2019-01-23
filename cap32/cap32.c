/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2005 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


   Oct 11, 2000 - 22:12 preliminary IN/OUT handlers; started with the GA OUT register write routines
   Oct 12, 2000 - 23:31 added GA ROM select to z80_OUT_handler
   Oct 14, 2000 - 13:58 added PPI IN/OUT handlers
   Oct 15, 2000 - 15:33 added CRTC IN/OUT handlers
   Oct 20, 2000 - 23:23 fixed some IN/OUT handler bugs
   Oct 27, 2000 - 17:39 added reset_CPC
   Oct 30, 2000 - 21:05 found the problem with the streched display: scr_bps needs to be in uint32_ts, not bytes!
   Nov 01, 2000 - 23:43 aargh! found the BASIC 'reset' bug: the pbROMhi variable was pointing to the wrong location!
   Nov 03, 2000 - 18:03 added keyboard_interrupt handler
   Nov 03, 2000 - 19:10 added preliminary PSG register write handlers
   Nov 03, 2000 - 23:37 fixed the PPI port C handler; CPC keyboard works now!
   Dec 12, 2000 - 18:14 changed load_dsk() to work with a C based disk info structure
   Jan 24, 2001 - 22:28 loading from disk images works (again)!
   Apr 06, 2001 - 12:52 fixed the keyboard translation table (Allegro WIP 3.9.34 added 3 more keys to the array)

   Jun 13, 2001 - 17:25 added DirectDraw and DirectInput8 init routines for the new Windows port of Caprice32
   Jun 13, 2001 - 20:50 keyboard emualtion works now via the DI8 action mapping
   Jun 21, 2001 - 00:30 changed DDraw code to work in windowed mode; added routines to render CPC screen in 32bpp; added drag'n drop functionality for CPC drive A
   Jun 23, 2001 - 16:15 only the visible portion of the CPC screen is rendered to the frame buffer; Windows window is now sized to the proper CPC screen dimensions
   Jun 28, 2001 - 18:42 fixed the extended DSK parsing; loading DSK images via common dialog file selector; handling of joystick axis data
   Jun 29, 2001 - 20:16 speed throttling to original CPC speed; fixed the joystick mapping in the CPC keyboard matrix; implemented more menu commands
   Jun 30, 2001 - 22:57 settings stored in and retrieved from the registry; updated emulator_init and emulator_shutdown; file selector remembers path and selected file
   Jul 01, 2001 - 17:54 ROM slot cofiguration is stored in and retrieved from the registry; automatically loads last DSK images on startup
   Jul 04, 2001 - 00:34 colour tables for 16bpp (555 & 565) and 8bpp PC video modes; DDraw init updated with bit depth dependant code
   Jul 04, 2001 - 22:06 fixed 16bpp colour tables; updated VDU screen centering code to work with all bit depths
   Jul 06, 2001 - 00:35 implemented pause option; emulation is halted when application becomes inactive
   Jul 06, 2001 - 19:34 zip decompression for DSK and SNA loading
   Jul 07, 2001 - 00:58 fixed setting of GA colour table in load_snapshot; last DSK image load on startup supports ZIP files
   Jul 18, 2001 - 18:40 DirectSound init code finally works correctly!
   Jul 20, 2001 - 18:17 found the problem with the sound emulation: snd_multiplier calculation was done with integer arithmetic! grrrr...
   Jul 25, 2001 - 00:48 *finally* got a clean sound code that won't break up anymore!
   Aug 01, 2001 - 17:38 sound configuration is pulled from and stored in the registry
   Aug 02, 2001 - 18:31 floppy drive LED is now shown on the CPC screen during data transfers
   Aug 03, 2001 - 19:26 added About box
   Aug 03, 2001 - 23:49 altered load_dsk() to allocate memory on a track by track basis (in preparation of fdc_writeID() support)
   Aug 06, 2001 - 18:42 started work on the Options dialog
   Aug 10, 2001 - 19:10 finished the General Options property sheet
   Aug 11, 2001 - 17:29 up to 576KB of RAM supported
   Aug 12, 2001 - 21:43 extracted psg_reg_write() from the psg_write #define; fixed the PSG setup in load_snapshot()
   Aug 16, 2001 - 00:49 load_snapshot() now adjusts RAM size, and loads the correct CPC ROM as necessary
   Aug 20, 2001 - 00:59 added save_snapshot(); header contains preliminary v3 info
   Aug 20, 2001 - 16:10 updated load_snapshot() with processing of the 'official' v3 info
   Aug 22, 2001 - 23:26 added Audio Options property sheet to control PSG emulation
   Oct 06, 2001 - 13:22 removed the "experimental" joystick 2 support from the action map code - did not work as expected
   Oct 07, 2001 - 20:27 added the save_dsk() routine
   Oct 13, 2001 - 18:51 completed rewrite of zip handling; users can now choose images from multiple file archives
   Nov 08, 2001 - 19:08 bug fix: the zip_dir routine caused a crash if a zip archive would not open properly
   Nov 09, 2001 - 00:04 the ROM loading routine now checks the validity of the image, and skips a possible 128 bytes header

   May 09, 2002 - 19:07 removed the DDSCAPS_3DDEVICE flag from the InitDirectDraw code; modified UpdateBounds to prevent strechblits

   Jun 12, 2002 - 18:24 started converting DirectX specific stuff to Allegro
   Jun 14, 2002 - 17:49 emulator runs via GDI blits; re-implemented Allegro keyboard handler
   Jun 22, 2002 - 17:34 finally got the windowed DirectX mode working using the new Allegro CVS version
   Jun 25, 2002 - 18:41 added timer based speed throttling
   Jun 27, 2002 - 18:08 CPC screen blits correctly now: source coordinates were being mangled
   Jul 14, 2002 - 17:42 rewrote the PC sound playback routine to dynamically adjust the rate at which the buffer is filled
   Jul 24, 2002 - 22:16 fixed a possible crash on startup if a zipped DSK in drive A/B had been (re)moved between sessions
   Jul 27, 2002 - 16:45 added processing of a "language" file
   Jul 27, 2002 - 19:14 changed LoadConfiguration and SaveConfiguration to use the Allegro configuration rountines
   Jul 29, 2002 - 22:56 traced the 'access violation' problem back to the sound pause/resume functions
   Aug 02, 2002 - 22:37 added some error condition checks and modified sub-routines to report them properly

   Aug 24, 2002 - 23:00 determined Allegro to be unsuitable - reverting everything back to DirectX; implemented DirectInput keyboard translation
   Aug 26, 2002 - 22:51 fixed the initial application window size to use the correct dimensions
   Aug 27, 2002 - 18:23 streamlined the code to be more Windows friendly; app now goes to sleep when minimized
   Sep 07, 2002 - 18:28 added fullscreen mode; corrected 8bpp palette init; GDI calls (e.g. file selector) still work
   Sep 08, 2002 - 16:06 rewrote PCVideo init routines to support windowed/fullscreen mode switching
   Sep 12, 2002 - 00:02 fixed 50Hz timer by using QueryPerformanceCounter
   Sep 20, 2002 - 18:19 re-implemented DirectSound support; fixed timing problems - sound playback should no longer pop
   Sep 21, 2002 - 15:35 LoadConfiguration & SaveConfiguration now use the cap32.cfg file instead of the Registry
   Sep 25, 2002 - 22:49 full screen resolution is selectable in the Options dialog; auto-sizes visible portion; allows mode change while being fs
   Sep 26, 2002 - 22:26 added scanline and line doubling (in software) rendering modes
   Sep 28, 2002 - 17:38 added a speed slider to the General page of the Options dialog
   Sep 29, 2002 - 19:31 added colour monitor and green screen option; switched to QueryPerformanceCounter for the FPS counter
   Sep 29, 2002 - 23:03 added the 15 and 16 bpp green monitor colour maps - thanks to Ralf's Excel sheet!
   Oct 02, 2002 - 23:43 added fault condition checks to zip_dir, zip_extract and load_snapshot
   Oct 04, 2002 - 18:29 added fault condition checks to save_snapshot, load_dsk and save_dsk; added altered DSK check on exit
   Oct 04, 2002 - 22:34 added disk drive activity indicator
   Oct 07, 2002 - 17:48 fixed switching CPC monitor type "on-the-fly"; fixed surface restore in fullscreen mode
   Oct 07, 2002 - 21:58 added line doubling (in hardware) rendering mode
   Oct 16, 2002 - 22:18 added enumeration and initilization of all installed keyboards and joysticks
   Oct 17, 2002 - 23:40 replaced the controls property page in the options dialog
   Oct 28, 2002 - 19:50 added support for custom controls: CPC joystick functions can now be mapped to any attached input device(s)
   Nov 01, 2002 - 15:54 added custom print routine for on-screen messages (e.g. FPS counter)
   Nov 02, 2002 - 16:44 mouse cursor now auto-hides with no mouse movement for 2 seconds
   Nov 10, 2002 - 17:38 added ROM patching to support the french and spanish CPC keyboard layouts
   Nov 10, 2002 - 21:31 fixed PPI port C handler
   Nov 12, 2002 - 22:08 changes to the OUT handler: GA and CRTC cannot be accessed at the same time; fixed the GA pen selection
   Nov 13, 2002 - 22:48 tweaked the IN/OUT handlers a bit more
   Dec 05, 2002 - 00:06 fixed the problem with the tape startup delay: the PPI Port C bit manipulation only considered 4 instead of 8 bits
   Dec 08, 2002 - 22:05 updated the Z80_OUT_handler to accept FDC data on ports 0x7f and 0x7e, as per Kevin's I/O port document
   Dec 10, 2002 - 23:50 print now draws double spaced in scanline mode: don't have to remove the text again this way
   Dec 19, 2002 - 23:28 added 48 & 96 kHz audio options; added sanity checks for LoadConfiguration
   Dec 21, 2002 - 15:50 PCAudio_init reports if selected sample rate is not supported; added empty path handling to LoadConfiguration
   Dec 22, 2002 - 17:21 found a typo in my keyboard lookup table: CPC key 8 was returning the same as CPC key 0
   Jan 12, 2003 - 01:05 added the ability to search for multiple extensions to zip_dir
   Jan 12, 2003 - 15:57 completed processing of dropped dsk, sna and cdt files
   Jan 15, 2003 - 17:30 added the removal of the temp file to process_drop when uncompressing zip archives
   Jan 21, 2003 - 18:53 changed the window handle in all MessageBox calls from NULL to hAppWnd: errors display correctly in full-screen mode now
   Jan 24, 2003 - 00:44 altered the validity check on loading expansion ROMs, as some may not contain a jump instruction at offset 0x06
   Jan 25, 2003 - 16:49 fixed the FDC port handling in z80_in_handler: "Famous 5" loads now!
   Jan 27, 2003 - 18:47 updated the resource file to support themed Windows XP controls
   Jan 28, 2003 - 19:18 added insert_voc: converts a sound sample into a CDT direct recording block
   Jan 30, 2003 - 20:37 added 24bpp display mode support
   Jan 31, 2003 - 16:50 CPC speed slider range is now 50% to 800%, and can be set in 25% increments
   Jan 31, 2003 - 22:42 AMSDOS is placed in ROM slot 7 if the config file does not yet exist
   Feb 23, 2003 - 14:48 fixed the Windows system palette problems in 256 colour mode
   Mar 04, 2003 - 21:04 removed the background brush in the app window class; replaced "return 0L" with "break" in WindowProc
   Mar 11, 2003 - 22:38 emulation can now continue to run when focus is lost; added more invalid object checks to ensure clean exits
   Mar 12, 2003 - 18:08 had to add the background brush again: fullscreen mode would otherwise not update the unused area
   Mar 12, 2003 - 19:02 added the 'auto-pause on loss of focus' feature to the Options dialog
   Mar 12, 2003 - 22:07 added the display of HRESULT codes for DirectDraw function calls
   Mar 17, 2003 - 22:40 added a check_drvX call to the eject disk menu functions
   Mar 22, 2003 - 18:39 added the ability to insert blank disks in either CPC drive; the new Disk page in the Options dialog allows you to choose the format; implemented the Flip Disk option to reverse sides
   Mar 23, 2003 - 21:41 custom disk formats can now be specified in the config file; modified LoadConfiguration and SaveConfiguration to support this feature
   Mar 29, 2003 - 16:40 tape motor is not turned on if there is no tape in the drive
   Apr 02, 2003 - 21:31 changes to the Options dialog: moved the ROM slots to their own page; some cosmetic changes; remembers last page user was on
   Apr 03, 2003 - 22:12 added the option to capture printer output to file; aborting a save dialog for a changed DSK now drops back to the emulation without taking any further action
   Apr 07, 2003 - 19:09 modified the z80_OUT_handler to capture port writes for the MF2; added all the necessary bits for MF2 emulation, but for now it doesn't display the MF2 menu correctly (text is invisible!)...
   Apr 07, 2003 - 21:59 doh! fixed the MF2 problem: the MF2 page-out port always set RAM bank0 instead if checking if the lower ROM was active
   Apr 09, 2003 - 15:49 MF2 ROM is now restored at every reset to ensure the ROM area has not been corrupted by memory writes
   Apr 13, 2003 - 16:18 added code to display the logo in the About box with a transparent background
   Apr 16, 2003 - 16:18 joystick emulation can be toggled on/off; keyboard control assignments now override regular CPC keyboard actions; all dialogs now feature the "Caprice32 - " string to make identification easier; ZIP selector shows ZIP file name
   Apr 26, 2003 - 14:03 added CaptureScreenshot to save the screen contents to a PNG file
   Apr 26, 2003 - 16:25 finished screen capture implementation: added file selector and path/file storage to config file
   May 05, 2003 - 22:46 updated the Audio page in the Options dialog with controls to choose the sample size and adjust the volume
   May 06, 2003 - 18:59 load_snapshot and save_snapshot now use the v3 PSG information + the printer port data
   May 13, 2003 - 17:41 fixed the file selector problem on drag and drop: the exit condition check was reversed
   May 18, 2003 - 01:01 moved Gate Array memory handling to ga_memory_manager; save_snapshot correctly stores current RAM bank now
   May 21, 2003 - 00:31 changed the colour palette generation to support an intensity level slider
   May 21, 2003 - 14:42 added the Intensity slider control to the Video options
   May 26, 2003 - 20:06 emulation loop needs to keep running to produce proper key events: fixes the Pause problem
   May 28, 2003 - 13:59 modified the shadow of the print routine to make it more readable on a white background
   May 29, 2003 - 15:09 implemented the info message display system and added strings for most emulator actions and keyboard shortcuts
   May 30, 2003 - 12:17 added the tape PLAY/STOP button control for proper tape operation
   Jun 02, 2003 - 15:05 if the CreateSurface call fails with video memory, it now attempts to allocate it in system memory
   Jun 04, 2003 - 19:38 added the 'on printer port' drop-down to the Audio options page
   Aug 10, 2003 - 14:35 z80_IN_handler: CRTC write only registers return 0; load_snapshot: fixed broken snapshot support (PPI values were written to the wrong ports!); digiblaster/soundplayer combined into one - changed from a drop down to a check box

   May 19, 2004 - 23:13 removed all DirectX/Windows specific parts and replaced them (wherever possible) with the SDL equivalent
   May 23, 2004 - 17:48 dropped double buffer scheme in favour of a back buffer/custom blit operation; fixed colour palette init in 8bpp mode; added support for half size render mode; back buffer is cleared in video_init(); replaced the SDL_Flip operation with an SDL_BlitSurface; initial ExitCondition is now EC_FRAME_COMPLETE to ensure we have a valid video memory pointer to start with
   May 24, 2004 - 00:49 reintroduced snapshot_load & snapshot_save; modified vdu_init to update the two SDL_Rect structures to center or crop the CPC screen; introduced the dwXScale var to support the half size render mode
   May 29, 2004 - 18:09 reintroduced tape_eject, tape_insert and tape_insert_voc; added sound support via the native SDL audio routines
*/

#include <zlib.h>

#ifdef __CELLOS_LV2__
#define tmpnam(a)
#endif

#define AUTODELAY 50

/* forward declarations - some libretro port callbacks */
void retro_loop(void);
void doCleanUp (void);
void theloop(void);
int capmain (int argc, char **argv);
void retro_audio_cb( short l, short r);
void mixsnd (void);
void theloop(void);
long GetTicks(void);
int HandleExtension(char *path,char *ext);

#include "libretro-core.h"
extern unsigned int bmp[WINDOW_MAX_SIZE];
extern char RPATH[512];
extern int SND;
extern int autorun;
extern bool kbd_runcmd;
int autoboot_delay=0;

extern void kbd_buf_feed(char *s);
extern void kbd_buf_update();

extern char DISKA_NAME[512];
extern char DISKB_NAME[512];
extern char TAPE_NAME[512];
extern char cart_name[512];

#include "cap32.h"
#include "crtc.h"
#include "tape.h"
#include "cart.h"
#include "z80.h"
#include "asic.h"
#include "errors.h"

#define VERSION_STRING "v4.2.0"

#define MSG_SNA_LOAD             32
#define MSG_SNA_SAVE             33
#define MSG_DSK_LOAD             34
#define MSG_DSK_SAVE             35
#define MSG_JOY_ENABLE           36
#define MSG_JOY_DISABLE          37
#define MSG_SPD_NORMAL           38
#define MSG_SPD_FULL             39
#define MSG_TAP_INSERT           40
#define MSG_SDUMP_SAVE           41
#define MSG_PAUSED               42
#define MSG_TAP_PLAY             43
#define MSG_TAP_STOP             44

#define MAX_LINE_LEN 256

#define MIN_SPEED_SETTING 2
#define MAX_SPEED_SETTING 32
#define DEF_SPEED_SETTING 4

extern uint8_t bTapeLevel;
extern t_z80regs z80;

extern uint32_t *ScanPos;
extern uint32_t *ScanStart;
extern uint16_t MaxVSync;
extern t_flags1 flags1;
extern t_new_dt new_dt;

//video_plugin* vid_plugin;
uint32_t dwSndMinSafeDist=0, dwSndMaxSafeDist=2*2*882;

uint32_t dwTicks, dwTicksOffset, dwTicksTarget, dwTicksTargetFPS;
uint32_t dwFPS, dwFrameCount;
uint32_t dwXScale, dwYScale;
uint32_t dwSndBufferCopied;

uint32_t dwBreakPoint, dwTrace, dwMF2ExitAddr;
uint32_t dwMF2Flags = 0;
uint8_t *pbGPBuffer = NULL;
uint8_t *pbSndBuffer = NULL;
uint8_t *pbSndBufferEnd = NULL;
uint8_t *pbSndStream = NULL;
uint8_t *membank_read[4], *membank_write[4], *memmap_ROM[256];
uint8_t *pbRAM = NULL;
uint8_t *pbROM = NULL;
uint8_t *pbROMlo = NULL;
uint8_t *pbROMhi = NULL;
uint8_t *pbExpansionROM = NULL;
uint8_t *pbMF2ROMbackup = NULL;
uint8_t *pbMF2ROM = NULL;
uint8_t *pbTapeImage = NULL;
uint8_t *pbTapeImageEnd = NULL;
uint8_t keyboard_matrix[16];

static uint8_t *membank_config[8][4];

FILE *pfileObject;
FILE *pfoPrinter;

#ifdef DEBUG
//#define DEBUG_KEY RETROK_F9
uint32_t dwDebugFlag = 1;
FILE *pfoDebug;
#endif

#define MAX_FREQ_ENTRIES 5
uint32_t freq_table[MAX_FREQ_ENTRIES] = {
   11025,
   22050,
   44100,
   48000,
   96000
};

static double colours_rgb[32][3] = {
   { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 },{ 0.0, 1.0, 0.5 }, { 1.0, 1.0, 0.5 },
   { 0.0, 0.0, 0.5 }, { 1.0, 0.0, 0.5 },{ 0.0, 0.5, 0.5 }, { 1.0, 0.5, 0.5 },
   { 1.0, 0.0, 0.5 }, { 1.0, 1.0, 0.5 },{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0 },
   { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 1.0 },{ 1.0, 0.5, 0.0 }, { 1.0, 0.5, 1.0 },
   { 0.0, 0.0, 0.5 }, { 0.0, 1.0, 0.5 },{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 },
   { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },{ 0.0, 0.5, 0.0 }, { 0.0, 0.5, 1.0 },
   { 0.5, 0.0, 0.5 }, { 0.5, 1.0, 0.5 },{ 0.5, 1.0, 0.0 }, { 0.5, 1.0, 1.0 },
   { 0.5, 0.0, 0.0 }, { 0.5, 0.0, 1.0 },{ 0.5, 0.5, 0.0 }, { 0.5, 0.5, 1.0 }
};

static double colours_green[32] = {
   0.5647, 0.5647, 0.7529, 0.9412,
   0.1882, 0.3765, 0.4706, 0.6588,
   0.3765, 0.9412, 0.9098, 0.9725,
   0.3451, 0.4078, 0.6275, 0.6902,
   0.1882, 0.7529, 0.7216, 0.7843,
   0.1569, 0.2196, 0.4392, 0.5020,
   0.2824, 0.8471, 0.8157, 0.8784,
   0.2510, 0.3137, 0.5333, 0.5961
};

uint32_t colours[32];

static uint8_t bit_values[8] = {
   0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

#define MOD_CPC_SHIFT   (0x01 << 8)
#define MOD_CPC_CTRL    (0x02 << 8)
#define MOD_EMU_KEY     (0x10 << 8)

typedef enum {
   CAP32_EXIT = MOD_EMU_KEY,
   CAP32_FPS,
   CAP32_FULLSCRN,
   CAP32_JOY,
   CAP32_LOADDRVA,
   CAP32_LOADDRVB,
   CAP32_LOADSNAP,
   CAP32_LOADTAPE,
   CAP32_MF2RESET,
   CAP32_MF2STOP,
   CAP32_OPTIONS,
   CAP32_PAUSE,
   CAP32_RESET,
   CAP32_SAVESNAP,
   CAP32_SCRNSHOT,
   CAP32_SPEED,
   CAP32_TAPEPLAY
} CAP32_KEYS;


#define KMOD_CTRL (KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT  (KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT  (KMOD_LALT|KMOD_RALT)
#define KMOD_META (KMOD_LMETA|KMOD_RMETA)

#define MOD_PC_SHIFT    (KMOD_SHIFT << 16)
#define MOD_PC_CTRL     (KMOD_CTRL << 16)
#define MOD_PC_MODE     (KMOD_MODE << 16)

#define KBD_MAX_ENTRIES 160

#include "libretro.h"

//TAKEN FORM ARNOLD
typedef enum
{
	/* line 0, bit 0..bit 7 */
	CPC_KEY_CURSOR_UP = 0,
	CPC_KEY_CURSOR_RIGHT,
	CPC_KEY_CURSOR_DOWN,
	CPC_KEY_F9,
	CPC_KEY_F6,
	CPC_KEY_F3,
	CPC_KEY_SMALL_ENTER,
	CPC_KEY_FDOT,
	/* line 1, bit 0..bit 7 */ //8
	CPC_KEY_CURSOR_LEFT,
	CPC_KEY_COPY,
	CPC_KEY_F7,
	CPC_KEY_F8,
	CPC_KEY_F5,
	CPC_KEY_F1,
	CPC_KEY_F2,
	CPC_KEY_F0,
	/* line 2, bit 0..bit 7 */  //16
	CPC_KEY_CLR,
	CPC_KEY_OPEN_SQUARE_BRACKET,
	CPC_KEY_RETURN,
	CPC_KEY_CLOSE_SQUARE_BRACKET,
	CPC_KEY_F4,
	CPC_KEY_SHIFT,
	CPC_KEY_FORWARD_SLASH,
	CPC_KEY_CONTROL,
	/* line 3, bit 0.. bit 7 */ //24
	CPC_KEY_HAT,
	CPC_KEY_MINUS,
	CPC_KEY_AT,
	CPC_KEY_P,
	CPC_KEY_SEMICOLON,
	CPC_KEY_COLON,
	CPC_KEY_BACKSLASH,
	CPC_KEY_DOT,
	/* line 4, bit 0..bit 7 */ //32
	CPC_KEY_ZERO,
	CPC_KEY_9,
	CPC_KEY_O,
	CPC_KEY_I,
	CPC_KEY_L,
	CPC_KEY_K,
	CPC_KEY_M,
	CPC_KEY_COMMA,
	/* line 5, bit 0..bit 7 */ //40
	CPC_KEY_8,
	CPC_KEY_7,
	CPC_KEY_U,
	CPC_KEY_Y,
	CPC_KEY_H,
	CPC_KEY_J,
	CPC_KEY_N,
	CPC_KEY_SPACE,
	/* line 6, bit 0..bit 7 */ //48
	CPC_KEY_6,
	CPC_KEY_5,
	CPC_KEY_R,
	CPC_KEY_T,
	CPC_KEY_G,
	CPC_KEY_F,
	CPC_KEY_B,
	CPC_KEY_V,
	/* line 7, bit 0.. bit 7 */ //56
	CPC_KEY_4,
	CPC_KEY_3,
	CPC_KEY_E,
	CPC_KEY_W,
	CPC_KEY_S,
	CPC_KEY_D,
	CPC_KEY_C,
	CPC_KEY_X,
	/* line 8, bit 0.. bit 7 */ //64
	CPC_KEY_1,
	CPC_KEY_2,
	CPC_KEY_ESC,
	CPC_KEY_Q,
	CPC_KEY_TAB,
	CPC_KEY_A,
	CPC_KEY_CAPS_LOCK,
	CPC_KEY_Z,
	/* line 9, bit 7..bit 0 */  //72
	CPC_KEY_JOY_UP,
	CPC_KEY_JOY_DOWN,
	CPC_KEY_JOY_LEFT,
	CPC_KEY_JOY_RIGHT,
	CPC_KEY_JOY_FIRE1,
	CPC_KEY_JOY_FIRE2,
	CPC_KEY_SPARE,
	CPC_KEY_DEL,


	/* no key press */
	CPC_KEY_NULL
} CPC_KEY_ID;

void CPC_SetKey(int KeyID)
{
	if (KeyID!=CPC_KEY_NULL)
	{
		int Line = KeyID>>3;
		int Bit = KeyID & 0x07;
		keyboard_matrix/*KeyboardData*/[Line] &= ~(1<<Bit);
	}
}

void CPC_ClearKey(int KeyID)
{
	if (KeyID!=CPC_KEY_NULL)
	{
		int Line = KeyID>>3;
		int Bit = KeyID & 0x07;
		keyboard_matrix/*KeyboardData*/[Line] |= (1<<Bit);
	}
}

static int KeySymToCPCKey[512];

void	retro_InitialiseKeyboardMapping()
{
	int	 i;

	for (i=0; i<512; i++)
	{
		KeySymToCPCKey[i] = CPC_KEY_NULL;
	}

	/* International key mappings */
	KeySymToCPCKey[RETROK_0] = CPC_KEY_ZERO;
	KeySymToCPCKey[RETROK_1] = CPC_KEY_1;
	KeySymToCPCKey[RETROK_2] = CPC_KEY_2;
	KeySymToCPCKey[RETROK_3] = CPC_KEY_3;
	KeySymToCPCKey[RETROK_4] = CPC_KEY_4;
	KeySymToCPCKey[RETROK_5] = CPC_KEY_5;
	KeySymToCPCKey[RETROK_6] = CPC_KEY_6;
	KeySymToCPCKey[RETROK_7] = CPC_KEY_7;
	KeySymToCPCKey[RETROK_8] = CPC_KEY_8;
	KeySymToCPCKey[RETROK_9] = CPC_KEY_9;
	KeySymToCPCKey[RETROK_a] = CPC_KEY_A;
	KeySymToCPCKey[RETROK_b] = CPC_KEY_B;
	KeySymToCPCKey[RETROK_c] = CPC_KEY_C;
	KeySymToCPCKey[RETROK_d] = CPC_KEY_D;
	KeySymToCPCKey[RETROK_e] = CPC_KEY_E;
	KeySymToCPCKey[RETROK_f] = CPC_KEY_F;
	KeySymToCPCKey[RETROK_g] = CPC_KEY_G;
	KeySymToCPCKey[RETROK_h] = CPC_KEY_H;
	KeySymToCPCKey[RETROK_i] = CPC_KEY_I;
	KeySymToCPCKey[RETROK_j] = CPC_KEY_J;
	KeySymToCPCKey[RETROK_k] = CPC_KEY_K;
	KeySymToCPCKey[RETROK_l] = CPC_KEY_L;
	KeySymToCPCKey[RETROK_m] = CPC_KEY_M;
	KeySymToCPCKey[RETROK_n] = CPC_KEY_N;
	KeySymToCPCKey[RETROK_o] = CPC_KEY_O;
	KeySymToCPCKey[RETROK_p] = CPC_KEY_P;
	KeySymToCPCKey[RETROK_q] = CPC_KEY_Q;
	KeySymToCPCKey[RETROK_r] = CPC_KEY_R;
	KeySymToCPCKey[RETROK_s] = CPC_KEY_S;
	KeySymToCPCKey[RETROK_t] = CPC_KEY_T;
	KeySymToCPCKey[RETROK_u] = CPC_KEY_U;
	KeySymToCPCKey[RETROK_v] = CPC_KEY_V;
	KeySymToCPCKey[RETROK_w] = CPC_KEY_W;
	KeySymToCPCKey[RETROK_x] = CPC_KEY_X;
	KeySymToCPCKey[RETROK_y] = CPC_KEY_Y;
	KeySymToCPCKey[RETROK_z] = CPC_KEY_Z;
	KeySymToCPCKey[RETROK_SPACE] = CPC_KEY_SPACE;
	KeySymToCPCKey[RETROK_COMMA] = CPC_KEY_COMMA;
	KeySymToCPCKey[RETROK_PERIOD] = CPC_KEY_DOT;
	KeySymToCPCKey[RETROK_SEMICOLON] = CPC_KEY_COLON;
	KeySymToCPCKey[RETROK_MINUS] = CPC_KEY_MINUS;
	KeySymToCPCKey[RETROK_EQUALS] = CPC_KEY_HAT;
	KeySymToCPCKey[RETROK_LEFTBRACKET] = CPC_KEY_AT;
	KeySymToCPCKey[RETROK_RIGHTBRACKET] =CPC_KEY_OPEN_SQUARE_BRACKET;

	KeySymToCPCKey[RETROK_TAB] = CPC_KEY_TAB;
	KeySymToCPCKey[RETROK_RETURN] = CPC_KEY_RETURN;
	KeySymToCPCKey[RETROK_BACKSPACE] = CPC_KEY_DEL;
	KeySymToCPCKey[RETROK_ESCAPE] = CPC_KEY_ESC;

	//KeySymToCPCKey[RETROK_Equals & 0x0ff)] = CPC_KEY_CLR;

	KeySymToCPCKey[RETROK_UP] = CPC_KEY_CURSOR_UP;
	KeySymToCPCKey[RETROK_DOWN] = CPC_KEY_CURSOR_DOWN;
	KeySymToCPCKey[RETROK_LEFT] = CPC_KEY_CURSOR_LEFT;
	KeySymToCPCKey[RETROK_RIGHT] = CPC_KEY_CURSOR_RIGHT;

	KeySymToCPCKey[RETROK_KP0] = CPC_KEY_F0;
	KeySymToCPCKey[RETROK_KP1] = CPC_KEY_F1;
	KeySymToCPCKey[RETROK_KP2] = CPC_KEY_F2;
	KeySymToCPCKey[RETROK_KP3] = CPC_KEY_F3;
	KeySymToCPCKey[RETROK_KP4] = CPC_KEY_F4;
	KeySymToCPCKey[RETROK_KP5] = CPC_KEY_F5;
	KeySymToCPCKey[RETROK_KP6] = CPC_KEY_F6;
	KeySymToCPCKey[RETROK_KP7] = CPC_KEY_F7;
	KeySymToCPCKey[RETROK_KP8] = CPC_KEY_F8;
	KeySymToCPCKey[RETROK_KP9] = CPC_KEY_F9;

	KeySymToCPCKey[RETROK_KP_PERIOD] = CPC_KEY_FDOT;

	KeySymToCPCKey[RETROK_LSHIFT] = CPC_KEY_SHIFT;
	KeySymToCPCKey[RETROK_RSHIFT] = CPC_KEY_SHIFT;
	KeySymToCPCKey[RETROK_LCTRL] = CPC_KEY_CONTROL;
	KeySymToCPCKey[RETROK_RCTRL] = CPC_KEY_CONTROL;
	KeySymToCPCKey[RETROK_CAPSLOCK] = CPC_KEY_CAPS_LOCK;

	KeySymToCPCKey[RETROK_KP_ENTER] = CPC_KEY_SMALL_ENTER;

	KeySymToCPCKey[RETROK_DELETE] = CPC_KEY_JOY_LEFT;
	KeySymToCPCKey[RETROK_END] = CPC_KEY_JOY_DOWN;
	KeySymToCPCKey[RETROK_PAGEDOWN] = CPC_KEY_JOY_RIGHT;
	KeySymToCPCKey[RETROK_INSERT] = CPC_KEY_JOY_FIRE1;
	KeySymToCPCKey[RETROK_HOME] = CPC_KEY_JOY_UP;
	KeySymToCPCKey[RETROK_PAGEUP] = CPC_KEY_JOY_FIRE2;

	KeySymToCPCKey[0x0134] = CPC_KEY_COPY;			/* Alt */
	KeySymToCPCKey[0x0137] = CPC_KEY_COPY;			/* Compose */

}


#define MAX_ROM_MODS 2
extern /*static*/ char cpc_keytrans[MAX_ROM_MODS][240];
#include "rom_mods.c"

char chAppPath[_MAX_PATH + 1];
char chROMSelected[_MAX_PATH + 1];
char chROMFile[3][14] = {
   "cpc464.rom",
   "cpc664.rom",
   "cpc6128.rom"
};

t_CPC CPC;
t_CRTC CRTC;
t_FDC FDC;
t_GateArray GateArray;
t_PPI PPI;
t_PSG PSG;
t_VDU VDU;

t_drive driveA;
t_drive driveB;

t_zip_info zip_info;

#define MAX_DISK_FORMAT 8
#define DEFAULT_DISK_FORMAT 0
#define FIRST_CUSTOM_DISK_FORMAT 2
t_disk_format disk_format[MAX_DISK_FORMAT] = {
   { "178K Data Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0xc1, 0xc6, 0xc2, 0xc7, 0xc3, 0xc8, 0xc4, 0xc9, 0xc5 }} },
   { "169K Vendor Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0x41, 0x46, 0x42, 0x47, 0x43, 0x48, 0x44, 0x49, 0x45 }} }
};


#define psg_write \
{ \
   uint8_t control = PSG.control & 0xc0; /* isolate PSG control bits */ \
   if (control == 0xc0) { /* latch address? */ \
      PSG.reg_select = psg_data; /* select new PSG register */ \
   } else if (control == 0x80) { /* write? */ \
      if (PSG.reg_select < 16) { /* valid register? */ \
         SetAYRegister(PSG.reg_select, psg_data); \
      } \
   } \
}



void ga_init_banking (void)
{
   uint8_t *romb0, *romb1, *romb2, *romb3, *romb4, *romb5, *romb6, *romb7;
   uint8_t *pbRAMbank;

   romb0 = pbRAM;
   romb1 = pbRAM + 1*16384;
   romb2 = pbRAM + 2*16384;
   romb3 = pbRAM + 3*16384;

   pbRAMbank = pbRAM + ((GateArray.RAM_bank + 1) * 65536);
   romb4 = pbRAMbank;
   romb5 = pbRAMbank + 1*16384;
   romb6 = pbRAMbank + 2*16384;
   romb7 = pbRAMbank + 3*16384;

   membank_config[0][0] = romb0;
   membank_config[0][1] = romb1;
   membank_config[0][2] = romb2;
   membank_config[0][3] = romb3;

   membank_config[1][0] = romb0;
   membank_config[1][1] = romb1;
   membank_config[1][2] = romb2;
   membank_config[1][3] = romb7;

   membank_config[2][0] = romb4;
   membank_config[2][1] = romb5;
   membank_config[2][2] = romb6;
   membank_config[2][3] = romb7;

   membank_config[3][0] = romb0;
   membank_config[3][1] = romb3;
   membank_config[3][2] = romb2;
   membank_config[3][3] = romb7;

   membank_config[4][0] = romb0;
   membank_config[4][1] = romb4;
   membank_config[4][2] = romb2;
   membank_config[4][3] = romb3;

   membank_config[5][0] = romb0;
   membank_config[5][1] = romb5;
   membank_config[5][2] = romb2;
   membank_config[5][3] = romb3;

   membank_config[6][0] = romb0;
   membank_config[6][1] = romb6;
   membank_config[6][2] = romb2;
   membank_config[6][3] = romb3;

   membank_config[7][0] = romb0;
   membank_config[7][1] = romb7;
   membank_config[7][2] = romb2;
   membank_config[7][3] = romb3;
}



void ga_memory_manager (void)
{
   unsigned n;
   uint32_t mem_bank;
   if (CPC.ram_size == 64) { // 64KB of RAM?
      mem_bank = 0; // no expansion memory
      GateArray.RAM_config = 0; // the only valid configuration is 0
   } else {
      mem_bank = (GateArray.RAM_config >> 3) & 7; // extract expansion memory bank
      if (((mem_bank+2)*64) > CPC.ram_size) { // selection is beyond available memory?
         mem_bank = 0; // force default mapping
      }
   }
   if (mem_bank != GateArray.RAM_bank) { // requested bank is different from the active one?
      GateArray.RAM_bank = mem_bank;
      ga_init_banking();
   }
   for (n = 0; n < 4; n++) { // remap active memory banks
      membank_read[n] = membank_config[GateArray.RAM_config & 7][n];
      membank_write[n] = membank_config[GateArray.RAM_config & 7][n];
   }
   if (!(GateArray.ROM_config & 0x04)) { // lower ROM is enabled?
      if (dwMF2Flags & MF2_ACTIVE) { // is the Multiface 2 paged in?
         membank_read[GateArray.lower_ROM_bank] = pbMF2ROM;
         membank_write[GateArray.lower_ROM_bank] = pbMF2ROM;
      } else {
         membank_read[GateArray.lower_ROM_bank] = pbROMlo; // 'page in' lower ROM
      }
   }
   if (CPC.model >= 3 && GateArray.registerPageOn) {
      membank_read[1] = pbRegisterPage;
      membank_write[1] = pbRegisterPage;
   }
   if (!(GateArray.ROM_config & 0x08)) { // upper/expansion ROM is enabled?
      membank_read[3] = pbExpansionROM; // 'page in' upper/expansion ROM
   }
}



uint8_t z80_IN_handler (reg_pair port)
{
   uint8_t ret_val;

   ret_val = 0xff; // default return value
// CRTC -----------------------------------------------------------------------
   if (!(port.b.h & 0x40)) { // CRTC chip select?
      if ((port.b.h & 3) == 3) { // read CRTC register?
         if ((CRTC.reg_select > 11) && (CRTC.reg_select < 18)) { // valid range?
            ret_val = CRTC.registers[CRTC.reg_select];
         }
         else {
            ret_val = 0; // write only registers return 0
         }
      }
   }
// PPI ------------------------------------------------------------------------
   else if (!(port.b.h & 0x08)) { // PPI chip select?
      uint8_t ppi_port = port.b.h & 3;
      switch (ppi_port) {
         case 0: // read from port A?
            if (PPI.control & 0x10) { // port A set to input?
               if ((PSG.control & 0xc0) == 0x40) { // PSG control set to read?
                  if (PSG.reg_select < 16) { // within valid range?
                     if (PSG.reg_select == 14) { // PSG port A?
                        if (!(PSG.RegisterAY.Index[7] & 0x40)) { // port A in input mode?
                           ret_val = keyboard_matrix[CPC.keyboard_line & 0x0f]; // read keyboard matrix node status
                        } else {
                           ret_val = PSG.RegisterAY.Index[14] & (keyboard_matrix[CPC.keyboard_line & 0x0f]); // return last value w/ logic AND of input
                        }
                     } else if (PSG.reg_select == 15) { // PSG port B?
                        if ((PSG.RegisterAY.Index[7] & 0x80)) { // port B in output mode?
                           ret_val = PSG.RegisterAY.Index[15]; // return stored value
                        }
                     } else {
                        ret_val = PSG.RegisterAY.Index[PSG.reg_select]; // read PSG register
                     }
                  }
               }
            } else {
               ret_val = PPI.portA; // return last programmed value
            }
            break;

         case 1: // read from port B?
            if (PPI.control & 2) { // port B set to input?
               ret_val = bTapeLevel | // tape level when reading
                         (CPC.printer ? 0 : 0x40) | // ready line of connected printer
                         (CPC.jumpers & 0x7f) | // manufacturer + 50Hz
								 (CRTC.flag_invsync ? 1 : 0); // VSYNC status
            } else {
               ret_val = PPI.portB; // return last programmed value
            }
            break;

         case 2: // read from port C?
            {
               uint8_t direction = PPI.control & 9; // isolate port C directions
               ret_val = PPI.portC; // default to last programmed value
               if (direction) { // either half set to input?
                  if (direction & 8) { // upper half set to input?
                     ret_val &= 0x0f; // blank out upper half
                     uint8_t val = PPI.portC & 0xc0; // isolate PSG control bits
                     if (val == 0xc0) { // PSG specify register?
                        val = 0x80; // change to PSG write register
                     }
                     ret_val |= val | 0x20; // casette write data is always set
                     if (CPC.tape_motor) {
                        ret_val |= 0x10; // set the bit if the tape motor is running
                     }
                  }
                  if (!(direction & 1)) { // lower half set to output?
                     ret_val |= 0x0f; // invalid - set all bits
                  }
               }
            }
            break;
      }
   }
// ----------------------------------------------------------------------------
   else if (!(port.b.h & 0x04)) { // external peripheral?
      if ((port.b.h == 0xfb) && (!(port.b.l & 0x80))) { // FDC?
         if (!(port.b.l & 0x01)) { // FDC status register?
            ret_val = fdc_read_status();
         } else { // FDC data register
            ret_val = fdc_read_data();
         }
      }
   }
   return ret_val;
}



void z80_OUT_handler (reg_pair port, uint8_t val)
{
   /* Gate Array */

   /* GA chip select? */
   if ((port.b.h & 0xc0) == 0x40)
   {
      switch (val >> 6) {
         case 0: // select pen
            #ifdef DEBUG_GA
            if (dwDebugFlag) {
               fprintf(pfoDebug, "pen 0x%02x\r\n", val);
            }
            #endif
            GateArray.pen = val & 0x10 ? 0x10 : val & 0x0f; // if bit 5 is set, pen indexes the border colour
            if (CPC.mf2) { // MF2 enabled?
               *(pbMF2ROM + 0x03fcf) = val;
            }
            break;
         case 1: // set colour
            #ifdef DEBUG_GA
            if (dwDebugFlag) {
               fprintf(pfoDebug, "clr 0x%02x\r\n", val);
            }
            #endif
            {
               uint8_t colour = val & 0x1f; // isolate colour value
               GateArray.ink_values[GateArray.pen] = colour;
               GateArray.palette[GateArray.pen] =colours[colour];//SDL_MapRGB(back_surface->format,
                //colours[colour].r, colours[colour].g, colours[colour].b);
               if (GateArray.pen < 2) {

//FIXME RETRO
unsigned char r,g,b,r2,g2,b2;
r=(colours[GateArray.ink_values[0]]>>16)&0xFF;
g=(colours[GateArray.ink_values[0]]>>8)&0xFF;
b=colours[GateArray.ink_values[0]]&0xFF;
r2=(colours[GateArray.ink_values[1]]>>16)&0xFF;
g2=(colours[GateArray.ink_values[1]]>>8)&0xFF;
b2=colours[GateArray.ink_values[1]]&0xFF;
GateArray.palette[18] = (b+b2)>>1 | ((g+g2)<< 7) | ((r+r2) << 15);

               }
            }
            if (CPC.mf2) { // MF2 enabled?
               int iPen = *(pbMF2ROM + 0x03fcf);
               *(pbMF2ROM + (0x03f90 | ((iPen & 0x10) << 2) | (iPen & 0x0f))) = val;
            }
            break;
         case 2: // set mode
            if (val& 0x20){
               // 6128+ RMR2 register
               if (!asic_locked) {
                  int membank = (val >> 3) & 3;
                  if (membank == 3) { // Map register page at 0x4000
                     //printf("Register page on\n");
                     GateArray.registerPageOn = true;
                     membank = 0;
                  } else {
                     //printf("Register page off\n");
                     GateArray.registerPageOn = false;
                  }
                  GateArray.lower_ROM_bank = membank;
                  pbROMlo = pbCartridgePages[(val & 0x7)];
                  ga_memory_manager();
               }
            }else{
               #ifdef DEBUG_GA
               if (dwDebugFlag) {
                  fprintf(pfoDebug, "rom 0x%02x\r\n", val);
               }
               #endif
               GateArray.ROM_config = val;
               GateArray.requested_scr_mode = val & 0x03; // request a new CPC screen mode
               ga_memory_manager();
               if (val & 0x10) { // delay Z80 interrupt?
                  z80.int_pending = 0; // clear pending interrupts
                  GateArray.sl_count = 0; // reset GA scanline counter
               }
               if (CPC.mf2) { // MF2 enabled?
                  *(pbMF2ROM + 0x03fef) = val;
               }
            }
            break;
         case 3: // set memory configuration
            #ifdef DEBUG_GA
            if (dwDebugFlag) {
               fprintf(pfoDebug, "mem 0x%02x\r\n", val);
            }
            #endif
            GateArray.RAM_config = val;
            ga_memory_manager();
            if (CPC.mf2) { // MF2 enabled?
               *(pbMF2ROM + 0x03fff) = val;
            }
            break;
      }
   }

   /* CRTC */
   if (!(port.b.h & 0x40)) { // CRTC chip select?
      uint8_t crtc_port = port.b.h & 3;
      if (crtc_port == 0) { // CRTC register select?
         // 6128+: this is where we should detect the ASIC (un)locking sequence
         asic_poke_lock_sequence(val);
         CRTC.reg_select = val;
         if (CPC.mf2) { // MF2 enabled?
            *(pbMF2ROM + 0x03cff) = val;
         }
      }
      else if (crtc_port == 1) { // CRTC write data?
         if (CRTC.reg_select < 16) { // only registers 0 - 15 can be written to
            switch (CRTC.reg_select) {
               case 0: // horizontal total
                  CRTC.registers[0] = val;
                  break;
               case 1: // horizontal displayed
                  CRTC.registers[1] = val;
                  update_skew();
                  break;
               case 2: // horizontal sync position
                  CRTC.registers[2] = val;
                  break;
               case 3: // sync width
                  CRTC.registers[3] = val;
                  CRTC.hsw = val & 0x0f; // isolate horizontal sync width
                  CRTC.vsw = val >> 4; // isolate vertical sync width
                  break;
               case 4: // vertical total
                  CRTC.registers[4] = val & 0x7f;
                  if (CRTC.CharInstMR == (void(*)(void))CharMR2) {
                     if (CRTC.line_count == CRTC.registers[4]) { // matches vertical total?
                        if (CRTC.raster_count == CRTC.registers[9]) { // matches maximum raster address?
                           CRTC.flag_startvta = 1;
                        }
                     }
                  }
                  break;
               case 5: // vertical total adjust
                  CRTC.registers[5] = val & 0x1f;
                  break;
               case 6: // vertical displayed
                  CRTC.registers[6] = val & 0x7f;
                  if (CRTC.line_count == CRTC.registers[6]) { // matches vertical displayed?
                     new_dt.NewDISPTIMG = 0;
                  }
                  break;
               case 7: // vertical sync position
                  CRTC.registers[7] = val & 0x7f;
                  {
                     register uint32_t temp = 0;
                     if (CRTC.line_count == CRTC.registers[7]) { // matches vertical sync position?
                        temp++;
                        if (CRTC.r7match != temp) {
                           CRTC.r7match = temp;
                           if (CRTC.char_count >= 2) {
                              CRTC.flag_resvsync = 0;
                              if (!CRTC.flag_invsync) {
                                 CRTC.vsw_count = 0;
                                 CRTC.flag_invsync = 1;
                                 flags1.monVSYNC = 26;
                                 GateArray.hs_count = 2; // GA delays its VSYNC by two CRTC HSYNCs
                              }
                           }
                        }
                     }
                     else {
                        CRTC.r7match = 0;
                     }
                  }
                  break;
               case 8: // interlace and skew
                  CRTC.registers[8] = val;
                  update_skew();
                  break;
               case 9: // maximum raster count
                  CRTC.registers[9] = val & 0x1f;
                  {
                     register uint32_t temp = 0;

                     // matches maximum raster address?
                     if (CRTC.raster_count == CRTC.registers[9])
                     {
                        temp = 1;
                        CRTC.flag_resscan = 1; // request a raster counter reset
                     }

                     if (CRTC.r9match != temp)
                     {
                        CRTC.r9match = temp;
                        if (temp)
                           CRTC.CharInstMR = (void(*)(void))CharMR1;
                     }

                     // matches maximum raster address?
                     if (CRTC.raster_count == CRTC.registers[9])
                     {
                        if (CRTC.char_count == CRTC.registers[1])
                           CRTC.next_addr = CRTC.addr + CRTC.char_count;

                        /* matches horizontal total? */
                        if (CRTC.char_count == CRTC.registers[0])
                           CRTC.flag_reschar = 1; // request a line count update
                        if (!CRTC.flag_startvta)
                           CRTC.flag_resscan = 1;
                     }
                     else
                     {
                        // not in vertical total adjust?
                        if (!CRTC.flag_invta)
                           CRTC.flag_resscan = 0;
                     }
                  }
                  break;
               case 10: // cursor start raster
                  CRTC.registers[10] = val & 0x7f;
                  break;
               case 11: // cursor end raster
                  CRTC.registers[11] = val & 0x1f;
                  break;
               case 12: // start address high byte
                  CRTC.registers[12] = val & 0x3f;
                  CRTC.requested_addr = CRTC.registers[13] + (CRTC.registers[12] << 8);
                  break;
               case 13: // start address low byte
                  CRTC.registers[13] = val;
                  CRTC.requested_addr = CRTC.registers[13] + (CRTC.registers[12] << 8);
                  break;
               case 14: // cursor address high byte
                  CRTC.registers[14] = val & 0x3f;
                  break;
               case 15: // cursor address low byte
                  CRTC.registers[15] = val;
                  break;
            }
         }
         if (CPC.mf2) { // MF2 enabled?
            *(pbMF2ROM + (0x03db0 | (*(pbMF2ROM + 0x03cff) & 0x0f))) = val;
         }
         #ifdef DEBUG_CRTC
         if (dwDebugFlag) {
            fprintf(pfoDebug, "%02x = %02x\r\n", CRTC.reg_select, val);
         }
         #endif


      }
   }

   /* ROM select */
   if (!(port.b.h & 0x20))
   {
      /* ROM select? */
      GateArray.upper_ROM = val;
      if (CPC.model <= 2) {
         pbExpansionROM = memmap_ROM[val];

         /* selected expansion ROM not present? */
         if (pbExpansionROM == NULL)
            pbExpansionROM = pbROMhi; /* revert to BASIC ROM */

         /* upper/expansion ROM is enabled? */
         if (!(GateArray.ROM_config & 0x08))
            membank_read[3] = pbExpansionROM; /* 'page in' upper/expansion ROM */

         /* MF2 enabled? */
         if (CPC.mf2)
            *(pbMF2ROM + 0x03aac) = val;
      } else {
         //printf("ROM select: %u\n", (int) val);
         if (val == 7) {
            pbExpansionROM = pbCartridgePages[3];
         } else if (val >= 128) {
            pbExpansionROM = pbCartridgePages[val & 31];
         } else {
            pbExpansionROM = pbCartridgePages[1];
         }
         //printf("ROM-PAGE select: %u\n", (int) page);
         //printf("ROM-PAGE val: %u\n", (int) pbExpansionROM[0]);
      }

   }

   /* printer port */
   if (!(port.b.h & 0x10))
   { // printer port?
      CPC.printer_port = val ^ 0x80; // invert bit 7
      if (pfoPrinter)
      {
         /* only grab data bytes; ignore the strobe signal */
         if (!(CPC.printer_port & 0x80))
            fputc(CPC.printer_port, pfoPrinter); // capture printer output to file
      }
   }

   /* PPI */
   if (!(port.b.h & 0x08))
   {
      /* PPI chip select? */
      switch (port.b.h & 3)
      {
         case 0: // write to port A?
            PPI.portA = val;
            if (!(PPI.control & 0x10)) { // port A set to output?
               uint8_t psg_data = val;
               psg_write
            }
            break;
         case 1: // write to port B?
            PPI.portB = val;
            break;
         case 2: // write to port C?
            PPI.portC = val;
            if (!(PPI.control & 1)) { // output lower half?
               CPC.keyboard_line = val;
            }
            if (!(PPI.control & 8)) { // output upper half?
               CPC.tape_motor = val & 0x10; // update tape motor control
               PSG.control = val; // change PSG control
               uint8_t psg_data = PPI.portA;
               psg_write
            }
            break;
         case 3: // modify PPI control
            if (val & 0x80) { // change PPI configuration
               PPI.control = val; // update control byte
               PPI.portA = 0; // clear data for all ports
               PPI.portB = 0;
               PPI.portC = 0;
            } else { // bit manipulation of port C data
               if (val & 1) { // set bit?
                  uint8_t bit = (val >> 1) & 7; // isolate bit to set
                  PPI.portC |= bit_values[bit]; // set requested bit
                  if (!(PPI.control & 1)) { // output lower half?
                     CPC.keyboard_line = PPI.portC;
                  }
                  if (!(PPI.control & 8)) { // output upper half?
                     CPC.tape_motor = PPI.portC & 0x10;
                     PSG.control = PPI.portC; // change PSG control
                     uint8_t psg_data = PPI.portA;
                     psg_write
                  }
               } else {
                  uint8_t bit = (val >> 1) & 7; // isolate bit to reset
                  PPI.portC &= ~(bit_values[bit]); // reset requested bit
                  if (!(PPI.control & 1)) { // output lower half?
                     CPC.keyboard_line = PPI.portC;
                  }
                  if (!(PPI.control & 8)) { // output upper half?
                     CPC.tape_motor = PPI.portC & 0x10;
                     PSG.control = PPI.portC; // change PSG control
                     uint8_t psg_data = PPI.portA;
                     psg_write
                  }
               }
            }

            if (CPC.mf2) // MF2 enabled?
               *(pbMF2ROM + 0x037ff) = val;
            break;
      }
   }

   if ((port.b.h == 0xfa) && (!(port.b.l & 0x80))) { // floppy motor control?
      //printf("FDC motor control access: %u - %u\n",  (int) port.b.l, (int) val);
      FDC.motor = val & 0x01;
      #ifdef DEBUG_FDC
      fputs(FDC.motor ? "\r\n--- motor on" : "\r\n--- motor off", pfoDebug);
      #endif
      FDC.flags |= STATUSDRVA_flag | STATUSDRVB_flag;
   }
   else if ((port.b.h == 0xfb) && (!(port.b.l & 0x80))) { // FDC data register?
      fdc_write_data(val);
   }
   else if ((CPC.mf2) && (port.b.h == 0xfe)) { // Multiface 2?
      if ((port.b.l == 0xe8) && (!(dwMF2Flags & MF2_INVISIBLE))) { // page in MF2 ROM?
         dwMF2Flags |= MF2_ACTIVE;
         ga_memory_manager();
      }
      else if (port.b.l == 0xea) { // page out MF2 ROM?
         dwMF2Flags &= ~MF2_ACTIVE;
         ga_memory_manager();
      }
   }
}

int file_size (int file_num)
{
   struct stat s;

   if (!fstat(file_num, &s)) {
      return s.st_size;
   } else {
      return 0;
   }
}

int zip_dir (t_zip_info *zi)
{
   int n, iFileCount;
   long lFilePosition;
   uint32_t dwCentralDirPosition, dwNextEntry;
   uint16_t wCentralDirEntries, wCentralDirSize, wFilenameLength;
   uint8_t *pbPtr;
   char *pchStrPtr;
   uint32_t dwOffset;

   iFileCount = 0;
   if ((pfileObject = fopen(zi->pchZipFile, "rb")) == NULL) {
      return ERR_FILE_NOT_FOUND;
   }

   wCentralDirEntries = 0;
   wCentralDirSize = 0;
   dwCentralDirPosition = 0;
   lFilePosition = -256;
   do {
      fseek(pfileObject, lFilePosition, SEEK_END);
      if (fread(pbGPBuffer, 256, 1, pfileObject) == 0) {
         fclose(pfileObject);
         return ERR_FILE_BAD_ZIP; // exit if loading of data chunck failed
      }
      pbPtr = pbGPBuffer + (256 - 22); // pointer to end of central directory (under ideal conditions)
      while (pbPtr != (uint8_t *)pbGPBuffer) {
         if (*(uint32_t *)pbPtr == 0x06054b50) { // check for end of central directory signature
            wCentralDirEntries = *(uint16_t *)(pbPtr + 10);
            wCentralDirSize = *(uint16_t *)(pbPtr + 12);
            dwCentralDirPosition = *(uint32_t *)(pbPtr + 16);
            break;
         }
         pbPtr--; // move backwards through buffer
      }
      lFilePosition -= 256; // move backwards through ZIP file
   } while (wCentralDirEntries == 0);
   if (wCentralDirSize == 0) {
      fclose(pfileObject);
      return ERR_FILE_BAD_ZIP; // exit if no central directory was found
   }
   fseek(pfileObject, dwCentralDirPosition, SEEK_SET);
   if (fread(pbGPBuffer, wCentralDirSize, 1, pfileObject) == 0) {
      fclose(pfileObject);
      return ERR_FILE_BAD_ZIP; // exit if loading of data chunck failed
   }

   pbPtr = pbGPBuffer;
   if (zi->pchFileNames) {
      free(zi->pchFileNames); // dealloc old string table
   }
   zi->pchFileNames = (char *)malloc(wCentralDirSize); // approximate space needed by using the central directory size
   pchStrPtr = zi->pchFileNames;

   for (n = wCentralDirEntries; n; n--) {
      wFilenameLength = *(uint16_t *)(pbPtr + 28);
      dwOffset = *(uint32_t *)(pbPtr + 42);
      dwNextEntry = wFilenameLength + *(uint16_t *)(pbPtr + 30) + *(uint16_t *)(pbPtr + 32);
      pbPtr += 46;
      char *pchThisExtension = zi->pchExtension;
      while (*pchThisExtension != '\0') { // loop for all extensions to be checked
         if (strncasecmp((char *)pbPtr + (wFilenameLength - 4), pchThisExtension, 4) == 0) {
            strncpy(pchStrPtr, (char *)pbPtr, wFilenameLength); // copy filename from zip directory
            pchStrPtr[wFilenameLength] = 0; // zero terminate string
            pchStrPtr += wFilenameLength+1;
            *(uint32_t *)pchStrPtr = dwOffset; // associate offset with string
            pchStrPtr += 4;
            iFileCount++;
            break;
         }
         pchThisExtension += 4; // advance to next extension
      }
      pbPtr += dwNextEntry;
   }
   fclose(pfileObject);

   if (iFileCount == 0) { // no files found?
      return ERR_FILE_EMPTY_ZIP;
   }

   zi->iFiles = iFileCount;
   return 0; // operation completed successfully
}

int zip_extract (char *pchZipFile, char *pchFileName, uint32_t dwOffset)
{
   int iStatus, iCount;
   uint32_t dwSize;
   uint8_t *pbInputBuffer, *pbOutputBuffer;
   FILE *pfileOut, *pfileIn;
   z_stream z;

   tmpnam(pchFileName); // generate a unique (temporary) file name for the decompression process

   if (!(pfileOut = fopen(pchFileName, "wb")))
      return ERR_FILE_UNZIP_FAILED; // couldn't create output file

   pfileIn = fopen(pchZipFile, "rb"); // open ZIP file for reading
   fseek(pfileIn, dwOffset, SEEK_SET); // move file pointer to beginning of data block
   if(!fread(pbGPBuffer, 30, 1, pfileIn)) { // read local header
      fclose(pfileIn);
      fclose(pfileOut);
      return ERR_FILE_UNZIP_FAILED;
   }
   dwSize = *(uint32_t *)(pbGPBuffer + 18); // length of compressed data
   dwOffset += 30 + *(uint16_t *)(pbGPBuffer + 26) + *(uint16_t *)(pbGPBuffer + 28);
   fseek(pfileIn, dwOffset, SEEK_SET); // move file pointer to start of compressed data

   pbInputBuffer = pbGPBuffer; // space for compressed data chunck
   pbOutputBuffer = pbInputBuffer + 16384; // space for uncompressed data chunck
   z.zalloc = (alloc_func)0;
   z.zfree = (free_func)0;
   z.opaque = (voidpf)0;
   iStatus = inflateInit2(&z, -MAX_WBITS); // init zlib stream (no header)
   do {
      z.next_in = pbInputBuffer;
      if (dwSize > 16384) { // limit input size to max 16K or remaining bytes
         z.avail_in = 16384;
      } else {
         z.avail_in = dwSize;
      }
      z.avail_in = fread(pbInputBuffer, 1, z.avail_in, pfileIn); // load compressed data chunck from ZIP file
      while ((z.avail_in) && (iStatus == Z_OK)) { // loop until all data has been processed
         z.next_out = pbOutputBuffer;
         z.avail_out = 16384;
         iStatus = inflate(&z, Z_NO_FLUSH); // decompress data
         iCount = 16384 - z.avail_out;
         if (iCount) { // save data to file if output buffer is full
            fwrite(pbOutputBuffer, 1, iCount, pfileOut);
         }
      }
      dwSize -= 16384; // advance to next chunck
   } while ((dwSize > 0) && (iStatus == Z_OK)) ; // loop until done
   if (iStatus != Z_STREAM_END) {
      return ERR_FILE_UNZIP_FAILED; // abort on error
   }
   iStatus = inflateEnd(&z); // clean up
   fclose(pfileIn);
   fclose(pfileOut);

   return 0; // data was successfully decompressed
}

extern int snapshot_load (char *pchFileName);

int snapshot_load (char *pchFileName)
{
   int n;
   uint32_t dwSnapSize, dwModel, dwFlags;
   char chPath[_MAX_PATH + 1];
   uint8_t val;
   reg_pair port;
   t_SNA_header sh;

   memset(&sh, 0, sizeof(sh));
   if ((pfileObject = fopen(pchFileName, "rb")) != NULL)
   {
      if(!fread(&sh, sizeof(sh), 1, pfileObject)) { // read snapshot header
         fclose(pfileObject);
         return ERR_SNA_INVALID;
      }
      if (memcmp(sh.id, "MV - SNA", 8) != 0) { // valid SNApshot image?
         fclose(pfileObject);
         return ERR_SNA_INVALID;
      }
      dwSnapSize = sh.ram_size[0] + (sh.ram_size[1] * 256); // memory dump size
      dwSnapSize &= ~0x3f; // limit to multiples of 64
      if (!dwSnapSize) {
         fclose(pfileObject);
         return ERR_SNA_SIZE;
      }
      if (dwSnapSize > CPC.ram_size) { // memory dump size differs from current RAM size?
         uint8_t *pbTemp;

         pbTemp = (uint8_t*) malloc(dwSnapSize*1024 * sizeof(uint8_t));
         if (pbTemp) {
            free(pbRAM);
            CPC.ram_size = dwSnapSize;
            pbRAM = pbTemp;
         } else {
            fclose(pfileObject);
            return ERR_OUT_OF_MEMORY;
         }
      }
      emulator_reset(false);
      n = fread(pbRAM, dwSnapSize*1024, 1, pfileObject); // read memory dump into CPC RAM
      fclose(pfileObject);
      if (!n) {
         emulator_reset(false);
         return ERR_SNA_INVALID;
      }

      // Z80
      _A = sh.AF[1];
      _F = sh.AF[0];
      _B = sh.BC[1];
      _C = sh.BC[0];
      _D = sh.DE[1];
      _E = sh.DE[0];
      _H = sh.HL[1];
      _L = sh.HL[0];
      _R = sh.R & 0x7f;
      _Rb7 = sh.R & 0x80; // bit 7 of R
      _I = sh.I;
      if (sh.IFF0)
         _IFF1 = Pflag;
      if (sh.IFF1)
         _IFF2 = Pflag;
      _IXh = sh.IX[1];
      _IXl = sh.IX[0];
      _IYh = sh.IY[1];
      _IYl = sh.IY[0];
      z80.SP.b.h = sh.SP[1];
      z80.SP.b.l = sh.SP[0];
      z80.PC.b.h = sh.PC[1];
      z80.PC.b.l = sh.PC[0];
      _IM = sh.IM; // interrupt mode
      z80.AFx.b.h = sh.AFx[1];
      z80.AFx.b.l = sh.AFx[0];
      z80.BCx.b.h = sh.BCx[1];
      z80.BCx.b.l = sh.BCx[0];
      z80.DEx.b.h = sh.DEx[1];
      z80.DEx.b.l = sh.DEx[0];
      z80.HLx.b.h = sh.HLx[1];
      z80.HLx.b.l = sh.HLx[0];
      // Gate Array
      port.b.h = 0x7f;
      for (n = 0; n < 17; n++) { // loop for all colours + border
         GateArray.pen = n;
         val = sh.ga_ink_values[n]; // GA palette entry
         z80_OUT_handler(port, val | (1<<6));
      }
      val = sh.ga_pen; // GA pen
      z80_OUT_handler(port, (val & 0x3f));
      val = sh.ga_ROM_config; // GA ROM configuration
      z80_OUT_handler(port, (val & 0x3f) | (2<<6));
      val = sh.ga_RAM_config; // GA RAM configuration
      z80_OUT_handler(port, (val & 0x3f) | (3<<6));
      // CRTC
      port.b.h = 0xbd;
      for (n = 0; n < 18; n++) { // loop for all CRTC registers
         val = sh.crtc_registers[n];
         CRTC.reg_select = n;
         z80_OUT_handler(port, val);
      }
      port.b.h = 0xbc;
      val = sh.crtc_reg_select; // CRTC register select
      z80_OUT_handler(port, val);
      // ROM select
      port.b.h = 0xdf;
      val = sh.upper_ROM; // upper ROM number
      z80_OUT_handler(port, val);
      // PPI
      port.b.h = 0xf4; // port A
      z80_OUT_handler(port, sh.ppi_A);
      port.b.h = 0xf5; // port B
      z80_OUT_handler(port, sh.ppi_B);
      port.b.h = 0xf6; // port C
      z80_OUT_handler(port, sh.ppi_C);
      port.b.h = 0xf7; // control
      z80_OUT_handler(port, sh.ppi_control);
      // PSG
      PSG.control = PPI.portC;
      PSG.reg_select = sh.psg_reg_select;
      for (n = 0; n < 16; n++) { // loop for all PSG registers
         SetAYRegister(n, sh.psg_registers[n]);
      }

      if (sh.version > 1)
      { // does the snapshot have version 2 data?
         dwModel = sh.cpc_model; // determine the model it was saved for
         if (dwModel != CPC.model) { // different from what we're currently running?
            if (dwModel > 3) { // not one of the known models?
               emulator_reset(false);
               return ERR_SNA_CPC_TYPE;
            }
            strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
            strcat(chPath, "/");
            strncat(chPath, chROMFile[dwModel], sizeof(chPath)-1 - strlen(chPath)); // path to the required ROM image
            if ((pfileObject = fopen(chPath, "rb")) != NULL) {
               n = fread(pbROM, 2*16384, 1, pfileObject);
               fclose(pfileObject);
               if (!n) {
                  emulator_reset(false);
                  return ERR_CPC_ROM_MISSING;
               }
               CPC.model = dwModel;
            } else { // ROM image load failed
               emulator_reset(false);
               return ERR_CPC_ROM_MISSING;
            }
         }
      }

      if (sh.version > 2)
      { // does the snapshot have version 3 data?
         FDC.motor = sh.fdc_motor;
         driveA.current_track = sh.drvA_current_track;
         driveB.current_track = sh.drvB_current_track;
         CPC.printer_port = sh.printer_data ^ 0x80; // invert bit 7 again
         PSG.AmplitudeEnv = sh.psg_env_step << 1; // multiply by 2 to bring it into the 0 - 30 range
         PSG.FirstPeriod = false;
         if (sh.psg_env_direction == 0x01) { // up
            switch (PSG.RegisterAY.EnvType)
            {
               case 4:
               case 5:
               case 6:
               case 7:
               case 13:
               case 14:
               case 15:
                  PSG.FirstPeriod = true;
                  break;
            }
         } else if (sh.psg_env_direction == 0xff) { // down
            switch (PSG.RegisterAY.EnvType)
            {
               case 0:
               case 1:
               case 2:
               case 3:
               case 9:
               case 10:
               case 11:
                  PSG.FirstPeriod = true;
                  break;
            }
         }
         CRTC.addr = sh.crtc_addr[0] + (sh.crtc_addr[1] * 256);
         VDU.scanline = sh.crtc_scanline[0] + (sh.crtc_scanline[1] * 256);
         if (VDU.scanline > MaxVSync) {
            VDU.scanline = MaxVSync; // limit to max value
         }
         CRTC.char_count = sh.crtc_char_count[0];
         CRTC.line_count = sh.crtc_line_count & 127;
         CRTC.raster_count = sh.crtc_raster_count & 31;
         CRTC.hsw_count = sh.crtc_hsw_count & 15;
         CRTC.vsw_count = sh.crtc_vsw_count & 15;
         dwFlags = sh.crtc_flags[0] + (sh.crtc_flags[1] * 256);
         CRTC.flag_invsync = dwFlags & 1 ? 1 : 0; // vsync active?
         if (dwFlags & 2) { // hsync active?
            flags1.inHSYNC = 0xff;
            CRTC.flag_hadhsync = 1;
            if ((CRTC.hsw_count >= 3) && (CRTC.hsw_count < 7)) {
               CRTC.flag_inmonhsync = 1;
            }
         }
         CRTC.flag_invta = dwFlags & 0x80 ? 1 : 0; // in vertical total adjust?
         GateArray.hs_count = sh.ga_int_delay & 3;
         GateArray.sl_count = sh.ga_sl_count;
         z80.int_pending = sh.z80_int_pending;
      }
   }
   else
      return ERR_FILE_NOT_FOUND;

   return 0;
}

int snapshot_save (char *pchFileName)
{
   t_SNA_header sh;
   int n;
   uint32_t dwFlags;

   memset(&sh, 0, sizeof(sh));

   memcpy(sh.id, "MV - SNA", sizeof(sh.id));

   sh.version = 3;

   /* Z80 */
   sh.AF[1] = _A;
   sh.AF[0] = _F;
   sh.BC[1] = _B;
   sh.BC[0] = _C;
   sh.DE[1] = _D;
   sh.DE[0] = _E;
   sh.HL[1] = _H;
   sh.HL[0] = _L;
   sh.R = (_R & 0x7f) | (_Rb7 & 0x80);
   sh.I = _I;
   if (_IFF1)
      sh.IFF0 = 1;
   if (_IFF2)
      sh.IFF1 = 1;
   sh.IX[1] = _IXh;
   sh.IX[0] = _IXl;
   sh.IY[1] = _IYh;
   sh.IY[0] = _IYl;
   sh.SP[1] = z80.SP.b.h;
   sh.SP[0] = z80.SP.b.l;
   sh.PC[1] = z80.PC.b.h;
   sh.PC[0] = z80.PC.b.l;
   sh.IM = _IM;
   sh.AFx[1] = z80.AFx.b.h;
   sh.AFx[0] = z80.AFx.b.l;
   sh.BCx[1] = z80.BCx.b.h;
   sh.BCx[0] = z80.BCx.b.l;
   sh.DEx[1] = z80.DEx.b.h;
   sh.DEx[0] = z80.DEx.b.l;
   sh.HLx[1] = z80.HLx.b.h;
   sh.HLx[0] = z80.HLx.b.l;
   // Gate Array
   sh.ga_pen = GateArray.pen;
   for (n = 0; n < 17; n++) { // loop for all colours + border
      sh.ga_ink_values[n] = GateArray.ink_values[n];
   }
   sh.ga_ROM_config = GateArray.ROM_config;
   sh.ga_RAM_config = GateArray.RAM_config;
   // CRTC
   sh.crtc_reg_select = CRTC.reg_select;
   for (n = 0; n < 18; n++) { // loop for all CRTC registers
      sh.crtc_registers[n] = CRTC.registers[n];
   }

   /* ROM select */
   sh.upper_ROM = GateArray.upper_ROM;

   /* PPI */
   sh.ppi_A = PPI.portA;
   sh.ppi_B = PPI.portB;
   sh.ppi_C = PPI.portC;
   sh.ppi_control = PPI.control;

   /* PSG */
   sh.psg_reg_select = PSG.reg_select;

   for (n = 0; n < 16; n++)
   {
      /* loop for all PSG registers */
      sh.psg_registers[n] = PSG.RegisterAY.Index[n];
   }

   sh.ram_size[0] = CPC.ram_size & 0xff;
   sh.ram_size[1] = (CPC.ram_size >> 8) & 0xff;
   /* version 2 info */
   sh.cpc_model = CPC.model;
   /* version 3 info */
   sh.fdc_motor = FDC.motor;
   sh.drvA_current_track = driveA.current_track;
   sh.drvB_current_track = driveB.current_track;
   sh.printer_data = CPC.printer_port ^ 0x80; // invert bit 7 again
   sh.psg_env_step = PSG.AmplitudeEnv >> 1; // divide by 2 to bring it into the 0 - 15 range
   if (PSG.FirstPeriod)
   {
      switch (PSG.RegisterAY.EnvType)
      {
         case 0:
         case 1:
         case 2:
         case 3:
         case 8:
         case 9:
         case 10:
         case 11:
            sh.psg_env_direction = 0xff; // down
            break;
         case 4:
         case 5:
         case 6:
         case 7:
         case 12:
         case 13:
         case 14:
         case 15:
            sh.psg_env_direction = 0x01; // up
            break;
      }
   }
   else
   {
      switch (PSG.RegisterAY.EnvType)
      {
         case 0:
         case 1:
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
         case 7:
         case 9:
         case 11:
         case 13:
         case 15:
            sh.psg_env_direction = 0x00; // hold
            break;
         case 8:
         case 14:
            sh.psg_env_direction = 0xff; // down
            break;
         case 10:
         case 12:
            sh.psg_env_direction = 0x01; // up
            break;
      }
   }
   sh.crtc_addr[0]       = CRTC.addr & 0xff;
   sh.crtc_addr[1]       = (CRTC.addr >> 8) & 0xff;
   sh.crtc_scanline[0]   = VDU.scanline & 0xff;
   sh.crtc_scanline[1]   = (VDU.scanline >> 8) & 0xff;
   sh.crtc_char_count[0] = CRTC.char_count;
   sh.crtc_line_count    = CRTC.line_count;
   sh.crtc_raster_count  = CRTC.raster_count;
   sh.crtc_hsw_count     = CRTC.hsw_count;
   sh.crtc_vsw_count     = CRTC.vsw_count;
   dwFlags               = 0;

   if (CRTC.flag_invsync) // vsync active?
      dwFlags |= 1;
   if (flags1.inHSYNC) // hsync active?
      dwFlags |= 2;
   if (CRTC.flag_invta) // in vertical total adjust?
      dwFlags |= 0x80;

   sh.crtc_flags[0] = dwFlags & 0xff;
   sh.crtc_flags[1] = (dwFlags >> 8) & 0xff;
   sh.ga_int_delay = GateArray.hs_count;
   sh.ga_sl_count = GateArray.sl_count;
   sh.z80_int_pending = z80.int_pending;

   if ((pfileObject = fopen(pchFileName, "wb")) != NULL) {
      if (fwrite(&sh, sizeof(sh), 1, pfileObject) != 1) { // write snapshot header
         fclose(pfileObject);
         return ERR_SNA_WRITE;
      }
      if (fwrite(pbRAM, CPC.ram_size*1024, 1, pfileObject) != 1) { // write memory contents to snapshot file
         fclose(pfileObject);
         return ERR_SNA_WRITE;
      }
      fclose(pfileObject);
   } else {
      return ERR_SNA_WRITE;
   }


   return 0;
}

void dsk_eject (t_drive *drive)
{
   uint32_t track, side;
   uint32_t dwTemp;

   for (track = 0; track < DSK_TRACKMAX; track++)
   {
      /* loop for all tracks */
      for (side = 0; side < DSK_SIDEMAX; side++)
      {
         /* loop for all sides */
         if (drive->track[track][side].data) /* track is formatted? */
            free(drive->track[track][side].data); // release memory allocated for this track
      }
   }

   dwTemp = drive->current_track; // save the drive head position
   memset(drive, 0, sizeof(t_drive)); // clear drive info structure
   drive->current_track = dwTemp;
}

int dsk_load (char *pchFileName, t_drive *drive, char chID)
{
   int iRetCode;
   uint32_t dwTrackSize, track, side, sector, dwSectorSize, dwSectors;
   uint8_t *pbPtr, *pbDataPtr, *pbTempPtr, *pbTrackSizeTable;

   iRetCode = 0;
   dsk_eject(drive);
   if ((pfileObject = fopen(pchFileName, "rb")) != NULL)
   {
      if(!fread(pbGPBuffer, 0x100, 1, pfileObject)) { // read DSK header
         iRetCode = ERR_DSK_INVALID;
         goto exit;
      }
      pbPtr = pbGPBuffer;

      if (memcmp(pbPtr, "MV - CPC", 8) == 0) { // normal DSK image?
         drive->tracks = *(pbPtr + 0x30); // grab number of tracks
         if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
            drive->tracks = DSK_TRACKMAX; // limit to maximum
         }
         drive->sides = *(pbPtr + 0x31); // grab number of sides
         if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
            iRetCode = ERR_DSK_SIDES;
            goto exit;
         }
         dwTrackSize = (*(pbPtr + 0x32) + (*(pbPtr + 0x33) << 8)) - 0x100; // determine track size in bytes, minus track header
         drive->sides--; // zero base number of sides
         for (track = 0; track < drive->tracks; track++) { // loop for all tracks
            for (side = 0; side <= drive->sides; side++) { // loop for all sides
               if(!fread(pbGPBuffer+0x100, 0x100, 1, pfileObject)) { // read track header
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
               pbPtr = pbGPBuffer + 0x100;
               if (memcmp(pbPtr, "Track-Info", 10) != 0) { // abort if ID does not match
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
               dwSectorSize = 0x80 << *(pbPtr + 0x14); // determine sector size in bytes
               dwSectors = *(pbPtr + 0x15); // grab number of sectors
               if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
                  iRetCode = ERR_DSK_SECTORS;
                  goto exit;
               }
               drive->track[track][side].sectors = dwSectors; // store sector count
               drive->track[track][side].size = dwTrackSize; // store track size
               drive->track[track][side].data = (uint8_t *)malloc(dwTrackSize); // attempt to allocate the required memory
               if (drive->track[track][side].data == NULL) { // abort if not enough
                  iRetCode = ERR_OUT_OF_MEMORY;
                  goto exit;
               }
               pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
               pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
               for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
                  memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
                  memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
                  drive->track[track][side].sector[sector].size = dwSectorSize;
                  drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
                  pbDataPtr += dwSectorSize;
                  pbPtr += 8;
               }
               if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject)) { // read entire track data in one go
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
            }
         }
         drive->altered = 0; // disk is as yet unmodified
      } else {
         if (memcmp(pbPtr, "EXTENDED", 8) == 0) { // extended DSK image?
            drive->tracks = *(pbPtr + 0x30); // number of tracks
            if (drive->tracks > DSK_TRACKMAX) {  // limit to maximum possible
               drive->tracks = DSK_TRACKMAX;
            }
            drive->random_DEs = *(pbPtr + 0x31) & 0x80; // simulate random Data Errors?
            drive->sides = *(pbPtr + 0x31) & 3; // number of sides
            if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
               iRetCode = ERR_DSK_SIDES;
               goto exit;
            }
            pbTrackSizeTable = pbPtr + 0x34; // pointer to track size table in DSK header
            drive->sides--; // zero base number of sides
            for (track = 0; track < drive->tracks; track++) { // loop for all tracks
               for (side = 0; side <= drive->sides; side++) { // loop for all sides
                  dwTrackSize = (*pbTrackSizeTable++ << 8); // track size in bytes
                  if (dwTrackSize != 0) { // only process if track contains data
                     dwTrackSize -= 0x100; // compensate for track header
                     if(!fread(pbGPBuffer+0x100, 0x100, 1, pfileObject)) { // read track header
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                     pbPtr = pbGPBuffer + 0x100;
                     if (memcmp(pbPtr, "Track-Info", 10) != 0) { // valid track header?
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                     dwSectors = *(pbPtr + 0x15); // number of sectors for this track
                     if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
                        iRetCode = ERR_DSK_SECTORS;
                        goto exit;
                     }
                     drive->track[track][side].sectors = dwSectors; // store sector count
                     drive->track[track][side].size = dwTrackSize; // store track size
                     drive->track[track][side].data = (uint8_t *)malloc(dwTrackSize); // attempt to allocate the required memory
                     if (drive->track[track][side].data == NULL) { // abort if not enough
                        iRetCode = ERR_OUT_OF_MEMORY;
                        goto exit;
                     }
                     pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
                     pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
                     for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
                        memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
                        memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
                        dwSectorSize = *(pbPtr + 0x1e) + (*(pbPtr + 0x1f) << 8); // sector size in bytes
                        drive->track[track][side].sector[sector].size = dwSectorSize;
                        drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
                        pbDataPtr += dwSectorSize;
                        pbPtr += 8;
                     }
                     if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject)) { // read entire track data in one go
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                  } else {
                     memset(&drive->track[track][side], 0, sizeof(t_track)); // track not formatted
                  }
               }
            }
            drive->altered = 0; // disk is as yet unmodified
         } else {
            iRetCode = ERR_DSK_INVALID; // file could not be identified as a valid DSK
         }
      }

exit:
      fclose(pfileObject);
   }
   else
      iRetCode = ERR_FILE_NOT_FOUND;

   if (iRetCode != 0) // on error, 'eject' disk from drive
      dsk_eject(drive);

   return iRetCode;
}

int dsk_save (char *pchFileName, t_drive *drive, char chID)
{
   t_DSK_header dh;
   t_track_header th;
   uint32_t track, side, pos, sector;

   if ((pfileObject = fopen(pchFileName, "wb")) != NULL) {
      memset(&dh, 0, sizeof(dh));
      memcpy(dh.id, "EXTENDED CPC DSK File\r\nDisk-Info\r\n", sizeof(dh.id));
      strcpy(dh.unused1, "Caprice32\r\n");
      dh.tracks = drive->tracks;
      dh.sides = (drive->sides+1) | (drive->random_DEs); // correct side count and indicate random DEs, if necessary
      pos = 0;

      for (track = 0; track < drive->tracks; track++)
      { // loop for all tracks
         for (side = 0; side <= drive->sides; side++)
         { // loop for all sides
            if (drive->track[track][side].size) // track is formatted?
               dh.track_size[pos] = (drive->track[track][side].size + 0x100) >> 8; // track size + header in bytes
            pos++;
         }
      }

      if (!fwrite(&dh, sizeof(dh), 1, pfileObject))
      { // write header to file
         fclose(pfileObject);
         return ERR_DSK_WRITE;
      }

      memset(&th, 0, sizeof(th));
      memcpy(th.id, "Track-Info\r\n", sizeof(th.id));
      for (track = 0; track < drive->tracks; track++) { // loop for all tracks
         for (side = 0; side <= drive->sides; side++) { // loop for all sides
            if (drive->track[track][side].size) { // track is formatted?
               th.track = track;
               th.side = side;
               th.bps = 2;
               th.sectors = drive->track[track][side].sectors;
               th.gap3 = 0x4e;
               th.filler = 0xe5;
               for (sector = 0; sector < th.sectors; sector++) {
                  memcpy(&th.sector[sector][0], drive->track[track][side].sector[sector].CHRN, 4); // copy CHRN
                  memcpy(&th.sector[sector][4], drive->track[track][side].sector[sector].flags, 2); // copy ST1 & ST2
                  th.sector[sector][6] = drive->track[track][side].sector[sector].size & 0xff;
                  th.sector[sector][7] = (drive->track[track][side].sector[sector].size >> 8) & 0xff; // sector size in bytes
               }

               if (!fwrite(&th, sizeof(th), 1, pfileObject)) { // write track header
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
               if (!fwrite(drive->track[track][side].data, drive->track[track][side].size, 1, pfileObject)) { // write track data
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
            }
         }
      }
      fclose(pfileObject);
   }
   else
      return ERR_DSK_WRITE; // write attempt failed

   return 0;
}

int dsk_format (t_drive *drive, int iFormat)
{
   uint32_t track, side;
   int iRetCode = 0;

   drive->tracks = disk_format[iFormat].tracks;
   if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
      drive->tracks = DSK_TRACKMAX; // limit to maximum
   }
   drive->sides = disk_format[iFormat].sides;
   if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
      iRetCode = ERR_DSK_SIDES;
      goto exit;
   }
   drive->sides--; // zero base number of sides
   for (track = 0; track < drive->tracks; track++)
   {
      /* loop for all tracks */

      for (side = 0; side <= drive->sides; side++)
      {
         /* loop for all sides */
         uint32_t dwTrackSize;
         uint32_t sector;
         uint32_t dwSectorSize = 0x80 << disk_format[iFormat].sector_size; // determine sector size in bytes
         uint32_t dwSectors = disk_format[iFormat].sectors;

         if (dwSectors > DSK_SECTORMAX)
         { // abort if sector count greater than maximum
            iRetCode = ERR_DSK_SECTORS;
            goto exit;
         }

         dwTrackSize = dwSectorSize * dwSectors; // determine track size in bytes, minus track header
         drive->track[track][side].sectors = dwSectors; /* store sector count */
         drive->track[track][side].size    = dwTrackSize; /* store track size */
         drive->track[track][side].data    = (uint8_t *)malloc(dwTrackSize); /* attempt to allocate the required memory */
         if (drive->track[track][side].data == NULL)
         {
            /* abort if not enough */
            iRetCode = ERR_OUT_OF_MEMORY;
            goto exit;
         }

         uint8_t *pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
         uint8_t *pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
         uint8_t CHRN[4];
         CHRN[0] = (uint8_t)track;
         CHRN[1] = (uint8_t)side;
         CHRN[3] = (uint8_t)disk_format[iFormat].sector_size;

         for (sector = 0; sector < dwSectors; sector++)
         {
            // loop for all sectors
            CHRN[2] = disk_format[iFormat].sector_ids[side][sector];
            memcpy(drive->track[track][side].sector[sector].CHRN, CHRN, 4); // copy CHRN
            drive->track[track][side].sector[sector].size = dwSectorSize;
            drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
            pbDataPtr += dwSectorSize;
         }
         memset(pbTempPtr, disk_format[iFormat].filler_byte, dwTrackSize);
      }
   }
   drive->altered = 1; // flag disk as having been modified

exit:
   if (iRetCode != 0) // on error, 'eject' disk from drive
      dsk_eject(drive);
   return iRetCode;
}

void tape_eject (void)
{
   free(pbTapeImage);
   pbTapeImage = NULL;
}

int tape_insert (char *pchFileName)
{
   long lFileSize;
   int iBlockLength;
   uint8_t bID;
   uint8_t *pbPtr, *pbBlock;

   tape_eject();

   if ((pfileObject = fopen(pchFileName, "rb")) == NULL)
      return ERR_FILE_NOT_FOUND;

   if(!fread(pbGPBuffer, 10, 1, pfileObject)) { // read CDT header
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   pbPtr = pbGPBuffer;

   if (memcmp(pbPtr, "ZXTape!\032", 8) != 0)
   {
      /* valid CDT file? */
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }

   if (*(pbPtr + 0x08) != 1)
   {
      /* major version must be 1 */
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }

   lFileSize = file_size(fileno(pfileObject)) - 0x0a;

   if (lFileSize <= 0)
   { // the tape image should have at least one block...
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   pbTapeImage = (uint8_t *)malloc(lFileSize+6);
   *pbTapeImage = 0x20; // start off with a pause block
   *(uint16_t *)(pbTapeImage+1) = 2000; // set the length to 2 seconds
   if(!fread(pbTapeImage+3, lFileSize, 1, pfileObject)) { // append the entire CDT file
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   fclose(pfileObject);
   *(pbTapeImage+lFileSize+3) = 0x20; // end with a pause block
   *(uint16_t *)(pbTapeImage+lFileSize+3+1) = 2000; // set the length to 2 seconds

   #ifdef DEBUG_TAPE
   fputs("--- New Tape\r\n", pfoDebug);
   #endif
   pbTapeImageEnd = pbTapeImage + lFileSize+6;
   pbBlock = pbTapeImage;
   bool bolGotDataBlock = false;
   while (pbBlock < pbTapeImageEnd)
   {
      bID = *pbBlock++;
      switch(bID)
      {
         case 0x10: // standard speed data block
            iBlockLength = *(uint16_t *)(pbBlock+2) + 4;
            bolGotDataBlock = true;
            break;
         case 0x11: // turbo loading data block
            iBlockLength = (*(uint32_t *)(pbBlock+0x0f) & 0x00ffffff) + 0x12;
            bolGotDataBlock = true;
            break;
         case 0x12: // pure tone
            iBlockLength = 4;
            bolGotDataBlock = true;
            break;
         case 0x13: // sequence of pulses of different length
            iBlockLength = *pbBlock * 2 + 1;
            bolGotDataBlock = true;
            break;
         case 0x14: // pure data block
            iBlockLength = (*(uint32_t *)(pbBlock+0x07) & 0x00ffffff) + 0x0a;
            bolGotDataBlock = true;
            break;
         case 0x15: // direct recording
            iBlockLength = (*(uint32_t *)(pbBlock+0x05) & 0x00ffffff) + 0x08;
            bolGotDataBlock = true;
            break;
         case 0x20: // pause
            if ((!bolGotDataBlock) && (pbBlock != pbTapeImage+1)) {
               *(uint16_t *)pbBlock = 0; // remove any pauses (execept ours) before the data starts
            }
            iBlockLength = 2;
            break;
         case 0x21: // group start
            iBlockLength = *pbBlock + 1;
            break;
         case 0x22: // group end
            iBlockLength = 0;
            break;
         case 0x23: // jump to block
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 2;
            break;
         case 0x24: // loop start
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 2;
            break;
         case 0x25: // loop end
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 0;
            break;
         case 0x26: // call sequence
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = (*(uint16_t *)pbBlock * 2) + 2;
            break;
         case 0x27: // return from sequence
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 0;
            break;
         case 0x28: // select block
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = *(uint16_t *)pbBlock + 2;
            break;
         case 0x30: // text description
            iBlockLength = *pbBlock + 1;
            break;
         case 0x31: // message block
            iBlockLength = *(pbBlock+1) + 2;
            break;
         case 0x32: // archive info
            iBlockLength = *(uint16_t *)pbBlock + 2;
            break;
         case 0x33: // hardware type
            iBlockLength = (*pbBlock * 3) + 1;
            break;
         case 0x34: // emulation info
            iBlockLength = 8;
            break;
         case 0x35: // custom info block
            iBlockLength = *(uint32_t *)(pbBlock+0x10) + 0x14;
            break;
         case 0x40: // snapshot block
            iBlockLength = (*(uint32_t *)(pbBlock+0x01) & 0x00ffffff) + 0x04;
            break;
         case 0x5A: // another tzx/cdt file
            iBlockLength = 9;
            break;

         default: // "extension rule"
            iBlockLength = *(uint32_t *)pbBlock + 4;
      }

#ifdef DEBUG_TAPE
      fprintf(pfoDebug, "%02x %d\r\n", bID, iBlockLength);
#endif

      pbBlock += iBlockLength;
   }

   if (pbBlock != pbTapeImageEnd)
   {
      tape_eject();
      return ERR_TAP_INVALID;
   }

   Tape_Rewind();

   sprintf(TAPE_NAME,"%s",pchFileName);

   return 0;
}

int tape_insert_voc (char *pchFileName)
{
   long lFileSize, lOffset, lInitialOffset, lSampleLength;
   int iBlockLength;
   uint8_t *pbPtr, *pbTapeImagePtr, *pbVocDataBlock, *pbVocDataBlockPtr;
   bool bolDone;

   tape_eject();

   if ((pfileObject = fopen(pchFileName, "rb")) == NULL)
      return ERR_FILE_NOT_FOUND;

   if(!fread(pbGPBuffer, 26, 1, pfileObject)) { // read VOC header
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }
   pbPtr = pbGPBuffer;

   if (memcmp(pbPtr, "Creative Voice File\032", 20) != 0)
   {
      // valid VOC file?
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }
   lOffset =
   lInitialOffset = *(uint16_t *)(pbPtr + 0x14);
   lFileSize = file_size(fileno(pfileObject));

   if ((lFileSize-26) <= 0)
   { // should have at least one block...
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }

   #ifdef DEBUG_TAPE
   fputs("--- New Tape\r\n", pfoDebug);
   #endif
   iBlockLength = 0;
   lSampleLength = 0;
   uint8_t bSampleRate = 0;
   bolDone = false;
   while ((!bolDone) && (lOffset < lFileSize))
   {
      fseek(pfileObject, lOffset, SEEK_SET);
      if(!fread(pbPtr, 16, 1, pfileObject)) { // read block ID + size
         fclose(pfileObject);
         return ERR_TAP_BAD_VOC;
      }
#ifdef DEBUG_TAPE
      fprintf(pfoDebug, "%02x %d\r\n", *pbPtr, *(uint32_t *)(pbPtr+0x01) & 0x00ffffff);
#endif

      switch(*pbPtr)
      {
         case 0x0: // terminator
            bolDone = true;
            break;
         case 0x1: // sound data
            iBlockLength = (*(uint32_t *)(pbPtr+0x01) & 0x00ffffff) + 4;
            lSampleLength += iBlockLength - 6;

            if ((bSampleRate) && (bSampleRate != *(pbPtr+0x04)))
            { // no change in sample rate allowed
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }

            bSampleRate = *(pbPtr+0x04);

            if (*(pbPtr+0x05) != 0)
            { // must be 8 bits wide
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            break;
         case 0x2: // sound continue
            iBlockLength = (*(uint32_t *)(pbPtr+0x01) & 0x00ffffff) + 4;
            lSampleLength += iBlockLength - 4;
            break;
         case 0x3: // silence
            iBlockLength = 4;
            lSampleLength += *(uint16_t *)(pbPtr+0x01) + 1;
            if ((bSampleRate) && (bSampleRate != *(pbPtr+0x03))) { // no change in sample rate allowed
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            bSampleRate = *(pbPtr+0x03);
            break;
         case 0x4: // marker
            iBlockLength = 3;
            break;
         case 0x5: // ascii
            iBlockLength = (*(uint32_t *)(pbPtr+0x01) & 0x00ffffff) + 4;
            break;
         default:
            fclose(pfileObject);
            return ERR_TAP_BAD_VOC;
      }
      lOffset += iBlockLength;
   }
   #ifdef DEBUG_TAPE
   fprintf(pfoDebug, "--- %ld bytes\r\n", lSampleLength);
   #endif

   uint32_t dwTapePulseCycles = 3500000L / (1000000L / (256 - bSampleRate)); // length of one pulse in ZX Spectrum T states
   uint32_t dwCompressedSize = lSampleLength >> 3; // 8x data reduction
   if (dwCompressedSize > 0x00ffffff)
   {
      /* we only support one direct recording block right now */
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }

   pbTapeImage = (uint8_t *)malloc(dwCompressedSize+1+8+6);

   if (pbTapeImage == NULL)
   {
      /* check if the memory allocation has failed */
      fclose(pfileObject);
      return ERR_OUT_OF_MEMORY;
   }
   *pbTapeImage = 0x20; // start off with a pause block
   *(uint16_t *)(pbTapeImage+1) = 2000; // set the length to 2 seconds

   *(pbTapeImage+3)             = 0x15; // direct recording block
   *(uint16_t *)(pbTapeImage+4) = (uint16_t)dwTapePulseCycles; // number of T states per sample
   *(uint16_t *)(pbTapeImage+6) = 0; // pause after block
   *(pbTapeImage+8)             = lSampleLength & 7 ? lSampleLength & 7 : 8; // bits used in last byte
   *(uint32_t *)(pbTapeImage+9) = dwCompressedSize & 0x00ffffff; // data length
   pbTapeImagePtr               = pbTapeImage + 12;

   lOffset = lInitialOffset;
   bolDone = false;
   uint32_t dwBit = 8;
   uint8_t bByte = 0;
   unsigned iBytePos;
   while ((!bolDone) && (lOffset < lFileSize))
   {
      fseek(pfileObject, lOffset, SEEK_SET);
      if(!fread(pbPtr, 1, 1, pfileObject)) { // read block ID
         fclose(pfileObject);
         return ERR_TAP_BAD_VOC;
      }

      switch(*pbPtr)
      {
         case 0x0: // terminator
            bolDone = true;
            break;
         case 0x1: // sound data
            if(!fread(pbPtr, 3+2, 1, pfileObject)) { // get block size and sound info
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            iBlockLength   = (*(uint32_t *)(pbPtr) & 0x00ffffff) + 4;
            lSampleLength  = iBlockLength - 6;
            pbVocDataBlock = (uint8_t *)malloc(lSampleLength);

            if (pbVocDataBlock == NULL)
            {
               fclose(pfileObject);
               tape_eject();
               return ERR_OUT_OF_MEMORY;
            }

            if(!fread(pbVocDataBlock, lSampleLength, 1, pfileObject)) {
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            pbVocDataBlockPtr = pbVocDataBlock;

            for (iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
            {
               uint8_t bVocSample = *pbVocDataBlockPtr++;

               dwBit--;

               if (bVocSample > VOC_THRESHOLD)
                  bByte |= bit_values[dwBit];

               if (!dwBit)
               {
                  /* got all 8 bits? */
                  *pbTapeImagePtr++ = bByte;
                  dwBit = 8;
                  bByte = 0;
               }
            }
            free(pbVocDataBlock);
            break;
         case 0x2: // sound continue
            if(!fread(pbPtr, 3, 1, pfileObject)) { // get block size
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            iBlockLength = (*(uint32_t *)(pbPtr) & 0x00ffffff) + 4;
            lSampleLength = iBlockLength - 4;
            pbVocDataBlock = (uint8_t *)malloc(lSampleLength);
            if (pbVocDataBlock == NULL)
            {
               fclose(pfileObject);
               tape_eject();
               return ERR_OUT_OF_MEMORY;
            }
            if(!fread(pbVocDataBlock, lSampleLength, 1, pfileObject)) {
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            pbVocDataBlockPtr = pbVocDataBlock;
            for (iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
            {
               uint8_t bVocSample = *pbVocDataBlockPtr++;
               dwBit--;
               if (bVocSample > VOC_THRESHOLD)
                  bByte |= bit_values[dwBit];
               if (!dwBit)
               { // got all 8 bits?
                  *pbTapeImagePtr++ = bByte;
                  dwBit = 8;
                  bByte = 0;
               }
            }
            free(pbVocDataBlock);
            break;
         case 0x3: // silence
            iBlockLength = 4;
            lSampleLength = *(uint16_t *)(pbPtr) + 1;
            for (iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
            {
               dwBit--;
               if (!dwBit)
               { // got all 8 bits?
                  *pbTapeImagePtr++ = bByte;
                  dwBit = 8;
                  bByte = 0;
               }
            }
            break;
         case 0x4: // marker
            iBlockLength = 3;
            break;
         case 0x5: // ascii
            iBlockLength = (*(uint32_t *)(pbPtr) & 0x00ffffff) + 4;
            break;
      }
      lOffset += iBlockLength;
   }
   fclose(pfileObject);

   *pbTapeImagePtr = 0x20; // end with a pause block
   *(uint16_t *)(pbTapeImagePtr+1) = 2000; // set the length to 2 seconds

   pbTapeImageEnd = pbTapeImagePtr + 3;

   Tape_Rewind();

   sprintf(TAPE_NAME,"%s",pchFileName);

   return 0;
}

int emulator_patch_ROM (void)
{
   char chPath[_MAX_PATH + 1];
   uint8_t *pbPtr;

   strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
   strcat(chPath, "/");
   strncat(chPath, chROMFile[CPC.model], sizeof(chPath)-1 - strlen(chPath)); // determine the ROM image name for the selected model

   if(CPC.model <= 2) { // Normal CPC range
      if ((pfileObject = fopen(chPath, "rb")) != NULL)
      {
         /* load CPC OS + Basic */
         if(!fread(pbROM, 2*16384, 1, pfileObject)) {
            fclose(pfileObject);
            return ERR_CPC_ROM_MISSING;
         }
         pbROMlo = pbROM;
         fclose(pfileObject);
      }
      else
         return ERR_CPC_ROM_MISSING;
   } else { // Plus range
      if (pbCartridgeImage != NULL) {
         pbROMlo = &pbCartridgeImage[0];
      }
   }

   if (CPC.keyboard)
   {
      pbPtr = pbROMlo;
      switch(CPC.model)
      {
         case 0: // 464
            pbPtr += 0x1d69; // location of the keyboard translation table
            break;
         case 1: // 664
         case 2: // 6128
            pbPtr += 0x1eef; // location of the keyboard translation table
            break;
      }

      if (pbPtr != pbROMlo)
      {
         memcpy(pbPtr, cpc_keytrans[CPC.keyboard-1], 240); // patch the CPC OS ROM with the chosen keyboard layout
         pbPtr = pbROMlo + 0x3800;
         memcpy(pbPtr, cpc_charset[CPC.keyboard-1], 2048); // add the corresponding character set
      }
   }

   return 0;
}

void emulator_reset (bool bolMF2Reset)
{
   int n;

   // Z80
   memset(&z80, 0, sizeof(z80)); // clear all Z80 registers and support variables

   _IX = _IY = 0xffff; // IX and IY are FFFF after a reset!
   _F = Zflag; // set zero flag

   z80.break_point = 0xffffffff; // clear break point

   // CPC
   CPC.cycle_count      = CYCLE_COUNT_INIT;
   memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix)); // clear CPC keyboard matrix
   CPC.tape_motor       = 0;
   CPC.tape_play_button = 0;
   CPC.printer_port     = 0xff;

   // VDU
   memset(&VDU, 0, sizeof(VDU)); // clear VDU data structure
   VDU.flag_drawing = 1;

   // CRTC
   crtc_reset();

   asic_locked = true;

   // Gate Array
   memset(&GateArray, 0, sizeof(GateArray)); // clear GA data structure
   GateArray.scr_mode = GateArray.requested_scr_mode = 1; // set to mode 1
   GateArray.registerPageOn = false;
   ga_init_banking();

   // PPI
   memset(&PPI, 0, sizeof(PPI)); // clear PPI data structure

   // PSG
   PSG.control = 0;
   ResetAYChipEmulation();

   // FDC
   memset(&FDC, 0, sizeof(FDC)); // clear FDC data structure
   FDC.phase = CMD_PHASE;
   FDC.flags = STATUSDRVA_flag | STATUSDRVB_flag;

   // memory
   if (bolMF2Reset)
      memset(pbRAM, 0, 64*1024); // clear only the first 64K of CPC memory
   else
   {
      memset(pbRAM, 0, CPC.ram_size*1024); // clear all memory used for CPC RAM
      if (pbMF2ROM)
         memset(pbMF2ROM+8192, 0, 8192); // clear the MF2's RAM area
   }

   for (n = 0; n < 4; n++)
   {
      /* initialize active read/write bank configuration */
      membank_read[n] = membank_config[0][n];
      membank_write[n] = membank_config[0][n];
   }
   membank_read[0] = pbROMlo; // 'page in' lower ROM
   membank_read[3] = pbROMhi; // 'page in' upper ROM

   // Multiface 2
   dwMF2Flags = 0;
   dwMF2ExitAddr = 0xffffffff; // clear MF2 return address
   if ((pbMF2ROM) && (pbMF2ROMbackup))
      memcpy(pbMF2ROM, pbMF2ROMbackup, 8192); // copy the MF2 ROM to its proper place
}

#include "rom/6128.h"
#include "rom/amsdos.h"

int emulator_init (void)
{
   int iErr, iRomNum;
   char chPath[_MAX_PATH + 1];
   char *pchRomData = NULL;

   (void)pchRomData;
   (void)chPath;
   (void)iRomNum;
   (void)iErr;

   pbRegisterPage = (uint8_t*) malloc(16 * 1024 * sizeof(uint8_t));
   pbGPBuffer    = (uint8_t*) malloc(128 * 1024 * sizeof(uint8_t)); // attempt to allocate the general purpose buffer
   pbRAM         = (uint8_t*) malloc(CPC.ram_size * 1024 * sizeof(uint8_t)); // allocate memory for desired amount of RAM
   pbROM         = (uint8_t *)&OS[0]; // CPC 6128

   // FIXME
   if(cart_name[0] == '\0') {
      pbROMlo = pbROM;
      printf("no cart!\n");
   } else if (pbCartridgeImage != NULL) {
      pbROMlo = &pbCartridgeImage[0];
      printf("loaded cart: %s\n", cart_name);
   }

   if (!pbGPBuffer || !pbRAM || !pbRegisterPage)
      return ERR_OUT_OF_MEMORY;

   pbROMhi = pbExpansionROM = (uint8_t *)pbROMlo + 16384;

   memset(memmap_ROM, 0, sizeof(memmap_ROM[0]) * 256); // clear the expansion ROM map
   ga_init_banking(); // init the CPC memory banking map

   memmap_ROM[7] = (uint8_t*)&AMSDOS[0];

   CPC.mf2 = 0;
   crtc_init();

   emulator_reset(false);
   CPC.paused &= ~1;

   return 0;
}

void emulator_shutdown (void)
{
   int iRomNum;

   if(pbRegisterPage)
      free(pbRegisterPage);

   if (pbMF2ROMbackup)
      free(pbMF2ROMbackup);
   if (pbMF2ROM)
      free(pbMF2ROM);
   pbMF2ROM = NULL;

   for (iRomNum = 2; iRomNum < 16; iRomNum++) // loop for ROMs 2-15
   {
      if (memmap_ROM[iRomNum] != NULL && iRomNum!=7) // was a ROM assigned to this slot?
         free(memmap_ROM[iRomNum]); // if so, release the associated memory
   }

   if (pbRAM)
      free(pbRAM);
   if (pbGPBuffer)
      free(pbGPBuffer);
}

int cart_insert (char *pchFileName) {

   sprintf(cart_name,"%s",pchFileName);
   int result = cpr_fload(pchFileName);

   if(result != 0) {
      fprintf(stderr, "Load of cartridge failed. Aborting.\n");
      return result;
   }

   // set mode cpc+
   CPC.model=3;
   /* Reconfigure emulator */
   emulator_shutdown();
   emulator_init();

   return 0;
}

int printer_start (void)
{
   if (!pfoPrinter)
   {
      if(!(pfoPrinter = fopen(CPC.printer_file, "wb")))
         return 0; // failed to open/create file
   }
   return 1; // ready to capture printer output
}

void printer_stop (void)
{
   if (pfoPrinter)
      fclose(pfoPrinter);
   pfoPrinter = NULL;
}

void audio_update (void *userdata, uint8_t *stream, int len) { }

int audio_align_samples (int given)
{
   int actual = 1;
   while (actual < given)
      actual <<= 1;
   return actual; // return the closest match as 2^n
}

int audio_init (void)
{
   unsigned n;

   if (!CPC.snd_enabled)
      return 0;

   CPC.snd_buffersize = 2*2*882;//audio_spec->size; // size is samples * channels * bytes per sample (1 or 2)
   pbSndBuffer        = (uint8_t *)malloc(CPC.snd_buffersize); // allocate the sound data buffer
   pbSndBufferEnd     = pbSndBuffer + CPC.snd_buffersize;
   memset(pbSndBuffer, 0, CPC.snd_buffersize);
   CPC.snd_bufferptr  = pbSndBuffer; // init write cursor

   InitAY();

   for (n = 0; n < 16; n++)
      SetAYRegister(n, PSG.RegisterAY.Index[n]); // init sound emulation with valid values

   return 0;
}

void audio_shutdown (void) {}
void audio_pause (void) {}
void audio_resume (void) {}

#ifdef M16B
    #define RGB2COLOR(r, g, b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))
#else
    #define RGB2COLOR(r, g, b) (b | ((g << 8) | (r << 16)))
#endif

int video_set_palette (void)
{

   int n;


   if (!CPC.scr_tube)
   {
      for (n = 0; n < 32; n++)
      {
         uint32_t red, green, blue;

         red = (uint32_t)(colours_rgb[n][0] * (CPC.scr_intensity / 10.0) * 255);
         if (red > 255) /* limit to the maximum */
            red = 255;

         green = (uint32_t)(colours_rgb[n][1] * (CPC.scr_intensity / 10.0) * 255);
         if (green > 255)
            green = 255;

         blue = (uint32_t)(colours_rgb[n][2] * (CPC.scr_intensity / 10.0) * 255);
         if (blue > 255)
            blue = 255;

         colours[n] = (PIXEL_TYPE) RGB2COLOR(red, green, blue);

      }
   }
   else
   {
      for (n = 0; n < 32; n++)
      {
         uint32_t green = (uint32_t)(colours_green[n] * (CPC.scr_intensity / 10.0) * 255);

         if (green > 255)
            green = 255;

         colours[n] = (PIXEL_TYPE) RGB2COLOR(0, green, 0);

      }
   }

   for (n = 0; n < 17; n++)
   {
      /* loop for all colours + border */
      int i = GateArray.ink_values[n];
      (void)i;
      GateArray.palette[n] = colours[GateArray.ink_values[n]];
   }

   return 0;
}

void video_set_style (void)
{
   if (CPC.scr_style == 3) //384x272
   {
      dwXScale = 1;
      dwYScale = 1;

   }
   else                    //768x544
   {
      dwXScale = 2;
      dwYScale = 2;
   }
   printf("style: %u, dwScale: %ux%u, offset: %u\n", CPC.scr_style, dwXScale, dwYScale, CPC.scr_line_offs);

   switch (dwXScale)
   {
      case 1:
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal_half;
         CPC.scr_prerenderbord = (void(*)(void))prerender_border_half;
         CPC.scr_prerendersync = (void(*)(void))prerender_sync_half;
         break;
      case 2:
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal;
         CPC.scr_prerenderbord = (void(*)(void))prerender_border;
         CPC.scr_prerendersync = (void(*)(void))prerender_sync;
         break;
   }

   switch(CPC.scr_bpp)
   {
      case 32:
         if(dwYScale == 2)
            CPC.scr_render = (void(*)(void))render32bpp_doubleY;
         else
            CPC.scr_render = (void(*)(void))render32bpp;
         break;
      case 24:
         CPC.scr_render = (void(*)(void))render24bpp;
         break;
      case 16:
      case 15:
         if(dwYScale == 2)
            CPC.scr_render = (void(*)(void))render16bpp_doubleY;
         else
            CPC.scr_render = (void(*)(void))render16bpp;
         break;
      case 8:
         CPC.scr_render = (void(*)(void))render8bpp;
         break;
   }
}

int video_init (void)
{
   int error_code;
   CPC.scr_bpp = retro_getGfxBpp();

   error_code = video_set_palette(); // init CPC colours and hardware palette (in 8bpp mode)
   if (error_code)
      return error_code;

   CPC.scr_style     = retro_getStyle();
   CPC.scr_bps       = retro_getGfxBps();
   CPC.scr_pos       = CPC.scr_base = (uint32_t *)&bmp[0];
   CPC.scr_line_offs = ((CPC.scr_bps * (CPC.scr_style - 2)) // because is double height
                         / (2 / PIXEL_BYTES) ) ;

   video_set_style();
   memset(bmp, 0, sizeof(bmp));

   return 0;
}

void video_shutdown (void)
{
}

void video_display (void)
{
}

void input_swap_joy (void)
{

}

int input_init (void)
{

   return 0;
}

int getConfigValueInt (char* pchFileName, char* pchSection, char* pchKey, int iDefaultValue)
{
   FILE* pfoConfigFile;
   char chLine[MAX_LINE_LEN + 1];
   char* pchToken;

   if ((pfoConfigFile = fopen(pchFileName, "r")) != NULL) { // open the config file
      while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL) { // grab one line
         pchToken = strtok(chLine, "[]"); // check if there's a section key
         if((pchToken != NULL) && (pchToken[0] != '#') && (strcmp(pchToken, pchSection) == 0)) {
            while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL) { // get the next line
               pchToken = strtok(chLine, "\t =\n\r"); // check if it has a key=value pair
               if((pchToken != NULL) && (pchToken[0] != '#') && (strcmp(pchToken, pchKey) == 0)) {
                  char* pchPtr = strtok(NULL, "\t =#\n\r"); // get the value if it matches our key
                  if (pchPtr != NULL) {
                     return (strtol(pchPtr, NULL, 0)); // return as integer
                  } else {
                     return iDefaultValue; // no value found
                  }
               }
            }
         }
      }
      fclose(pfoConfigFile);
   }
   return iDefaultValue; // no value found
}

void getConfigValueString (char* pchFileName, char* pchSection,
      char *pchKey, char *pchValue, int iSize, char *pchDefaultValue)
{
   FILE* pfoConfigFile;
   char chLine[MAX_LINE_LEN + 1];
   char* pchToken;

   if ((pfoConfigFile = fopen(pchFileName, "r")) != NULL)
   {
      /* open the config file */

      while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL)
      {
         /* grab one line */
         pchToken = strtok(chLine, "[]"); /* check if there's a section key */
         if((pchToken != NULL) && (pchToken[0] != '#')
               && (strcmp(pchToken, pchSection) == 0))
         {
            while(fgets(chLine, MAX_LINE_LEN, pfoConfigFile) != NULL)
            {
               /* get the next line */
               pchToken = strtok(chLine, "\t =\n\r"); // check if it has a key=value pair

               if((pchToken != NULL) && (pchToken[0] != '#') && (strcmp(pchToken, pchKey) == 0))
               {
                  char* pchPtr = strtok(NULL, "\t=#\n\r"); // get the value if it matches our key

                  if (pchPtr != NULL)
                     strncpy(pchValue, pchPtr, iSize); // copy to destination
                  else
                     strncpy(pchValue, pchDefaultValue, iSize); // no value found, return the default
                  return;
               }
            }
         }
      }
      fclose(pfoConfigFile);
   }
   strncpy(pchValue, pchDefaultValue, iSize); // no value found, return the default
}

void loadConfiguration (void)
{
   unsigned i, n, iSide, iSector, iRomNum;
   char chFileName[_MAX_PATH + 1];
   char chPath[_MAX_PATH + 1];

   (void)n;

   strncpy(chFileName, chAppPath, sizeof(chFileName)-10);
   strcat(chFileName, "/cap32.cfg");

   memset(&CPC, 0, sizeof(CPC));
   CPC.model = getConfigValueInt(chFileName, "system", "model", 2); // CPC 6128

   if (CPC.model > 3)
      CPC.model = 2;

   CPC.jumpers       = getConfigValueInt(chFileName, "system", "jumpers", 0x1e) & 0x1e; // OEM is Amstrad, video refresh is 50Hz
   CPC.ram_size      = getConfigValueInt(chFileName, "system", "ram_size", 128) & 0x02c0; // 128KB RAM

   if (CPC.ram_size > 576)
      CPC.ram_size   = 576;
   else if ((CPC.model >= 2) && (CPC.ram_size < 128))
      CPC.ram_size   = 128; // minimum RAM size for CPC 6128 is 128KB

   CPC.speed = getConfigValueInt(chFileName, "system", "speed", DEF_SPEED_SETTING); // original CPC speed

   if ((CPC.speed < MIN_SPEED_SETTING) || (CPC.speed > MAX_SPEED_SETTING))
      CPC.speed = DEF_SPEED_SETTING;
   CPC.limit_speed   = 1;
   CPC.auto_pause    = getConfigValueInt(chFileName, "system", "auto_pause", 1) & 1;
   CPC.printer       = getConfigValueInt(chFileName, "system", "printer", 0) & 1;
   CPC.mf2           = getConfigValueInt(chFileName, "system", "mf2", 0) & 1;
   CPC.keyboard      = getConfigValueInt(chFileName, "system", "keyboard", 0);

   if (CPC.keyboard > MAX_ROM_MODS)
      CPC.keyboard = 0;
   CPC.joysticks     = getConfigValueInt(chFileName, "system", "joysticks", 0) & 1;

   CPC.scr_fs_width  = getConfigValueInt(chFileName, "video", "scr_width", 384);
   CPC.scr_fs_height = getConfigValueInt(chFileName, "video", "scr_height", 288);
   CPC.scr_fs_bpp    = getConfigValueInt(chFileName, "video", "scr_bpp", 32);
   CPC.scr_style     = getConfigValueInt(chFileName, "video", "scr_style", 4);
   CPC.scr_oglfilter = getConfigValueInt(chFileName, "video", "scr_oglfilter", 0) & 1;
   CPC.scr_vsync     = getConfigValueInt(chFileName, "video", "scr_vsync", 1) & 1;
   CPC.scr_led       = getConfigValueInt(chFileName, "video", "scr_led", 1) & 1;
   CPC.scr_fps       = getConfigValueInt(chFileName, "video", "scr_fps", 0) & 1;
   CPC.scr_tube      = getConfigValueInt(chFileName, "video", "scr_tube", 0) & 1;
   CPC.scr_intensity = getConfigValueInt(chFileName, "video", "scr_intensity", 10);
   CPC.scr_remanency = getConfigValueInt(chFileName, "video", "scr_remanency", 0) & 1;

   if ((CPC.scr_intensity < 5) || (CPC.scr_intensity > 15))
      CPC.scr_intensity = 10;

   CPC.scr_window = getConfigValueInt(chFileName, "video", "scr_window", 0) & 1;

   CPC.snd_enabled = getConfigValueInt(chFileName, "sound", "enabled", 1) & 1;
   CPC.snd_playback_rate = getConfigValueInt(chFileName, "sound", "playback_rate", 2);
   if (CPC.snd_playback_rate > (MAX_FREQ_ENTRIES-1)) {
      CPC.snd_playback_rate = 2;
   }
   CPC.snd_bits = getConfigValueInt(chFileName, "sound", "bits", 1) & 1;
   CPC.snd_stereo = getConfigValueInt(chFileName, "sound", "stereo", 1) & 1;
   CPC.snd_volume = getConfigValueInt(chFileName, "sound", "volume", 80);

   if ((CPC.snd_volume < 0) || (CPC.snd_volume > 100))
      CPC.snd_volume = 80;
   CPC.snd_pp_device = getConfigValueInt(chFileName, "sound", "pp_device", 0) & 1;

   CPC.kbd_layout = getConfigValueInt(chFileName, "control", "kbd_layout", 0);
   if (CPC.kbd_layout > 3)
      CPC.kbd_layout = 0;

   CPC.max_tracksize = getConfigValueInt(chFileName, "file", "max_track_size", 6144-154);
   strncpy(chPath, chAppPath, sizeof(chPath)-7);
   strcat(chPath, "/snap");
   getConfigValueString(chFileName, "file", "snap_path", CPC.snap_path, sizeof(CPC.snap_path)-1, chPath);
   if (CPC.snap_path[0] == '\0') {
      strcpy(CPC.snap_path, chPath);
   }
   getConfigValueString(chFileName, "file", "snap_file", CPC.snap_file, sizeof(CPC.snap_file)-1, "");
   CPC.snap_zip = getConfigValueInt(chFileName, "file", "snap_zip", 0) & 1;
   strncpy(chPath, chAppPath, sizeof(chPath)-7);
   strcat(chPath, "/disk");
   getConfigValueString(chFileName, "file", "drvA_path", CPC.drvA_path, sizeof(CPC.drvA_path)-1, chPath);
   if (CPC.drvA_path[0] == '\0') {
      strcpy(CPC.drvA_path, chPath);
   }
   getConfigValueString(chFileName, "file", "drvA_file", CPC.drvA_file, sizeof(CPC.drvA_file)-1, "");
   CPC.drvA_zip = getConfigValueInt(chFileName, "file", "drvA_zip", 0) & 1;
   CPC.drvA_format = getConfigValueInt(chFileName, "file", "drvA_format", DEFAULT_DISK_FORMAT);
   getConfigValueString(chFileName, "file", "drvB_path", CPC.drvB_path, sizeof(CPC.drvB_path)-1, chPath);
   if (CPC.drvB_path[0] == '\0') {
      strcpy(CPC.drvB_path, chPath);
   }
   getConfigValueString(chFileName, "file", "drvB_file", CPC.drvB_file, sizeof(CPC.drvB_file)-1, "");
   CPC.drvB_zip = getConfigValueInt(chFileName, "file", "drvB_zip", 0) & 1;
   CPC.drvB_format = getConfigValueInt(chFileName, "file", "drvB_format", DEFAULT_DISK_FORMAT);
   strncpy(chPath, chAppPath, sizeof(chPath)-7);
   strcat(chPath, "/tape");
   getConfigValueString(chFileName, "file", "tape_path", CPC.tape_path, sizeof(CPC.tape_path)-1, chPath);
   if (CPC.tape_path[0] == '\0') {
      strcpy(CPC.tape_path, chPath);
   }
   getConfigValueString(chFileName, "file", "tape_file", CPC.tape_file, sizeof(CPC.tape_file)-1, "");
   CPC.tape_zip = getConfigValueInt(chFileName, "file", "tape_zip", 0) & 1;

   int iFmt = FIRST_CUSTOM_DISK_FORMAT;
   for (i = iFmt; i < MAX_DISK_FORMAT; i++)
   { // loop through all user definable disk formats
      uint32_t dwVal;
      char *pchTail;
      char chFmtId[14];
      disk_format[iFmt].label[0] = 0; // clear slot
      sprintf(chFmtId, "fmt%02d", i); // build format ID
      char chFmtStr[256];
      getConfigValueString(chFileName, "file", chFmtId, chFmtStr, sizeof(chFmtStr)-1, "");
      if (chFmtStr[0] != 0) { // found format definition for this slot?
         char chDelimiters[] = ",";
         char *pchToken;
         pchToken = strtok(chFmtStr, chDelimiters); // format label
         strncpy((char *)disk_format[iFmt].label, pchToken, sizeof(disk_format[iFmt].label)-1);
         pchToken = strtok(NULL, chDelimiters); // number of tracks
         if (pchToken == NULL) { // value missing?
            continue;
         }
         dwVal = strtoul(pchToken, &pchTail, 0);
         if ((dwVal < 1) || (dwVal > DSK_TRACKMAX)) { // invalid value?
            continue;
         }
         disk_format[iFmt].tracks = dwVal;
         pchToken = strtok(NULL, chDelimiters); // number of sides
         if (pchToken == NULL) { // value missing?
            continue;
         }
         dwVal = strtoul(pchToken, &pchTail, 0);
         if ((dwVal < 1) || (dwVal > DSK_SIDEMAX)) { // invalid value?
            continue;
         }
         disk_format[iFmt].sides = dwVal;
         pchToken = strtok(NULL, chDelimiters); // number of sectors
         if (pchToken == NULL) { // value missing?
            continue;
         }
         dwVal = strtoul(pchToken, &pchTail, 0);
         if ((dwVal < 1) || (dwVal > DSK_SECTORMAX)) { // invalid value?
            continue;
         }
         disk_format[iFmt].sectors = dwVal;
         pchToken = strtok(NULL, chDelimiters); // sector size as N value
         if (pchToken == NULL) { // value missing?
            continue;
         }
         dwVal = strtoul(pchToken, &pchTail, 0);
         if ((dwVal < 1) || (dwVal > 6)) { // invalid value?
            continue;
         }
         disk_format[iFmt].sector_size = dwVal;
         pchToken = strtok(NULL, chDelimiters); // gap#3 length
         if (pchToken == NULL) { // value missing?
            continue;
         }
         dwVal = strtoul(pchToken, &pchTail, 0);
         if ((dwVal < 1) || (dwVal > 255)) { // invalid value?
            continue;
         }
         disk_format[iFmt].gap3_length = dwVal;
         pchToken = strtok(NULL, chDelimiters); // filler byte
         if (pchToken == NULL) { // value missing?
            continue;
         }
         dwVal = strtoul(pchToken, &pchTail, 0);
         disk_format[iFmt].filler_byte = (uint8_t)dwVal;
         for (iSide = 0; iSide < (int)disk_format[iFmt].sides; iSide++)
         {
            for (iSector = 0; iSector < (int)disk_format[iFmt].sectors; iSector++)
            {
               pchToken = strtok(NULL, chDelimiters); // sector ID
               if (pchToken == NULL) // value missing?
                  dwVal = iSector+1;
               else
                  dwVal = strtoul(pchToken, &pchTail, 0);
               disk_format[iFmt].sector_ids[iSide][iSector] = (uint8_t)dwVal;
            }
         }
         iFmt++; // entry is valid
      }
   }
   strncpy(chPath, chAppPath, sizeof(chPath)-13);
   strcat(chPath, "/printer.dat");
   getConfigValueString(chFileName, "file", "printer_file", CPC.printer_file, sizeof(CPC.printer_file)-1, chPath);
   if (CPC.printer_file[0] == '\0') {
      strcpy(CPC.printer_file, chPath);
   }
   strncpy(chPath, chAppPath, sizeof(chPath)-12);
   strcat(chPath, "/screen.png");
   getConfigValueString(chFileName, "file", "sdump_file", CPC.sdump_file, sizeof(CPC.sdump_file)-1, chPath);
   if (CPC.sdump_file[0] == '\0') {
      strcpy(CPC.sdump_file, chPath);
   }

   strncpy(chPath, chAppPath, sizeof(chPath)-5);
   strcat(chPath, "/rom");
   getConfigValueString(chFileName, "rom", "rom_path", CPC.rom_path, sizeof(CPC.rom_path)-1, chPath);

   for (iRomNum = 0; iRomNum < 16; iRomNum++)
   { // loop for ROMs 0-15
      char chRomId[14];
      sprintf(chRomId, "slot%02d", iRomNum); // build ROM ID
      getConfigValueString(chFileName, "rom", chRomId, CPC.rom_file[iRomNum], sizeof(CPC.rom_file[iRomNum])-1, "");
   }

   if (CPC.rom_path[0] == '\0') // if the path is empty, set it to the default
      strcpy(CPC.rom_path, chPath);
   if ((pfileObject = fopen(chFileName, "rt")) == NULL)
      strcpy(CPC.rom_file[7], "amsdos.rom"); // insert AMSDOS in slot 7 if the config file does not exist yet
   else
      fclose(pfileObject);
   getConfigValueString(chFileName, "rom", "rom_mf2", CPC.rom_mf2, sizeof(CPC.rom_mf2)-1, "");
}

void splitPathFileName(char *pchCombined, char *pchPath, char *pchFile)
{
   char *pchPtr;

   pchPtr = strrchr(pchCombined, '/'); // start from the end and find the first path delimiter
   if (!pchPtr) {
      pchPtr = strrchr(pchCombined, '\\'); // try again with the alternate form
   }
   if (pchPtr)
   {
      char chOld;

      pchPtr++; /* advance the pointer to the next character */
      if (pchFile)
         strcpy(pchFile, pchPtr); /* copy the filename */

      chOld   = *pchPtr;
      *pchPtr = 0; /* cut off the filename part */

      if (pchPath != pchCombined)
      {
         if (pchPath)
            strcpy(pchPath, pchCombined); // copy the path
         *pchPtr = chOld; // restore original string
      }
   }
   else
   {
      if (pchFile)
         *pchFile = 0; // no filename found
      if (pchPath != pchCombined)
      {
         if (pchPath)
            strcpy(pchPath, pchCombined); // copy the path
      }
   }
}

void doCleanUp (void)
{
   printer_stop();
   emulator_shutdown();

   dsk_eject(&driveA);
   dsk_eject(&driveB);

   tape_eject();
   cpr_eject();

   if (zip_info.pchFileNames)
      free(zip_info.pchFileNames);

   audio_shutdown();
   video_shutdown();

   #ifdef DEBUG
   fclose(pfoDebug);
   #endif

}

void emu_reset(void)
{
	emulator_reset(false);
}

extern int retro_ui_finalized;

void change_model(int val){

	CPC.model=val;

   if ((CPC.model >= 2) && (CPC.ram_size < 128))
      CPC.ram_size   = 128; // minimum RAM size for CPC 6128 is 128KB

   /* Reconfigure emulator */
   emulator_shutdown();
   emulator_init();
//printf("change model %d ---------------\n",val);
}

void change_ram(int val){

	CPC.ram_size=val;

   if ((CPC.model >= 2) && (CPC.ram_size < 128))
      CPC.ram_size   = 128; // minimum RAM size for CPC 6128 is 128KB

   /* Reconfigure emulator */
   emulator_shutdown();
   emulator_init();
//printf("change ram %d ---------------\n",val);
}

void retro_key_down(int key)
{
	int code;

	if(key<512)
 		code=KeySymToCPCKey[key];
	else code = CPC_KEY_NULL;
	CPC_SetKey(code);
}

void retro_key_up(int key)
{
	int code;

	if(key<512)
 		code=KeySymToCPCKey[key];
	else code = CPC_KEY_NULL;
	CPC_ClearKey(code);
}

static int jflag[6]={0,0,0,0,0,0};
static int jflag0[6]={0,0,0,0,0,0};

void Keyhdl(unsigned char cpc_key,int press){

	if(press)keyboard_matrix[(unsigned char)cpc_key >> 4] &= ~bit_values[(unsigned char)cpc_key & 7]; // key is being held down
	else keyboard_matrix[(unsigned char)cpc_key >> 4] |= bit_values[(unsigned char)cpc_key & 7];

}

void retro_joy1(unsigned char joy1)
{
   //0x01,0x02,0x04,0x08,0x80
   // UP  DWN  LEFT RGT  BTN0
   // 0    1     2   3    4

   //UP
   if(joy1&0x01)
   {
      if(jflag[0]==0)
      {
         Keyhdl(0x60,1);
         jflag[0]=1;
      }
   }
   else
   {
      if(jflag[0]==1)
      {
         Keyhdl(0x60,0);
         jflag[0]=0;
      }
   }

   //Down
   if(joy1&0x02){
      if(jflag[1]==0){
         Keyhdl(0x61,1);
         jflag[1]=1;
      }
   }else {
      if(jflag[1]==1){
         Keyhdl(0x61,0);
         jflag[1]=0;
      }
   }

   //Left
   if(joy1&0x04){
      if(jflag[2]==0){
         Keyhdl(0x62,1);
         jflag[2]=1;
      }
   }else {
      if(jflag[2]==1){
         Keyhdl(0x62,0);
         jflag[2]=0;
      }
   }

   //Right
   if(joy1&0x08){
      if(jflag[3]==0){
         Keyhdl(0x63,1);
         jflag[3]=1;
      }
   }else {
      if(jflag[3]==1){
         Keyhdl(0x63,0);
         jflag[3]=0;
      }
   }

   //btn0
   if(joy1&0x80){
      if(jflag[4]==0){
         Keyhdl(0x64,1);
         jflag[4]=1;
      }
   }else {
      if(jflag[4]==1){
         Keyhdl(0x64,0);
         jflag[4]=0;
      }
   }

   //btn1
   if(joy1&0x40){
      if(jflag[5]==0){
         Keyhdl(0x65,1);
         jflag[5]=1;
      }
   }else {
      if(jflag[5]==1){
         Keyhdl(0x65,0);
         jflag[5]=0;
      }
   }

}

void retro_joy0(unsigned char joy0)
{
   //0x01,0x02,0x04,0x08,0x80
   // UP  DWN  LEFT RGT  BTN0
   // 0    1     2   3    4

   //UP
   if(joy0&0x01)
   {
      if(jflag0[0]==0)
      {
         retro_key_down(RETROK_HOME);
         jflag0[0]=1;
      }
   }
   else
   {
      if(jflag0[0]==1)
      {
         retro_key_up(RETROK_HOME);
         jflag0[0]=0;
      }
   }

   //Down
   if(joy0&0x02){
      if(jflag0[1]==0){
         retro_key_down(RETROK_END);
         jflag0[1]=1;
      }
   }else {
      if(jflag0[1]==1){
         retro_key_up(RETROK_END);
         jflag0[1]=0;
      }
   }

   //Left
   if(joy0&0x04){
      if(jflag0[2]==0){
         retro_key_down(RETROK_DELETE);
         jflag0[2]=1;
      }
   }else {
      if(jflag0[2]==1){
         retro_key_up(RETROK_DELETE);
         jflag0[2]=0;
      }
   }

   //Right
   if(joy0&0x08){
      if(jflag0[3]==0){
         retro_key_down(RETROK_PAGEDOWN);
         jflag0[3]=1;
      }
   }else {
      if(jflag0[3]==1){
         retro_key_up(RETROK_PAGEDOWN);
         jflag0[3]=0;
      }
   }

   //btn0
   if(joy0&0x80){
      if(jflag0[4]==0){
         retro_key_down(RETROK_INSERT);
         jflag0[4]=1;
      }
   }else {
      if(jflag0[4]==1){
         retro_key_up(RETROK_INSERT);
         jflag0[4]=0;
      }
   }

   //btn1
   if(joy0&0x40){
      if(jflag0[5]==0){
         retro_key_down(RETROK_PAGEUP);
         jflag0[5]=1;
      }
   }else {
      if(jflag0[5]==1){
         retro_key_up(RETROK_PAGEUP);
         jflag0[5]=0;
      }
   }
}

void mixsnd(void)
{
   int x;
   int16_t *p = NULL;

   if(SND != 1)
      return;

   p = (int16_t*)pbSndBuffer;

   for(x = 0; x < 882 * 2; x += 2)
      retro_audio_cb(p[x],p[x+1]);
}


int skel_main(int argc, char *argv[])
{
   retro_InitialiseKeyboardMapping();
   capmain(argc,argv);
   return 0;
}

int InitOSGLU(void)
{

   return 0;
}

int  UnInitOSGLU(void)
{
   doCleanUp();
   return 0;
}

uint32_t dwSndDist;
int iExitCondition;
bool bolDone;

//FIXME RETRO
//AUTOBOOT TAKEN FROM CPCDROID
#include "cpc_cat.h"

static int cur_name_id  = 0;

int cpc_dsk_system = 0;
int
cap32_disk_dir(char *FileName)
{
  cpc_dsk_system = 0;
  int error = cpc_dsk_dir(FileName);
  if (! error) {
    if (cpc_dsk_num_entry > 20) {
      int index;
      for (index = 0; index < cpc_dsk_num_entry; index++) {
        int cpos = 0;
        for (cpos = 0; cpc_dsk_dirent[index][cpos]; cpos++) {
          /* High number of files with no printable chars ? might be CPM */
          if (cpc_dsk_dirent[index][cpos] < 32) {
            cpc_dsk_system = 1;
            cpc_dsk_num_entry = 0;
            break;
          }
        }
      }
    }
  }
  return error;
}

int retro_disk_auto()
{
  char Buffer[128];
  int  index;
  int  found = 0;
  int  first_bas = -1;
  int  first_spc = -1;
  int  first_bin = -1;

  cur_name_id = 0;
/*
  char *RunName = psp_run_search(CPC.cpc_save_name);

  if (RunName != (char *)0 ) {

    if (!strcasecmp(RunName, "|CPM")) strcpy(Buffer, "|CPM");
    else  snprintf(Buffer, MAX_PATH, "RUN\"%s", RunName);

  } else */ {

    for (index = 0; index < cpc_dsk_num_entry; index++) {
      char* scan = strchr(cpc_dsk_dirent[index], '.');
      if (scan) {
        if (! strcasecmp(scan+1, "BAS")) {
          if (first_bas == -1) first_bas = index;
          found = 1;
        } else
        if (! strcasecmp(scan+1, "")) {
          if (first_spc == -1) first_spc = index;
          found = 1;
        } else
        if (! strcasecmp(scan+1, "BIN")) {
          if (first_bin == -1) first_bin = index;
          found = 1;
        }
      }
    }
    if (! found) {

      if (cpc_dsk_system) {
        strcpy(Buffer, "|CPM");
      } else {
			printf("autoload not found\n");
			return -1;
      }

    } else {
      if (first_bas != -1) cur_name_id = first_bas;
      else
      if (first_spc != -1) cur_name_id = first_spc;
      else
      if (first_bin != -1) cur_name_id = first_bin;

      sprintf(Buffer, "RUN\"%s", cpc_dsk_dirent[cur_name_id]);
    }
  }

  //if (CPC.psp_explore_disk == CPC_EXPLORE_FULL_AUTO)
  {
    strcat(Buffer, "\n");
  }

  //printf("(%s)\n",Buffer);
  kbd_buf_feed(Buffer);

  return 1;
}

int attach_disk(char *arv, int drive)
{
	int result = 1;

	if(!drive) {
		if((result = dsk_load( arv, &driveA, 'A')) == 0)
		{
			sprintf(DISKA_NAME,"%s",arv);
			cap32_disk_dir(arv);
		}
	} else {
		if((result = dsk_load( arv, &driveB, 'B')) == 0)
		{
			sprintf(DISKB_NAME,"%s",arv);
			cap32_disk_dir(arv);
		}
   }
   return result;
}

int detach_disk(int drive)
{
   if(!drive)
   {
      dsk_eject(&driveA);
      DISKA_NAME[0] = '\0';
   }
   else
   {
      dsk_eject(&driveB);
      DISKB_NAME[0] = '\0';
   }

   return 0;
}

int loadadsk (char *arv,int drive)
{
   if( HandleExtension(arv,"DSK") || HandleExtension(arv,"dsk") )
   {
	  if(attach_disk(arv, drive) == 0)
	  {
		  retro_disk_auto();
		  sprintf(RPATH,"%s%d.SNA",arv,drive);
	  }
   }
   else if( HandleExtension(arv,"sna") || HandleExtension(arv,"SNA") )
   {
      snapshot_load (arv);
      sprintf(RPATH,"%s",arv);
   }
   return 0;
}

void play_tape(){

	if (pbTapeImage) {

		if (CPC.tape_play_button) {
			CPC.tape_play_button = 0;
		} else {
			CPC.tape_play_button = 0x10;
		}

	}
}


void check_kbd_command()
{

   	if (autoboot_delay<AUTODELAY)
    	autoboot_delay++;
   	else if (autoboot_delay==AUTODELAY)
   	{
   		if (!autorun)
   			kbd_runcmd=false;

     		autoboot_delay++;
   	}

	if(kbd_runcmd==true && autoboot_delay>AUTODELAY){

	  	static int pair=-1;

      		pair=-pair;
      		if(pair==1)
      	   		return;

		kbd_buf_update();

	}

}

int RLOOP=1;

void retro_loop(void)
{

	while(RLOOP==1)
		theloop();
	RLOOP=1;

	check_kbd_command();
//printf("auto:%d run:%d cmd:%d\n",autoboot_delay,autorun,kbd_runcmd);
}

void theloop(void)
{

     if ((CPC.limit_speed) && (iExitCondition == EC_CYCLE_COUNT))
   {
      int iTicksAdj = 0; // no adjustment necessary by default

      if (CPC.snd_enabled)
      {

         if (pbSndStream < CPC.snd_bufferptr)
            dwSndDist = CPC.snd_bufferptr - pbSndStream; // determine distance between play and write cursors
         else
            dwSndDist = (pbSndBufferEnd - pbSndStream) + (CPC.snd_bufferptr - pbSndBuffer);

         if (dwSndDist < dwSndMinSafeDist)
            iTicksAdj = -5; // speed emulation up to compensate
         else if (dwSndDist > dwSndMaxSafeDist)
            iTicksAdj = 5; // slow emulation down to compensate
      }

   }

   uint32_t dwOffset = CPC.scr_pos - CPC.scr_base; // offset in current surface row
   if (VDU.scrln > 0)
      CPC.scr_base = (uint32_t *)&bmp[0] + (VDU.scrln * CPC.scr_line_offs); // determine current position
   else
      CPC.scr_base = (uint32_t *)&bmp[0]; // reset to surface start

   CPC.scr_pos = CPC.scr_base + dwOffset; // update current rendering position

   iExitCondition = z80_execute(); // run the emulation until an exit condition is met

   if (iExitCondition == EC_FRAME_COMPLETE)
   {
      /* emulation finished rendering a complete frame? */
      dwFrameCount++;
      RLOOP=0; /* exit retro_loop for retro_run */
   }
   else if (iExitCondition == EC_SOUND_BUFFER)
      mixsnd();

}

int capmain (int argc, char **argv)
{
   strcpy (chAppPath, "./");

   /* retrieve the emulator configuration */
   loadConfiguration();

   if (CPC.printer)
   {
      /* start capturing printer output, if enabled */
      if (!printer_start())
         CPC.printer = 0;
   }

   /* init Z80 emulation */
   z80_init_tables();

   if (video_init())
   {
      fprintf(stderr, "video_init() failed. Aborting.\n");
      exit(-1);
   }

   if (audio_init())
   {
      fprintf(stderr, "audio_init() failed. Disabling sound.\n");
      CPC.snd_enabled = 0; // disable sound emulation
   }

   if (emulator_init())
   {
      fprintf(stderr, "emulator_init() failed. Aborting.\n");
      exit(-1);
   }

   memset(&driveA, 0, sizeof(t_drive)); // clear disk drive A data structure
   memset(&driveB, 0, sizeof(t_drive)); // clear disk drive B data structure

   dwTicksOffset     = (int)(20.0 / (double)((CPC.speed * 25) / 100.0));
   dwTicksTarget     = GetTicks();
   dwTicksTargetFPS  = dwTicksTarget;
   dwTicksTarget    += dwTicksOffset;

   iExitCondition    = EC_FRAME_COMPLETE;
   bolDone           = false;

   retro_ui_finalized=1;

   return 0;
}
