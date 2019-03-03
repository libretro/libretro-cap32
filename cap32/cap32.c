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
#include "retro_snd.h"

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
extern char cart_name[512];

#include "cap32.h"
#include "crtc.h"
#include "tape.h"
#include "cart.h"
#include "z80.h"
#include "asic.h"
#include "slots.h"
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
uint8_t keyboard_matrix[16];

uint8_t *membank_config[8][4];

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

double colours_rgb[32][3] = {
   { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 },{ 0.0, 1.0, 0.5 }, { 1.0, 1.0, 0.5 },
   { 0.0, 0.0, 0.5 }, { 1.0, 0.0, 0.5 },{ 0.0, 0.5, 0.5 }, { 1.0, 0.5, 0.5 },
   { 1.0, 0.0, 0.5 }, { 1.0, 1.0, 0.5 },{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0 },
   { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 1.0 },{ 1.0, 0.5, 0.0 }, { 1.0, 0.5, 1.0 },
   { 0.0, 0.0, 0.5 }, { 0.0, 1.0, 0.5 },{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 },
   { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },{ 0.0, 0.5, 0.0 }, { 0.0, 0.5, 1.0 },
   { 0.5, 0.0, 0.5 }, { 0.5, 1.0, 0.5 },{ 0.5, 1.0, 0.0 }, { 0.5, 1.0, 1.0 },
   { 0.5, 0.0, 0.0 }, { 0.5, 0.0, 1.0 },{ 0.5, 0.5, 0.0 }, { 0.5, 0.5, 1.0 }
};

// original RGB color to GREEN LUMA converted by Ulrich Doewich
// unknown formula, check video_get_green to see an approximation.
double colours_green[32] = {
   0.5647, 0.5647, 0.7529, 0.9412,
   0.1882, 0.3765, 0.4706, 0.6588,
   0.3765, 0.9412, 0.9098, 0.9725,
   0.3451, 0.4078, 0.6275, 0.6902,
   0.1882, 0.7529, 0.7216, 0.7843,
   0.1569, 0.2196, 0.4392, 0.5020,
   0.2824, 0.8471, 0.8157, 0.8784,
   0.2510, 0.3137, 0.5333, 0.5961
};

PIXEL_TYPE colours[32];

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

#define MAX_ROM_MODS 2
#include "rom/rom_mods.h"

