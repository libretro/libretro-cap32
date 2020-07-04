/* nuklear - v1.00 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <libretro.h>
#include <libretro-core.h>

extern void filebrowser_init();
extern void filebrowser_free();

extern int snapshot_save (char *pchFileName);
extern void play_tape();
extern void Screen_SetFullUpdate(int scr);
extern void vkbd_key(int key,int pressed);

extern retro_input_poll_t input_poll_cb;
extern retro_input_state_t input_state_cb;

extern char RPATH[512];

//EMU FLAGS
int showkeyb=-1;
int SHIFTON=-1;
int KBMOD=-1;
int RSTOPON=-1;
int CTRLON=-1;
int NUMDRV=1;
int NPAGE=-1;
int KCOL=1;
int LOADCONTENT=-1;
int STATUTON=-1;
int LDRIVE=8;
int SND=1;
int vkey_pressed;
unsigned char MXjoy[2]; // joy
char LCONTENT[512];

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION

#define NK_RETRO_SOFT_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_retro_soft.h"
#include "retro_events.h"

static RSDL_Surface *screen_surface;
extern void restore_bgk();
extern void save_bkg();

/* macros */

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

/* Platform */

float bg[4];
struct nk_color background;
/* GUI */
struct nk_context *ctx;

static nk_retro_Font *RSDL_font;

#include "style.c"
#include "filebrowser.c"
#include "gui.i"

/* Forward declarations */
void app_vkb_handle();

void app_screen_init(int width, int height)
{
   #ifdef M16B
   screen_surface=Retro_CreateRGBSurface16(width,height,16,0,0,0,0);
   #else
   screen_surface=Retro_CreateRGBSurface32(width,height,32,0,0,0,0);
   #endif

   video_buffer=(PIXEL_TYPE *)screen_surface->pixels;

   /* GUI */
   ctx = nk_retro_init(RSDL_font,screen_surface,width,height);

   /* style.c */
   /* THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK */
   set_style(ctx, THEME_DARK);

   //printf("Init nuklear %ux%u\n", width, height);
}

void app_screen_free()
{
   Retro_FreeSurface(screen_surface);
   if (screen_surface)
      free(screen_surface);
   screen_surface = NULL;
}

int app_init()
{
    RSDL_font = (nk_retro_Font*)calloc(1, sizeof(nk_retro_Font));
    RSDL_font->width = 4;
    RSDL_font->height = 7;
    if (!RSDL_font)
        return -1;

    /* icons */
    filebrowser_init();
    sprintf(LCONTENT,"%s\0",RPATH);

 return 0;
}

int app_free()
{
   //FIXME: memory leak here
   if (RSDL_font)
      free(RSDL_font);
   RSDL_font = NULL;
   filebrowser_free();
   nk_retro_shutdown();

   return 0;
}

int app_event(int poll)
{
	int evt;

	nk_input_begin(ctx);
	nk_retro_handle_event(&evt,poll);
	nk_input_end(ctx);

	return 0;
}

int app_render(int poll)
{
	static int prevpoll=0,prevstate=0,reset_state=0;
	if(prevpoll!=poll || reset_state){nk_clear(ctx);reset_state=0;}

	if(poll==0)
		app_vkb_handle();
	else
		restore_bgk();

	app_event(poll);

    	int state=gui(&browser,ctx);
	if(state==1 && prevstate!=1)reset_state=1;

	nk_retro_render(nk_rgba(0,0,0,0));

	prevpoll=poll;
	prevstate=state;

    return 0;
}

void app_vkb_handle()
{
   static int oldi=-1;
   int i;

   if(oldi!=-1)
   {
      vkbd_key(oldi,0);
      oldi=-1;
   }

   if(vkey_pressed==-1)return;

	i=vkey_pressed;
	 vkey_pressed=-1;


         if(i==-1){
            oldi=-1;
		 }
         if(i==-2)
         {
            NPAGE=-NPAGE;oldi=-1;
         }
         else if(i==-3)
         {
            //KDB bgcolor
            KCOL=-KCOL;
            oldi=-1;
         }
         else if(i==-4)
         {
            //VKbd show/hide
            oldi=-1;
           // Screen_SetFullUpdate(0);
            showkeyb=-showkeyb;
         }
         else if(i==-5)
         {
			//FLIP DSK PORT 1-2
			NUMDRV=-NUMDRV;
            oldi=-1;
         }
         else if(i==-6)
         {
			//Exit
			retro_deinit();
			oldi=-1;
            exit(0);
         }
         else if(i==-7)
         {
			//SNA SAVE
			snapshot_save (RPATH);
            oldi=-1;
         }
         else if(i==-8)
         {
			//PLAY TAPE
			play_tape();
            oldi=-1;
         }

         else
         {

            if(i==0x25 /*i==-10*/) //SHIFT
            {

               SHIFTON=-SHIFTON;

               oldi=-1;
            }
            else if(i==0x27/*i==-11*/) //CTRL
            {

               CTRLON=-CTRLON;

               oldi=-1;
            }
			else if(i==-12) //RSTOP
            {

               RSTOPON=-RSTOPON;

               oldi=-1;
            }
			else if(i==-13) //GUI
            {
			    pauseg=1;

				showkeyb=-showkeyb;

				//Screen_SetFullUpdate(0);
               oldi=-1;
            }
			else if(i==-14) //JOY PORT TOGGLE
            {

               showkeyb=-showkeyb;
               oldi=-1;
            }
            else
            {
               oldi=i;
 	    	vkbd_key(oldi,1);
            }

         }


}

void Core_PollEvent()
{
   input_poll_cb(); // retroarch get keys

   // --- Player 1/2 Joystick code
   if(showkeyb < 0) {
      ev_joysticks();
   } else {
      ev_events();
   }

}
