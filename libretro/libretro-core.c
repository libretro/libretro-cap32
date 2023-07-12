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

#include "microui.h"
#include "libretro.h"
#include "libretro-core.h"
#include "string/stdstring.h"
#include "file/file_path.h"

#include "retro_events.h"
#include "retro_utils.h"
#include "retro_snd.h"
#include "retro_render.h"
#include "retro_ui.h"

#include "gfx/software.h"
#include "gfx/video.h"
#include "assets/assets.h"
#include "dsk/loader.h"
#include "db/database.h"

char loader_buffer[LOADER_MAX_SIZE];
#if defined(PSP) || defined(PS2)
__attribute__((aligned(16))) uint16_t retro_palette[256];
#else
uint16_t retro_palette[256];
#endif
static uint16_t* cpc_video_out;

// TIME
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

// DISK CONTROL
#include "retro_disk_control.h"
static dc_storage* dc;

// LIGHTGUN
#include "retro_gun.h"
extern t_lightgun_cfg lightgun_cfg;

// LOG
retro_log_printf_t log_cb;

computer_cfg_t retro_computer_cfg;
retro_video_t retro_video;
game_cfg_t game_configuration;

extern t_button_cfg btnPAD[MAX_PADCFG];

extern void change_model(int val);
extern int snapshot_load (char *pchFileName);
extern int attach_disk(char *arv, int drive);
extern int detach_disk(int drive);
extern int tape_insert (char *pchFileName);
extern int cart_start (char *pchFileName);
extern void enter_gui(void);
extern int Retro_PollEvent();
extern void retro_loop(void);
extern int video_set_palette (void);
extern int InitOSGLU(void);
extern int  UnInitOSGLU(void);
extern void emu_reset(void);
extern void emu_reconfigure(void);
extern void change_ram(int val);
extern uint8_t* get_ram_ptr();
extern size_t get_ram_size();
extern void change_lang(int val);
extern int snapshot_save (char *pchFileName);
extern void play_tape();
extern void retro_joy0(unsigned char joy0);
extern void retro_key_down(int key);
extern void retro_key_up(int key);

//VIDEO
uint32_t * video_buffer;
uint32_t * temp_buffer;

int32_t* audio_buffer = NULL;
int audio_buffer_size = 0;

// almost deprecated
int SHIFTON=-1;

int retro_scr_style = 0;
int gfx_buffer_size = 0;

unsigned amstrad_devices[ 2 ];

int emu_status = COMPUTER_OFF;

//CAP32 DEF BEGIN
#include "cap32.h"
#include "slots.h"
//#include "z80.h"
extern t_CPC CPC;
extern uint8_t *pbSndBuffer;
//CAP32 DEF END

extern void update_input(void);
extern void Emu_init();
extern void Emu_uninit();
extern void input_gui(void);

const char *retro_save_directory;
const char *retro_system_directory;
const char *retro_content_directory;
char retro_system_data_directory[512];
char retro_system_bios_directory[512];
char retro_content_filepath[512];

/*static*/ retro_input_state_t input_state_cb;
/*static*/ retro_input_poll_t input_poll_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
/*static*/ retro_audio_sample_batch_t audio_batch_cb;
/*static*/ retro_environment_t environ_cb;
/*static*/ retro_mouse_t mouse =
{
   EMULATION_SCREEN_WIDTH/2,
   EMULATION_SCREEN_WIDTH/2,
   EMULATION_SCREEN_HEIGHT/2 + EMULATION_SCREEN_HEIGHT/4,
   EMULATION_SCREEN_HEIGHT/2 + EMULATION_SCREEN_HEIGHT/4,
   0, 0, 0
};

static const struct retro_controller_description controllers[4] = {
   { "Amstrad Joystick", RETRO_DEVICE_AMSTRAD_JOYSTICK },
   { "Amstrad Keyboard", RETRO_DEVICE_AMSTRAD_KEYBOARD },
   { "Amstrad Lightgun", RETRO_DEVICE_AMSTRAD_LIGHTGUN },
   { "None", RETRO_DEVICE_NONE },
};

static const struct retro_controller_info ports[3] = {
   { controllers, 4 }, // port 1
   { controllers, 4 }, // port 2
   { NULL, 0 }
};

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

int retro_getStyle(){
   LOGI("getStyle: %u\n", retro_scr_style);
   return retro_scr_style;
}

int retro_getGfxBpp(){
   LOGI("getBPP: %u\n", 8 * retro_video.pitch);
   return 8 * retro_video.pitch;
}

int retro_getGfxBps(){
   LOGI("getBPS: %u\n", EMULATION_SCREEN_WIDTH);
   return EMULATION_SCREEN_WIDTH;
}

int retro_getAudioBuffer(){
   if(audio_buffer_size)
      return audio_buffer_size;

   int sample_size = AUDIO_BUFSIZE * AUDIO_CHANNELS * AUDIO_BYTES;
   audio_buffer_size = 1;

   while (audio_buffer_size < sample_size)
      audio_buffer_size <<= 1;

   LOGI("getAudioBuffer: %u\n", audio_buffer_size);
   return audio_buffer_size; // return the closest match as 2^n
}

INLINE unsigned int * retro_getScreenPtr(){
    return (unsigned int *) video_buffer;
}

#include <ctype.h>

//Args for experimental_cmdline
static char ARGUV[64][1024];
static unsigned char ARGUC=0;

// Args for Core
static char XARGV[64][1024];
static const char* xargv_cmd[64];
int PARAMCOUNT=0;

extern int  skel_main(int argc, char *argv[]);
void parse_cmdline( const char *argv );

