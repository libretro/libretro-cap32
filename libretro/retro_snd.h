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

#ifndef RETRO_SND_H__
#define RETRO_SND_H__

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// File helpers functions

enum retro_samples_snd
{
   SND_FDCMOTOR = 0,
   SND_FDCREAD  = 1,
   SND_FDCSEEK  = 2,
   SND_LAST     = 3
};

typedef enum
{
   ST_OFF = 0,
   ST_ON,
   ST_LOOP
} audio_status_t;

bool init_retro_snd(int16_t* pbuffer);
void free_retro_snd();

void retro_snd_cmd(int snd_type, audio_status_t new_status);
void retro_snd_mixer();

#endif
