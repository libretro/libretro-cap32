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

#ifndef GFX_SOFTWARE_H__
#define GFX_SOFTWARE_H__

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// Graph helpers functions

void draw_line(PIXEL_TYPE * buffer, int x, int y, int width, PIXEL_TYPE color);
void draw_rect(PIXEL_TYPE * buffer, int x, int y, int width, int height, PIXEL_TYPE color);
void draw_text(PIXEL_TYPE * buffer, int x, int y, const char * text, PIXEL_TYPE color);
void draw_char(PIXEL_TYPE * buffer, int x, int y, char chr_idx, PIXEL_TYPE color);
void draw_image(PIXEL_TYPE * buffer, PIXEL_TYPE * img, int x, int y, int width, int height);
void draw_image_linear(PIXEL_TYPE * buffer, PIXEL_TYPE * img, int x, int y, unsigned int size);
void convert_image(PIXEL_TYPE * buffer, const unsigned int * img, unsigned int size);
PIXEL_TYPE convert_color (unsigned int color);
#endif
