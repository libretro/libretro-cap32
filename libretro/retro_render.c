/* Copyright (C) 2020
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

#include "gfx/software.h"
#include "retro_render.h"
#include "assets/assets.h"

//unsigned int number = 0;

void render_rect(PIXEL_TYPE * buffer, mu_Rect rect, mu_Color color)
{
   //printf("%u rect: %ix%i %i-%i\n", number, rect.x, rect.y, rect.w, rect.h);
   draw_rect(buffer, rect.x, rect.y, rect.w, rect.h, RGB2COLOR(color.r, color.g, color.b));
}

void render_text(PIXEL_TYPE * buffer, const char *text, mu_Vec2 pos, mu_Color color)
{
   draw_text(buffer, pos.x, pos.y, text, RGB2COLOR(color.r, color.g, color.b));
}

void render_icon(PIXEL_TYPE * buffer, int id, mu_Rect rect, mu_Color color)
{
   char chr = 0;
   int x, y;
   x = rect.x + FNT_CHAR_WIDTH / 4;
   y = rect.y + FNT_CHAR_HEIGHT / 4;

   switch (id)
   {
      case MU_ICON_CLOSE:		chr = 'X'; break;
      case MU_ICON_CHECK:		chr = 'X'; break;
      case MU_ICON_COLLAPSED:	chr = '>'; break;
      case MU_ICON_EXPANDED:	chr = 'v'; break;
      //case MU_ICON_RESIZE:	chr = '+'; break;
  }

  draw_rect(buffer, rect.x, rect.y, rect.w, rect.h, RGB2COLOR(52, 25, 52));
  draw_char(buffer, x, y, chr, RGB2COLOR(color.r, color.g, color.b));
}

// unneed it
void render_clip(PIXEL_TYPE * buffer, mu_Rect rect)
{}

int text_width(mu_Font font, const char *text, int len)
{
  if (len == -1) { len = strlen(text); }
  return len * 8 * EMULATION_SCALE;
}

int text_height(mu_Font font)
{
  return 8 * EMULATION_SCALE;
}
