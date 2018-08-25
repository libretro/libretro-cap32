
# CAPRICE32 4.2 LIBRETRO 
	 
A quick port of caprice32-4.2.0 to LIBRETRO.
	
All the credit of the caprice32 Emulator to Ulrich Doewich 
Also 6128.h & amsdos.h (and psg for ps3 )files taken from wiituka (author:dantoine)
http://code.google.com/p/wiituka/

And of course for the RetroArch/Libretro team : "http://www.libretro.com"

You can download the caprice32 original source code here :
	http://sourceforge.net/projects/caprice32/

Remember, everything not working well, so expect bugs.

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

Example (Alive (F).m3u):
```
Alive (F) - Disk 1A.dsk
Alive (F) - Disk 1B.dsk
```
Path can be absolute or relative to the location of the M3U file.

In RetroArch you can change the current disk in the 'Disk Control' menu. Just select the right disk index when a game ask for it.

No need to eject disk ('Disk Cycle Tray Status'), you just have to cycle through disk indexes.

On start the first disk will be mounted and the first executable autostarted.

## Core options

### Autorun - 	disable/enable

If enabled a RUN the first bas/bin found in DSK

## Knows Bugs 

- Everything not working well.