void Add_Option(const char* option)
{
   static int first=0;

   if(first==0)
   {
      PARAMCOUNT=0;
      first++;
   }

   sprintf(XARGV[PARAMCOUNT++],"%s",option);
}

// TODO: clean this...
int pre_main(const char *argv)
{
   int i;
   bool Only1Arg;

   parse_cmdline(argv);

   Only1Arg = (strcmp(ARGUV[0],"x64") == 0) ? 0 : 1;

   for (i = 0; i<64; i++)
      xargv_cmd[i] = NULL;

   if(Only1Arg)
   {
      Add_Option("x64");
      if(file_check_extension(retro_content_filepath, sizeof(retro_content_filepath), "crt", 3))
         Add_Option("-cartcrt");

      Add_Option(retro_content_filepath);
   }
   else
   { // Pass all cmdline args
      for(i = 0; i < ARGUC; i++)
         Add_Option(ARGUV[i]);
   }

   for (i = 0; i < PARAMCOUNT; i++)
   {
      xargv_cmd[i] = (char*)(XARGV[i]);
      LOGI("%2d  %s\n",i,XARGV[i]);
   }

   skel_main(PARAMCOUNT,( char **)xargv_cmd);

   xargv_cmd[PARAMCOUNT - 2] = NULL;

   return 0;
}

void parse_cmdline(const char *argv)
{
   char *p,*p2,*start_of_word;
   int c,c2;
   static char buffer[512*4];
   enum states { DULL, IN_WORD, IN_STRING } state = DULL;

   strcpy(buffer,argv);
   strcat(buffer," \0");

   for (p = buffer; *p != '\0'; p++)
   {
      c = (unsigned char) *p; /* convert to unsigned char for is* functions */
      switch (state)
      {
         case DULL: /* not in a word, not in a double quoted string */
            if (isspace(c)) /* still not in a word, so ignore this char */
               continue;
            /* not a space -- if it's a double quote we go to IN_STRING, else to IN_WORD */
            if (c == '"')
            {
               state = IN_STRING;
               start_of_word = p + 1; /* word starts at *next* char, not this one */
               continue;
            }
            state = IN_WORD;
            start_of_word = p; /* word starts here */
            continue;
         case IN_STRING:
            /* we're in a double quoted string, so keep going until we hit a close " */
            if (c == '"')
            {
               /* word goes from start_of_word to p-1 */
               //... do something with the word ...
               for (c2 = 0,p2 = start_of_word; p2 < p; p2++, c2++)
                  ARGUV[ARGUC][c2] = (unsigned char) *p2;
               ARGUC++;

               state = DULL; /* back to "not in word, not in string" state */
            }
            continue; /* either still IN_STRING or we handled the end above */
         case IN_WORD:
            /* we're in a word, so keep going until we get to a space */
            if (isspace(c))
            {
               /* word goes from start_of_word to p-1 */
               //... do something with the word ...
               for (c2 = 0,p2 = start_of_word; p2 <p; p2++,c2++)
                  ARGUV[ARGUC][c2] = (unsigned char) *p2;
               ARGUC++;

               state = DULL; /* back to "not in word, not in string" state */
            }
            continue; /* either still IN_WORD or we handled the end above */
      }
   }
}

int HandleExtension(char *path,char *ext)
{
   int len = strlen(path);

   if (len >= 4 &&
         path[len-4] == '.' &&
         path[len-3] == ext[0] &&
         path[len-2] == ext[1] &&
         path[len-1] == ext[2])
   {
      return 1;
   }

   return 0;
}

void retro_mouse(int a,int b) {}
void retro_mouse_but0(int a) {}
void retro_mouse_but1(int a) {}
void enter_options(void) {}

#ifdef AND
#define DEFAULT_PATH "/mnt/sdcard/amstrad/"
#else

#ifdef PS3PORT
#define DEFAULT_PATH "/dev_hdd0/HOMEBREW/amstrad/"
#else
#define DEFAULT_PATH "/"
#endif

#endif

void retro_message(const char *text) {
   struct retro_message msg;
   char msg_local[256];

   snprintf(msg_local, sizeof(msg_local), "CPC: %s", text);
   msg.msg = msg_local;
   msg.frames = 100;

   environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, (void*)&msg);

}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   bool allow_no_game = true;
   cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &allow_no_game);

   environ_cb( RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports );

   struct retro_variable variables[] = {
      {
         "cap32_retrojoy0",
         "Controls > User 1 Controller Config; auto|qaop|incentive|joystick_port1",
      },
      {
         "cap32_retrojoy1",
         "Controls > User 2 Controller Config; auto|qaop|incentive|joystick_port1|joystick_port2",
      },
      {
         "cap32_combokey",
         "Controls > Combo Key; select|y|b|disabled",
      },
      {
         "cap32_db_mapkeys",
         "Controls > Use internal Remap DB; enabled|disabled",
      },
      {
         "cap32_lightgun_input",
         "Light Gun > Input; disabled|phaser|gunstick",
      },
      {
         "cap32_lightgun_show",
         "Light Gun > Show Crosshair; disabled|enabled",
      },
      {
         "cap32_model",
         "Model; 6128|464|664|6128+ (experimental)",
      },
      // rcheevos disallowed_setting: cap32_autorun disabled
      {
         "cap32_autorun",
         "Advanced > Autorun; enabled|disabled",
      },
      {
         "cap32_ram",
         "Advanced > Ram size; 128|64|192|576",
      },
      {
         "cap32_advanced_green_phosphor",
         "Video > Green Phosphor blueish; 15|20|30|5|10",
      },
      {
         "cap32_scr_intensity",
         "Video > Monitor Intensity; 8|9|10|11|12|13|14|15|5|6|7",
      },
      {
         "cap32_gfx_colors",
         #if defined(M8BPP)
         "Video Advanced > Color Depth; 8bit",
         #elif defined(M16BPP)
         "Video Advanced > Color Depth; 16bit",
         #else
         "Video Advanced > Color Depth; 16bit|24bit",
         #endif
      },
      #if 0
      {
         "cap32_resolution",
         #ifdef ANDROID
         // TODO: removed on android, need debug crash on hires is selected (issue #48)
         "Internal resolution; 384x272",
         #else
         "Internal resolution; 384x272|768x272",
         #endif
      },
      #endif
      {
         "cap32_statusbar",
         "Status Bar; onloading|enabled|disabled",
      },
      {
         "cap32_keyboard_transparency",
         "Keyboard Transparency; disabled|enabled",
      },
      {
         "cap32_floppy_sound",
         "Floppy Sound; enabled|disabled",
      },
      {
         "cap32_scr_tube",
         "Monitor Type; color|green|white",
      },
      {
         "cap32_lang_layout",
         "CPC Language; english|french|spanish",
      },

      { NULL, NULL },
   };

   environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
}

