	// GUI IN PAUSE
	    #define DEFHSZ 16
	    #define DEFWSZ 64

	    //joystick options
	    static int joy1on = nk_false;
    	    static int joy2on = nk_false;
    	    static int joyswap = nk_false;
	      
	     //misc options
	     static int showled = nk_false;

	    //floppy option
	     static char DF8NAME[512]="Choose Content\0";
	     static char DF9NAME[512]="Choose Content\0";

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
	    }

	    //joystick options
            nk_layout_row_dynamic(ctx, DEFHSZ, 3);
            nk_checkbox_label(ctx, "Joy1 on", &joy1on);
            nk_checkbox_label(ctx, "Joy2 on", &joy2on);
            nk_checkbox_label(ctx, "Swap Joy", &joyswap);

	    //misc options
            nk_layout_row_dynamic(ctx, DEFHSZ, 1);
            nk_checkbox_label(ctx, "Show LEDs", &showled);

	    //floppy option

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
	    if(LOADCONTENT==2 && strlen(LCONTENT) > 0){

		fprintf(stdout, "LOAD CONTENT DF%d (%s)\n",LDRIVE,LCONTENT);
//		sprintf(prefs->DrivePath[LDRIVE-8],"%s\0",LCONTENT);
		sprintf((LDRIVE==8? DISKA_NAME: DISKB_NAME),"%s\0",LCONTENT);
		LOADCONTENT=-1;
		loadadsk((char *)(LDRIVE==8? DISKA_NAME: DISKB_NAME),LDRIVE-8);
	    }
	    else if(LOADCONTENT==2)LOADCONTENT=-1;

