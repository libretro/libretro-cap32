# Future
- CORE: Fix flag file, moved slot check to init method.
- CORE: Added joystick button Fire3, thx @revvv.
- GUN: Added gunstick emulation from Wiituka and ported new Phaser emulation from Caprice32 (check documentation).
- AUTORUN: filename memory flags
- CPC+: Alcon homebrew load cart fix
- RMP: +140 classic games autoconfigured
- RMP: Knight Lore, 1942, 1943, 3D Grand Prix, Abu Simbel Profanation, Barbarian, Boloncio, Boulder Dash, Bruce Lee, Ghostbusters, Harrier Attack, Match Day II, Myth, Oh Mummy ([RMP WIP](https://github.com/libretro/libretro-cap32/wiki/RMP:-Done))
- CORE: Keyboard Transparency Option added
- DC/DB: Compute hash from M3U too
- DC: Implemented replace CMD, now you can load DSK/CDT from UI
- CORE: Fix RPI 1-B (old model)
- RMP: Target Renegade
- CORE: joystick fixes
- CORE: evercade support
- DB: added new tests, sync clean-cpc-db v1.1
- DB: sync clean-cpc-db v1.0
- DB: documented correctly RMP files
- AUTORUN: added better code inspired by Fredouille - caprice-forever
- DB: new games from retroachievements
- UI: added DB icon DSK to status bar
- DB: you could add direct tokens using $ (for joystick keybinds or cleans)
- DB: added DB v1 using clean-cpc-db info
- CORE: added model 664 to allow DSK and BASIC 1.0
- CORE: detect some configurations from filename
- VIDEO: minor fixes (requires more work)

# 4.5.3
- AUTORUN: revert system detection at first, CPM only if loader is failed
- CORE: fix memory chip configuration - thx @jcgamestoy
- CPC+: now using white cart also marked as experimental
- AUTORUN: fix ARMOURLOC protection
- AUTORUN: find for DISC/DISK filenames
- AUTORUN: computer restart fixes
- CORE: fixed max memory configuration
- CORE: do not reallocate ram on reset
- CHEEVOS: updated core to allow achievements - thx @Jamiras
- AUTORUN: better hexagon DSK detection
- CORE: removed unused old cpccat
- CORE: fixed PAL timming
- SNA: fix SNA offset - thx @Emashzed

# 4.5.2
- SND: fixed bad audio on lastest RA version - thx @jdgleaver
- CORE: cleaned core, removed hacks and custom code
- WIIU: loader now working on WIIU

# 4.5.1
- CORE: new loader endian compatible
- KOS: changed to RETRO_DEVICE_ID_POINTER_PRESSED
- KOS: fixed render problem (WIIU)
- EVENT: updated mouse code
- ANDROID: now using HIRES compile flags
- MIYOO: add target Miyoo
- CORE: unzip dependency removed
- KOS: added KoS multilang assets
- UI: new UI added
- EVENTS: new events code, with joy configs

# 4.5.0 ~ 4.4.0
- CPC+: ASIC chip emulation (ColinPitrat)
- DSK: fixed speedlock protection
- DSK: support weak sectors (ColinPitrat)
- CORE: added save-state support

# 4.3.0 ~ 4.2.0
- CORE: added slots core from pituka 3DS port
- CORE: floppy sounds added
- DSK: fixed dsk > 10 heads
- CORE: allow load core without content
- CPC+: asic monochrome monitor emulation
- CORE: implement external RAM access API
- CPC+: allow bigger carts - tested shadow of the beast
- CPC+: fixed asic dma
- CORE: added autorun again (thx @2xtime)
- ANDROID: now using lowres, very unstable using hires atm
- CORE: new keyboard/joy implementation, reorganized input code
- CPC+: first CPC+ games working, original code by ColinPitrat
- CORE: keyboard fully emulated
