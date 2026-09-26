#!/usr/bin/env python3
"""Run against a native shared core: test-gun-calibration.py CORE DISK SAVE_DIR.
Uses synthetic independent lightgun inputs; no frontend or physical guns required.
SAVE_DIR must be a scratch directory. Optional CAL_SCREEN writes one PPM frame.
"""
import ctypes as C
import math
import os
from pathlib import Path
import sys

core = C.CDLL(sys.argv[1])
save = Path(sys.argv[3]); save.mkdir(parents=True, exist_ok=True)
save_bytes = os.fsencode(save)
class Variable(C.Structure):
    _fields_ = [('key', C.c_char_p), ('value', C.c_char_p)]
class Game(C.Structure):
    _fields_ = [('path', C.c_char_p), ('data', C.c_void_p), ('size', C.c_size_t), ('meta', C.c_char_p)]
class OptionValue(C.Structure):
    _fields_ = [('value', C.c_char_p), ('label', C.c_char_p)]
class Definition(C.Structure):
    _fields_ = [(k,C.c_char_p) for k in ('key','desc','desc_categorized','info','info_categorized','category_key')] + [('values',OptionValue*128),('default_value',C.c_char_p)]
class OptionsV2(C.Structure):
    _fields_ = [('categories',C.c_void_p),('definitions',C.POINTER(Definition))]
opts = {}
allowed = {}
def register(key, values, default):
    assert default in values, (key, default)
    allowed[key] = values
    if opts.get(key) not in values: opts[key] = default

overrides = {b'cap32_lightgun_input': b'gunstick', b'cap32_gfx_colors': os.getenv('CAL_DEPTH', '16bit').encode(),
             b'cap32_statusbar': b'disabled', b'cap32_scr_crop': os.getenv('CAL_CROP', 'disabled').encode()}
changed = False
fmt = 2
messages = []
@C.CFUNCTYPE(C.c_bool, C.c_uint, C.c_void_p)
def env(cmd, data):
    global changed, fmt
    if cmd in (9, 30, 31):
        C.cast(data, C.POINTER(C.c_char_p))[0] = save_bytes
        return True
    if cmd == 16:
        values = C.cast(data, C.POINTER(Variable)); i = 0
        while values[i].key:
            entries = values[i].value.split(b'; ', 1)[1].split(b'|')
            register(values[i].key, entries, entries[0]); i += 1
        return True
    if cmd == 52:
        C.cast(data,C.POINTER(C.c_uint))[0] = int(os.getenv('CAL_OPTIONS_VERSION','2')); return True
    if cmd == 67:
        defs = C.cast(data,C.POINTER(OptionsV2)).contents.definitions; i = 0
        while defs[i].key:
            entries = [v.value for v in defs[i].values if v.value]
            register(defs[i].key, entries, defs[i].default_value); i += 1
        return True
    if cmd == 15:
        v = C.cast(data, C.POINTER(Variable)).contents
        v.value = overrides.get(v.key, opts.get(v.key))
        return v.value is not None
    if cmd == 17:
        C.cast(data, C.POINTER(C.c_bool))[0] = changed; changed = False; return True
    if cmd == 70:  # SET_VARIABLE
        v = C.cast(data, C.POINTER(Variable)).contents
        if v.value not in allowed.get(v.key, []): return False
        opts[v.key] = bytes(v.value); changed = True; return True
    if cmd == 10:
        fmt = C.cast(data, C.POINTER(C.c_int))[0]; return True
    if cmd == 6:
        messages.append(C.cast(data, C.POINTER(C.c_char_p))[0]); return True
    return False
xy = [[0, 0], [0, 0]]
trigger = [False, False]; middle = [False, False]; right = [False, False]; offscreen = [False, False]
@C.CFUNCTYPE(C.c_int16, C.c_uint, C.c_uint, C.c_uint, C.c_uint)
def input_state(port, device, index, button):
    if port > 1: return 0
    if device == 4:
        return {2: trigger[port], 13: xy[port][0], 14: xy[port][1], 15: offscreen[port]}.get(button, 0)
    if device == 2: return {6: middle[port], 3: right[port]}.get(button, 0)
    return 0
last_video = None
@C.CFUNCTYPE(None, C.c_void_p, C.c_uint, C.c_uint, C.c_size_t)
def video(data, w, h, pitch):
    global last_video
    if data: last_video = (C.string_at(data, pitch*h), w, h, pitch)
@C.CFUNCTYPE(None)
def poll(): pass
@C.CFUNCTYPE(None, C.c_int16, C.c_int16)
def sample(left, right): pass
@C.CFUNCTYPE(C.c_size_t, C.c_void_p, C.c_size_t)
def batch(data, frames): return frames
for name, callback in [('environment', env), ('video_refresh', video), ('input_poll', poll),
                       ('input_state', input_state), ('audio_sample', sample), ('audio_sample_batch', batch)]:
    getattr(core, 'retro_set_' + name)(callback)
