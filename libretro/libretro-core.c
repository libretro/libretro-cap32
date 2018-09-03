#include "libretro.h"
#include "libretro-core.h"

//#define RENDER16B
//#undef RENDER16B
#ifdef RENDER16B
#define M16B
#endif

//CORE VAR
#ifdef _WIN32
char slash = '\\';
#else
char slash = '/';
#endif

char RETRO_DIR[512];

char DISKA_NAME[512]="\0";
char DISKB_NAME[512]="\0";
char TAPE_NAME[512]="\0";

//TIME
#ifdef __CELLOS_LV2__
#include "sys/sys_time.h"
#include "sys/timer.h"
#define usleep  sys_timer_usleep
#else
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#endif

// DISK CONTROL
#include "retro_disk_control.h"
static dc_storage* dc;

// LOG
retro_log_printf_t log_cb;

extern int SHOWKEY;
extern int Core_PollEvent(void);

extern int retro_disk_auto();
extern void change_model(int val);
extern int snapshot_load (char *pchFileName);
extern int attach_disk(char *arv, int drive);
extern int detach_disk(int drive);
extern int tape_insert (char *pchFileName);
extern void enter_gui(void);
extern void kbd_buf_feed(char *s);
extern void kbd_buf_update();
extern int Retro_PollEvent();
extern void retro_loop(void);
extern int video_set_palette (void);
extern int InitOSGLU(void);
extern int  UnInitOSGLU(void);
extern void emu_reset(void);
extern void change_ram(int val);
extern int snapshot_save (char *pchFileName);
extern void play_tape();
extern void retro_joy0(unsigned char joy0);
extern void retro_key_down(int key);
extern void retro_key_up(int key);
extern void Screen_SetFullUpdate(int scr);

//VIDEO
unsigned int *Retro_Screen;
unsigned int save_Screen[WINDOW_SIZE];
unsigned int bmp[WINDOW_SIZE];

//SOUND
short signed int SNDBUF[1024*2];
int snd_sampler = 44100 / 50;

//PATH
char RPATH[512];
bool retro_load_ok = false;
int pauseg=0; //enter_gui

extern unsigned int *RetroScreen;
extern int app_init(void);
extern int app_free(void);
extern int app_render(int poll);

int CROP_WIDTH;
int CROP_HEIGHT;
int VIRTUAL_WIDTH ;
int retrow=WINDOW_WIDTH; 
int retroh=WINDOW_HEIGHT;

#include "vkbd.i"

unsigned amstrad_devices[ 2 ];

int autorun=0;

int RETROJOY=0,RETROSTATUS=0,RETRODRVTYPE=0;
int retrojoy_init=0,retro_ui_finalized=0;

int cap32_statusbar=0;

//CAP32 DEF BEGIN
#include "cap32.h"
//#include "z80.h"
extern t_CPC CPC;
//CAP32 DEF END

extern void update_input(void);
extern void texture_init(void);
extern void texture_uninit(void);
extern void Emu_init();
extern void Emu_uninit();
extern void input_gui(void);

const char *retro_save_directory;
const char *retro_system_directory;
const char *retro_content_directory;
char retro_system_data_directory[512];;

/*static*/ retro_input_state_t input_state_cb;
/*static*/ retro_input_poll_t input_poll_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
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

   sprintf(XARGV[PARAMCOUNT++],"%s\0",option);
}

