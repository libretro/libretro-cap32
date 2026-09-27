#!/usr/bin/env python3
"""Run against a native shared core: python3 test-dsk-overdump.py CORE_PATH."""
import ctypes
import pathlib
import sys
import tempfile

core = ctypes.CDLL(str(pathlib.Path(sys.argv[1]).resolve()))
buffer = ctypes.create_string_buffer(128 * 1024)
ctypes.c_void_p.in_dll(core, 'pbGPBuffer').value = ctypes.addressof(buffer)
drive = ctypes.c_byte.in_dll(core, 'driveA')
core.dsk_load.argtypes = [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_char]
core.dsk_load.restype = ctypes.c_int


def disk(declared, complete, sides=1, tail=b''):
    header = bytearray(256)
    header[:8] = b'MV - CPC'
    header[48:50] = bytes([declared, sides])
    header[50:52] = (768).to_bytes(2, 'little')
    result = header
    for track in range(complete):
        for side in range(sides):
            h = track_header(track, side)
            result += h + bytes([track]) * 512
    return result + tail


def track_header(track, side=0):
    h = bytearray(256)
    h[:10] = b'Track-Info'
    h[16:18] = bytes([track, side])
    h[20:22] = bytes([2, 1])
    h[24:28] = bytes([track, side, 1, 2])
    return h


cases = [
    ('complete-40', disk(40, 40), 0),
    ('complete-42', disk(42, 42), 0),
    ('extra-tracks-absent', disk(42, 40), 0),
    ('extra-header-only', disk(42, 40, tail=track_header(40)), 0),
    ('one-extra-complete', disk(42, 41), 0),
    ('partial-extra-header', disk(42, 40, tail=track_header(40)[:100]), 21),
    ('partial-extra-sector', disk(42, 40, tail=track_header(40) + b'x'), 21),
    ('missing-normal-track', disk(40, 39), 21),
    ('excessive-missing-tracks', disk(84, 40), 21),
    ('two-sided-complete-cylinders', disk(42, 40, 2), 0),
    ('two-sided-partial-cylinder', disk(42, 40, 2, track_header(40) + bytes(512)), 21),
]
with tempfile.TemporaryDirectory() as directory:
    for name, data, expected in cases:
        path = pathlib.Path(directory) / (name + '.dsk')
        path.write_bytes(data)
        result = core.dsk_load(str(path).encode(), ctypes.addressof(drive), b'A')
        assert result == expected, (name, result, expected)
        print('PASS', name)
