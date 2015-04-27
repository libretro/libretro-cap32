		
	CAPRICE32 4.2 LIBRETRO DEBUG VERSION WIP

	 
	What?: A quick port of caprice32-4.2.0 to LIBRETRO.
	Why? : It was the first computer i own.
	Who? : Rtype


	
All the credit of the caprice32 Emulator to Ulrich Doewich 
Also 6128.h & amsdos.h (and psg for ps3 )files taken from wiituka (author:dantoine)
http://code.google.com/p/wiituka/

And of course for the RetroArch/Libretro team : "http://www.libretro.org/"


You can download the caprice32 original source code here :
	http://sourceforge.net/projects/caprice32/

	
It's a quick hack.
At this time , works the basis but buggy on linux / android / PS3.

Remember ,Everything not working well, It's a debug release , so expect bugs.



Compile:

	Go to linux directory, and type make .



Usage:

	Launch retroarch , select a startup dsk/sna , and cap32 sould boot.


Control:

	- Joy/Overlay

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

	- In vkbd page 2 

	"EXT" EXIT EMU
	"SNA" SAVE SNA
	"DSK" SWITCH DSK DRIVE A/B
	"Col" SWTICH FGCOL ON/OFF

Core options:
  Autorun - 	disable/enable
	
							If enabled a RUN"DISK command will be done at startup. To make it
							work with your games, you need to create a DISK.BAS file on the
							game disk that. Currently this needs to be done using a different
							Amstrad emulator as libretro-cap32 currently does not save changes
							to disk files.
							
							Follow these steps using a different Amstrad emulator:
							1. Load the emulator and mount your game disk file
							2. Type "CAT" at the BASIC prompt and find the name of the game
							   file on the disk. Usually it's an abbreviation of the game. For
								 this example I will use Boulderdash which has a game file on
								 the disk called BOULDER.BAS								
							3. At the BASIC prompt type:
						  	   10 RUN"BOULDER.BAS
							  	 SAVE "DISK.BAS"
							
							When Autorun is enabled DISK.BAS will be launched at startup which
							will then run the real game file.
	
Knows Bugs: 
 

	- Everything not working well, It's a debug release , so expect to more bug.


