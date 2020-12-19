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


#ifndef RETRO_UI_H__
#define RETRO_UI_H__

typedef enum
{
   UI_DISABLED  = 0,
   UI_KEYBOARD  = 1,
   UI_MENU      = 2,
   UI_STATUSBAR = 3,
   UI_LED       = 4,
} retro_commands_ui_t;

void retro_ui_init(void);
void retro_ui_free(void);

void retro_ui_set_status(retro_commands_ui_t cmd, bool value);
void retro_ui_toggle_status(retro_commands_ui_t cmd);
void retro_ui_process();

void retro_show_statusbar();
void retro_ui_update_text();
void retro_ui_set_led(bool value);

#endif