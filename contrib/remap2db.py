#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
   Copyright (C) 2022 David Colmenero - D_Skywalk

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

from dataclasses import replace
import os, sys, time

db = {}
retro_key = {}

commands = []
input_names = []

input_prefix = 'input_player%i_key_%s'
hash_prefix = 'entry_hash_%i'
command_prefix = 'entry_command'
clean_prefix = 'entry_map_type'

RETRO_KEEP_KEY = 'DB_KEEP'
RETRO_CLEAN_KEY = 'DB_CLEAN'
RETRO_FIRST_LINE = 'enum retro_key'
RETRO_LAST_LINE = 'RETROK_LAST'

MODE_MIXED = 1
MODE_CLEAN = 2

retro_prefix = 'RETROK_'

MAX_HASH = 16
KEYS_MAP = ['b', 'y', 'select', 'start', 'up', 'down', 'left', 'right', 'a', 'x', 'l', 'r', 'l2', 'r2']

HEADER = """/****************************************************************************
 *  Caprice32 libretro port
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

#include <libretro.h>
#include "database.h"

// clean-cpc-db CRC database
t_file_entry dbinfo[] = {
"""
FOOTER = """
// gen hash: %s
"""

if sys.argv.__len__() != 3:
    print('Error: invalid args\nusage:', sys.argv[0], '<your-rmp-path> <your-libretro.h>')
    exit(1)

current_path = sys.argv[1]
lr_path = sys.argv[2]

if not os.path.isdir(current_path):
    print('Error: invalid rmp path.', current_path)
    exit(2)

if not os.path.isfile(lr_path):
    print('Error: libretro.h not found.', lr_path)
    exit(2)

file_list = [
    f for f in os.listdir(current_path)
        if os.path.isfile(os.path.join(current_path, f))
            and f[-4:] != '.off'
]

file_list.sort()

def map2text(o_Entry):
    map_text = ''
    for key in KEYS_MAP:
        # clean / keep
        if not o_Entry[key]:
            map_text += o_Entry['current_mode']
        # custom key/joy internal code
        elif o_Entry[key][0] == '$':
            map_text += o_Entry[key][1:]
        # retroarch keyboard code
        else:
            map_text += retro_key[o_Entry[key]]
        map_text += ', '
    return map_text[:-2]

def name2text(o_Entry):
    map_text = ''
    for key in KEYS_MAP:
        map_text += o_Entry[key + '_name'] if o_Entry[key + '_name'] else '-1'
        map_text += ', '
    return map_text[:-2]


def clean_value(text):
    if '#' in text:
        text = text.split('#')[0]
    if '"' in text:
        text = text.replace('"', '')
    if '\'' in text:
        text = text.replace('\'', '')
    return text.strip()

def file_get_value(l_File, s_Key):
    for line in l_File:
        if s_Key + ' ' in line:
            return clean_value(line.split('=')[1])

        if s_Key + '=' in line:
            return clean_value(line.split('=')[1])

def file_get_entry(l_File):
    entry = {}
    # get user player1 remap keys
    for key in KEYS_MAP:
        entry[key] = file_get_value(l_File, input_prefix % (1, key))
        name = file_get_value(l_File, input_prefix % (1, key) + '_name')
        if name:
            name = name.title()
            if not name in input_names:
                input_names.append(name)
            name = str(input_names.index(name))
        entry[key + '_name'] = name

    # get file CRC
    hash_list = []
    for number in range(1, MAX_HASH + 1):
        value = file_get_value(l_File, hash_prefix % (number))
        if value:
            value = int(value, 16)
            hash_list.append(hex(value))
    entry['hash_list'] = hash_list

    command = file_get_value(l_File, command_prefix)
    if command:
        command = command.upper() \
            .replace('[ENTER]', '\\n') \
            .replace('[QUOTE]', '\\"') \
            .replace('[WAIT]', '~')
        if not command in commands:
            commands.append(command)
        command = commands.index(command)
    entry['loader_command'] = command
    entry['current_mode'] = RETRO_CLEAN_KEY if file_get_value(l_File, clean_prefix) == 'clean' else RETRO_KEEP_KEY

    return entry

# get RMP database
for file in file_list:
    with open(os.path.join(current_path, file)) as f:
        file_lines = f.readlines()
        db[file] = file_get_entry(file_lines)

# get RETRO_K from includes
with open(lr_path) as lr:
    found = False
    file_lines = lr.readlines()
    for line in file_lines:
        if RETRO_FIRST_LINE in line:
            found = True
        if RETRO_LAST_LINE in line:
            break
        if found and retro_prefix in line:
            tmp = line.split('=')
            retro_key[tmp[1].replace(',', '').strip()] = tmp[0].strip()


# build final file
text = HEADER
for (key, entry) in db.items():
    # first line
    text += '   { // ' + key + '\n'
    text += '      {' + ', '.join(entry['hash_list']) + '},\n'
    text += '      {' + map2text(entry) + '},\n'
    text += '      {' + name2text(entry) + '},\n'
    text += '      ' + ( str(entry['loader_command']) if entry['loader_command'] != None else '-1') + ',\n'

    text += '   },\n'
text += '};\n\n'


text += 'char commands[] = {\n'
for cmd in commands:
   text += '   "' + cmd + '\\0"\n'
text += '};\n\n'

text += 'char input_names[] = {\n'
for name in input_names:
   text += '   "' + name + '\\0"\n'
text += '};\n\n'

text += FOOTER % ( str(time.time()) )

print(text)