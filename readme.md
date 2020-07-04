# CAPRICE32 4.5+ LIBRETRO

All the credit of the caprice32 Emulator to Ulrich Doewich

You can download the caprice32 original source code [here](http://sourceforge.net/projects/caprice32/)


This core also use some code from [wiituka/pituka](http://code.google.com/p/wiituka/) (c) 2004-2016 David Colmenero


CPC464+/CPC6128+/GX4000 (c) 2016-2019 Colin Pitrat : https://github.com/ColinPitrat/caprice32


And of course for the RetroArch/Libretro team : "http://www.libretro.com"

The core work pretty well and give the fantastic ability to use the RetroArch Shaders.

Try some CRT shaders and welcome back to the 80s on you're good old CPC.

## Usage

Launch a TAPE (cdt), DSK or M3U, and game should autostart.

### Keyboard emulated layout
#### English layout
![keyb_en](https://user-images.githubusercontent.com/560310/54316312-abdeb180-45e0-11e9-9063-faf78fec9d6d.png)
#### Spanish layout
![keyb_es](https://user-images.githubusercontent.com/560310/54316295-9ff2ef80-45e0-11e9-9ae4-a2e3fb064600.png)
#### French layout
![keyb_fr](https://user-images.githubusercontent.com/560310/54316280-97021e00-45e0-11e9-91b5-da73a87534d6.png)


### Keyboard Custom Binds
Choose **AMSTRAD KEYBOARD** in _Quick Menu > Controls_ and **User 1/2 config different than joystick** to map your retropad keys per game/rom.

Caprice brings some default keymaps confings, these configs are detailed at libretro docs: https://docs.libretro.com/library/caprice32/

## Features not covered in docs

 * New Model: [cap32_model] (6128 | 464 | 6128+)

Choose which Amstrad CPC model to emulate, currently added 6128+

 * New Combo Key: [cap32_combokey] (select | y | b | disabled)

Choose which combo key do you want use and it's ignored from user keymap. If you set as disabled, all joypad buttons are available for your game remaps.

### New Remaps

 * QAOP MAP changed to: 
```
   CPC_KEY_SPACE,       // BUTTON A
   CPC_KEY_B,           // BUTTON B
   CPC_KEY_N,           // BUTTON X
   CPC_KEY_Y,           // BUTTON Y
   CPC_KEY_S,           // SELECT
   CPC_KEY_K,           // START
   CPC_KEY_Q,           // PAD UP
   CPC_KEY_A,           // PAD DOWN
   CPC_KEY_O,           // PAD LEFT
   CPC_KEY_P,           // PAD RIGHT
```

* Simplified JOYSTICK MAP: BUTTON Y, X, SELECT, START - not binded to any key to allow customize on CONTROLS menu.
```
   CPC_KEY_JOY_FIRE1,   // BUTTON A
   CPC_KEY_JOY_FIRE2,   // BUTTON B
   UNUSED,              // BUTTON X
   UNUSED,              // BUTTON Y
   UNUSED,              // SELECT
   UNUSED,              // START
   CPC_KEY_JOY_UP,      // PAD UP
   CPC_KEY_JOY_DOWN,    // PAD DOWN
   CPC_KEY_JOY_LEFT,    // PAD LEFT
   CPC_KEY_JOY_RIGHT,   // PAD RIGHT
```
 * New joystick on port_2: 
```
   CPC_KEY_JOY2_FIRE1,  // BUTTON A
   CPC_KEY_JOY2_FIRE2,  // BUTTON B
   CPC_KEY_X,           // BUTTON X
   CPC_KEY_SPACE,       // BUTTON Y
   CPC_KEY_S,           // SELECT
   CPC_KEY_J,           // START
   CPC_KEY_JOY2_UP,     // PAD UP
   CPC_KEY_JOY2_DOWN,   // PAD DOWN
   CPC_KEY_JOY2_LEFT,   // PAD LEFT
   CPC_KEY_JOY2_RIGHT,  // PAD RIGHT
```

 * L/R L2/R2 changed to allow more combos:
```
   CPC_KEY_CONTROL,     // BUTTON L
   CPC_KEY_INTRO,       // BUTTON R
   CPC_KEY_F1,          // L2
   CPC_KEY_F2,          // R2
```

## M3U and Disk/Tape control

When you have a multi game image, you can use a m3u file to specify each disk or tape of the game and change them from the RetroArch Disk control interface.

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

- [CPC+ Emulation have some issues](https://github.com/libretro/libretro-cap32/issues/59)

- Android version is not fully tested/optimized.

- Old3DS need some optimizations (~18fps).
