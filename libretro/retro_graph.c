/* Copyright (C) 2018
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>
#include <libretro-core.h>

// FIXME PIXEL_BYTES

void DrawLine(unsigned int * buffer, int x, int y, int width, unsigned int color)
{
   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   while (width--) {
      *(buffer++) = color;
   }
}

void DrawRect(unsigned int * buffer, int x, int y, int width, int height, unsigned int color)
{
   buffer = (buffer + x) + (y * EMULATION_SCREEN_WIDTH);
   while (height--)
   {
      int loop_width = width;
      while (loop_width--) {
         *(buffer++) = color;
      }
      buffer += EMULATION_SCREEN_WIDTH - width;
   }
}
