#include "libretro.h"

#include "libretro-cap32.h"

#include "graph.h"
#include "vkbd.h"

#ifdef PS3PORT
#include "sys/sys_time.h"
#include "sys/timer.h"
#include <sys/time.h>
#include <time.h>
#define usleep  sys_timer_usleep
/*STATIC_INLINE*/ void gettimeofday (struct timeval *tv, void *blah)
{
    int64_t time = sys_time_get_system_time();

    tv->tv_sec  = time / 1000000;
    tv->tv_usec = time - (tv->tv_sec * 1000000);  // implicit rounding will take care of this for us
}

#else
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#endif

extern int  cmd_cpt,TYPE_CAT,TYPE_RUN,TYPE_ENTER,TYPE_DEL;
extern unsigned char keyboard_translation[320];

extern char * filebrowser(const char *path_and_name);

extern MRES Mres[5];
extern int ZOOM;

extern unsigned dwFPS;

unsigned short int bmp[TEX_WIDTH * TEX_HEIGHT];

int NPAGE=-1, KCOL=1, BKGCOLOR=0, MAXPAS=6;
int SHIFTON=-1,MOUSEMODE=-1,SHOWKEY=-1,PAS=2,STATUTON=-1;

short signed int SNDBUF[1024*2];
int snd_sampler = 44100 / 50;
char RPATH[512];
char DSKNAME[512];

int gmx=320,gmy=240; //gui mouse unused

unsigned char MXjoy0; // joy 
int touch=-1; // gui mouse btn unused
int fmousex,fmousey; // emu mouse unused
int pauseg=0; //enter_gui
int SND=1; //SOUND ON/OFF
int NUMjoy=1; //used for DISK A/B flag

static int mbt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

long frame=0;
unsigned long  Ktime=0 , LastFPSTime=0;

int BOXDEC= 32+2;
int STAT_BASEY=CROP_HEIGHT;

static retro_input_state_t input_state_cb;
static retro_input_poll_t input_poll_cb;

void retro_set_input_state(retro_input_state_t cb)
{
   	input_state_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   	input_poll_cb = cb;
}

void texture_init(){

	memset(bmp, 0, sizeof(bmp));
}

void Emu_init(){
	
}

void Emu_uninit(){


}

#ifdef AND
#define DEFAULT_PATH "/mnt/sdcard/amstrad/"
#else

#ifdef PS3PORT
#define DEFAULT_PATH "/dev_hdd0/HOMEBREW/amstrad/"
#else
#define DEFAULT_PATH "/"
#endif

#endif

#define MDEBUG
#ifdef MDEBUG
#define mprintf printf
#else
#define mprintf(...) 
#endif

long GetTicks(void)
{
#ifndef _ANDROID_

   	struct timeval tv;
   	gettimeofday (&tv, NULL);
   	return tv.tv_sec*1000000 + tv.tv_usec;
#else

    	struct timespec now;
    	clock_gettime(CLOCK_MONOTONIC, &now);
    	return now.tv_sec*1000000 + now.tv_nsec/1000;
#endif
                                                                              
} 

//DBG LOAD DSK
void enter_gui(){	
	
	char dskimg[512]="\0";
	static int inbrowser=1;
	int ret=0;	

	sprintf(dskimg,"%s\0",filebrowser(DEFAULT_PATH));

	if(strcmp(dskimg,"EMPTY\0")==0){
		mprintf("Cancel Fileselect(%s)\n",dskimg);
		inbrowser=0;	
		pauseg=0;
		Screen_SetFullUpdate();

		return;		
	}
	else if(strcmp(dskimg,"NO CHOICE\0")==0){
			
	}
	else{
		mprintf("Ok Fileselect(%s)\n",dskimg);	
		loadadsk((char *)dskimg,NUMjoy>0?0:1);
		inbrowser=0;
		pauseg=0;
		Screen_SetFullUpdate();

		return;
	}		
	
}
void Print_Statut(){

	DrawFBoxBmp(bmp,0,CROP_HEIGHT,CROP_WIDTH,STAT_YSZ,RGB565(0,0,0));

	Draw_text(bmp,STAT_DECX+40 ,STAT_BASEY,0xffff,0x8080,1,2,40,(SND>0?"SND":""));
        Draw_text(bmp,STAT_DECX+80 ,STAT_BASEY,0xffff,0x8080,1,2,40,"F:%d",dwFPS);
	Draw_text(bmp,STAT_DECX+120,STAT_BASEY,0xffff,0x8080,1,2,40,"DSK%c",NUMjoy>0?'A':'B');
	if(ZOOM>-1)Draw_text(bmp,(384-Mres[ZOOM].x)/2,(272-Mres[ZOOM].y)/2,0xffff,0x8080,1,2,40,"x:%3d y:%3d",Mres[ZOOM].x,Mres[ZOOM].y);

}

