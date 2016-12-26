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


extern int snapshot_save (char *pchFileName);
extern void play_tape();
extern void Screen_SetFullUpdate(int scr);
extern void vkbd_key(int key,int pressed);

extern long GetTicks(void); 

extern retro_input_poll_t input_poll_cb;
extern retro_input_state_t input_state_cb;
//extern retro_log_printf_t log_cb;
extern int retrow,retroh;
#define rwidth retrow
#define rheight retroh

extern char RPATH[512];

extern int MOUSE_EMULATED,MOUSEMODE,slowdown;
#define MOUSE_RELATIVE 10 //0 = absolute
extern int gmx,gmy; // mouse
int mouse_wu=0,mouse_wd=0;
int LSHIFTON=-1;
extern int STATUTON,bitstart,pushi,keydown,keyup,KBMOD,RSTOPON,CTRLON,NUMDRV;

//KEYBOARD
extern char Key_Sate[512];
extern char Key_Sate2[512];
static char old_Key_Sate[512];
extern int SHOWKEY,KCOL;
int LOADCONTENT=-1;
char LCONTENT[512];
int LDRIVE=8;
int vkey_pressed;

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION

#define NK_RETRO_SOFT_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_retro_soft.h"

// SDL surface (implementation from SDL_wrapper)
static RSDL_Surface *screen_surface;
extern unsigned int *Retro_Screen;
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

static nk_sdl_Font *RSDL_font;

#include "style.i"
#include "filebrowser.i"
#include "gui.i"

static const char *cross[] = {
  "X                               ",
  "XX                              ",
  "X.X                             ",
  "X..X                            ",
  "X...X                           ",
  "X....X                          ",
  "X.....X                         ",
  "X......X                        ",
  "X.......X                       ",
  "X........X                      ",
  "X.....XXXXX                     ",
  "X..X..X                         ",
  "X.X X..X                        ",
  "XX  X..X                        ",
  "X    X..X                       ",
  "     X..X                       ",
  "      X..X                      ",
  "      X..X                      ",
  "       XX                       ",
  "                                ",
};

void DrawPointBmp(unsigned int *buffer,int x, int y, unsigned int color)
{
   int idx;

   idx=x+y*rwidth;
   if(idx>=0 && idx<rwidth*rheight)
   	buffer[idx]=color;	
}

void draw_cross(int x,int y) {

	int i,j,idx;
	int dx=32,dy=20;
	unsigned  short color;

	for(j=y;j<y+dy;j++){
		idx=0;
		for(i=x;i<x+dx;i++){
			if(cross[j-y][idx]=='.')DrawPointBmp(Retro_Screen,i,j,0xffffffff);
			else if(cross[j-y][idx]=='X')DrawPointBmp(Retro_Screen,i,j,0);
			idx++;			
		}
	}
}

int app_init()
{

    screen_surface=Retro_CreateRGBSurface32(rwidth,rheight,32,0,0,0,0);

    Retro_Screen=(unsigned int *)screen_surface->pixels;

    RSDL_font = (nk_sdl_Font*)calloc(1, sizeof(nk_sdl_Font));
    RSDL_font->width = 8; /* Default in  the RSDL_gfx library */
    RSDL_font->height = 8; /* Default in  the RSDL_gfx library */
    if (!RSDL_font)
        return NULL;

    /* GUI */
    ctx = nk_sdl_init(RSDL_font,screen_surface,rwidth,rheight);

    /* style.c */
    /* THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK */
     set_style(ctx, THEME_DARK);

    /* icons */

     filebrowser_init();
     sprintf(LCONTENT,"%s\0",RPATH);

//    background = nk_rgb(28,48,62);

 printf("Init nuklear %d\n",0);

//LIBRETRO   
   //save_bkg();

   memset(Key_Sate,0,512);
   memset(Key_Sate2,0,512);

   memset(old_Key_Sate ,0, sizeof(old_Key_Sate));

   gmx=(rwidth/2)-1;
   gmy=(rheight/2)-1;

 return 0;
}

int app_free()
{
//FIXME: memory leak here
    free(RSDL_font);
    filebrowser_free();
    nk_sdl_shutdown();
    Retro_FreeSurface(screen_surface);

 return 0;
}

