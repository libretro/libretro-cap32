#!/usr/bin/env python3
"""Synthetic DATA disks: python3 test-autorun-bin.py /path/to/native/core."""
import ctypes
import pathlib
import sys
import tempfile

core = ctypes.CDLL(str(pathlib.Path(sys.argv[1]).resolve()))
buffer = ctypes.create_string_buffer(128 * 1024)
ctypes.c_void_p.in_dll(core, 'pbGPBuffer').value = ctypes.addressof(buffer)
drive = ctypes.c_byte.in_dll(core, 'driveA')
core.dsk_load.argtypes = [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_char]
core.loader_run.argtypes = [ctypes.c_void_p]


def disk(names=('DATA.BIN', 'START.BIN'), starts=(0, 0x4000),
         bad_header=None, load=0x4000, length=128, bad_block=False,
         headerless=False, system=False, interleave=True, hidden=False, plus3=None, bad_plus3=False, bad_signature=False, empty_tracks=False):
    logical = bytearray(40 * 9 * 512)
    logical[:2048] = b'\xe5' * 2048
    for i, (name, start) in enumerate(zip(names, starts)):
        stem, ext = name.split('.')
        raw = stem.ljust(8).encode() + ext.ljust(3).encode()
        entry = bytearray(32)
        entry[1:12] = raw
        entry[15:17] = bytes([2, i + 2])
        if bad_block and i == 0:
            entry[16] = 255
        if hidden and i == 1:
            entry[10] |= 128
        logical[i * 32:(i + 1) * 32] = entry
        h = bytearray(128)
        h[1:12] = raw
        h[18] = 2
        h[21:23] = load.to_bytes(2, 'little')
        h[24:26] = length.to_bytes(2, 'little')
        h[26:28] = start.to_bytes(2, 'little')
        h[67:69] = sum(h[:67]).to_bytes(2, 'little')
        if bad_header == i:
            h[67] ^= 1
        if headerless and i == 0:
            h = bytearray(128)
        if plus3 == i:
            h = bytearray(128)
            h[:9] = b'PLUS3DOS\x1a'
            h[9] = 1
            h[11:15] = (256).to_bytes(4, 'little')
            if bad_signature:
                h[8] = 0
            h[127] = sum(h[:127]) & 255
            if bad_plus3:
                h[127] ^= 1
        logical[(i + 2) * 1024:(i + 2) * 1024 + 128] = h
    result = bytearray(256)
    result[:8] = b'MV - CPC'
    result[48:50] = bytes([40, 1])
    result[50:52] = (4864).to_bytes(2, 'little')
    order = [0, 5, 1, 6, 2, 7, 3, 8, 4] if interleave else list(range(9))
    for t in range(40):
        h = bytearray(256)
        h[:10] = b'Track-Info'
        h[16] = t
        h[20:22] = bytes([2, 9])
        for i, sector in enumerate(order):
            h[24 + i * 8:28 + i * 8] = bytes([t, 0, (0x41 if system else 0xc1) + sector, 2])
        result += h
        for sector in order:
            source = (t - (2 if system else 0)) * 9 + sector
            result += logical[source * 512:(source + 1) * 512] if source >= 0 else bytes(512)
    if empty_tracks:
        result[:34] = b'EXTENDED CPC DSK File\r\nDisk-Info\r\n'
        result[48] = 99
        result[50:52] = bytes(2)
        result[52:92] = bytes([19]) * 40
        for track in range(40):
            for sector in range(9):
                offset = 256 + track * 4864 + 24 + sector * 8 + 6
                result[offset:offset + 2] = (512).to_bytes(2, 'little')
    return result


cases = [
    ('plus3-empty-tracks', {'names': ('DISK.', 'MENU.BAS'), 'plus3': 0, 'empty_tracks': True}, 'MENU.BAS'),
    ('plus3-disk', {'names': ('DISK.', 'MENU.BAS'), 'plus3': 0}, 'MENU.BAS'),
    ('plus3-bad-checksum', {'names': ('DISK.', 'MENU.BAS'), 'plus3': 0, 'bad_plus3': True}, 'DISK.'),
    ('plus3-bad-signature', {'names': ('DISK.', 'MENU.BAS'), 'plus3': 0, 'bad_signature': True}, 'DISK.'),
    ('plus3-basic', {'names': ('FIRST.BAS', 'MENU.BAS'), 'plus3': 0}, 'MENU.BAS'),
    ('plus3-bin', {'names': ('FIRST.BIN', 'MENU.BIN'), 'plus3': 0}, 'MENU.BIN'),
    ('plus3-hidden', {'names': ('FIRST.BIN', 'DISK.'), 'plus3': 1, 'hidden': True}, 'FIRST.BIN'),
    ('plus3-system-unchanged', {'names': ('DISK.', 'MENU.BAS'), 'plus3': 0, 'system': True}, 'DISK.'),
    ('prefer-entry-point', {}, 'START.BIN'),
    ('physical-sector-order', {'interleave': False}, 'START.BIN'),
    ('already-startable', {'starts': (0x4000, 0x4000)}, 'DATA.BIN'),
    ('unknown-first-header', {'bad_header': 0}, 'DATA.BIN'),
    ('invalid-alternative', {'bad_header': 1}, 'DATA.BIN'),
    ('no-entry-points', {'starts': (0, 0)}, 'DATA.BIN'),
    ('outside-loaded-range', {'starts': (0, 0x5000)}, 'DATA.BIN'),
    ('wrapped-range', {'load': 0xff80, 'length': 256, 'starts': (0, 0xff80)}, 'DATA.BIN'),
    ('missing-block', {'bad_block': True}, 'DATA.BIN'),
    ('headerless', {'headerless': True}, 'DATA.BIN'),
    ('hidden-alternative', {'hidden': True}, 'DATA.BIN'),
    ('single-file', {'names': ('DATA.BIN',), 'starts': (0,)}, 'DATA.BIN'),
    ('disc-priority', {'names': ('DISC.BIN', 'START.BIN')}, 'DISC.BIN'),
    ('basic-priority', {'names': ('DATA.BIN', 'START.BAS')}, 'START.BAS'),
    ('empty-extension-priority', {'names': ('DATA.BIN', 'START.')}, 'START.'),
    ('system-fallback', {'system': True}, 'DATA.BIN'),
]
with tempfile.TemporaryDirectory() as directory:
    for name, options, expected in cases:
        path = pathlib.Path(directory) / (name + '.dsk')
        path.write_bytes(disk(**options))
        assert core.dsk_load(str(path).encode(), ctypes.addressof(drive), b'A') == 0
        command = ctypes.create_string_buffer(256)
        core.loader_run(command)
        assert command.value == ('RUN"' + expected).encode(), (name, command.value)
        print('PASS', name)
