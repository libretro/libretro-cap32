# CAPRICE32 4.5+ LIBRETRO

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

Detailed usage at libretro docs: https://docs.libretro.com/library/caprice32/

### Keyboard emulated layout
![imagen](https://user-images.githubusercontent.com/560310/52812237-4c4fbd80-3097-11e9-8537-88f62e8ba5e8.png)


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


# Known Bugs

- CPC-PLUS model is in WIP state, do not use yet.

- Android version is not fully tested/optimized.

- Old3DS need some optimizations (~18fps).
