/* nuklear - v1.00 - public domain */

extern int pauseg;
extern int NPAGE,SHIFTON;
extern unsigned char *keymat,  *revmat,  *joy;
extern int vkey_pressed;

extern char DISKA_NAME[512];
extern char DISKB_NAME[512];
extern char TAPE_NAME[512];

extern void emu_reset(void);
extern int loadadsk (char *arv,int drive);

static void
gui(struct nk_context *ctx)
{

 if (nk_begin(ctx,"Caprice32 GUI", nk_rect(10,30, 364, 212),
NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            /*NK_WINDOW_MINIMIZABLE|*/NK_WINDOW_TITLE))
 //       NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE))
    {

	if(pauseg==1 && SHOWKEY==1)SHOWKEY=-1;

	// VKB IN GAME
	if(pauseg==0 && SHOWKEY==1)
    	{

        	size_t x = 0,y = 0;
        	int page = (NPAGE == -1) ? 0 : 5*NPLGN;

       		nk_layout_row_dynamic(ctx, 32, NPLGN);

		vkey_pressed=-1;

   		for(y=0;y<NLIGN;y++)
   		{
   	  		for(x=0;x<NPLGN;x++)
  	   		{               
		
				if (nk_button_text(ctx,SHIFTON==-1?MVk[(y*NPLGN)+x+page].norml:MVk[(y*NPLGN)+x+page].shift , \
				       SHIFTON==-1?strlen(MVk[(y*NPLGN)+x+page].norml):strlen(MVk[(y*NPLGN)+x+page].shift))) {

					LOGI("(%s) pressed! (%d,%d)\n",SHIFTON==-1?MVk[(y*NPLGN)+x+page].norml:MVk[(y*NPLGN)+x+page].shift,x,y);
					vkey_pressed=MVk[(y*NPLGN)+x+page].val;
				}

  	   		}
		}



    	}
	else
	// GUI IN PAUSE
	if(pauseg==1 && SHOWKEY==-1 && LOADCONTENT!=1)
        {
	    #define DEFHSZ 16
	    #define DEFWSZ 64
//	    prefs = new Prefs(ThePrefs);

	    //joystick options
	    static int joy1on = nk_false;
    	    static int joy2on = nk_false;
    	    static int joyswap = nk_false;
/*	
		if (ThePrefs.Joystick1Port) { // joy-1
			joy1on = nk_true;
		}
		else joy1on = nk_false;
		
		if (ThePrefs.Joystick2Port) { //joy-2
			joy2on = nk_true;
		}
		else joy2on = nk_false;
		
		if ( ThePrefs.JoystickSwap){ //swap
			joyswap = nk_true;
		}
		else joyswap = nk_false;

*/	      
	     //misc options
	     static int showled = nk_false;
/*		
		if (ThePrefs.ShowLEDs) { 
			showled = nk_true;
		}
		else showled = nk_false;
*/
	    //floppy option
	     static int emu1541 = nk_false;
	     static char DF8NAME[512]="Choose Content\0";
	     static char DF9NAME[512]="Choose Content\0";

/*
		if(ThePrefs.Emul1541Proc)emu1541=nk_true;
		else emu1541=nk_false;

*/
	    // button toggle GUI/EMU
            nk_layout_row_static(ctx, DEFHSZ, DEFWSZ, 2);
            if (nk_button_label(ctx, "Resume")){
                fprintf(stdout, "quit GUI\n");
		pauseg=0;
	    }
            if (nk_button_label(ctx, "Reset")){
                fprintf(stdout, "quit GUI & reset\n");
		pauseg=0;
		emu_reset();
	// 	TheC64->Reset();
	    }

	    //joystick options
            nk_layout_row_dynamic(ctx, DEFHSZ, 3);
            nk_checkbox_label(ctx, "Joy1 on", &joy1on);
            nk_checkbox_label(ctx, "Joy2 on", &joy2on);
            nk_checkbox_label(ctx, "Swap Joy", &joyswap);
/*
		if(joy1on){
			if(!ThePrefs.Joystick1Port)
				prefs->	Joystick1Port =1;
		}	
		else if(ThePrefs.Joystick1Port)
			prefs->	Joystick1Port =0;

		if(joy2on){
			if(!ThePrefs.Joystick2Port)
				prefs->	Joystick2Port =1;
		}
		else if(ThePrefs.Joystick2Port)
			prefs->	Joystick2Port =0;

		if(joyswap){
			if(!ThePrefs.JoystickSwap)
				prefs->	JoystickSwap = true;
		}
		else if(ThePrefs.JoystickSwap)
			prefs->	JoystickSwap =false;
*/
		//misc options
            nk_layout_row_dynamic(ctx, DEFHSZ, 1);
            nk_checkbox_label(ctx, "Show LEDs", &showled);
/*
		if(showled){
			if(!ThePrefs.ShowLEDs)
				prefs->	ShowLEDs = true;
		}
		else if(ThePrefs.ShowLEDs)
			prefs->	ShowLEDs =false;
*/
	    //floppy option
            nk_layout_row_dynamic(ctx, DEFHSZ, 1);
            nk_checkbox_label(ctx, "Emulate 1541", &emu1541);
/*
		if(emu1541){
			if(!ThePrefs.Emul1541Proc)
				prefs->	Emul1541Proc =1;
		}	
		else if(ThePrefs.Emul1541Proc)
			prefs->	Emul1541Proc =0;
*/
	        int i;

		for(i=0;i<2;i++)
			if(LOADCONTENT==2 && LDRIVE==(i+8));
			else if( (i==0? DISKA_NAME: DISKB_NAME)!=NULL){
			 	sprintf((i==0?DF8NAME:DF9NAME),"%s\0",(i==0? DISKA_NAME: DISKB_NAME));
			}
			//else sprintf(LCONTENT,"Choose Content\0");
	     
            nk_layout_row_dynamic(ctx, DEFHSZ, 1);
            nk_label(ctx, "DISKA:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, DEFHSZ, 1);

            if (nk_button_label(ctx, DF8NAME)){
                fprintf(stdout, "LOAD DISKA\n");
		LOADCONTENT=1;
		LDRIVE=8;
		//pauseg=0;
	    }

            nk_layout_row_dynamic(ctx, DEFHSZ, 1);
            nk_label(ctx, "DF9:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, DEFHSZ, 1);

            if (nk_button_label(ctx, DF9NAME)){
                fprintf(stdout, "LOAD DISKA\n");
		LOADCONTENT=1;
		LDRIVE=9;
		//pauseg=0;
	    }
	    if(LOADCONTENT==2){

		fprintf(stdout, "LOAD CONTENT DF%d (%s)\n",LDRIVE,LCONTENT);
//		sprintf(prefs->DrivePath[LDRIVE-8],"%s\0",LCONTENT);
		sprintf((LDRIVE==8? DISKA_NAME: DISKB_NAME),"%s\0",LCONTENT);
		LOADCONTENT=-1;
		loadadsk((char *)(LDRIVE==8? DISKA_NAME: DISKB_NAME),LDRIVE-8);
	    }
/*

	    if(ThePrefs!=*prefs){
		printf("pref change \n");
		TheC64->NewPrefs(prefs);
		ThePrefs = *prefs;
	    }

	    delete prefs;
*/
        }


   	nk_end(ctx);
   }
}