/**
 * controller_port_variable:
 * @port: user port (see DEVICE AMSTRAD)
 * @var: retro_variable ptr (modified)
 *
 * Query retro_environment callback to get joy values
 *
 * Returns: current user joy config (0/1/2/3/4),selected in GUI
 *          otherwise default config 0 (auto)
 **/
static int controller_port_variable(unsigned port, struct retro_variable *var)
{
   if ((!environ_cb) || port >= PORTS_NUMBER)
      return 0;

   var->value = NULL;
   switch (port) {
      case ID_PLAYER1:
         var->key = "cap32_retrojoy0";
         break;
      case ID_PLAYER2:
         var->key = "cap32_retrojoy1";
         break;
      }

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, var) && var->value)
   {
      if(strcmp(var->value, "qaop") == 0)
         return PADCFG_QAOP;
      if(strcmp(var->value, "incentive") == 0)
         return PADCFG_INCENTIVE;
      if(strcmp(var->value, "joystick_port1") == 0)
         return PADCFG_JOYSTICK_1;
      if(strcmp(var->value, "joystick_port2") == 0)
         return PADCFG_JOYSTICK_2;
   }

   return PADCFG_AUTO;
}

static void update_variables(void)
{
   struct retro_variable var;

   // user 1/2 - input config
   retro_computer_cfg.padcfg[ID_PLAYER1] = controller_port_variable(ID_PLAYER1, &var);
   retro_computer_cfg.padcfg[ID_PLAYER2] = controller_port_variable(ID_PLAYER2, &var);

   var.key = "cap32_autorun";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      retro_computer_cfg.autorun = strcmp(var.value, "disabled") != 0;
      if (!retro_computer_cfg.autorun)
         ev_set(EV_JOY);
      else
         ev_set(EV_AUTO);
   }

   var.key = "cap32_combokey";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "b") == 0)
         ev_combo_set(RETRO_DEVICE_ID_JOYPAD_B);
      else if (strcmp(var.value, "y") == 0)
         ev_combo_set(RETRO_DEVICE_ID_JOYPAD_Y);
      else if (strcmp(var.value, "select") == 0)
         ev_combo_set(RETRO_DEVICE_ID_JOYPAD_SELECT);
   }

   var.key = "cap32_db_mapkeys";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "disabled") == 0)
         retro_computer_cfg.use_internal_remap = false;
      else
         retro_computer_cfg.use_internal_remap = true;
   }

   var.key = "cap32_lightgun_input";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      lightgun_type val = LIGHTGUN_TYPE_NONE;
      if (strcmp(var.value, "phaser") == 0) val = LIGHTGUN_TYPE_PHASER;
      else if (strcmp(var.value, "gunstick") == 0) val = LIGHTGUN_TYPE_GUNSTICK;

      if (lightgun_cfg.guntype != val) {
         lightgun_cfg.guntype = val;
      }
   }

   var.key = "cap32_lightgun_show";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
         lightgun_cfg.show = 1;
      else
         lightgun_cfg.show = 0;
   }

   var.key = "cap32_model";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      int val = CPC_MODEL_6128; // DEFAULT 6128
      if (strcmp(var.value, "464") == 0) val = CPC_MODEL_464;
      else if (strcmp(var.value, "664") == 0) val = CPC_MODEL_664;
      else if (strcmp(var.value, "6128") == 0) val = CPC_MODEL_6128;
      else if (strcmp(var.value, "6128+ (experimental)") == 0) val = CPC_MODEL_PLUS;

      if (retro_computer_cfg.model != val) {
         retro_computer_cfg.model = val;
         if(emu_status & COMPUTER_READY) {
            LOGI("REBOOT - CPC MODEL: %u\n", val);
            change_model(val);
         }
      }
   }

   var.key = "cap32_ram";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      char str[100];
      int val;
      snprintf(str, sizeof(str), "%s", var.value);
      val = strtoul(str, NULL, 0);
      if (retro_computer_cfg.ram != val) {
         retro_computer_cfg.ram = val;
         if(emu_status & COMPUTER_READY) {
            LOGI("REBOOT - CPC RAM: %u\n", val);
            change_ram(val);
         }
      }
   }

   var.key = "cap32_floppy_sound";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "disabled") == 0)
          retro_computer_cfg.floppy_snd = 0;
      else
         retro_computer_cfg.floppy_snd = 1;
   }

   var.key = "cap32_statusbar";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "onloading") == 0)
      {
         if (retro_computer_cfg.statusbar != STATUSBAR_HIDE)
         {
            retro_computer_cfg.statusbar = STATUSBAR_HIDE;
            retro_show_statusbar();
         }
      }
      else if (strcmp(var.value, "enabled") == 0)
      {
         retro_computer_cfg.statusbar = STATUSBAR_SHOW;
         retro_show_statusbar();
      }
      else if (strcmp(var.value, "disabled") == 0)
      {
         retro_computer_cfg.statusbar = STATUSBAR_OFF;
         retro_ui_set_status(UI_STATUSBAR, false);
      }
   }

   var.key = "cap32_scr_tube";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if(emu_status & COMPUTER_READY) {
         if (strcmp(var.value, "color") == 0){
            CPC.scr_tube = CPC_MONITOR_COLOR;
            video_set_palette();
         }
      else if (strcmp(var.value, "green") == 0){
            CPC.scr_tube = CPC_MONITOR_GREEN;
            video_set_palette();
         }
         else if (strcmp(var.value, "white") == 0){
            CPC.scr_tube = CPC_MONITOR_WHITE;
            video_set_palette();
         }
      }
   }

   var.key = "cap32_advanced_green_phosphor";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      char str[100];
      int val;
      snprintf(str, sizeof(str), "%s", var.value);
      val = strtoul(str, NULL, 0);

      if(emu_status & COMPUTER_READY) {
         CPC.scr_phosphor_intensity = val;
         video_set_palette();
      }
   }

   var.key = "cap32_scr_intensity";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      char str[100];
      int val;
      snprintf(str, sizeof(str), "%s", var.value);
      val = strtoul(str, NULL, 0);

      if(emu_status & COMPUTER_READY) {
         CPC.scr_intensity = val;
         video_set_palette();
      }
   }

   var.key = "cap32_gfx_colors";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!(emu_status & COMPUTER_READY))
      {
         #ifdef M8BPP
         video_setup(DEPTH_8BPP);
         #else
         if (strcmp(var.value, "24bit") == 0)
         {
            video_setup(DEPTH_24BPP);
         }
         else {
            video_setup(DEPTH_16BPP);
         }
         #endif
      }
   }

   var.key = "cap32_keyboard_transparency";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!(emu_status & COMPUTER_READY))
      {
         if (strcmp(var.value, "enabled") == 0)
         {
            retro_video.draw_keyboard_func = draw_image_linear_blend;
         } else {
            retro_video.draw_keyboard_func = draw_image_linear;
         }
      }
   }

   var.key = "cap32_lang_layout";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      int val = 0; // DEFAULT ENGLISH
      if (strcmp(var.value, "french") == 0) val=1;
      else if (strcmp(var.value, "spanish") == 0) val=2;

      if (retro_computer_cfg.lang != val) {
         retro_computer_cfg.lang = val;
         if(emu_status & COMPUTER_READY) {
            change_lang(val);
            LOGI("REBOOT - CPC LANG: %u (%x)\n", val, emu_status);
         }
      }
   }

   // check if emulation need a restart (model/lang/... is changed)
   if(retro_computer_cfg.is_dirty)
   {
      emu_reconfigure();
      retro_ui_update_text();
      computer_reset();
   }
}