void app_retro_key(int key,int down)
{
	struct nk_context *ctx = &sdl.ctx;
	if(key<512);
	else return;

        if (key == RETROK_RSHIFT || key == RETROK_LSHIFT) nk_input_key(ctx, NK_KEY_SHIFT, down);
        else if (key == RETROK_DELETE)    nk_input_key(ctx, NK_KEY_DEL, down);
        else if (key == RETROK_RETURN)    nk_input_key(ctx, NK_KEY_ENTER, down);
        else if (key == RETROK_TAB)       nk_input_key(ctx, NK_KEY_TAB, down);
        else if (key == RETROK_LEFT)      nk_input_key(ctx, NK_KEY_LEFT, down);
        else if (key == RETROK_RIGHT)     nk_input_key(ctx, NK_KEY_RIGHT, down);
        else if (key == RETROK_UP)        nk_input_key(ctx, NK_KEY_UP, down);
        else if (key == RETROK_DOWN)      nk_input_key(ctx, NK_KEY_DOWN, down);
        else if (key == RETROK_BACKSPACE) nk_input_key(ctx, NK_KEY_BACKSPACE, down);
        else if (key == RETROK_HOME)      nk_input_key(ctx, NK_KEY_TEXT_START, down);
        else if (key == RETROK_END)       nk_input_key(ctx, NK_KEY_TEXT_END, down);
        else if (key == RETROK_SPACE && !down) nk_input_char(ctx, ' ');

        else if (key >= RETROK_0 && key <= RETROK_9) {
                    nk_rune rune = '0' + key - RETROK_0;
                    nk_input_unicode(ctx, rune);
        }
	else if (key >= RETROK_a && key <= RETROK_z) {
                    nk_rune rune = 'a' + key - RETROK_a;
                    rune = ((1 == LSHIFTON) ? (nk_rune)nk_to_upper((int)rune):rune);
                    nk_input_unicode(ctx, rune);
        }
}


void app_mousebut(int but,int down,int x,int y){

	struct nk_context *ctx = &sdl.ctx;

 	if(but==1)nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
 	else if(but==2)nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
 	else if(but==3)nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
	else if(but==4)nk_input_scroll(ctx,(float)down);
	//printf("but:%d %s (%d,%d)\n",but,down==1?"pressed":"released",x,y);
}

void app_Process_key()
{
	int i;

	for(i=0;i<320;i++)
        	Key_Sate[i]=input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,i) ? 0x80: 0;
   
	if(memcmp( Key_Sate,old_Key_Sate , sizeof(Key_Sate) ) )
	 	for(i=0;i<320;i++)
			if(Key_Sate[i] && Key_Sate[i]!=old_Key_Sate[i]  )
        	{
	
				if(i==RETROK_RSHIFT){
					LSHIFTON=-LSHIFTON;
					printf("Modifier shift pressed %d \n",LSHIFTON); 
					continue;
				}
/*
				if(i==RETROK_F12){
					//play_tape();
					continue;
				}

				if(i==RETROK_RCTRL){
					//CTRLON=-CTRLON;
					printf("Modifier crtl pressed %d \n",CTRLON); 
					continue;
				}
				if(i==RETROK_RSHIFT){
					//SHITFON=-SHITFON;
					printf("Modifier shift pressed %d \n",SHIFTON); 
					continue;
				}

				if(i==RETROK_LALT){
					//KBMOD=-KBMOD;
					printf("Modifier alt pressed %d \n",KBMOD); 
					continue;
				}
//printf("press: %d \n",i);
*/
				app_retro_key(i,1);
	
        	}	
        	else if ( !Key_Sate[i] && Key_Sate[i]!=old_Key_Sate[i]  )
        	{
				if(i==RETROK_LSHIFT){
					LSHIFTON=-LSHIFTON;
					printf("Modifier shift released %d \n",LSHIFTON); 
					continue;
				}
/*
				if(i==RETROK_F12){
      				//kbd_buf_feed("|tape\nrun\"\n^");
					continue;
				}

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

				if(i==RETROK_LALT){
					KBMOD=-KBMOD;
					printf("Modifier alt released %d \n",KBMOD); 
					continue;
				}
//printf("release: %d \n",i);
*/
				app_retro_key(i,0);
	
        	}	

	memcpy(old_Key_Sate,Key_Sate , sizeof(Key_Sate) );

}


int app_poll_mouse(int poll,int processkey)
{
#if defined(__ANDROID__) || defined(ANDROID)
static int holdleft=0;
long int timehold,starthold;
#endif

   #define PAS 2

   if(poll)input_poll_cb();

   static int mmbL=0,mmbR=0,mmbM=0;

   int mouse_l,mouse_m,mouse_r;

   int16_t mouse_x,mouse_y;
   mouse_x=mouse_y=0;

   if(processkey) app_Process_key();

   mouse_wu = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELUP);
   mouse_wd = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELDOWN);
   if(mouse_wu || mouse_wd)app_mousebut(4,mouse_wd?-1:1,0,0);

if(MOUSE_EMULATED==1){

      if(slowdown>0)return 1;

      mouse_l=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
      mouse_r=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
   mouse_m    =input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y);


}
else {
   mouse_l    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
   mouse_r    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);
   mouse_m    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE); 
}

