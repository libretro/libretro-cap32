#include "libretro.h"

#include "libretro-cap32.h"

#include "CPCkeymap.h"

extern void retro_key_down(int key);
extern void retro_key_up(int key);
extern int InitOSGLU(void);
extern int  UnInitOSGLU(void);
extern void retro_loop();

extern unsigned short int bmp[TEX_WIDTH * TEX_HEIGHT];
extern int SHOWKEY,pauseg,SHIFTON,CTRLON,snd_sampler;
extern short signed int SNDBUF[1024*2];
extern char RPATH[512];

extern void update_input(void);
extern void texture_init(void);

extern unsigned short * sndbuffer;
extern int sndbufsize;
signed short rsnd=0;

unsigned short zoom[MXZO*MYZO*2];
int ZOOM=-1;

MRES Mres[5]={ {256,192},{256,232},{272,232},{320,232}, {320,256}};

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
//static retro_input_poll_t input_poll_cb;
//static retro_input_state_t input_state_cb;

void retro_init(void)
{
	texture_init();
	InitOSGLU();
}

void retro_deinit(void)
{	 
	UnInitOSGLU();	
}

unsigned retro_api_version(void)
{
   	return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   	(void)port;
   	(void)device;
}

void retro_get_system_info(struct retro_system_info *info)
{
   	memset(info, 0, sizeof(*info));
   	info->library_name     = "CAPRICE32 4.2";
   	info->library_version  = "v1";
   	info->valid_extensions = "dsk|DSK|SNA|sna";
   	info->need_fullpath    = true;//false;
   	//info->valid_extensions = NULL; // Anything is fine, we don't care.
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   	struct retro_game_geometry geom = { 200, 150, TEX_WIDTH, TEX_HEIGHT,4.0 / 3.0 };
   	struct retro_system_timing timing = { 50.0, 44100.0 };
   
   	info->geometry = geom;
   	info->timing   = timing;
}
 
void retro_set_environment(retro_environment_t cb)
{
   	environ_cb = cb;
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
/*
void retro_set_input_poll(retro_input_poll_t cb)
{
   	input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   	input_state_cb = cb;
}
*/

void retro_reset(void){}

void retro_audio_cb( short l, short r){
	audio_cb(l,r);
}

void display_zoom(){

	int i;
	int XZO=Mres[ZOOM].x,YZO=Mres[ZOOM].y;

	unsigned short *ptr=&bmp[ (384-XZO)/2 +400*(272-YZO)/2];
	unsigned short *out=&zoom[0];

	for(i=0;i<YZO;i++){
		memcpy(out,ptr,XZO*2);
		ptr+=TEX_WIDTH;
		out+=XZO;
	}
	video_cb(zoom,XZO, YZO, XZO << 1);
}

void retro_run(void)
{
   	int x;

	if(pauseg==0){
		
		retro_loop();		
		update_input();
		//if(SND==1)mixsnd(); moved to cap32.cpp;
	}
	else enter_gui();

   	if(ZOOM==-1)video_cb(bmp,TEX_WIDTH-16, TEX_HEIGHT, TEX_WIDTH << 1);
	else display_zoom();

}
 
static void keyboard_cb(bool down, unsigned keycode, uint32_t character, uint16_t mod)
{
	 int retrok=keyboard_translation[keycode];

  	// printf( "Down: %s, Code: %d, Char: %u, Mod: %u. ,(%d)\n",
  	//       down ? "yes" : "no", keycode, character, mod,retrok);

	if (keycode>=321);
	else{
		if(SHOWKEY==1)return;

		if(down && retrok==0x25){
										
			if(SHIFTON == 1)retro_key_up(retrok);
			else retro_key_down(retrok);
			SHIFTON=-SHIFTON;							
			
		}
		else if(down && retrok==0x27){
										
			if(CTRLON == 1)retro_key_up(retrok);
			else retro_key_down(retrok);
			CTRLON=-CTRLON;							
			
		}
		else {
			if(down && retrok!=-1)		
				retro_key_down(retrok);	
			else if(!down && retrok!=-1)
				retro_key_up(retrok);
		}
	}

}

bool retro_load_game(const struct retro_game_info *info)
{
    	const char *full_path;

    	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
    	if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
    	{
    		fprintf(stderr, "RGB565 is not supported.\n");
    		return false;
    	}

    	struct retro_keyboard_callback cb = { keyboard_cb };
    	environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cb);

    	(void)info;

    	full_path = info->path;

    	strcpy(RPATH,full_path);
        loadadsk((char *)full_path,0);


    	return true;
}

void retro_unload_game(void){}

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

