# CAPRICE32 4.3+ LIBRETRO

All the credit of the caprice32 Emulator to Ulrich Doewich

You can download the caprice32 original source code here :
	http://sourceforge.net/projects/caprice32/


This core also use some code from wiituka/pituka (c) 2004-2016 David Colmenero
http://code.google.com/p/wiituka/

CPC464+/CPC6128+/GX4000 - ATM as **Work in Progress** (c) 2016-2019 Colin Pitrat
https://github.com/ColinPitrat/caprice32

And of course for the RetroArch/Libretro team : "http://www.libretro.com"

The core work pretty well and give the fantastic ability to use the RetroArch Shaders.

Try some CRT shaders and welcome back to the 80s on you're good old CPC.

## Usage

Launch a TAPE (cdt), DSK or M3U, and game should autostart.

## Control

### Joy/Overlay  (Choose AMSTRAD JOYSTICK in input controller setting)


JOY CONFIG DEFAULT (JOYSTICK)
------------
JOY BUTTON | CPC
------------ | -------------
B | FIRE1
Y| SPACE
START | J (to select [J]OYSTICK on game-menu)
DPAD-UP | JOY-UP
DPAD-DOWN | JOY-DW
DPAD-LEFT | JOY-LEFT
DPAD-RIGHT | JOY-RIGHT
A | FIRE2
X | S
-|-
L | INTRO
R | SHIFT
L2 | CTRL
R2 | COPY

JOY CONFIG QAOP (GENERAL KEYB)
------------
JOY BUTTON | CPC
------------ | -------------
B | F1
Y| F2
START | K (to select [K]EYBOARD on game-menu)
DPAD-UP | Q
DPAD-DOWN | A
DPAD-LEFT | O
DPAD-RIGHT | P
A | SPACE
X | H
-|-
L | INTRO
R | SHIFT
L2 | CTRL
R2 | COPY

JOY INCENTIVE ([INCENTIVE GAMES](https://en.wikipedia.org/wiki/Incentive_Software))
------------
JOY BUTTON | CPC
------------ | -------------
B | SPACE
Y| W
START | F
DPAD-UP | CURSOR-UP
DPAD-DOWN | CURSOR-DW
DPAD-LEFT | CURSOR-LEFT
DPAD-RIGHT | CURSOR-RIGHT
A | A
X | C
-|-
L | P
R | L
L2 | R
R2 | U

If you press [SELECT] you could make a combo with other buttons:

JOY BUTTON | ON CPC WRITES
------------ | -------------
SELECT+B | CAT
SELECT+Y| \|CPM
SELECT+A | RUN"DISK & RUN"DISC
SELECT+X | \|TAPE & RUN"
SELECT+START | SHOW VIRTUAL-KEYBOARD
SELECT+DPAD-UP | [1], [Y]
SELECT+DPAD-DOWN | [2], [N]
SELECT+DPAD-RIGHT | [3], [R]
SELECT+DPAD-LEFT | [4], [S]

### Keyboard emulated layout
![imagen](https://user-images.githubusercontent.com/560310/52812237-4c4fbd80-3097-11e9-8537-88f62e8ba5e8.png)

```
F9 MEM SNAPSHOT LOAD/SAVE
F10 MAIN GUI
F12 PLAY TAPE
```

### Keyboard Custom Binds
Choose **AMSTRAD KEYBOARD** in _Quick Menu > Controls_ to customize your retropad keys per game.

## M3U and Disk control

When you have a multi disk game, you can use a m3u file to specify each disk of the game and change them from the RetroArch Disk control interface.

A M3U file is a simple text file with one disk per line (see https://en.wikipedia.org/wiki/M3U).

Example :

Alive (F).m3u
```
Alive (F) - Disk 1A.dsk
Alive (F) - Disk 1B.dsk
```
Path can be absolute or relative to the location of the M3U file.

When a game ask for it, you can change the current disk in the RetroArch 'Disk Control' menu :
- Eject the current disk with 'Disk Cycle Tray Status'.
- Select the right disk index.
- Insert the new disk with 'Disk Cycle Tray Status'.

When the core start, the first disk will be mounted and the first executable autostarted (if autostart is enabled).

### Specify a specific command to launch a game

If the autolaunch option of the core does a pretty good job to guess what command must be executed to launch a game on the CPC, there is some problems (cpm disk and strange catalogs for the most).

You can specify a command to be executed on the CPC when the emu launch.

All you have to do is to add a comment like this in the m3u file :

```
#COMMAND:<YOUR_COMMAND_HERE>
```

Even for one disk game, you can create a m3u file like this one :

Jack the Nipper II... In Coconut Capers.m3u
```
#COMMAND:|CPM
Jack the Nipper II... In Coconut Capers (E).dsk
```

## Core options

### Autorun - 	enable/disable

If enabled a RUN the first bas/bin found in DSK

### User 1/2 Amstrad Joystick Config - joystick, qaop or incentive

Select Joy/Overlay configuration

### Internal resolution - 384x272/768x544

Select internal resolution, the 544p option is almost for games that uses mode 2 (hires mode, very rare)

### Model - 6128/464

Select disk/tape model (6128) or just tape model (464), 6128 is the most compatible model to play

### Monitor Type - color/green/white

Select color monitor or monochrome

### Monitor Intensity - 5 to 15

Emulated Monitor brightness, default value is 8.

## Known Bugs

- CPC-PLUS model is in WIP state, do not use yet.

- Android version is not fully tested/optimized.

- Old3DS need some optimizations (~18fps).
