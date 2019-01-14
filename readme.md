
# CAPRICE32 4.2 LIBRETRO 
	 
A quick port of caprice32-4.2.0 to LIBRETRO.
	
All the credit of the caprice32 Emulator to Ulrich Doewich 
Also 6128.h & amsdos.h (and psg for ps3 )files taken from wiituka (author:dskywalk)
http://code.google.com/p/wiituka/

And of course for the RetroArch/Libretro team : "http://www.libretro.com"

You can download the caprice32 original source code here :
	http://sourceforge.net/projects/caprice32/

The core work pretty well and give the fantastic ability to use the RetroArch Shaders.

Try some CRT shaders and welcome back to the 80s on you're good old CPC.

## Usage 

Launch a TAPE (cdt), DSK or M3U, and game should autostart.

## Control

### Joy/Overlay  (Choose AMSTRAD JOYSTICK in input controller setting)

```
L2  STATUS ON/OFF
R2  AUTOLOAD TAPE
L   CAT
R   RESET
SEL MOUSE/JOY IN GUI
STR ENTER/RETURN
A   FIRE1/VKBD KEY
B   RUN
X   FIRE2
Y   VKBD ON/OFF

- In vkbd (page 2) 

"EXT" EXIT EMU
"SNA" SAVE SNA
"DSK" SWITCH DSK DRIVE A/B
"Col" SWTICH FGCOL ON/OFF
```

### Keyboard (Choose AMSTRAD KEYBOARD in input controller setting)

```
F8 LOAD DSK/TAPE
F9 MEM SNAPSHOT LOAD/SAVE
F10 MAIN GUI
F12 PLAY TAPE
```

## M3U and Disk control

This core now support the usage of M3U file and disk control from within RetroArch for multi disk games.

A M3U file is a simple text file with one disk per line (see https://en.wikipedia.org/wiki/M3U).

Example :

Alive (F).m3u
```
Alive (F) - Disk 1A.dsk
Alive (F) - Disk 1B.dsk
```
Path can be absolute or relative to the location of the M3U file.

In RetroArch you can change the current disk in the 'Disk Control' menu. Just select the right disk index when a game ask for it.

No need to eject disk ('Disk Cycle Tray Status'), you just have to cycle through disk indexes.

On start, the first disk will be mounted and the first executable autostarted.

### Special feature for frontend users

You can specify a command to be executed on the CPC when the emu launch.

All you have to do is to add a comment like this in the m3u file :

```
#COMMAND:<YOUR_COMMAND_HERE>
```

If the autolaunch option of the core does a pretty good job to guess what command must be executed to launch a game on the CPC, there is some problems (cpm disk and strange catalogs for the most).

Here, this little option come to the rescue.

Even for one disk game, you can create a m3u file like this one :

Jack the Nipper II... In Coconut Capers.m3u
```
#COMMAND:|CPM
Jack the Nipper II... In Coconut Capers (E).dsk
```

And it's done.

## Core options

### Autorun - 	disable/enable

If enabled a RUN the first bas/bin found in DSK

## Known Bugs 

- Sometime tricky to use (Keyboard usage for instance).