core.retro_load_game.restype = C.c_bool
core.gun_calibration_active.restype = C.c_bool
core.gun_calibration_apply.argtypes = [C.c_uint, C.POINTER(C.c_int), C.POINTER(C.c_int)]
core.gun_calibration_apply.restype = C.c_bool
core.retro_serialize_size.restype = C.c_size_t
core.retro_serialize.argtypes = [C.c_void_p, C.c_size_t]
core.retro_serialize.restype = C.c_bool
core.retro_init()
for p in range(2): core.retro_set_controller_port_device(p, 260)
game = Game(os.fsencode(sys.argv[2]), None, 0, None)
assert core.retro_load_game(C.byref(game))
for _ in range(60): core.retro_run()

def ticks(n=21):
    for _ in range(n): core.retro_run()
def option(value):
    global changed
    opts[b'cap32_lightgun_calibration'] = value.encode(); changed = True; core.retro_run()
def apply(p, x, y):
    xx, yy = C.c_int(round(x*32767)), C.c_int(round(y*32767))
    ok = core.gun_calibration_apply(p, C.byref(xx), C.byref(yy))
    return ok, xx.value/32767, yy.value/32767
# Both guns see the same shader distortion, with independent axes.
def measured(p, x, y):
    return (.74*x + .08, .81*y - .06)
def aim(p, x, y): xy[p] = [round(x*32767), round(y*32767)]
def shoot(p, x, y):
    aim(p, x, y); ticks(); trigger[p] = True; core.retro_run(); trigger[p] = False; core.retro_run()
class Gun(C.Structure):
    _fields_ = [('x', C.c_int), ('y', C.c_int), ('state', C.c_int), ('pressed', C.c_int)]
guns = (Gun*2).in_dll(core, 'gun')
def mapped(p):
    for x,y in [(-.65, -.55), (.1,.3), (.6,-.4), (0,0)]:
        ok, a,b = apply(p, *measured(p,x,y)); assert ok and abs(a-x)<.0002 and abs(b-y)<.0002, (p,x,y,a,b)
        aim(p,*measured(p,x,y)); core.ev_lightgun(p)
        _,w,h,_ = last_video; crop = 64 if overrides[b'cap32_scr_crop'] == b'enabled' else 0
        assert abs(guns[p].x-(crop+(x+1)*.5*(w-1)))<2
        assert abs(guns[p].y-(y+1)*.5*(h-1))<2
def snapshot():
    n = core.retro_serialize_size(); b = C.create_string_buffer(n)
    assert core.retro_serialize(b,n); return b.raw