/*
L2  STATUS ON/OFF
R2  SND ON/OFF
L   VKBD ON/OFF
R   RESET
SEL ENTER
STR ZOOM/DEL
A   FIRE/VKBD KEY
B   RUN
X   CAT
Y   LOAD DSK
*/

void Screen_SetFullUpdate(){	
	memset(bmp, 0, sizeof(bmp));
}

void retro_mouse(int a,int b){}
void retro_mouse_but0(int a){}
void retro_mouse_but1(int a){}
void enter_options(){}


void update_input(void)
{
	int i;	
	//   RETRO      B    Y    SLT  STA  UP   DWN  LEFT RGT  A    X    L    R    L2   R2   L3   R3
        //   INDEX      0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
	static int vbt[16]={0x1C,0x39,0x01,0x3B,0x01,0x02,0x04,0x08,0x80,0x6D,0x15,0x31,0x24,0x1F,0x6E,0x6F};
	static int oldi=-1;
	static int vkx=0,vky=0;

 	MXjoy0=0;
	if(oldi!=-1){retro_key_up(oldi);oldi=-1;}

   	input_poll_cb();

        if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F11) || input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y) )
		pauseg=1;

	i=10;//show vkey toggle
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		SHOWKEY=-SHOWKEY;
		//Screen_SetFullUpdate();
	}

	i=2;//type ENTER
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		TYPE_ENTER=!TYPE_ENTER;
		if(TYPE_ENTER)cmd_cpt=0;
	}

	i=3;//type DEL / ZOOM
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		TYPE_DEL=!TYPE_DEL;
		if(TYPE_DEL)cmd_cpt=0;
		ZOOM++;if(ZOOM>4)ZOOM=-1;

	}
		
        i=0;//type RUN
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		TYPE_RUN=!TYPE_RUN;
		if(TYPE_RUN)cmd_cpt=0;
	}

        i=9;//Type CAT 
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		TYPE_CAT=!TYPE_CAT; 
		if(TYPE_CAT)cmd_cpt=0;
		//Screen_SetFullUpdate();
	}

	i=12;//show/hide statut
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		STATUTON=-STATUTON;
		Screen_SetFullUpdate();
	}

	i=13;//snd on/off
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		SND=-SND;		
	}

	i=11;//reset
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		emu_reset();		
	}

	if(SHOWKEY==1){

		static int vkflag[5]={0,0,0,0,0};		
		
		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) && vkflag[0]==0 )
		    	vkflag[0]=1;
		else if (vkflag[0]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) ){
		   	vkflag[0]=0;
			vky -= 1; 
		}

		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) && vkflag[1]==0 )
		    	vkflag[1]=1;
		else if (vkflag[1]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) ){
		   	vkflag[1]=0;
			vky += 1; 
		}

		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) && vkflag[2]==0 )
		    	vkflag[2]=1;
		else if (vkflag[2]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) ){
		   	vkflag[2]=0;
			vkx -= 1;
		}

		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) && vkflag[3]==0 )
		    	vkflag[3]=1;
		else if (vkflag[3]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) ){
		   	vkflag[3]=0;
			vkx += 1;
		}

		if(vkx<0)vkx=9;
		if(vkx>9)vkx=0;
		if(vky<0)vky=4;
		if(vky>4)vky=0;

		virtual_kdb(bmp,vkx,vky);

		i=8;
		if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i)  && vkflag[4]==0) 	
			vkflag[4]=1;
		else if( !input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i)  && vkflag[4]==1) {

			vkflag[4]=0;
			i=check_vkey2(vkx,vky);

			if(i==-2){
				NPAGE=-NPAGE;oldi=-1;							
				//Screen_SetFullUpdate();
				
			}
			else if(i==-1)oldi=-1;
			else if(i==-3){//KDB bgcolor
				//Screen_SetFullUpdate();
				KCOL=-KCOL;
				oldi=-1;
			}
			else if(i==-4){//VKbd show/hide 			
				
				oldi=-1;
				Screen_SetFullUpdate();
				SHOWKEY=-SHOWKEY;
			}
			else if(i==-5){//Change DSK A/B for loaddsk
				NUMjoy=-NUMjoy;
				oldi=-1;
			}
			else if(i==-6){//EXIT				
				oldi=-1;
				retro_deinit();
				exit(0);
			}
			else if(i==-7){//save sna
				snapshot_save (RPATH);
				oldi=-1;
			}
			else {	
				i=keyboard_translation[i];
				if(i==0x25){
										
					if(SHIFTON == 1)retro_key_up(i);
					else retro_key_down(i);

					SHIFTON=-SHIFTON;

					//Screen_SetFullUpdate();
					
					oldi=-1;
				}
				else {
					oldi=i;
					retro_key_down(i);
				}
			}				

		}

         	if(STATUTON==1)Print_Statut();

		return ;
	}

   	static int mbL=0,mbR=0;
	int mouse_l;
	int mouse_r;
  	int16_t mouse_x;
   	int16_t mouse_y;
  
	if(MOUSEMODE==-1){ //Joy mode

		for(i=4;i<9;i++)if( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) )MXjoy0 |= vbt[i]; // Joy press	
		retro_joy0(MXjoy0);

	   	mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
	   	mouse_y = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
	   	mouse_l    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
	   	mouse_r    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);

		fmousex=mouse_x;
  		fmousey=mouse_y;

	}
	else {  //Mouse mode
		fmousex=fmousey=0;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))fmousex += PAS;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))fmousex -= PAS;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))fmousey += PAS;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))fmousey -= PAS;

		mouse_l=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
		mouse_r=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
       }

	if(mbL==0 && mouse_l){
		mbL=1;		
		retro_mouse_but0(1);
	}
	else if(mbL==1 && !mouse_l) {

 		
		retro_mouse_but0(0);
                mbL=0;
	}

	if(mbR==0 && mouse_r){
		mbR=1;
		retro_mouse_but1(1);
	}
	else if(mbR==1 && !mouse_r) {

 		retro_mouse_but1(0);
                mbR=0;
	}

	retro_mouse(fmousex, fmousey);

	if(STATUTON==1)Print_Statut();

	return ;

}