void Emu_init()
{
   audio_buffer = (int32_t*) retro_malloc(retro_getAudioBuffer());
   if(!audio_buffer){
      LOGI("emu init - audio error: when allocation mem...\n");
      return;
   }

   #ifdef M8BPP
   video_retro_palette_prepare();
   #endif

   emu_status = COMPUTER_BOOTING;
   pre_main(retro_content_filepath);
}

void Emu_uninit()
{
   if(audio_buffer)
      retro_free(audio_buffer);

   audio_buffer = NULL;
   //quit_cap32_emu();
}

void retro_shutdown_core(void)
{
   LOGD("SHUTDOWN\n");

   //quit_cap32_emu();

   environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
}

void retro_reset(void)
{
   emu_reset();
   computer_reset();
}

//*****************************************************************************
//*****************************************************************************
// Disk control

static int get_image_unit()
{
   int unit = dc->unit;
   if (dc->index < dc->count)
   {
      if (dc_get_image_type(dc->files[dc->index]) == DC_IMAGE_TYPE_TAPE)
         dc->unit = DC_IMAGE_TYPE_TAPE;
      else if (dc_get_image_type(dc->files[dc->index]) == DC_IMAGE_TYPE_FLOPPY)
         dc->unit = DC_IMAGE_TYPE_FLOPPY;
      else
         dc->unit = DC_IMAGE_TYPE_FLOPPY;
   }
   else
      unit = DC_IMAGE_TYPE_FLOPPY;

   return unit;
}

static void retro_insert_image()
{
   if(dc->unit == DC_IMAGE_TYPE_TAPE)
   {
      int error = tape_insert ((char *) dc->files[dc->index]);
      if (!error)
      {
         strcpy(loader_buffer, LOADER_TAPE_STR);
         ev_autorun_prepare(loader_buffer);
         LOGI("[retro_insert_image] Tape (%d) inserted: %s\n", dc->index+1, dc->names[dc->index]);
         retro_computer_cfg.slot = SLOT_TAP;
      }
      else
      {
         LOGI("[retro_insert_image] Tape Error (%d): %s\n", error, dc->files[dc->index]);
      }
   }
   else if(dc->unit == DC_IMAGE_TYPE_FLOPPY)
   {
      int error = attach_disk((char *)dc->files[dc->index],0);
      if (error)
      {
         retro_message("Error Loading DSK...");
         LOGI("[retro_insert_image] Disk (%d) Error : %s\n", dc->index+1, dc->files[dc->index]);
         return;
      }
      LOGI("[retro_insert_image] Disk (%d) inserted into drive A : %s\n", dc->index+1, dc->files[dc->index]);
      retro_computer_cfg.slot = SLOT_DSK;
   }
   else
   {
      LOGE("[retro_insert_image] unsupported image-type : %u\n", dc->unit);
   }
}