def ppm():
    if not os.getenv('CAL_SCREEN'): return
    data,w,h,pitch = last_video; rgb = bytearray()
    for y in range(h):
        for x in range(w):
            q = int.from_bytes(data[y*pitch+x*(4 if fmt==1 else 2):y*pitch+(x+1)*(4 if fmt==1 else 2)], 'little')
            rgb.extend(((q>>16)&255,(q>>8)&255,q&255) if fmt==1 else ((q>>11)*255//31,((q>>5)&63)*255//63,(q&31)*255//31))
    Path(os.environ['CAL_SCREEN']).write_bytes(f'P6\n{w} {h}\n255\n'.encode()+rgb)

option('reset')
assert apply(0,.2,-.3)[0]
for p in range(2):
    option('reset')
    if p == 1: core.retro_set_controller_port_device(0,0)
    option('start'); assert core.gun_calibration_active(); before = snapshot()
    ticks(); ppm()
    # The target must actually reach the frontend in each pixel format.
    data,w,h,pitch = last_video
    bpp = 4 if fmt == 1 else 2
    tx,ty = int(.15*(w-1)), int(.15*(h-1))
    assert any(any(data[y*pitch+x*bpp:y*pitch+(x+1)*bpp])
               for y in range(ty-3,ty+4) for x in range(tx-4,tx+5)), 'Invisible target'
    # Invalid offscreen click must not become a sample.
    offscreen[p] = True; shoot(p,0,0); offscreen[p] = False
    for x,y in [(-.7,-.7),(.7,-.7),(-.7,.7)]: shoot(p,*measured(p,x,y))
    # Rejected fourth point must not install the candidate.
    shoot(p,0,0); assert core.gun_calibration_active()
    assert abs(apply(p,.2,.3)[1]-.2)<.0001
    shoot(p,*measured(p,.7,.7)); assert not core.gun_calibration_active(); mapped(p)
    assert snapshot() == before, 'Emulation advanced during calibration'
    core.retro_set_controller_port_device(0,260)
    mapped(0); mapped(1)
print('PASS: shared scale/offset correction from either gun, rejected check/offscreen shots, paused emulation')

# Held trigger must not advance targets; the remaining three shots complete calibration.
option('start'); ticks(); aim(0,*measured(0,-.7,-.7)); trigger[0]=True; ticks(80)
trigger[0]=False; core.retro_run()
for x,y in [(.7,-.7),(-.7,.7),(.7,.7)]: shoot(0,*measured(0,x,y))
assert not core.gun_calibration_active(); mapped(0)
# Cancellation retains the previous profile.
option('start'); ticks(); middle[0]=True; core.retro_run()
assert core.gun_calibration_active(); middle[0]=False; core.retro_run()
assert not core.gun_calibration_active(); mapped(0)
# Degenerate samples are rejected; another valid calibration remains possible.
option('start')
for _ in range(4): shoot(0,.1,.1)
assert core.gun_calibration_active(); mapped(0)
right[0]=True; core.retro_run(); right[0]=False; core.retro_run()
for x,y in [(-.7,-.7),(.7,-.7),(-.7,.7),(.7,.7)]: shoot(0,*measured(0,x,y))
assert not core.gun_calibration_active(); mapped(0); mapped(1)
assert not apply(0,1,1)[0], 'Transformed offscreen shot was clamped onscreen'
print('PASS: hold/release, cancel, retry, degenerate points, transformed offscreen rejection')
core.retro_unload_game(); assert core.retro_load_game(C.byref(game)); mapped(0); mapped(1)
# Calibrated numbers are selected values, not just labels or rounded presets.
keys = [b'cap32_lightgun_'+k for k in (b'scale_x',b'offset_x',b'scale_y',b'offset_y')]
def displayed_map(x,y):
    m = [float(opts[k]) for k in keys]
    return m[0]*x+m[1], m[2]*y+m[3]
for x,y in [(.2,.3),(-.5,.4)]:
    ok,a,b=apply(0,x,y); c,d=displayed_map(x,y)
    assert ok and abs(a-c)<.0001 and abs(b-d)<.0001
assert all(opts[k] in allowed[k] for k in keys)
# The full fixed range is present before/after calibration and editing.
assert not any(b'skew' in key for key in allowed)
for key in (keys[0], keys[2]):
    for value in (b'0.1', b'0.5', b'1', b'1.2', b'2', b'3.2'): assert value in allowed[key]
# Manual edits affect both ports and survive a load with stale frontend options.
assert b'1.5' in allowed[keys[0]] and b'0.1' in allowed[keys[1]]
opts[keys[0]]=b'1.5'; opts[keys[1]]=b'0.1'; changed=True; core.retro_run()
for port in range(2):
    ok,a,b=apply(port,.2,0); assert ok and abs(a-.4)<.0001
core.retro_unload_game()
opts[keys[0]]=b'1'; opts[keys[1]]=b'0'
assert core.retro_load_game(C.byref(game))
assert float(opts[keys[0]]) == 1.5 and float(opts[keys[1]]) == .1
assert abs(apply(1,.2,0)[1]-.4)<.0001
# Re-registering calibration options must preserve unrelated user settings.
opts[b'cap32_lightgun1_keyboard']=b'right'; changed=True; core.retro_run()
option('reset'); assert opts[b'cap32_lightgun1_keyboard']==b'right'
for port in range(2): assert abs(apply(port,.2,.3)[1]-.2)<.0001
core.retro_unload_game(); assert core.retro_load_game(C.byref(game))
assert all(float(opts[k]) == (1 if i in (0,2) else 0) for i,k in enumerate(keys))
print('PASS: exact displayed values, manual edits on both ports, persistence, shared reset')
# Both scale endpoints remain selectable after edits, without closing/reopening.
for value in (b'0.1', b'3.2'):
    for key in (keys[0],keys[2]):
        assert value in allowed[key]; opts[key]=value
    changed=True; core.retro_run()
    assert abs(apply(0,.1,.1)[1]-.1*float(value))<.0001
    assert b'0.1' in allowed[keys[0]] and b'3.2' in allowed[keys[0]]
option('reset')
# Existing calibration files migrate from Gun 1; invalid Gun 1 falls back to Gun 2.
for fallback in (False, True):
    first='0' if fallback else '1'
    for f in save.glob('cap32-gun-*.cal'):
        f.write_text(f'CAP32_GUN_CAL_1\n{first} 768 272 1.25 0.2 0.1 0.1 1.5 0\n1 768 272 1.75 0.2 0.1 0.1 1.5 0\n')
    core.retro_unload_game(); assert core.retro_load_game(C.byref(game))
    expected=1.75 if fallback else 1.25
    assert float(opts[keys[0]])==expected
    for port in range(2): assert abs(apply(port,.2,0)[1]-(.2*expected+.1))<.0001
for f in save.glob('cap32-gun-*.cal'): f.write_text('CAP32_GUN_CAL_2\n1 768 272 nan 0 0 0 1 0\n')
core.retro_unload_game(); assert core.retro_load_game(C.byref(game))
assert abs(apply(1,.2,.3)[1]-.2)<.0001
assert float(opts[keys[0]])==1
print('PASS: legacy profile migration, corrupt profile fallback')
core.retro_unload_game(); core.retro_deinit()
