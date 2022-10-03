/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2021)
 *  Copyright Daniel De Matteis (2012-2021)
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

#ifndef GFX_SOFTWARE_H__
#define GFX_SOFTWARE_H__

#include <stdint.h>
#include <stdbool.h>

union TPixel
{
   struct
   {
      #ifdef MSB_FIRST
      uint8_t b;
      uint8_t g;
      uint8_t r;
      #else
      uint8_t r;
      uint8_t g;
      uint8_t b;
      #endif
   };
   unsigned int colour;
};

union TColor
{
   struct
   {
      #ifdef MSB_FIRST
      unsigned short high;
      unsigned short low;
      #else
      unsigned short low;
      unsigned short high;
      #endif
   };
    unsigned int colour;
};

//*****************************************************************************
// Graph helpers functions

void draw_rect(uint32_t * buffer, int x, int y, int width, int height, uint32_t color);
void draw_text(uint32_t * buffer, int x, int y, const char * text, uint32_t color);
void draw_char(uint32_t * buffer, int x, int y, char chr_idx, uint32_t color);
void draw_image_linear(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size);
void draw_image_linear_blend(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size);
void draw_image_transparent(unsigned int * buffer, const unsigned int * img, int x, int y, unsigned int size);
void convert_image(unsigned int * buffer, const unsigned int * img, unsigned int size);

//void draw_line(uint32_t * buffer, int x, int y, int width, uint32_t color);
//void draw_image(uint32_t * buffer, uint32_t * img, int x, int y, int width, int height);
#endif