static bool retro_set_eject_state(bool ejected)
{
   if (dc)
   {
      int unit = get_image_unit();

      if (dc->eject_state == ejected)
         return true;
      
      if (ejected && dc->index <= dc->count && dc->files[dc->index] != NULL)
      {
         if (unit == DC_IMAGE_TYPE_TAPE)
         {
            tape_eject();
            LOGI("[retro_set_eject_state] Tape (%d/%d) ejected %s\n", dc->index+1, dc->count, dc->names[dc->index]);
         }
         else
         {
            detach_disk(0);
            LOGI("[retro_set_eject_state] Disk (%d/%d) ejected: %s\n", dc->index+1, dc->count, dc->names[dc->index]);
         }
      }
      else if (!ejected && dc->index < dc->count && dc->files[dc->index] != NULL)
      {
         retro_insert_image();
      }

      dc->eject_state = ejected;
      return true;
   }

   return false;
}

/* Gets current eject state. The initial state is 'not ejected'. */
static bool retro_get_eject_state(void)
{
   if (dc)
      return dc->eject_state;

   return true;
}

static unsigned retro_get_image_index(void)
{
   if (dc)
      return dc->index;

   return 0;
}

/* Sets image index. Can only be called when disk is ejected.
 * The implementation supports setting "no disk" by using an
 * index >= get_num_images().
 */
static bool retro_set_image_index(unsigned index)
{
   // Insert image
   if (dc)
   {
      if (index == dc->index)
         return true;

      if (dc->replace)
      {
         dc->replace = false;
         index = 0;
      }

      if (index < dc->count && dc->files[index])
      {
         dc->index = index;
         int unit = get_image_unit();
         LOGI("[retro_set_image_index] Unit (%d) image (%d/%d) inserted: %s\n", dc->index+1, unit,  dc->count, dc->files[dc->index]);
         return true;
      }
   }

   return false;
}

static unsigned retro_get_num_images(void)
{
   if (dc)
      return dc->count;

   return 0;
}

static bool retro_replace_image_index(unsigned index, const struct retro_game_info *info)
{
if (dc)
    {
        if (info != NULL)
        {
            dc_replace_file(dc, index, info->path);
        }
        else
        {
            dc_remove_file(dc, index);
        }
        return true;
    }

    return false;	
    
}

/* Adds a new valid index (get_num_images()) to the internal disk list.
 * This will increment subsequent return values from get_num_images() by 1.
 * This image index cannot be used until a disk image has been set
 * with replace_image_index. */
static bool retro_add_image_index(void)
{
   if (dc)
   {
      if (dc->count <= DC_MAX_SIZE)
      {
         dc->files[dc->count] = NULL;
         dc->names[dc->count] = NULL;
         dc->types[dc->count] = DC_IMAGE_TYPE_NONE;
         dc->count++;
         return true;
      }
   }

   return false;
}

static bool retro_get_image_path(unsigned index, char *path, size_t len)
{
   if (len < 1)
      return false;

   if (dc)
   {
      if (index < dc->count)
      {
         if (!string_is_empty(dc->files[index]))
         {
            strlcpy(path, dc->files[index], len);
            return true;
         }
      }
   }

   return false;
}

static bool retro_get_image_label(unsigned index, char *label, size_t len)
{
   if (len < 1)
      return false;

   if (dc)
   {
      if (index < dc->count)
      {
         if (!string_is_empty(dc->names[index]))
         {
            strlcpy(label, dc->names[index], len);
            return true;
         }
      }
   }

   return false;
}

static struct retro_disk_control_callback disk_interface = {
   retro_set_eject_state,
   retro_get_eject_state,
   retro_get_image_index,
   retro_set_image_index,
   retro_get_num_images,
   retro_replace_image_index,
   retro_add_image_index,
};

static struct retro_disk_control_ext_callback diskControlExt = {
   retro_set_eject_state,
   retro_get_eject_state,
   retro_get_image_index,
   retro_set_image_index,
   retro_get_num_images,
   retro_replace_image_index,
   retro_add_image_index,
   NULL, // set_initial_image
   retro_get_image_path,
   retro_get_image_label,
};

//*****************************************************************************
//*****************************************************************************
// Init
static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
}

void computer_set_model(int model)
{

   if (CPC.model == model)
      return;

   LOGI("[computer_set_model] model [%i => %i]\n", CPC.model, model);

   CPC.model = model;
   retro_computer_cfg.model = model;

   emu_reconfigure();
   retro_ui_update_text();
   computer_reset();
}

void computer_set_ram(int size)
{
   if (CPC.ram_size == size)
      return;

   LOGI("[computer_set_ram] RAM [%i => %i]\n", CPC.ram_size, size);

   change_ram(size);
   retro_computer_cfg.ram = CPC.ram_size;

   emu_reconfigure();
   retro_ui_update_text();
   computer_reset();
}

