# CAPRICE32 4.5+ LIBRETRO - [changelog](CHANGES.md)

**All the credit of the caprice32 Emulator to Ulrich Doewich**

You can download the caprice32 original source code [here](http://sourceforge.net/projects/caprice32/)

* This core also use some code from [wiituka/pituka](http://code.google.com/p/wiituka/) (c) 2004-2016 David Colmenero
* CPC464+/CPC6128+/GX4000 (c) 2016-2019 Colin Pitrat : https://github.com/ColinPitrat/caprice32
* Part of the autorun fixes (see catalog/amsdos_catalog and format.c) are inspired on the great work of Frédéric Coste (Fredouille) on [caprice-forever](https://www.cpc-power.com/cpcarchives/index.php?page=articles&num=445).

And of course for the RetroArch/Libretro team : "http://www.libretro.com"

The core works pretty well and give the fantastic ability to use the RetroArch Shaders.

Try some CRT shaders and welcome back to the 80s on your good old CPC.

## Usage

Launch a TAPE (cdt), DSK or M3U, and game should autostart.


## Game Database

![imagen](https://user-images.githubusercontent.com/560310/172238790-09ca924a-099c-4c45-a0be-1738b8d6fa6f.png)

Our game database is based on [clean-cpc-db](https://github.com/clean-cpc-db). A database that [has checked every game](https://github.com/libretro/libretro-cap32/issues/112) so that you don't have to spend whole day of finding the correct and functional version of your favorite game. In clean-cpc-db you'll find **only clean versions with no weird intros or menus, just working games!**

Due to the creation of this database, we have started a small game mapping project, to bring an easyest CPC plug and play experiencie! You can help us...

* [Contribute with your RMP files](https://github.com/libretro/libretro-cap32/wiki/%5Bcolaborate%5D-create-a-new-RMP-for-your-favorite-game!) ([Work in Progress](https://github.com/libretro/libretro-cap32/wiki/RMP:-Done)).

When a game is detected as a **clean version**, a green disc icon appears at the bottom.
![imagen](https://user-images.githubusercontent.com/560310/172237853-0a18ebee-be5e-4dec-90fa-e4ca7b77a2d7.png)


## Keyboard emulated layout
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

 * New Model: [cap32_model] (6128 | 464 | 664 | 6128+)

Choose which Amstrad CPC model to emulate, currently added 646 to allow BASIC 1.0 games and DSK

### New filename flags (case insensitive)

* "[664]": Force BASIC 1.0 and AMSDOS ROMs, to allow play correctly first CPC games.
* "BASIC 1.0": Force 464 model when you load CDT or 664 model if use DSK.
* "[CPM]": Force load CPM BIOS first, useful for some infocom CPM games.
* "[576K]": RAM mode with 512+64KB, useful for some homebrew games. _Also allows [128K] and [064K]_.

Examples :

```
Alive (1990)(Company)(fr)[664].dsk => 664
Manic Miner (UK) (1984) (Version Basic 1.0) [Amsoft].dsk => 664
Harrier Attack (1986)(Amsoft) [Basic 1.0].cdt => 464
Shadows Of Sergoth, The (2018)(PD)(M3)(v1.0)(Side A)[576K].dsk => 576 RAM
```

# Lightgun Emulation

Gunstick and Phaser (phaser code thanks to [Colin Pitrat](https://github.com/ColinPitrat/caprice32/issues/34)).

## Configure your Weapon
You need set on Options > Controls > Device Type: **Amstrad Lightgun** and on Core Options > Light Gun Input (and show cross if your want).

### Gunstick Games
```
Mike Gunner 
Bestial Warrior
Cosmic Sheriff
El Equipo A
Guillermo Tell
Outlaws
Solo
Sootland
Space Smugglers
Target Plus
Trigger
```
### Amstrad Magnum Phaser
```
Operation Wolf
Bullseye
Missile Ground Zero
Robot Attack
Rookie
Solar Invasion
American Turbo-King
Billy the Kid
Bronx Street Cop
F-16 Fighting Falcon
Jungle Warfare
```
### Trojan Light Phazer
```
The Enforcer
Skee Shoot
```
# Known Bugs

- [CPC+ Emulation have some issues](https://github.com/libretro/libretro-cap32/issues/59) and **need 24bit color depth**.

- Old3DS need some optimizations (~18fps).

- [Minor CPC Bugs and FDC needs more work](https://github.com/libretro/libretro-cap32/issues/110) to accurately emulate complex protections.

- Gunstick emulation is not working with green phosphor or lowres modes.