int pre_main(const char *argv)
{
   int i;
   bool Only1Arg;

   parse_cmdline(argv); 

   Only1Arg = (strcmp(ARGUV[0],"x64") == 0) ? 0 : 1;

   for (i = 0; i<64; i++)
      xargv_cmd[i] = NULL;


   if(Only1Arg)
   {  Add_Option("x64");

      if (strlen(RPATH) >= strlen("crt"))
         if(!strcasecmp(&RPATH[strlen(RPATH)-strlen("crt")], "crt"))
            Add_Option("-cartcrt");

      Add_Option(RPATH/*ARGUV[0]*/);
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

long GetTicks(void)
{ // in MSec
#ifndef _ANDROID_

#ifdef __CELLOS_LV2__

   //#warning "GetTick PS3\n"

   unsigned long	ticks_micro;
   uint64_t secs;
   uint64_t nsecs;

   sys_time_get_current_time(&secs, &nsecs);
   ticks_micro =  secs * 1000000UL + (nsecs / 1000);

   return ticks_micro;///1000;
#else
   struct timeval tv;
   gettimeofday (&tv, NULL);
   return (tv.tv_sec*1000000 + tv.tv_usec);///1000;

#endif

#else

   struct timespec now;
   clock_gettime(CLOCK_MONOTONIC, &now);
   return (now.tv_sec*1000000 + now.tv_nsec/1000);///1000;
#endif

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

void save_bkg()
{
	memcpy(save_Screen,Retro_Screen,PITCH*WINDOW_SIZE/*400*300*/);

}

void restore_bgk()
{
	memcpy(Retro_Screen,save_Screen,PITCH*WINDOW_SIZE/*400*300*/);
}

void texture_uninit(void)
{
}

void texture_init(void)
{
}

void Screen_SetFullUpdate(int scr)
{
   if(scr==0 ||scr>1)
      memset(&Retro_Screen, 0, sizeof(Retro_Screen));
   if(scr>0)
      memset(&bmp,0,sizeof(bmp));
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

  static const struct retro_controller_description p1_controllers[] = {
    { "Amstrad Joystick", RETRO_DEVICE_AMSTRAD_JOYSTICK },
    { "Amstrad Keyboard", RETRO_DEVICE_AMSTRAD_KEYBOARD },
  };
  static const struct retro_controller_description p2_controllers[] = {
    { "Amstrad Joystick", RETRO_DEVICE_AMSTRAD_JOYSTICK },
    { "Amstrad Keyboard", RETRO_DEVICE_AMSTRAD_KEYBOARD },
  };


  static const struct retro_controller_info ports[] = {
    { p1_controllers, 2  }, // port 1
    { p2_controllers, 2  }, // port 2
    { NULL, 0 }
  };

  cb( RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports );

   struct retro_variable variables[] = {

	  { 
		"cap32_autorun",
		"Autorun; disabled|enabled" ,
	  },
      {
         "cap32_resolution",
         "Internal resolution; 384x272|400x300",
      },
      {
         "cap32_Model",
         "Model:; 464|664|6128",
      },
      {
         "cap32_Ram",
         "Ram size:; 64|128|192|512|576",
      },
      {
         "cap32_Statusbar",
         "Status Bar; disabled|enabled",
      },
      {
         "cap32_Drive",
         "Drive:; 0|1",
      },
      {
         "cap32_scr_tube",
         "scr_tube; disabled|enabled",
      },
      {
         "cap32_scr_intensity",
         "scr_intensity; 5|6|7|8|9|10|11|12|13|14|15",
      },
#if 0

      {
         "cap32_scr_remanency",
         "scr_remanency; disabled|enabled",
      },
#endif
      {
         "cap32_RetroJoy",
         "Retro joy0; disabled|enabled",
      },

      { NULL, NULL },
   };

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
}

static void update_variables(void)
{

   struct retro_variable var;

   var.key = "cap32_autorun";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
     if (strcmp(var.value, "enabled") == 0)
			 autorun = 1;
   }

   var.key = "cap32_resolution";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      char *pch;
      char str[100];
      snprintf(str, sizeof(str), "%s", var.value);

      pch = strtok(str, "x");
      if (pch)
         retrow = strtoul(pch, NULL, 0);
      pch = strtok(NULL, "x");
      if (pch)
         retroh = strtoul(pch, NULL, 0);

	//FIXME remove force res
	retrow=WINDOW_WIDTH;
	retroh=WINDOW_HEIGHT;

      fprintf(stderr, "[libretro-cap32]: Got size: %u x %u.\n", retrow, retroh);

      CROP_WIDTH =retrow;
      CROP_HEIGHT= (retroh-80);
      VIRTUAL_WIDTH = retrow;
      texture_init();
      //reset_screen();
   }

   var.key = "cap32_Model";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      char str[100];
      int val;
      snprintf(str, sizeof(str), "%s", var.value);
      val = strtoul(str, NULL, 0);
      if(val==464)val=0;
      else if(val==664)val=1;
      else if(val==6128)val=2;
      if(retro_ui_finalized)
         change_model(val);

   }

   var.key = "cap32_Ram";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      char str[100];
      int val;
      snprintf(str, sizeof(str), "%s", var.value);
      val = strtoul(str, NULL, 0);

      if(retro_ui_finalized)
         change_ram(val);

   }

   var.key = "cap32_Statusbar";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
		if(retro_ui_finalized){
    		  if (strcmp(var.value, "enabled") == 0)
    		     cap32_statusbar=1;
    		  if (strcmp(var.value, "disabled") == 0)
    		     cap32_statusbar=0;
		}
		else {
				if (strcmp(var.value, "enabled") == 0)RETROSTATUS=1;
				if (strcmp(var.value, "disabled") == 0)RETROSTATUS=0;
		}
   }


   var.key = "cap32_Drive";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      char str[100];
      int val;
      snprintf(str, sizeof(str), "%s", var.value);
      val = strtoul(str, NULL, 0);

      if(retro_ui_finalized)
         ;//set_drive_type(8, val);
      else RETRODRVTYPE=val;
   }

   var.key = "cap32_scr_tube";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
		if(retro_ui_finalized){
      		if (strcmp(var.value, "enabled") == 0){
         		  CPC.scr_tube=1;video_set_palette();}
      		if (strcmp(var.value, "disabled") == 0){
         		 CPC.scr_tube=0;video_set_palette();}
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

      if(retro_ui_finalized){
         CPC.scr_intensity = val;
         video_set_palette();
      }	
   }