void check_flags(const char *filename, unsigned int size)
{
   if (file_check_flag(filename, size, FLAG_BIOS_664, 5))
   {
      computer_set_model(1);
   }

   if (file_check_flag(filename, size, FLAG_BIOS_B10, 10))
   {
      if (retro_computer_cfg.slot == SLOT_DSK)
         computer_set_model(1);
      else
         computer_set_model(0);
   }

   // model 464 using disk => 664
   if (CPC.model == CPC_MODEL_464 && retro_computer_cfg.slot == SLOT_DSK)
   {
      computer_set_model(1);
   }

   if (file_check_flag(filename, size, FLAG_BIOS_CPM, 5))
   {
      game_configuration.is_cpm = true;
   }

   if (file_check_flag(filename, size, FLAG_MEMORY_576, 6))
   {
      computer_set_ram(576);
   }

   if (file_check_flag(filename, size, FLAG_MEMORY_128, 6))
   {
      computer_set_ram(128);
   }

   if (file_check_flag(filename, size, FLAG_MEMORY_064, 6))
   {
      computer_set_ram(64);
   }
}

void computer_autoload()
{
   if (game_configuration.has_btn && retro_computer_cfg.use_internal_remap)
   {
      LOGI("[computer_autoload][DB] game remap applied.\n");
      memcpy(btnPAD[0].buttons, game_configuration.btn_config.buttons, sizeof(t_button_cfg));
   }

   if (!retro_computer_cfg.autorun)
      return;

   if (game_configuration.has_command)
   {
      strncpy(loader_buffer, game_configuration.loader_command, LOADER_MAX_SIZE);
   } else {
      loader_run(loader_buffer);
   }

   strcat(loader_buffer, "\n");
   LOGI("[computer_autoload] DSK autorun: %s", loader_buffer);
   if (game_configuration.is_cpm)
   {
      cpm_boot(loader_buffer);
   }
   else
   {
      ev_autorun_prepare(loader_buffer);
   }
}

void computer_reset()
{
   retro_ui_draw_db();

   if (!retro_computer_cfg.autorun)
      return;

   LOGI("[computer_reset] DSK autorun: \"%s\"\n", loader_buffer);
   ev_autorun_prepare(loader_buffer);
}

// load bios content
void computer_load_bios() {
   // TODO add load customs bios

   // cart is like a system bios
   if(file_check_extension(retro_content_filepath, sizeof(retro_content_filepath), EXT_FILE_CPR, 3))
   {
      int result = cart_start(retro_content_filepath);
      if(result != 0) {
         retro_message("[computer_load_bios] Error Loading Cart...");
      }
   }
}

void computer_hash_file(char* filepath)
{
   uint32_t hash = get_hash(filepath);
   if (!hash)
   {
      return;
   }

   // check clean-cpc-db hash
   game_configuration.is_fail = db_fail(hash);
   if(!game_configuration.is_fail)
   {
      // cleaned games appear with GREEN DSK
      game_configuration.is_clean = db_clean(hash);
      db_info(hash);
   }
   else
   {
      // warn user is a unsupported ROM
      retro_message("[computer_hash_file] ROM marked as NOT WORKING.");
   }

   LOGI("[computer_hash_file][DB] >>> file hash: 0x%x [ b=%u, l=%u, f=%u, c=%u ]\n",
      hash,
      game_configuration.has_btn,
      game_configuration.has_command,
      game_configuration.is_fail,
      game_configuration.is_clean
   );
}

// load content
void computer_load_file() {
   // check custom filename config
   check_flags(retro_content_filepath, sizeof(retro_content_filepath));

   // If it's a snapshot
   if(retro_computer_cfg.slot == SLOT_SNA)
   {
      int error = snapshot_load (retro_content_filepath);
      if (!error) {
         LOGI("[computer_load_file] SNA loaded: %s\n", (char *)retro_content_filepath);
         
      } else {
         LOGE("[computer_load_file] SNA Error (%d): %s", error, (char *)retro_content_filepath);
      }

      return;
   }

   // If it's a m3u file
   if(retro_computer_cfg.slot == SLOT_M3U)
   {
      // Parse the m3u file
      dc_parse_m3u(dc, retro_content_filepath);

      // Some debugging
      LOGI("[computer_load_file] m3u file parsed, %d file(s) found\n", dc->count);
      for(int i = 0; i < dc->count; i++)
      {
         LOGI("file %d: %s\n", i+1, dc->files[i]);
      }

      // Init first image
      dc->eject_state = false;
      dc->index = 0;
      computer_hash_file((char *)dc->files[dc->index]);
      retro_insert_image();

      // If command was specified
      if(dc->command)
      {
         // Execute the command
         LOGI("[computer_load_file] Executing the specified command: %s\n", dc->command);
         snprintf(loader_buffer, LOADER_MAX_SIZE - 2, "%s\n", dc->command);
         ev_autorun_prepare(loader_buffer);
      }
      else if (dc->unit == DC_IMAGE_TYPE_FLOPPY)
      {
         // Autoplay
         computer_autoload();
      }
   }

   // If it's a disk
   if(retro_computer_cfg.slot == SLOT_DSK)
   {
      // Add the file to disk control context
      // Maybe, in a later version of retroarch, we could add disk on the fly (didn't find how to do this)
      dc_add_file(dc, retro_content_filepath);

      // Init first disk
      dc->index = 0;
      dc->eject_state = false;
      computer_hash_file((char *)dc->files[dc->index]);

      LOGI("[computer_load_file] Disk (%d) inserted into drive A : %s\n", dc->index+1, dc->files[dc->index]);
      int error = attach_disk((char *)dc->files[dc->index],0);
      if (error) {
         retro_message("[computer_load_file] Error Loading DSK...");
         LOGE("[computer_load_file] DSK Error (%d): %s\n", error, (char *)retro_content_filepath);
      } else {
         computer_autoload();
      }
   }

   // If it's a tape
   if(retro_computer_cfg.slot == SLOT_TAP)
   {
      int error = tape_insert ((char *)retro_content_filepath);
      if (!error) {
         computer_hash_file((char *)retro_content_filepath);
         strcpy(loader_buffer, LOADER_TAPE_STR);
         ev_autorun_prepare(loader_buffer);
         LOGI("[computer_load_file] Tape inserted: %s\n", (char *)retro_content_filepath);
      } else {
         LOGE("[computer_load_file] Tape Error (%d): %s\n", error, (char *)retro_content_filepath);
      }
   }

   // Prepare SNA
   strncat(retro_content_filepath, "0.SNA", sizeof(retro_content_filepath) - 1);
}

