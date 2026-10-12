# Future
- AUTORUN: Avoid unsuitable files, skip verified PLUS3DOS files during CPC autorun selection, and prefer startable binary.
- BUILD: Optimized build for RG35XX, small buildfix for ctr, and build compat/fopen_utf8.c.
- BUILD: Add webOS to CI, thx @cscd98, and linux-aarch64 build, thx @warmenhoven.
- CORE: Added CPC+ loader flag and better, simplified DDI support.
- CORE: Use CPC_MODEL to correctly set/check the expected model.
- CORE: Build disk names without fill_pathname(), thx @jbdemonte.
- CORE: Route all file I/O through the frontend VFS, thx @WizzardSK.
- CORE: Clean up logging messages and added retro_core_options_v2, thx @rtomasa.
- CPC+: Correct hardware sprite screen alignment, clipping, and coordinates for non-32-bit framebuffers.
- CPC+: Fix Eerie Forest crashes, vertical scrolling and PRI reprogramming, thx @LilaQ.
- CPC+: Acknowledge Plus ASIC interrupts in all Z80 interrupt modes and schedule raster interrupts independently of HSYNC width, thx @LilaQ.
- CPC+: Fix Upper ROM initialization on CPC Plus models, thx @LilaQ.
- CPC+: Fix a bug where sprites disappear sometimes (Dick Tracy), thx @ColinPitrat.
- CPC+: Fix plus rom, now using english PLUS rom, thx @2play.
- CRTC: Changed MinVHOLD to have more tolerance range (fixes Bonanza Bros, Thunder Blade), thx @ArcadeWithin.
- DB: Updated clean-cpc-db v1.2 to release 1.4.
- DSK: Probe format directory before scanning fallback tracks, thx @LilaQ.
- DSK: Tolerate absent extra tracks in over-dumped images and add warnings.
- DSK: Insert disk after change as fceumm does.
- DSK: Added granularity to loader DISC/DISK and better debug logs.
- DSK: Fix m3u loading, thx @rtomasa.
- DSK: New save game DSK diff support. Allows save games and do not modify original DSK file, reported by @Jamiras.
- DSK: Added disk swap functionality using Combo and L/R buttons. Partially based on @madcock [work](https://github.com/libretro/libretro-cap32/commit/a7f6f74c0467051a67e6203e200213658173a08a), thx!
- FDC: Add support for Orion Prime.
- FDC: Allow head seeks and recalibration with the spindle motor stopped.
- FDC: Avoid spurious drive-ready notifications and separate read startup from the byte overrun deadline.
- GUN: Added Loriciel West Phaser and Trojan Phazer lightgun modes, thx @LilaQ.
- GUN: Fixed gunstick init.
- KBD: allow autorun commands without press return at the end.
- KBD: Add L3 shortcut to toggle on-screen keyboard.
- RMP: Added 2 players support and added some RMPs from great two players games as start.
- RMP: updated new binds, added Afterburner and Oh Mummy! Thanks to Haritz and Pablo from LOVG to point me.
- RMP: Added Amsoft loader fixes and new classics and puzzle games.
- RMP: Reapply binds after update_variables, added Silent Shadow RMP.
- TAPE: Add optional automatic fast-forward during tape loading and gate polling through shared frontend capability state.
- TEST: Refactor python tests to cmocka, added some fixes.
- UI: Fix wrong height for status bar and missing statusbar on fullscreen after crop changes.
- VIDEO: Fixed cropped geometry and added 8bpp mode thx @fjtrujy.

# 4.5.4
- FIX: Check retro_set_controller_port_device to avoid third-party UI problems
- VIDEO: Added software crop-scaler
- CORE: RG35XX support
- RMP: +250 games configured and tested!!
- CORE: Fix flag file, moved slot check to init method
- CORE: Added joystick button Fire3, thx @revvv
- GUN: Added gunstick emulation from Wiituka and ported new Phaser emulation from Caprice32 (check documentation)
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