#if 0
   var.key = "cap32_scr_remanency";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
		if(retro_ui_finalized){
      		if (strcmp(var.value, "enabled") == 0){
         		  CPC.scr_remanency=1;video_set_palette();}//set_truedrive_emultion(1);
      		if (strcmp(var.value, "disabled") == 0){
         		 CPC.scr_remanency=0;video_set_palette();}//set_truedrive_emultion(0);
		}
   }
#endif

   var.key = "cap32_RetroJoy";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
		if(retrojoy_init){
		      if (strcmp(var.value, "enabled") == 0)
		         ;//resources_set_int( "RetroJoy", 1);
		      if (strcmp(var.value, "disabled") == 0)
		         ;//resources_set_int( "RetroJoy", 0);
		}
		else {
			if (strcmp(var.value, "enabled") == 0)RETROJOY=1;
			if (strcmp(var.value, "disabled") == 0)RETROJOY=0;
		}

   }

}


void Emu_init(){

   pre_main(RPATH);

   update_variables();
}

void Emu_uninit(){

	//quit_cap32_emu();

   texture_uninit();
}

void retro_shutdown_core(void)
{
   LOGI("SHUTDOWN\n");

	//quit_cap32_emu();

   texture_uninit();
   environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
}

void retro_reset(void){

	emu_reset();
}

//*****************************************************************************
//*****************************************************************************
// Disk control
static bool disk_set_eject_state(bool ejected)
{
	if (dc)
	{
		dc->eject_state = ejected;
		
		if(dc->eject_state)
			detach_disk(0);
		else
			attach_disk((char *)dc->files[dc->index],0);
	}
	
	return true;
}

static bool disk_get_eject_state(void)
{
	if (dc)
		return dc->eject_state;
	
	return true;
}

static unsigned disk_get_image_index(void)
{
	if (dc)
		return dc->index;
	
	return 0;
}

static bool disk_set_image_index(unsigned index)
{
	// Insert disk
	if (dc)
	{
		// Same disk...
		// This can mess things in the emu
		if(index == dc->index)
			return true;
		
		if ((index < dc->count) && (dc->files[index]))
		{
			dc->index = index;
			log_cb(RETRO_LOG_INFO, "Disk (%d) inserted into drive A : %s\n", dc->index+1, dc->files[dc->index]);
			return true;
		}
	}
	
	return false;
}

static unsigned disk_get_num_images(void)
{
	if (dc)
		return dc->count;

	return 0;
}

