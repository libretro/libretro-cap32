# Future

# 4.5.3
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