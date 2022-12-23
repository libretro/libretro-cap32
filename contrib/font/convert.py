#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
   Simple PNG to bit-font converter

   Copyright (C) 2020 David Colmenero - D_Skywalk
    http://david.dantoine.org

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
import sys
from PIL import Image
import numpy as np

BLACK = [0, 0, 0]

def is_black(data, mode):
    if mode == "RGBA":
        (r, g, b, a) = data
        return [r, g, b] == BLACK
    else:
        (r, g, b) = data
        return [r, g, b] == BLACK


def open(filename):
    img = Image.open(filename)
    im_array = np.array(img)
    (width, height) = img.size
    mode = img.mode
    # print("pix", is_black(im_array[0][0], mode))

    print ("static unsigned char bFont[] = {")

    for y in range(0, height):
        data = 0
        for x in range(0, width):
            if not is_black(im_array[y][x], mode):
                data |= 1
            data <<= 1
        data >>= 1
        print("0x%02X, " % data, end='')
        if y % 16 == 0:
            print("")

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("error $> convert.py image.png")
    else:
        open(sys.argv[1])


