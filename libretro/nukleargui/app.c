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

extern long GetTicks(void);

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
char Core_Key_Sate[512];
char Core_old_Key_Sate[512];

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

int app_init(int width, int height)
{
    #ifdef M16B
    screen_surface=Retro_CreateRGBSurface16(width,height,16,0,0,0,0);
    #else
    screen_surface=Retro_CreateRGBSurface32(width,height,32,0,0,0,0);
    #endif

    Retro_Screen=(PIXEL_TYPE *)screen_surface->pixels;

    RSDL_font = (nk_retro_Font*)calloc(1, sizeof(nk_retro_Font));
    RSDL_font->width = 4;
    RSDL_font->height = 7;
    if (!RSDL_font)
        return -1;

    /* GUI */
    ctx = nk_retro_init(RSDL_font,screen_surface,width,height);

    /* style.c */
    /* THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK */
    set_style(ctx, THEME_DARK);

    /* icons */

    filebrowser_init();
    sprintf(LCONTENT,"%s\0",RPATH);

	memset(Core_Key_Sate,0,512);
	memset(Core_old_Key_Sate ,0, sizeof(Core_old_Key_Sate));

    printf("Init nuklear %ux%u\n", width, height);

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

   Retro_FreeSurface(screen_surface);
   if (screen_surface)
      free(screen_surface);
   screen_surface = NULL;

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

int Core_PollEvent()
{
   input_poll_cb(); // retroarch get keys

   // --- Player 1/2 Joystick code
   if(showkeyb < 0){
      ev_joysticks();
   } else {
      ev_vkeyboard();
   }

   // --- Keyboard code --
   // TODO: clean and change to callback
   //if(showkeyb==-1 && pauseg==0)Core_Processkey();

   int i = 0;
   static int kbt[4]={0,0,0,0};

   // F9 vkbd
   if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F9) && kbt[i]==0){
      kbt[i]=1;
   }
   else if ( kbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F9) ){
      kbt[i]=0;
      showkeyb=-showkeyb;
   }
   // F10 GUI
   i=1;
   if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F10) && kbt[i]==0){
      kbt[i]=1;
   }
   else if ( kbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F10) ){
      kbt[i]=0;
      pauseg=1;
      save_bkg();
      printf("enter gui!\n");
   }


   return 1;

}

/**
 * old functions - temporarily keep as reference
 *     trying to simplify and optimize previous implementation
 */
#if 0

// Core input Key(not GUI)
void Core_Processkey()
{
	int i;

	for(i=0;i<320;i++)
        	Core_Key_Sate[i]=input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,i) ? 0x80: 0;

	if(memcmp( Core_Key_Sate,Core_old_Key_Sate , sizeof(Core_Key_Sate) ) )
	 	for(i=0;i<320;i++)
			if(Core_Key_Sate[i] && Core_Key_Sate[i]!=Core_old_Key_Sate[i]  )
        	{
				if(i==RETROK_F12){
					//play_tape();
					continue;
				}

				if(i==RETROK_LALT){
					//KBMOD=-KBMOD;
					printf("Modifier alt pressed %d \n",KBMOD);
					continue;
				}
				//printf("press: %d \n",i);
				retro_key_down(i);

        	}
        	else if ( !Core_Key_Sate[i] && Core_Key_Sate[i]!=Core_old_Key_Sate[i]  )
        	{
				if(i==RETROK_F12){
      				kbd_buf_feed("|tape\nrun\"\n^");
					continue;
				}
/*
				if(i==RETROK_RCTRL){
					CTRLON=-CTRLON;
					printf("Modifier crtl released %d \n",CTRLON);
					continue;
				}
				if(i==RETROK_RSHIFT){
					SHIFTON=-SHIFTON;
					printf("Modifier shift released %d \n",SHIFTON);
					continue;
				}
*/
				if(i==RETROK_LALT){
					KBMOD=-KBMOD;
					printf("Modifier alt released %d \n",KBMOD);
					continue;
				}
				//printf("release: %d \n",i);
				retro_key_up(i);

        	}

	memcpy(Core_old_Key_Sate,Core_Key_Sate , sizeof(Core_Key_Sate) );

}

// Core input (not GUI)
int Core_PollEvent_old()
{
    //   RETRO        B    Y    SLT  STA  UP   DWN  LEFT RGT  A    X    L    R    L2   R2   L3   R3
    //   INDEX        0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
    //   AMSTRAD      RUN  VKB  M/J  RTRN UP   DWN  LEFT RGT  B1   B2   CAT  STAT RST  TAPE ?    ?

   int i,j;
   static int jbt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   static int vbt[16]={0x1C,0x39,0x01,0x3B,0x01,0x02,0x04,0x08,0x80,0x40,0x15,0x31,0x24,0x1F,0x6E,0x6F};
   static int kbt[4]={0,0,0,0};

   MXjoy[0]=0;
   MXjoy[1]=0;

   if(emu_status != COMPUTER_READY)
      return 1;

   input_poll_cb();

   int mouse_l;
   int mouse_r;
   int16_t mouse_x,mouse_y;
   mouse_x=mouse_y=0;

   if(showkeyb==-1 && pauseg==0)Core_Processkey();

/*
if(amstrad_devices[0]==RETRO_DEVICE_AMSTRAD_JOYSTICK){

    i=2;//mouse/joy toggle
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      retro_evt->MOUSE_EMULATED=-retro_evt->MOUSE_EMULATED;
   }

}
*/

if(pauseg==0){ // if emulation running

	//Joy mode - only when keyboard is not selected
   if(amstrad_devices[0]!=RETRO_DEVICE_AMSTRAD_KEYBOARD){
      for(j=0;j<2;j++) {
         for(i=4;i<10;i++) {
            if( input_state_cb(j, RETRO_DEVICE_JOYPAD, 0, i))
               MXjoy[j] |= vbt[i]; // Joy press
         }
         if(showkeyb==-1) {
		      if(j==0)retro_joy0(MXjoy[j]);
		      else if (j==1)retro_joy1(MXjoy[j]);
         }
      }
   }

if(amstrad_devices[0]==RETRO_DEVICE_AMSTRAD_JOYSTICK){
   //shortcut for joy mode only

   i=1;//show vkbd toggle
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) )
   {
      jbt[i]=0;
      showkeyb=-showkeyb;
   }

   i=3;//type ENTER
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) )
   {
      jbt[i]=0;
	  kbd_buf_feed("\n");
   }

   i=0;//type RUN"
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
	  kbd_buf_feed("RUN\"");
   }

   i=10;//Type CAT\n
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
	  kbd_buf_feed("CAT\n");
      //Screen_SetFullUpdate();
   }

   i=11;//show/hide statut
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      STATUTON=-STATUTON;
     // Screen_SetFullUpdate();
   }

   i=12;//reset
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      emu_reset();
   }

   i=13;//auto load tape
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      kbd_buf_feed("|tape\nrun\"\n^");
   }



}//if amstrad_devices=joy


}// if pauseg=0

return 1;

}
#endif