static bool disk_replace_image_index(unsigned index, const struct retro_game_info *info)
{
	// Not implemented
	// No many infos on this in the libretro doc...
	return false;
}

static bool disk_add_image_index(void)
{
	// Not implemented
	// No many infos on this in the libretro doc...
	return false;
}

static struct retro_disk_control_callback disk_interface = {
   disk_set_eject_state,
   disk_get_eject_state,
   disk_get_image_index,
   disk_set_image_index,
   disk_get_num_images,
   disk_replace_image_index,
   disk_add_image_index,
};

//*****************************************************************************
//*****************************************************************************
// Init
static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
}

void retro_init(void)
{    	
   struct retro_log_callback log;	
   const char *system_dir = NULL;
   dc = dc_create();

	// Init log
	if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
		log_cb = log.log;
	else
		log_cb = fallback_log;
	
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

   if(retro_system_directory==NULL)sprintf(RETRO_DIR, "%s\0",".");
   else sprintf(RETRO_DIR, "%s\0", retro_system_directory);

   sprintf(retro_system_data_directory, "%s/data\0",RETRO_DIR);

   LOGI("Retro SYSTEM_DIRECTORY %s\n",retro_system_directory);
   LOGI("Retro SAVE_DIRECTORY %s\n",retro_save_directory);
   LOGI("Retro CONTENT_DIRECTORY %s\n",retro_content_directory);

#ifndef RENDER16B
    	enum retro_pixel_format fmt =RETRO_PIXEL_FORMAT_XRGB8888;
#else
    	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
#endif
   
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      fprintf(stderr, "PIXEL FORMAT is not supported.\n");
LOGI("PIXEL FORMAT is not supported.\n");
      exit(0);
   }

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

	// Disk control interface
	environ_cb(RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE, &disk_interface);

	texture_init();

}

extern void main_exit();
void retro_deinit(void)
{
   app_free(); 
 
   Emu_uninit(); 

   UnInitOSGLU();

   // Clean the m3u storage
	if(dc)
	{
		dc_free(dc);
	}

   LOGI("Retro DeInit\n");
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}


void retro_set_controller_port_device( unsigned port, unsigned device )
{
   if ( port < 2 )
   {
      amstrad_devices[ port ] = device;

      printf(" (%d)=%d \n",port,device);
   }
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "cap32";
   info->library_version  = "4.2";
   info->valid_extensions = "dsk|sna|zip|tap|cdt|voc|m3u";
   info->need_fullpath    = true;
   info->block_extract = false;

}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   /* FIXME handle PAL/NTSC */
   struct retro_game_geometry geom = { retrow, retroh, 400, 300,4.0 / 3.0 };
   struct retro_system_timing timing = { 50.0, 44100.0 };

   info->geometry = geom;
   info->timing   = timing;
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

void retro_audio_cb( short l, short r)
{
	audio_cb(l,r);
}

void retro_audiocb(signed short int *sound_buffer,int sndbufsize){
   int x; 
   if(pauseg==0)for(x=0;x<sndbufsize;x++)audio_cb(sound_buffer[x],sound_buffer[x]);	
}

void retro_blit()
{
   memcpy(Retro_Screen,bmp,PITCH*WINDOW_SIZE);
}

#define CDT_FILE_EXT "cdt"
#define DSK_FILE_EXT "dsk"
#define M3U_FILE_EXT "m3u"
#define SNA_FILE_EXT "sna"

