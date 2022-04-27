#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
   Copyright (C) 2022 David Colmenero - D_Skywalk
   Caprice32 libretro unit-tests for clean-cpc-db

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   higher any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
"""

import sys, os, time
from zlib import crc32
from subprocess import Popen, PIPE

HEADER = """/****************************************************************************
 *  Caprice32 libretro unit-tests for clean-cpc-db
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2022)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/

#include <libretro-core.h>

typedef struct {
   uint32_t hash;
   char autorun[LOADER_MAX_SIZE];
   char filepath[512];
} t_test_entry;

// clean-cpc-db test database
t_test_entry cleancpcdb[] = {
"""
FOOTER = """
// gen hash: %s - %s"""

if sys.argv.__len__() != 4:
    print('Error: invalid args\nusage:', sys.argv[0], '<path/to/clean-cpc-db.dat> <path/to/test> <path/to/roms>')
    exit(1)

dat_path = sys.argv[1]
test_path = sys.argv[2]
rom_path = sys.argv[3]

if not os.path.isfile(dat_path):
    print('Error: dat file not found.', dat_path)
    exit(2)

def calculate_crc(fname):
    hash_crc = 0
    with open(fname, "rb") as dsk:
        data = dsk.read()
        hash_crc = "%X" % (crc32(data) & 0xffffffff)
    return int(hash_crc, 16)

def to_entry(filepath, crc, cmd):
    tmp = '   {'
    tmp += '0x%x, ' % crc
    tmp += '{"%s"}, ' % cmd.replace('"', "\\\"")
    tmp += '{"%s"} },\n' % filepath
    return tmp

def test_entry(gamename, rom, crc):
    filepath = os.path.join(rom_path, gamename, rom)
    calc_crc = calculate_crc(filepath)
    if calc_crc != crc:
        print("CRC ERROR!", calc_crc, crc)

    command = [test_path, filepath]
    process = Popen(command, stdout=PIPE)
    (output, _err) = process.communicate()
    exit_code = process.wait()
    if exit_code != 0:
        print(output, command)
        print("ERROR!", exit_code)
        exit(exit_code)

    result = str(output).split("[")[1].split("]")[0]
    return (filepath, crc, result)

gamename = ''
rom = ''
crc = ''

# build final file
text = HEADER
line = '<im-not-empty>'
f = open(dat_path, "r")

while(line):
    line = f.readline().strip()
    # print(line)
    if line[:4] == 'name':
        gamename = line.split('"')[1]
    if line[:4] == 'rom ':
        # only test first Side/Disk
        if '(side ' in line.lower() and '(side a)' not in line.lower():
            continue
        if '(disk ' in line.lower() and '(disk 1 side a)' not in line.lower():
            continue
        rom = line.split('"')[1]
        crc = int(line.split('crc')[1].split('md5')[0].strip(), 16)
    if line == ')' and gamename and rom:
        (filepath, crc, result) = test_entry(gamename, rom, crc)
        text += to_entry(filepath, crc, result)

f.close()
text += "};\n"
text += FOOTER % (str(time.time()), os.path.basename(sys.argv[0]))
print(text)

