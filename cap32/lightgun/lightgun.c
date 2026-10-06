/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2023)
 *  - original header -
 *  Pituka - Nintendo Wii/Gamecube Port
 *  (c) Copyright 2008-2009 David Colmenero (aka D_Skywalk)
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>
#include <libretro-core.h>

#include "gfx/software.h"
#include "gfx/video.h"
#include "assets/assets.h"

#include "cap32.h"
#include "lightgun.h"
#include "retro_gun.h"

extern uint32_t * video_buffer;


unsigned lightgun_luminance(uint32_t pixel)
{
   unsigned r, g, b;

   switch (retro_video.depth) {
      case DEPTH_16BPP:
         r = (pixel >> 11) & 0x1F; r = (r << 3) | (r >> 2);
         g = (pixel >> 5)  & 0x3F; g = (g << 2) | (g >> 4);
         b = pixel         & 0x1F; b = (b << 3) | (b >> 2);
         break;

      case DEPTH_8BPP:
         r = ((pixel >> 5) & 0x07) * 36;
         g = ((pixel >> 2) & 0x07) * 36;
         b = (pixel & 0x03) * 85;
         break;

      default:
         r = (pixel >> 16) & 0xFF;
         g = (pixel >> 8)  & 0xFF;
         b = pixel         & 0xFF;
         break;
   }

   return (299 * r) + (587 * g) + (114 * b);
}

unsigned int lightgun_get_screen(int x, int y)
{
   if ((unsigned)x >= (unsigned)EMULATION_SCREEN_WIDTH || 
       (unsigned)y >= (unsigned)EMULATION_SCREEN_HEIGHT)
      return 0;

   int index = y * EMULATION_SCREEN_WIDTH + x - 1;

   switch (retro_video.depth) {
      case DEPTH_8BPP:
         return ((uint8_t *)video_buffer)[index];
      case DEPTH_16BPP:
         return ((uint16_t *)video_buffer)[index];
      default:
         return video_buffer[index];
   }
}