void retro_init(void)
{
   struct retro_log_callback log;
   const char *system_dir = NULL;
   dc = dc_create();
   bool achievements = true;

   // Init log
   if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      log_cb = log.log;
   else
      log_cb = fallback_log;

   environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS, &achievements);

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir) && system_dir)
   {
      // if defined, use the system directory
      retro_system_directory=system_dir;
   }

   const char *content_dir = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_CONTENT_DIRECTORY, &content_dir) && content_dir)
   {
      // if defined, use the system directory
      retro_content_directory=content_dir;
   }

   const char *save_dir = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save_dir) && save_dir)
   {
      // If save directory is defined use it, otherwise use system directory
      retro_save_directory = *save_dir ? save_dir : retro_system_directory;
   }
   else
   {
      // make retro_save_directory the same in case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY is not implemented by the frontend
      retro_save_directory=retro_system_directory;
   }

   if(retro_system_directory == NULL)
   {
      strcpy(retro_system_bios_directory, ".");
   }
   else
   {
      strncpy(
         retro_system_bios_directory,
         retro_system_directory,
         sizeof(retro_system_bios_directory) - 1
      );
   }

   // TODO: future use to load custom bios
   path_join(
      retro_system_data_directory,
      retro_system_bios_directory,
      "data"
   );

   LOGI("Retro SYSTEM_DIRECTORY %s\n", retro_system_directory);
   LOGI("Retro SAVE_DIRECTORY %s\n", retro_save_directory);
   LOGI("Retro CONTENT_DIRECTORY %s\n", retro_content_directory);

   // events initialize - joy and keyboard
   ev_init();

   // Disk control interface
   unsigned dci_version = 0;
   if (environ_cb(RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION, &dci_version) && (dci_version >= 1))
      environ_cb(RETRO_ENVIRONMENT_SET_DISK_CONTROL_EXT_INTERFACE, &diskControlExt);
   else
      environ_cb(RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE, &disk_interface);

   memset(&game_configuration, 0, sizeof(game_cfg_t));

   // prepare shared variables
   retro_computer_cfg.model = -1;
   retro_computer_cfg.ram = -1;
   retro_computer_cfg.lang = -1;
   retro_computer_cfg.padcfg[ID_PLAYER1] = 0;
   retro_computer_cfg.padcfg[ID_PLAYER2] = 1;
   retro_computer_cfg.statusbar = STATUSBAR_HIDE;
   retro_computer_cfg.use_internal_remap = false;

   update_variables();

   #ifdef LOWRES
   retro_scr_style = 3;
   #else
   retro_scr_style = 4;
   #endif
   gfx_buffer_size = EMULATION_SCREEN_WIDTH * EMULATION_SCREEN_HEIGHT * retro_video.pitch;

   fprintf(stderr, "[libretro-cap32]: Got size: %u x %u (s%d rs%d).\n",
         EMULATION_SCREEN_WIDTH, EMULATION_SCREEN_HEIGHT, retro_scr_style, gfx_buffer_size);

   video_buffer = (uint32_t *) retro_malloc(gfx_buffer_size * PIXEL_DEPTH_DEFAULT_SIZE);
   temp_buffer = (uint32_t *) retro_malloc(WINDOW_MAX_SIZE * PIXEL_DEPTH_DEFAULT_SIZE);
   cpc_video_out = retro_malloc(gfx_buffer_size * PIXEL_DEPTH_DEFAULT_SIZE);

   memset(video_buffer, 0, gfx_buffer_size);
   memset(temp_buffer, 0, WINDOW_MAX_SIZE * PIXEL_DEPTH_DEFAULT_SIZE); // buffer UI
   memset(cpc_video_out, 0, gfx_buffer_size);

   retro_ui_init();

   Emu_init();

   if(!init_retro_snd((int16_t*) pbSndBuffer, audio_buffer_size))
      LOGI("AUDIO FORMAT is not supported.\n");
}