int update_input_gui()
{
	int ret=0;	
	static int dskflag[7]={0,0,0,0,0,0,0};// UP -1 DW 1 A 2 B 3 LFT -10 RGT 10 X 4	

   	input_poll_cb();	
		
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) && dskflag[0]==0 ){
	    	dskflag[0]=1;
		ret= -1; 
	}
	else /*if (dskflag[0]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) ){
	   	dskflag[0]=0;
		ret= -1; 
	}*/dskflag[0]=0;

	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) && dskflag[1]==0 ){
	    	dskflag[1]=1;
		ret= 1;
	} 
	else/* if (dskflag[1]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) ){
	   	dskflag[1]=0;
		ret= 1; 
	}*/dskflag[1]=0;
		
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) && dskflag[4]==0 )
	    	dskflag[4]=1;
	else if (dskflag[4]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) ){
	   	dskflag[4]=0;
		ret= -10; 
	}

	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) && dskflag[5]==0 )
	    	dskflag[5]=1;
	else if (dskflag[5]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) ){
	   	dskflag[5]=0;
		ret= 10; 
	}

	if ( ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) || \
		input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LCTRL) ) && dskflag[2]==0 ){
	    	dskflag[2]=1;
		
	}
	else if (dskflag[2]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)\
		&& !input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LCTRL) ){

	   	dskflag[2]=0;
		ret= 2;
	}

	if ( ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) || \
		input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LALT) ) && dskflag[3]==0 ){
	    	dskflag[3]=1;
	}
	else if (dskflag[3]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) &&\
		!input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LALT) ){

	   	dskflag[3]=0;
		ret= 3;
	}

	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X) && dskflag[6]==0 )
	    	dskflag[6]=1;
	else if (dskflag[6]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X) ){
	   	dskflag[6]=0;
		ret= 4; 
	}

	return ret;

}

