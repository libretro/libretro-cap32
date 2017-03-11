
            int x = 0,y = 0;
            int page = (NPAGE == -1) ? 0 : NLIGN*NPLGN;

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