#include "rom/464.h"
#include "rom/6128.h"
#include "rom/6128p.h"
#include "rom/amsdos.h"


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
            // 6128+: always use port B as input as this fixes Tintin on the moon.
            // This should always be the case anyway but do not activate it for other model for now, let's validate it before.
            // TODO: verify with CPC (non-plus) if we go in the else in some cases
            if (CPC.model > 2 || PPI.control & 2) { // port B set to input?
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
               GateArray.palette[GateArray.pen] =colours[colour];
               // mode 2 - 'anti-aliasing' colour
               if (GateArray.pen < 2) {
                  video_set_palette_antialias();
               }
            }
            if (CPC.mf2) { // MF2 enabled?
               int iPen = *(pbMF2ROM + 0x03fcf);
               *(pbMF2ROM + (0x03f90 | ((iPen & 0x10) << 2) | (iPen & 0x0f))) = val;
            }
            break;
         case 2: // set mode
            if (!asic.locked && (val & 0x20) && CPC.model > 2) {
               // 6128+ RMR2 register
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
            } else {
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
            if (asic.locked) {
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
            } else {
               // 6128+ memory mapping register
               printf("Memory mapping register (RAM)\n");
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
      if (CPC.model <= 2) {
         GateArray.upper_ROM = val;
         pbExpansionROM = memmap_ROM[val];

         /* selected expansion ROM not present? */
         if (pbExpansionROM == NULL)
            pbExpansionROM = pbROMhi; /* revert to BASIC ROM */
      } else {
         //printf("ROM select: %u\n", (int) val);
         if (val == 7) {
            GateArray.upper_ROM = 3;
            pbExpansionROM = pbCartridgePages[3];
         } else if (val >= 128) {
            GateArray.upper_ROM = val & 31;
            pbExpansionROM = pbCartridgePages[val & 31];
         } else {
            GateArray.upper_ROM = 1;
            pbExpansionROM = pbCartridgePages[1];
         }
         //printf("ROM-PAGE select: %u\n", (int) page);
         //printf("ROM-PAGE val: %u\n", (int) pbExpansionROM[0]);
      }
      /* upper/expansion ROM is enabled? */
      if (!(GateArray.ROM_config & 0x08))
         membank_read[3] = pbExpansionROM; /* 'page in' upper/expansion ROM */

      /* MF2 enabled? */
      if (CPC.mf2)
         *(pbMF2ROM + 0x03aac) = val;
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
      if(FDC.motor)
         retro_snd_cmd(SND_FDCMOTOR, ST_LOOP);
      else
         retro_snd_cmd(SND_FDCMOTOR, ST_OFF);
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

int emulator_select_ROM (void)
{
   uint8_t *pbPtr;

   // TODO load custom bios
   switch(CPC.model)
   {
      case 0: // 464
         memcpy(pbROM, OS_464, (32*1024)); // FIXME FOR CPC 464
         break;
              // 664
      case 2: // 6128
         memcpy(pbROM, OS_6128, (32*1024));
         memmap_ROM[7] = (uint8_t*)&AMSDOS[0];
         break;
      case 3: // 6128+
         if(cart_name[0] == '\0') {
            cpr_load(&OS_6128P[0]);
            if (pbCartridgePages[0] != NULL)
               pbROMlo = pbCartridgePages[0];
            LOGI("used internal bios!\n");
         } else if (pbCartridgeImage != NULL) {
            LOGI("loaded cart: %s\n", cart_name);
         }
         break;
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
         case 3: // 6128+
            if(cart_name[0] == '\0')
               pbPtr += 0x1eef; // Only patch system cartridge
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
   if(CPC.model > 2){
      if (pbCartridgePages[0] != NULL)
         pbROMlo = pbCartridgePages[0];
   }

   asic_reset();
   // FIXME - generate plus palette correctly
   video_set_palette();

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

   asic.locked = true;

   // Gate Array
   memset(&GateArray, 0, sizeof(GateArray)); // clear GA data structure
   GateArray.scr_mode = GateArray.requested_scr_mode = 1; // set to mode 1
   GateArray.registerPageOn = false;
   GateArray.lower_ROM_bank = 0;
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

int emulator_init (void)
{
   int iErr, iRomNum;
   char chPath[_MAX_PATH + 1];
   char *pchRomData = NULL;

   (void)pchRomData;
   (void)chPath;
   (void)iRomNum;
   (void)iErr;

   pbGPBuffer     = (uint8_t*) malloc(128 * 1024 * sizeof(uint8_t)); // attempt to allocate the general purpose buffer
   pbRAM          = (uint8_t*) malloc(CPC.ram_size * 1024 * sizeof(uint8_t)); // allocate memory for desired amount of RAM
   pbROM          = (uint8_t*) malloc(32 * 1024 * sizeof(uint8_t));
   pbRegisterPage = (uint8_t*) malloc(16 * 1024 * sizeof(uint8_t));

   if (!pbGPBuffer || !pbRAM || !pbRegisterPage)
      return ERR_OUT_OF_MEMORY;

   pbROMlo = pbROM;
   pbROMhi = pbExpansionROM = (uint8_t *)pbROMlo + 16384;

   memset(memmap_ROM, 0, sizeof(memmap_ROM[0]) * 256); // clear the expansion ROM map
   ga_init_banking(); // init the CPC memory banking map

   emulator_select_ROM();

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

   if(retro_computer_cfg.model != 3) {
      fprintf(stderr, "Cartridge ERROR: Please select CPC6128+.\n");
      return ERR_CPR_INVALID;
   }

   int result = cpr_fload(pchFileName);

   if(result != 0) {
      fprintf(stderr, "Load of cartridge failed. Aborting.\n");
      return result;
   }

   sprintf(cart_name,"%s",pchFileName);

   /* Restart emulator if initiated */
   if(emu_status & COMPUTER_READY) {
      emulator_shutdown();
      emulator_init();
   }

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

uint32_t video_monitor_colour (double r, double g, double b)
{
   uint32_t red = (uint32_t)(r * (CPC.scr_intensity / 10.0) * 255);
   if (red > 255) /* limit to the maximum */
      red = 255;

   uint32_t green = (uint32_t)(g * (CPC.scr_intensity / 10.0) * 255);
   if (green > 255)
      green = 255;

   uint32_t blue = (uint32_t)(b * (CPC.scr_intensity / 10.0) * 255);
   if (blue > 255)
      blue = 255;

   return RGB2COLOR(red, green, blue);
}

// Convert RGB color to GREEN LUMA
// check colours_green vs colours_rgb, we could use here a logarithm.
// https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
#define G_LUMA_R     0.2427
#define G_LUMA_G     0.6380
#define G_LUMA_B     0.1293
#define G_LUMA_BASE  0.071
#define G_LUMA_COEF  0.100
#define G_LUMA_PRIM  0.050

uint32_t video_monitor_green(double r, double g, double b) {
   double green_luma = ((G_LUMA_R * r) + (G_LUMA_G * g) + (G_LUMA_B * b));
   green_luma += (G_LUMA_BASE + G_LUMA_PRIM - (G_LUMA_COEF * green_luma));

   uint32_t green = (uint32_t) (green_luma * (CPC.scr_intensity / 10.0) * 255);

   if (green > 255)
      green = 255;

   return RGB2COLOR(0, green, 0);
}

// Convert RGB to LUMA
#define BN_LUMA_R     0.299
#define BN_LUMA_G     0.587
#define BN_LUMA_B     0.144
uint32_t video_monitor_grey(double r, double g, double b) {
   double grey_luma = ((BN_LUMA_R * r) + (BN_LUMA_G * g) + (BN_LUMA_B * b));
   uint32_t grey = (uint32_t) (grey_luma * (CPC.scr_intensity / 10.0) * 255);

   if (grey > 255)
      grey = 255;

   return RGB2COLOR(grey, grey, grey);
}

void video_update_tube() {
   switch(CPC.scr_tube) {
      case CPC_MONITOR_COLOR:
         CPC.video_monitor = video_monitor_colour;
         break;
      case CPC_MONITOR_GREEN:
         CPC.video_monitor = video_monitor_green;
         break;
      case CPC_MONITOR_WHITE:
         CPC.video_monitor = video_monitor_grey;
         break;
   }
}

/**
 * generate antialias values using 32/16bits macros
 *
 * RGB[10] 00CE60 || CC CC 00
 * 00CE60 (10)    || C8 CC 00
 */
void video_set_palette_antialias (void)
{
   uint8_t r2,g2,b2;
   r2=RGB2RED(colours[GateArray.ink_values[0]]) + RGB2RED(colours[GateArray.ink_values[1]]);
   g2=RGB2GREEN(colours[GateArray.ink_values[0]]) + RGB2GREEN(colours[GateArray.ink_values[1]]);
   b2=RGB2BLUE(colours[GateArray.ink_values[0]]) + RGB2BLUE(colours[GateArray.ink_values[1]]);
   GateArray.palette[33] = (PIXEL_TYPE) RGB2COLOR(r2/2, g2/2, b2/2);
}

int video_set_palette (void)
{

   int n;
   video_update_tube();

   for (n = 0; n < 32; n++)
   {
      colours[n] = (PIXEL_TYPE) CPC.video_monitor(colours_rgb[n][0], colours_rgb[n][1], colours_rgb[n][2]);
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
   printf("model:%u, style: %u, dwScale: %ux%u, offset: %u\n", CPC.model, CPC.scr_style, dwXScale, dwYScale, CPC.scr_line_offs);

   switch (dwXScale)
   {
      case 1:
      if (CPC.model > 2) {
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal_half_plus;
      } else {
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal_half;
      }
         CPC.scr_prerenderbord = (void(*)(void))prerender_border_half;
         CPC.scr_prerendersync = (void(*)(void))prerender_sync_half;
         break;
      case 2:
      if (CPC.model > 2) {
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal_plus;
      } else {
         CPC.scr_prerendernorm = (void(*)(void))prerender_normal;
      }
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
      case 16:
      case 15:
      default:
         if(dwYScale == 2)
            CPC.scr_render = (void(*)(void))render16bpp_doubleY;
         else
            CPC.scr_render = (void(*)(void))render16bpp;
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

   //CPC.model = getConfigValueInt(chFileName, "system", "model", 2); // CPC 6128
   CPC.model = retro_computer_cfg.model;
   if (CPC.model > 3)
      CPC.model = 2;

   CPC.jumpers       = getConfigValueInt(chFileName, "system", "jumpers", 0x1e) & 0x1e; // OEM is Amstrad, video refresh is 50Hz
   //CPC.ram_size      = getConfigValueInt(chFileName, "system", "ram_size", 128) & 0x02c0; // 128KB RAM
   CPC.ram_size = retro_computer_cfg.ram;

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
   //CPC.keyboard      = getConfigValueInt(chFileName, "system", "keyboard", 0);
   CPC.keyboard = retro_computer_cfg.lang;

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

void emu_restart(void)
{
   /* Reconfigure emulator */
   emulator_shutdown();
   emulator_init();

   retro_computer_cfg.is_dirty = false;
}

void change_model(int val){

	CPC.model=val;

   if ((CPC.model >= 2) && (CPC.ram_size < 128))
      CPC.ram_size   = 128; // minimum RAM size for CPC 6128 is 128KB

   retro_computer_cfg.is_dirty = true;
}

void change_ram(int val){

	CPC.ram_size=val;

   if ((CPC.model >= 2) && (CPC.ram_size < 128))
      CPC.ram_size   = 128; // minimum RAM size for CPC 6128 is 128KB

   retro_computer_cfg.is_dirty = true;
}

uint8_t* get_ram_ptr() {
	return pbRAM;
}

size_t get_ram_size(void) {
	return CPC.ram_size * 1024;
}

void change_lang(int val){
   CPC.keyboard=val;
   retro_computer_cfg.is_dirty = true;
}


void mixsnd(void)
{
   int x;
   int16_t *p = NULL;

   if(SND != 1)
      return;

   retro_snd_mixer();

   p = (int16_t*)pbSndBuffer;

   for(x = 0; x < 882 * 2; x += 2)
      retro_audio_cb(p[x],p[x+1]);
}


int skel_main(int argc, char *argv[])
{
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
   int error = cpc_dsk_dir(FileName);
   if (! error) {
      cpc_dsk_system = (cpc_dsk_type == DSK_TYPE_SYSTEM);
      printf("INFO-DSK: num: %d sys(%d)\n", cpc_dsk_num_entry, cpc_dsk_system);
      if (cpc_dsk_num_entry > 20) {
         int index;
         for (index = 0; index < cpc_dsk_num_entry; index++) {
            int cpos = 0;
            printf("INFO: DIR-INIT: i(%d) p(%d) = %x\n", index, cpos, cpc_dsk_dirent[index][cpos]);
            for (cpos = 0; cpc_dsk_dirent[index][cpos]; cpos++) {
               /* with no printable chars in first ? might be CPM */
               if (cpc_dsk_dirent[index][cpos] < 32) {
                  if(!index) {
                     cpc_dsk_num_entry = 0;
                  } else {
                     // some filenames are loaded used it! -- fixed cracked custom DSKs
                     cpc_dsk_num_entry = index;
                  }
                  printf("DSK_LOAD INFO-SYS: dsk: i(%d) p(%d) = %d \n", index, cpos, cpc_dsk_dirent[index][cpos]);
                  break;
               }
            }
         }
      } else {
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
         strcpy(Buffer, "CAT");
			printf("autoload not found\n");
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
			result = cap32_disk_dir(arv);
         if(result)
            printf("error dsk: %d\n", result);
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

   emu_status = COMPUTER_READY; // set computer init as completed

   return 0;
}