//relative
if(MOUSE_RELATIVE){

   if(MOUSE_EMULATED==1){

      if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))mouse_x += PAS;
      if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))mouse_x -= PAS;
      if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))mouse_y += PAS;
      if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))mouse_y -= PAS;

   }
   else {

   mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
   mouse_y = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);

   }

   gmx+=mouse_x;
   gmy+=mouse_y;
   if(gmx<0)gmx=0;
   if(gmx>rwidth-1)gmx=rwidth-1;
   if(gmy<0)gmy=0;
   if(gmy>rheight-1)gmy=rheight-1;

}
else{

//absolute
//FIXME FULLSCREEN no pointer
 int p_x = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
 int p_y = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

 if(p_x!=0 && p_y!=0){
	int px=(int)((p_x+0x7fff)*rwidth/0xffff);
	int py=(int)((p_y+0x7fff)*rheight/0xffff);
//LOGI("p_(%d,%d) p(%d,%d) \n",p_x,p_y,px,py);
	gmx=px;
	gmy=py;
#if defined(__ANDROID__) || defined(ANDROID)
  //    mouse_l=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
 //     mouse_r=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);

if(holdleft==0){
	starthold=GetTicks();
	holdleft=1;	
}
else if(holdleft==1){

	timehold=GetTicks()-starthold;

	if(timehold>250){
		mouse_l=input_state_cb(0, RETRO_DEVICE_POINTER, 0,RETRO_DEVICE_ID_POINTER_PRESSED);
	}
}

//mouse_l=input_state_cb(0, RETRO_DEVICE_POINTER, 0,RETRO_DEVICE_ID_POINTER_PRESSED);

//FIXME: mouse left button in scale widget.
#endif

 }

}


   if(mmbL==0 && mouse_l){

	mmbL=1;		
	app_mousebut(1,1,gmx,gmy);

   }
   else if(mmbL==1 && !mouse_l) {
#if defined(__ANDROID__) || defined(ANDROID)
holdleft=0;
#endif
	mmbL=0;
	app_mousebut(1,0,gmx,gmy);
   }

   if(mmbR==0 && mouse_r){

      	mmbR=1;
	app_mousebut(2,1,gmx,gmy);		
   }
   else if(mmbR==1 && !mouse_r) {

      	mmbR=0;
	app_mousebut(2,0,gmx,gmy);
   }

   if(mmbM==0 && mouse_m){

      	mmbM=1;
	app_mousebut(3,1,gmx,gmy);		
   }
   else if(mmbM==1 && !mouse_m) {

      	mmbM=0;
	app_mousebut(3,0,gmx,gmy);
   }

 return 0;
}

int app_event(int type){

	nk_input_begin(ctx);

	app_poll_mouse(type,type);

	static int lmx=0,lmy=0;
	if(gmx!=lmx || lmy!=gmy){
		nk_input_motion(ctx, gmx, gmy);
		//printf("mx:%d my:%d \n",gmx,gmy);
	}
	lmx=gmx;lmy=gmy;
	nk_input_end(ctx);

	return 0;
}

int app_render()
{
	if(pauseg==1)restore_bgk();

        gui(ctx);
	if(LOADCONTENT==1)filebrowser(ctx);

        /* Draw */
        //nk_color_fv(bg, background);
	nk_sdl_render(nk_rgba(0,0,0,0));

	draw_cross(gmx,gmy);

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
            Screen_SetFullUpdate(0);
            SHOWKEY=-SHOWKEY;
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
//			   if(SHIFTON == 1)retro_key_up(RETROK_RSHIFT);
//			   else retro_key_down(RETROK_LSHIFT);
               SHIFTON=-SHIFTON;

               oldi=-1;
            }
            else if(i==0x27/*i==-11*/) //CTRL
            {     
//         	   if(CTRLON == 1)retro_key_up(RETROK_LCTRL);
///			   else retro_key_down(RETROK_LCTRL);
               CTRLON=-CTRLON;

               oldi=-1;
            }
			else if(i==-12) //RSTOP
            {
//           	   if(RSTOPON == 1)retro_key_up(RETROK_ESCAPE);
//			   else retro_key_down(RETROK_ESCAPE);            
               RSTOPON=-RSTOPON;

               oldi=-1;
            }
			else if(i==-13) //GUI
            {     
			    pauseg=1;
				SHOWKEY=-SHOWKEY;
				Screen_SetFullUpdate(0);
               oldi=-1;
            }
			else if(i==-14) //JOY PORT TOGGLE
            {    
 				//cur joy toggle
				//cur_port++;if(cur_port>2)cur_port=1;
               SHOWKEY=-SHOWKEY;
               oldi=-1;
            }
            else
            {
               oldi=i;
			   //retro_key_down(oldi); 
			  vkbd_key(oldi,1);            
            }

         }



}