void retro_deinit(void)
{
   free_retro_snd();
   Emu_uninit();
   retro_ui_free();

   UnInitOSGLU();

   // Clean the m3u storage
   if(dc)
      dc_free(dc);

   retro_free(video_buffer);
   retro_free(cpc_video_out);
   retro_free(temp_buffer);

   LOGI("Retro DeInit\n");
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

// FIXME: why evercade do not call retro_set_controller_port_device?
//       atm, just log when this function is called with more detail
void retro_set_controller_port_device( unsigned port, unsigned device )
{
   if ( port > 1 )
      return;

   switch (device)
   {
      case RETRO_DEVICE_AMSTRAD_LIGHTGUN:
         lightgun_prepare(lightgun_cfg.guntype);
         amstrad_devices[port] = RETRO_DEVICE_AMSTRAD_LIGHTGUN;
         break;

      default:
         // please do not deinit the lightgun config
         if (!lightgun_cfg.gunconfigured)
         {
            lightgun_prepare(LIGHTGUN_TYPE_NONE);
         }
         amstrad_devices[port] = device;
         break;
   }

   LOGI("retro_set_controller_port_device: (%d)=%d\n", port, device);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "cap32";
   #ifndef GIT_VERSION
   #define GIT_VERSION ""
   #endif
   info->library_version  = VERSION_STRING GIT_VERSION SCREENMODE_STR;
   info->valid_extensions = "dsk|sna|zip|tap|cdt|voc|cpr|m3u";
   info->need_fullpath    = true;
   info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   info->geometry.base_width = CPC_SCREEN_WIDTH;
   info->geometry.base_height = CPC_SCREEN_HEIGHT;
   info->geometry.max_width = TEX_MAX_WIDTH;
   info->geometry.max_height = TEX_MAX_HEIGHT;
   info->geometry.aspect_ratio = 24.0 / 17.0;

   info->timing.fps = CPC_TIMING;
   info->timing.sample_rate = 44100.0;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_audio_mix_sample(int16_t left, int16_t right)
{
   if(retro_computer_cfg.floppy_snd)
      retro_snd_mixer_sample(&left, &right);

   audio_cb(left, right);
}

void retro_audio_mix_batch()
{
   if(retro_computer_cfg.floppy_snd)
      retro_snd_mixer_batch();

   memcpy(audio_buffer, pbSndBuffer, audio_buffer_size);
   audio_batch_cb((int16_t*) audio_buffer, audio_buffer_size / AUDIO_BYTES / AUDIO_CHANNELS);
}

void retro_PollEvent()
{
   input_poll_cb(); // retroarch get keys
   lightgun_cfg.gun_update(); // update lightguns
   process_events();
}

__attribute__((optimize("unroll-loops"))) static inline void blit_8bpp(int size){
   uint8_t *src_row = (uint8_t *) video_buffer;
   uint16_t *dest_row =  (uint16_t *) cpc_video_out;

   while(size--)
   {
      //uint8_t color = *(src_row++);
      *(dest_row++) = retro_palette[*(src_row++)];
   }
}

void retro_run(void)
{
   bool updated = false;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
   {
      update_variables();
      retro_message("[retro_run] options updated, changes applied!");
   }

   retro_loop();

   retro_PollEvent();
   retro_ui_process();
   lightgun_cfg.gun_draw();

   #ifdef M8BPP
      blit_8bpp(EMULATION_SCREEN_WIDTH * EMULATION_SCREEN_HEIGHT);
      video_cb(cpc_video_out, EMULATION_SCREEN_WIDTH, EMULATION_SCREEN_HEIGHT, EMULATION_SCREEN_WIDTH << retro_video.bytes);
   #else
      video_cb(video_buffer, EMULATION_SCREEN_WIDTH, EMULATION_SCREEN_HEIGHT, EMULATION_SCREEN_WIDTH << retro_video.bytes);
   #endif
}

bool retro_load_game(const struct retro_game_info *game)
{
   // notify the frontend of the retro_pixel_format we want use.
   enum retro_pixel_format fmt = retro_video.fmt;
   //enum retro_pixel_format rgb565;

   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      LOGI("[retro_load_game] PIXEL FORMAT is not supported (%u).\n", fmt);
      return false;
   }

   LOGI("[retro_load_game] PIXEL FORMAT selected (%u).\n", fmt);

   if (game) {
      strcpy(retro_content_filepath, (const char *) game->path);
   } else {
      retro_content_filepath[0]='\0';
   }

   update_variables();

   // get file type
   if(file_check_extension(retro_content_filepath, sizeof(retro_content_filepath), EXT_FILE_SNA, 3)) {
      retro_computer_cfg.slot = SLOT_SNA;
   } else if(file_check_extension(retro_content_filepath, sizeof(retro_content_filepath), EXT_FILE_M3U, 3)) {
      retro_computer_cfg.slot = SLOT_M3U;
   } else if(file_check_extension(retro_content_filepath, sizeof(retro_content_filepath), EXT_FILE_DSK, 3)) {
      retro_computer_cfg.slot = SLOT_DSK;
   } else if(file_check_extension(retro_content_filepath, sizeof(retro_content_filepath), EXT_FILE_CDT, 3)) {
      retro_computer_cfg.slot = SLOT_TAP;
   } else {
      retro_computer_cfg.slot = SLOT_UNK;
   }
   LOGI("[retro_load_game] detected file type, using slot [%i]\n", retro_computer_cfg.slot);

   computer_load_bios();
   computer_load_file();
   retro_ui_draw_db();

   return true;
}

void retro_unload_game(void){}

unsigned retro_get_region(void)
{
   return RETRO_REGION_PAL;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   return false;
}

size_t retro_serialize_size(void)
{
   int dwSnapSize = sizeof(t_SNA_header);
   dwSnapSize += get_ram_size();
   return dwSnapSize;
}

bool retro_serialize(void *data, size_t size)
{
   int error;
   error = snapshot_save_mem((uint8_t *) data, size);
   if(!error)
      return true;

   LOGI("[retro_serialize] SNA-serialized: error %d\n", error);
   return false;
}

bool retro_unserialize(const void *data, size_t size)
{
   return !snapshot_load_mem((uint8_t *) data, size);
}

void *retro_get_memory_data(unsigned id)
{
   switch ( id & RETRO_MEMORY_MASK )
   {
      case RETRO_MEMORY_SYSTEM_RAM:
         return get_ram_ptr();
   }

   /* not supported */
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   switch ( id & RETRO_MEMORY_MASK )
   {
      case RETRO_MEMORY_SYSTEM_RAM:
         return get_ram_size();
   }

   /* not supported */
   return 0;
}

void retro_cheat_reset(void) {}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}