void retro_run(void)
{
   static int mfirst=1;
   bool updated = false;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      update_variables();

   if(mfirst==1)
   {
		mfirst++;
		printf("MAIN FIRST\n");
		retro_load_ok=true;

		Emu_init();

		// If it's a m3u file
		if (strlen(RPATH) >= strlen(M3U_FILE_EXT))
			if(!strcasecmp(&RPATH[strlen(RPATH)-strlen(M3U_FILE_EXT)], M3U_FILE_EXT))
			{
				// Parse the m3u file
				dc_parse_m3u(dc, RPATH);

				// Some debugging
				log_cb(RETRO_LOG_INFO, "m3u file parsed, %d file(s) found\n", dc->count);
				for(unsigned i = 0; i < dc->count; i++)
				{
					log_cb(RETRO_LOG_INFO, "file %d: %s\n", i+1, dc->files[i]);
				}
				
				// Init first disk
				dc->index = 0;
				dc->eject_state = false;
				printf("Disk (%d) inserted into drive A : %s\n", dc->index+1, dc->files[dc->index]);
				attach_disk((char *)dc->files[dc->index],0);

				// If command was specified
				if(dc->command)
				{
					// Execute the command
					log_cb(RETRO_LOG_INFO, "Executing the specified command: %s\n", dc->command);
					char* command = calloc(strlen(dc->command) + 1, sizeof(char));
					sprintf(command, "%s\n", dc->command);
					kbd_buf_feed(command);
					free(command);
				}						
				else
				{
					// Autoplay
					retro_disk_auto();
				}
				
				// Prepare SNA
				sprintf(RPATH,"%s%d.SNA",RPATH,0);

				return;
			}

		// If it's a disk
		if (strlen(RPATH) >= strlen(DSK_FILE_EXT))
			if(!strcasecmp(&RPATH[strlen(RPATH)-strlen(DSK_FILE_EXT)], DSK_FILE_EXT))
			{
				// Add the file to disk control context
				// Maybe, in a later version of retroarch, we could add disk on the fly (didn't find how to do this)
				dc_add_file(dc, RPATH);
				
				// Init first disk
				dc->index = 0;
				dc->eject_state = false;
				printf("Disk (%d) inserted into drive A : %s\n", dc->index+1, dc->files[dc->index]);
				attach_disk((char *)dc->files[dc->index],0);
				retro_disk_auto();

				// Prepare SNA
				sprintf(RPATH,"%s%d.SNA",RPATH,0);
				
				return;
			}
			
		// If it's a tape
		if (strlen(RPATH) >= strlen(CDT_FILE_EXT))
			if(!strcasecmp(&RPATH[strlen(RPATH)-strlen(CDT_FILE_EXT)], CDT_FILE_EXT))
			{
				tape_insert ((char *)RPATH);
				kbd_buf_feed("|tape\nrun\"\n^");
				return;

			}

		// If it's a snapshot
		if (strlen(RPATH) >= strlen(SNA_FILE_EXT))
			if(!strcasecmp(&RPATH[strlen(RPATH)-strlen(SNA_FILE_EXT)], SNA_FILE_EXT))
			{
				snapshot_load (RPATH);
				sprintf(RPATH,"%s",RPATH);
				return;
			}
		
		return;
   }

   if(pauseg==0)
   {
      	retro_loop();
	retro_blit();
	Core_PollEvent();
	if(SHOWKEY==1)app_render(0);
   }
   else if (pauseg==1)app_render(1);

   video_cb(Retro_Screen,retrow,retroh,retrow<<PIXEL_BYTES);

}

/*
unsigned int lastdown,lastup,lastchar;
static void keyboard_cb(bool down, unsigned keycode,
      uint32_t character, uint16_t mod)
{

  printf( "Down: %s, Code: %d, Char: %u, Mod: %u.\n",
         down ? "yes" : "no", keycode, character, mod);


if(down)lastdown=keycode;
else lastup=keycode;
lastchar=character;

}
*/

bool retro_load_game(const struct retro_game_info *info)
{
   const char *full_path;

   (void)info;

   if (!info)
      return false;

/*
   struct retro_keyboard_callback cb = { keyboard_cb };
   environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cb);
*/
   full_path = info->path;

   strcpy(RPATH,full_path);

   update_variables();

   app_init();

	memset(SNDBUF,0,1024*2*2);
/*
	Emu_init();

	if (strlen(RPATH) >= strlen("cdt"))
		if(!strcasecmp(&RPATH[strlen(RPATH)-strlen("cdt")], "cdt")){
			tape_insert ((char *)full_path);
      			kbd_buf_feed("|tape\nrun\"\n^");
   			return true;
		}
	loadadsk((char *)full_path,0);
*/
   return true;
}

void retro_unload_game(void){

   pauseg=-1;
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   (void)type;
   (void)info;
   (void)num;
   return false;
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data_, size_t size)
{
   return false;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return false;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void) {}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

