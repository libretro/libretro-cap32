/****************************************************************************
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
   { // 180 (1986).rmp
      {0x90b13e33},
      {
         {RETROK_3, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // 1942 (1986).rmp
      {0xab928e90},
      {
         {RETROK_3, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // 1943 (1988).rmp
      {0x8769d0b1},
      {
         {RETROK_RSHIFT, RETROK_RETURN, RETROK_2, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {2, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // 3D Grand Prix (1985).rmp
      {0xe1e61287},
      {
         {RETROK_SLASH, RETROK_SPACE, DB_KEEP, RETROK_SPACE, RETROK_EXCLAIM, RETROK_EXCLAIM, DB_KEEP, DB_KEEP, RETROK_QUOTE, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {4, 5, 0, 0, 0, 0, 0, 0, 6, 5, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // AMC - Astro Marine Corps.rmp
      {0xbee977b4, 0x8b94c44c},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Abadia Del Crimen (1988).rmp
      {0xaa05ed4a, 0xd37cf8e7},
      {
         {RETROK_z, RETROK_s, DB_CLEAN, RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_k, RETROK_l, RETROK_a, RETROK_n, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {7, 8, 0, 9, 0, 0, 10, 11, 12, 13, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Abbey Of Crime (2017).rmp
      {0xbf188ce3, 0x6e5f98ff},
      {
         {RETROK_z, RETROK_y, DB_CLEAN, RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_k, RETROK_l, RETROK_a, RETROK_n, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {14, 15, 0, 16, 0, 0, 17, 18, 19, 20, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Abu Simbel Profanation (1986).rmp
      {0x7037c06c},
      {
         {RETROK_q, DB_CLEAN, DB_CLEAN, RETROK_2, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_a, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {21, 0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Addams Family (1991).rmp
      {0x4c117ec5},
      {
         {DB_KEEP, RETROK_4, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // After Burner (1988).rmp
      {0x74458dd9},
      {
         {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // After The War (1989).rmp
      {0x8b94c44c, 0x62d17d76},
      {
         {RETROK_RALT, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {25, 0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Alien 8 (1985).rmp
      {0x160644e9},
      {
         {RETROK_q, RETROK_2, RETROK_3, RETROK_0, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_a, RETROK_1, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {27, 28, 0, 0, 0, 0, 17, 18, 19, 28, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Amsoft Loader (1985).rmp
      {0xeebfb943, 0xf2a3ca3e, 0xe0111415, 0x50f59666, 0xb49849a5, 0xc48122b1},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      2,
   },
   { // Angel Nieto Pole 500.rmp
      {0x6d278d3e},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Arcade Flight Simulator (1986).rmp
      {0x790a9ac2},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Archon (1988).rmp
      {0x551da885},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Archon II (1989).rmp
      {0x1fae9dcb},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Arkanoid (1987).rmp
      {0xe4bf51},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Army Moves (1986).rmp
      {0xe49fc694, 0x3db8fc89},
      {
         {RETROK_RALT, RETROK_2, DB_KEEP, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {29, 30, 0, 0, 0, 0, 0, 0, 31, 32, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Arquimedes XXI (2011).rmp
      {0xeca7624},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      3,
   },
   { // Aspar Grand Prix Master (1988).rmp
      {0x8714bccc, 0x36c70444},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Asterix and the Magic Carpet (1988).rmp
      {0x392fbb50, 0x33e38c87, 0x31d37ba2},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      4,
   },
   { // Auf Wiedersehen Monty (1987).rmp
      {0x5c74e277, 0xa32c19fb},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Aventura Original (1989).rmp
      {0xb0c8cc9e},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // BMX Simulator (1987).rmp
      {0x6cf1612c},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Babas Palace (2017).rmp
      {0x9cb1e0a7},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Barbarian (Palace Software) (1987).rmp
      {0xc761e738, 0x60e1f5f0},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Batman (1986).rmp
      {0x814d462},
      {
         {RETROK_KP1, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {28, 0, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Beach Buggy (1987).rmp
      {0x10bd9015},
      {
         {204, DB_KEEP, DB_KEEP, RETROK_2, DB_CLEAN, DB_KEEP, DB_KEEP, DB_KEEP, 200, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {35, 0, 33, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Beach Volley (1989).rmp
      {0xb25430db},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Beyond the Ice Palace (1988).rmp
      {0x2c2eb1e9},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Blood Valley (1987).rmp
      {0x7724c437},
      {
         {DB_KEEP, DB_KEEP, RETROK_j, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_j, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      5,
   },
   { // Boloncio (1986).rmp
      {0x5fc8032d},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_s, DB_CLEAN, DB_CLEAN, RETROK_KP1, RETROK_KP3, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 37, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Bomb Jack (1986).rmp
      {0x4bf042ca},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Boulder Dash (1985).rmp
      {0x46b5fe2c, 0xfed514e8, 0x474a9722, 0x5ebc61a1, 0x48fcdc1e},
      {
         {RETROK_1, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Bruce Lee (1984).rmp
      {0xcf73ebd6},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Bubble Bobble 1-2.rmp
      {0x95d9f222, 0xdc214555},
      {
         {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_q, RETROK_a, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {38, 0, 0, 39, 0, 0, 0, 0, 27, 40, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Budokan - The Martial Spirit (1990).rmp
      {0x480c9e65},
      {
         {DB_KEEP, RETROK_2, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 30, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Camelot Warriors (1986).rmp
      {0x2669032b},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Capitan Trueno (1989).rmp
      {0x823ac51},
      {
         {RETROK_5, RETROK_7, DB_KEEP, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_6, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {41, 42, 0, 0, 0, 0, 0, 0, 43, 44, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Carlos Sainz (1990).rmp
      {0x1913117d},
      {
         {RETROK_a, RETROK_SPACE, RETROK_2, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_q, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {45, 46, 0, 0, 0, 0, 0, 0, 47, 46, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Castle Master I-II.rmp
      {0x276db46d, 0x680a8d67, 0xd415b403},
      {
         {RETROK_SPACE, RETROK_c, RETROK_i, RETROK_RETURN, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_a, RETROK_p, RETROK_l, RETROK_r, RETROK_w},
         {48, 49, 50, 0, 0, 0, 0, 0, 35, 51, 52, 53, 54, 55},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Championship Sprint (1988).rmp
      {0x8564c2d7},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Chase H.Q (1989).rmp
      {0xac748c5b},
      {
         {RETROK_n, RETROK_z, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_k, RETROK_l, RETROK_a, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {5, 4, 0, 0, 0, 0, 0, 0, 0, 56, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Chessmaster 2000 (1990).rmp
      {0x87cce8},
      {
         {DB_KEEP, RETROK_y, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_n, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 57, 0, 0, 0, 0, 0, 0, 0, 58, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      6,
   },
   { // Chibi Akuma's Episode 1 Invasion (v1.666).rmp
      {0xd851fde0, 0x5c3cfc2c},
      {
         {RETROK_i, RETROK_RETURN, RETROK_p, DB_CLEAN, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_o, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {59, 3, 60, 0, 0, 0, 0, 0, 61, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Chuckie Egg (1985).rmp
      {0x12354538},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Chuckie Egg II (1985).rmp
      {0x11b9e71d},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Classic Axiens (1988).rmp
      {0x84e1c9fe},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_m, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      7,
   },
   { // Classic Invaders (1986).rmp
      {0xe698b5f6},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Clever and Smart (1987).rmp
      {0x3a5242b9},
      {
         {RETROK_n, DB_KEEP, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_m, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {62, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Cobra - Loriciels.rmp
      {0x504d8d95},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      8,
   },
   { // Colossus Mahjong (1987).rmp
      {0xaf529a6a},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      9,
   },
   { // Comando Tracer (1988).rmp
      {0x261b785},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 63, 0, 0, 64, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Combat School (1987).rmp
      {0x9e263bd7},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Commando - Space Invasion (1985).rmp
      {0x791eae64, 0x3b5ab32},
      {
         {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_s, RETROK_2, RETROK_w, RETROK_9, RETROK_0, RETROK_z, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {3, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Continental Circus (1989).rmp
      {0x7b2efe8b},
      {
         {RETROK_SPACE, RETROK_k, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_z, RETROK_x, RETROK_o, RETROK_k, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {5, 65, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Corsarios (1988).rmp
      {0x4dca3178},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Cutthroats (1984).rmp
      {0x5d21b329},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      10,
   },
   { // Cybernoid I-II.rmp
      {0x94d17670, 0x2c70c790},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Cyrus II (1985).rmp
      {0xe894c4e9},
      {
         {RETROK_b, RETROK_i, RETROK_m, RETROK_SPACE, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_RETURN, RETROK_ESCAPE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {66, 67, 68, 69, 0, 0, 0, 0, 70, 71, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      2,
   },
   { // Dark Fusion (1988).rmp
      {0x1d1e61db},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Dark Side (1988).rmp
      {0x596a99e0},
      {
         {RETROK_SPACE, RETROK_x, RETROK_i, RETROK_j, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_s, RETROK_p, RETROK_l, RETROK_r, RETROK_f},
         {48, 72, 50, 73, 0, 0, 0, 0, 35, 74, 52, 53, 75, 76},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Dead By Dawn (2014).rmp
      {0xaa88f4b9},
      {
         {RETROK_a, RETROK_u, RETROK_i, RETROK_SPACE, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_b, RETROK_LCTRL, RETROK_p, RETROK_l, RETROK_r, RETROK_f},
         {77, 78, 79, 48, 0, 0, 0, 0, 35, 54, 52, 53, 75, 76},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      11,
   },
   { // Deathsville (1986).rmp
      {0xb0dd3611},
      {
         {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_RETURN, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_RETURN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {80, 0, 0, 0, 0, 0, 0, 0, 81, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Defend or Die (1985).rmp
      {0xc147e40e},
      {
         {RETROK_TAB, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Deflektor (1987).rmp
      {0x61a1bea2},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Desperado (1987).rmp
      {0xcb00cc73},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Donkey Kong (1986).rmp
      {0x9b7c1f2},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_LEFTBRACKET, RETROK_RIGHTBRACKET, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 39, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Dragon Ninja (1988).rmp
      {0x8c5e3ea2},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Driller (1988).rmp
      {0x24a54215, 0x73cff695},
      {
         {RETROK_SPACE, RETROK_c, RETROK_i, RETROK_u, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_d, RETROK_p, RETROK_l, RETROK_r, RETROK_f},
         {48, 82, 50, 83, 0, 0, 0, 0, 35, 84, 52, 53, 75, 76},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Dynamite Dan (1985).rmp
      {0x550ac9f9},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_RETURN, DB_CLEAN, DB_CLEAN, RETROK_a, RETROK_s, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Emilio Butragueno Futbol (1988).rmp
      {0x4f86b3c8},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Enchanter (1984).rmp
      {0xcca0759},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      12,
   },
   { // Espada Sagrada (1990).rmp
      {0x5795493d},
      {
         {RETROK_b, RETROK_m, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_x, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {85, 86, 0, 0, 0, 0, 0, 0, 43, 87, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Exolon (1987).rmp
      {0x69219b4f},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Fernando Martin - Basket Master (1987).rmp
      {0x708351db, 0x471c5eac},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Freddy Hardest (1987).rmp
      {0x7a8fe41d, 0x874e9bb6},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Freddy Hardest in South Manhattan (1989).rmp
      {0x887d173f},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Gabrielle (1987).rmp
      {0x20dcbf76},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Galactic Plague (1984).rmp
      {0x674fbced},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Galactic Tomb (2018).rmp
      {0xfe13c50},
      {
         {RETROK_z, RETROK_s, DB_KEEP, RETROK_1, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_UP, RETROK_y, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {35, 88, 0, 0, 0, 0, 0, 0, 27, 89, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Game Over (1987).rmp
      {0x4d723f39, 0x8d8e3122},
      {
         {RETROK_RALT, RETROK_2, DB_KEEP, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {90, 30, 0, 0, 0, 0, 0, 0, 64, 32, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Garfield (1988).rmp
      {0x51e77955},
      {
         {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_q, DB_CLEAN, RETROK_a, RETROK_o, RETROK_p, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {91, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      13,
   },
   { // Gauntlet (1986).rmp
      {0x7c16d340},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Ghost n Goblins (1986).rmp
      {0x8ceb4fa3},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Ghostbusters (1985).rmp
      {0xc8149ef6, 0xe71162a4},
      {
         {RETROK_e, RETROK_1, RETROK_3, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {92, 32, 93, 94, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Ghouls n Ghosts (1989).rmp
      {0xc8d7f575},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Gnome Ranger (1987).rmp
      {0xb0a79287},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      14,
   },
   { // Goody (1987).rmp
      {0xbc911bd9, 0x795638dc},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Goonies (1986).rmp
      {0xb6e80000},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_5, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_RSHIFT, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 95, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Gryzor (1987).rmp
      {0xa2b01221},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Guerra de Gamber (2014) copy.rmp
      {0x3cba528d},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Gunfright (1985).rmp
      {0xfe56bf61},
      {
         {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_q, RETROK_v, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {64, 0, 0, 0, 0, 0, 17, 18, 19, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Hair Boy (2016).rmp
      {0xca1ccfa9},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_LEFT, RETROK_RIGHT, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Harrier Attack (1984).rmp
      {0x762a657},
      {
         {RETROK_SPACE, RETROK_RETURN, RETROK_3, RETROK_2, RETROK_KP_PERIOD, RETROK_KP_ENTER, RETROK_z, RETROK_x, RETROK_KP0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {96, 94, 93, 30, 0, 0, 97, 98, 99, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Head over Hells (1986).rmp
      {0xc613efa0},
      {
         {RETROK_KP1, RETROK_KP5, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_KP8, RETROK_SPACE, RETROK_SPACE, DB_KEEP, DB_KEEP},
         {28, 35, 0, 0, 0, 0, 0, 0, 0, 100, 101, 101, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Heartland (1986).rmp
      {0xc34b7449},
      {
         {RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      15,
   },
   { // Helter Skelter (1990).rmp
      {0x83c9554},
      {
         {DB_KEEP, DB_KEEP, RETROK_d, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      16,
   },
   { // Hero Quest (1991).rmp
      {0xec279622, 0x257af370, 0xafe22434, 0x386ab47d},
      {
         {DB_CLEAN, RETROK_0, DB_CLEAN, RETROK_SPACE, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_SPACE, RETROK_3, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 104, 0, 0, 0, 0, 0, 0, 105, 106, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Hitchhiker's Guide to the Galaxy, The (1984).rmp
      {0xa523527c},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      17,
   },
   { // Hustler (1985).rmp
      {0xe460536c},
      {
         {DB_KEEP, DB_KEEP, RETROK_2, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 107, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Ice-Breaker (1990).rmp
      {0x55b67795},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_s, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Ikari Warriors (1986).rmp
      {0x36d943ea, 0xe7df0e60, 0x3e88f3f7},
      {
         {RETROK_t, DB_KEEP, RETROK_2, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_3, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {109, 0, 110, 39, 0, 0, 0, 0, 0, 111, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Infidel (1986).rmp
      {0xdc3c19fc},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      18,
   },
   { // Island of Dr Destructo (1987).rmp
      {0xaf33badf},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Italia 90 (1990).rmp
      {0xaae46015},
      {
         {DB_KEEP, DB_KEEP, RETROK_2, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Ivan Ironman Super Off Road (1990).rmp
      {0xc3ce5121},
      {
         {DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_4, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 112, 0, 0, 0, 0, 0, 0, 0, 113, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // J.T 37 (1990).rmp
      {0xa1b3336e},
      {
         {DB_KEEP, DB_KEEP, RETROK_d, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      19,
   },
   { // Jack The Nipper I (1986).rmp
      {0xb06159a2},
      {
         {RETROK_RETURN, RETROK_2, RETROK_m, RETROK_SPACE, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_0, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {114, 115, 116, 0, 0, 0, 0, 0, 64, 117, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Jack The Nipper II (1987).rmp
      {0x2bec9bd},
      {
         {RETROK_o, DB_KEEP, RETROK_m, RETROK_SPACE, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {27, 0, 116, 0, 0, 118, 0, 0, 64, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Jarlac (2018).rmp
      {0x3b4ddeb5},
      {
         {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {35, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Jungle Book (1989).rmp
      {0xccc3a70b},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      20,
   },
   { // Klax (1990).rmp
      {0x96b53a41},
      {
         {RETROK_a, DB_CLEAN, DB_CLEAN, RETROK_0, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {119, 0, 0, 0, 0, 0, 0, 0, 120, 47, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Knight Lore (1984).rmp
      {0xa2d5c9db},
      {
         {RETROK_q, RETROK_2, RETROK_3, RETROK_0, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_a, RETROK_1, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {27, 28, 0, 0, 0, 0, 17, 18, 19, 28, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Lancelot (1988).rmp
      {0xc5ad3e3e},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      14,
   },
   { // Leather Goddesses of Phobos (1986).rmp
      {0xeb127e47},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      21,
   },
   { // Legend of Steel (2018).rmp
      {0x56701a62},
      {
         {RETROK_c, DB_CLEAN, DB_CLEAN, RETROK_r, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {121, 0, 0, 122, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Light Corridor (1990).rmp
      {0xb5dd9b44},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 0, 0, 0, 43, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Mad Mix Game (1988).rmp
      {0xc36d1efd, 0x97b8b3a9},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Manic Miner (1984).rmp
      {0xcd9dbf9c, 0x7a60b38e},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Match Day II (1987).rmp
      {0x35c029d5},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Mega Phoenix (1991).rmp
      {0xbd48490a},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Mikie (1986).rmp
      {0x48684cf4},
      {
         {RETROK_RETURN, DB_KEEP, DB_KEEP, RETROK_s, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {123, 0, 0, 0, 0, 0, 0, 0, 0, 124, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Miss Input 1 (2019).rmp
      {0xe8d15029},
      {
         {DB_CLEAN, RETROK_m, RETROK_2, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 125, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Miss Input 2 (2020).rmp
      {0x7c096b33},
      {
         {DB_CLEAN, RETROK_m, RETROK_2, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_w, RETROK_e, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 125, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Molecule Man (1986).rmp
      {0x532f7930},
      {
         {RETROK_b, DB_KEEP, DB_KEEP, RETROK_SPACE, RETROK_a, RETROK_z, RETROK_n, RETROK_m, RETROK_SPACE, RETROK_x, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {126, 0, 0, 0, 0, 0, 0, 0, 127, 128, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      22,
   },
   { // Moon Cresta (1986).rmp
      {0x6abcc9b6},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_6, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_RSHIFT, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Mortadelo y Filemon II (1989).rmp
      {0xeeae24a},
      {
         {DB_KEEP, RETROK_2, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 129, 0, 0, 0, 0, 0, 0, 43, 130, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Motos (1987).rmp
      {0x148447dc},
      {
         {DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Mundial de Futbol (1990).rmp
      {0x6f132709},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 131, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Murder on the Atlantic (1985).rmp
      {0xb40c5cb2},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      23,
   },
   { // Mutan Zone (1989).rmp
      {0x62a4deae},
      {
         {DB_KEEP, RETROK_1, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 132, 0, 0, 0, 0, 0, 0, 0, 133, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Myth (1989).rmp
      {0x4d8464d3},
      {
         {RETROK_j, RETROK_i, DB_CLEAN, RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_q, RETROK_w, RETROK_o, RETROK_i, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {134, 0, 0, 135, 0, 0, 0, 0, 136, 137, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Nanako Descends to Hell (2009).rmp
      {0x6c2cdcea},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_SPACE, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 138, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Navy Moves (1988).rmp
      {0xacf62400, 0x9299697c},
      {
         {RETROK_m, RETROK_2, DB_KEEP, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {139, 30, 0, 0, 0, 0, 0, 0, 35, 32, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Nemesis (1987).rmp
      {0xb35ed50},
      {
         {RETROK_RETURN, RETROK_m, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {140, 125, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Octoplex (1989).rmp
      {0x2519a761},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      24,
   },
   { // Oh Mummy (1984).rmp
      {0x2ff420da, 0x6abd6ce2},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Operation Wolf (1988).rmp
      {0x7c6c1e2e},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Phantis (1989).rmp
      {0xa60eba03},
      {
         {DB_KEEP, RETROK_1, DB_KEEP, RETROK_e, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 132, 0, 0, 0, 0, 0, 0, 35, 133, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Phantomas Saga Infinity (2006).rmp
      {0x73d96734},
      {
         {RETROK_DOWN, DB_CLEAN, DB_CLEAN, RETROK_s, DB_CLEAN, DB_CLEAN, RETROK_LEFT, RETROK_RIGHT, RETROK_UP, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {22, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Planetfall (1983).rmp
      {0x9af2061b},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      25,
   },
   { // Powerplay - cr (1986).rmp
      {0xf49d6612},
      {
         {RETROK_RETURN, RETROK_SPACE, RETROK_2, RETROK_l, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_ESCAPE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {141, 142, 0, 0, 0, 0, 0, 0, 0, 143, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Powerplay - org (1986).rmp
      {0x3ea9bb91},
      {
         {RETROK_RETURN, RETROK_SPACE, RETROK_3, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_ESCAPE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {141, 142, 0, 0, 0, 0, 0, 0, 0, 143, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      26,
   },
   { // Prince of Persia (1990).rmp
      {0xf964db5d, 0xa14a1e0f, 0xb9d14a6d, 0x18d230b1},
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_ESCAPE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 60, 0, 0, 0, 0, 0, 144, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Pro Tennis (1985).rmp
      {0x25af0923},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Rainbow Islands (1989).rmp
      {0xd11aa568},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Ramparts (1987).rmp
      {0x31f0ac5b},
      {
         {DB_KEEP, DB_KEEP, RETROK_1, RETROK_s, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Red Heat (1989).rmp
      {0xf2d804ab},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Red Sunset (2021).rmp
      {0x3803e3fc},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      27,
   },
   { // Renegade (1987).rmp
      {0xdf16b5d3},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Rescate Atlantida (1989).rmp
      {0x39e92a63, 0x10712a0c},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Rex (1989).rmp
      {0x6fbe410c},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      28,
   },
   { // Robocop (1989).rmp
      {0xb6017864, 0xda1142c4, 0xf70eee66},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Roland In The Caves - Pulga (1984).rmp
      {0x371eaac1, 0xf81c4337},
      {
         {RETROK_BACKQUOTE, DB_KEEP, RETROK_a, RETROK_g, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_z, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 0, 0, 0, 0, 0, 145, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Roland On The Ropes - Fred (1984).rmp
      {0x324cfc6, 0xc3997a3d},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Roland in Time (1985).rmp
      {0xaca0e0f},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Rygar (1987).rmp
      {0x558abe44},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 43, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Sabre Wulf (1985).rmp
      {0x36596d2c},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Samurai Warrior - The Battles of Usagi Yojimbo (1988).rmp
      {0xfbd21696},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Satan (1989).rmp
      {0xd66de504, 0xae64ea1d},
      {
         {DB_CLEAN, RETROK_1, RETROK_2, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_2, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 132, 0, 0, 0, 0, 0, 0, 35, 133, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Savage (1988).rmp
      {0xca6ffd7a, 0xae64ea1d},
      {
         {DB_CLEAN, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_2, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 112, 0, 0, 0, 0, 0, 0, 35, 146, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Secte Noire, La (1990).rmp
      {0x95b94081},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      29,
   },
   { // Sgrizam (1986).rmp
      {0x8c8a37bd},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Shadow Dancer (1991).rmp
      {0x20e80d12},
      {
         {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {147, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Shadow Maze - Celebration (2023).rmp
      {0x82d52c01},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      30,
   },
   { // Shinobi (1989).rmp
      {0x6f1e3eb0, 0xbd6c0af},
      {
         {RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Shinobu (2019).rmp
      {0x37ee404f},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Sir Fred (1986).rmp
      {0x92345cde},
      {
         {RETROK_z, DB_CLEAN, DB_CLEAN, RETROK_4, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_m, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {149, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Skate Crazy (1988).rmp
      {0xef7871dc, 0x5ac9f3f4},
      {
         {RETROK_SPACE, RETROK_1, DB_KEEP, RETROK_p, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 112, 0, 60, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Smash TV (1991).rmp
      {0xc18a84f9},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Sol Negro (1989).rmp
      {0xb0540671},
      {
         {DB_KEEP, RETROK_1, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 132, 0, 0, 0, 0, 0, 0, 0, 133, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Solomons Key (1986).rmp
      {0x29671a87},
      {
         {RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Sorcerer (1984)(Infocom)[CPM].rmp
      {0x636a30d8},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      31,
   },
   { // Space Hawks - Aguilas Del Espacio (1984).rmp
      {0xa041e903},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Space Moves (2020).rmp
      {0x49586e15},
      {
         {DB_KEEP, RETROK_2, DB_KEEP, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 152, 0, 0, 0, 0, 0, 0, 35, 153, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Space Smugglers (1989).rmp
      {0x99821ea9},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Spellbreaker (1985).rmp
      {0xd519f3ce},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      32,
   },
   { // Spitfire 40 (1985).rmp
      {0xdf6b3ee6},
      {
         {RETROK_b, RETROK_w, RETROK_m, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_q, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {154, 155, 156, 157, 0, 0, 0, 0, 0, 158, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      33,
   },
   { // Sport (1989).rmp
      {0xabd24f9a},
      {
         {RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RALT, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {159, 0, 0, 0, 0, 0, 0, 0, 105, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      34,
   },
   { // Stormlord II (1990).rmp
      {0x375d4353},
      {
         {RETROK_q, RETROK_1, DB_KEEP, RETROK_LALT, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {27, 112, 0, 60, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Super Skweek (1990).rmp
      {0x1993c727, 0x7a97eed},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Super Stuntman (1988).rmp
      {0x1115c8c8},
      {
         {RETROK_a, RETROK_SPACE, RETROK_ASTERISK, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_q, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {160, 35, 0, 0, 0, 0, 0, 0, 6, 35, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Suspended (1983).rmp
      {0x819d6ed7},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      35,
   },
   { // Tapper (1986).rmp
      {0xb699edb3},
      {
         {DB_CLEAN, RETROK_a, DB_CLEAN, RETROK_s, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_p, RETROK_e, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 161, 0, 0, 0, 0, 0, 0, 35, 162, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Target Renegade (1988).rmp
      {0xe4154a1f},
      {
         {DB_KEEP, DB_KEEP, RETROK_ESCAPE, RETROK_1, RETROK_q, RETROK_a, RETROK_k, RETROK_l, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 60, 39, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, 200, 201, 203, 202, 204, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 0, 163, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Techno Cop (1988).rmp
      {0xcbdafd77},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Teenage Mutant Hero Turtles (1990).rmp
      {0xca4f0b5, 0x8dd0cf27},
      {
         {RETROK_s, RETROK_RSHIFT, DB_KEEP, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {140, 164, 0, 0, 0, 0, 0, 0, 35, 165, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Thanatos (1986).rmp
      {0x827e83ac, 0x8d64a322, 0xf9fce391, 0x59d6aef1},
      {
         {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_4, RETROK_a, RETROK_z, RETROK_n, RETROK_m, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {0, 0, 0, 0, 0, 0, 0, 0, 166, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Three Weeks in Paradise (1985).rmp
      {0x60a4a40f},
      {
         {DB_KEEP, RETROK_1, DB_KEEP, RETROK_a, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 167, 0, 0, 0, 0, 0, 0, 0, 168, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Total Eclipse I-II (1988).rmp
      {0x41f8051c, 0xd8e6113b},
      {
         {RETROK_SPACE, RETROK_h, RETROK_i, RETROK_u, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_r, RETROK_p, RETROK_l, RETROK_s, RETROK_a},
         {48, 169, 50, 83, 0, 0, 0, 0, 35, 170, 52, 53, 171, 172},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Trantor The Last Stormtrooper (1987).rmp
      {0x3365f862},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Tres Luces de Glaurung (1986).rmp
      {0x3a5242b9},
      {
         {RETROK_n, DB_KEEP, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_m, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {62, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Trivial Pursuit (1988).rmp
      {0xc46c46de, 0x23c7634e, 0x668247d3},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Tuareg (1988).rmp
      {0x5665d718},
      {
         {DB_KEEP, RETROK_5, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_e, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 173, 0, 0, 0, 0, 0, 0, 43, 126, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Turbo Esprit (1986).rmp
      {0x37c07361, 0x8a357ad3},
      {
         {RETROK_m, DB_KEEP, DB_KEEP, RETROK_5, RETROK_s, RETROK_a, RETROK_j, RETROK_l, RETROK_k, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {156, 0, 0, 0, 0, 0, 0, 0, 35, 130, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Turbo Girl (1988).rmp
      {0x77c7c536, 0x1e737e6c},
      {
         {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Tyrann (1985).rmp
      {0x1a6f0fff},
      {
         {RETROK_c, RETROK_o, RETROK_SPACE, RETROK_RETURN, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_LALT, RETROK_n, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {174, 175, 0, 0, 0, 0, 0, 0, 114, 58, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      36,
   },
   { // Tyrann - es (1985).rmp
      {0x9ba80f9d},
      {
         {RETROK_c, RETROK_s, DB_KEEP, RETROK_SPACE, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_LALT, RETROK_n, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {176, 177, 0, 0, 0, 0, 0, 0, 114, 58, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      36,
   },
   { // Vendetta (1990).rmp
      {0xdfb023f8},
      {
         {RETROK_RETURN, RETROK_SPACE, DB_KEEP, RETROK_s, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {140, 178, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Vera Cruz Affair, The (1986).rmp
      {0xd5a4c3f2, 0x4f6b8ae6, 0xd276b5bc},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Victory Road (1988).rmp
      {0xaf4b1161},
      {
         {RETROK_z, RETROK_2, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
         {109, 146, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Vuelta al Mundo en 80 Dias (2022).rmp
      {0xd34f4df},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      37,
   },
   { // WEC Le Mans (1988).rmp
      {0x54c40e54},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // WWF Wrestlemania (1991).rmp
      {0x6533fb5},
      {
         {RETROK_RETURN, DB_KEEP, RETROK_RETURN, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Warlock (1987).rmp
      {0xa1b83838},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Way Of The Exploding Fist (1985).rmp
      {0x6fda459c},
      {
         {DB_KEEP, DB_KEEP, RETROK_CAPSLOCK, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_TAB, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 0, 179, 0, 0, 0, 0, 0, 0, 180, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // West Bank (1985).rmp
      {0x361c4a1e},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Winter Games (1986).rmp
      {0xba91af38, 0x4794cfb0},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Wishbringer (1985).rmp
      {0x5332c3ad},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      38,
   },
   { // Witness, The (1984).rmp
      {0xb16409a2},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      39,
   },
   { // Wonderboy (1987).rmp
      {0xb1026135},
      {
         {204, RETROK_BACKSPACE, RETROK_ESCAPE, RETROK_1, DB_CLEAN, DB_KEEP, DB_KEEP, DB_KEEP, 200, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {0, 125, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Worm Slickers (1991).rmp
      {0x4f36745},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      40,
   },
   { // Xanadu (2020).rmp
      {0xde97e3ab},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      41,
   },
   { // Xenon (1988).rmp
      {0x91f26a80, 0x34cd836c},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Xyphoes Fantasy (1991).rmp
      {0xdb70571b, 0xf0a1468e},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Yie Ar Kung-Fu.rmp
      {0x651cd158},
      {
         {RETROK_KP0, RETROK_KP7, DB_KEEP, RETROK_1, RETROK_KP8, RETROK_KP2, RETROK_KP4, RETROK_KP6, RETROK_KP0, RETROK_KP9, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
         {43, 181, 0, 0, 0, 0, 0, 0, 43, 181, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      0,
   },
   { // Zap t Balls (1992).rmp
      {0xe434e0db, 0x8559909a},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      1,
   },
   { // Zork I - The Great Underground Empire (1986).rmp
      {0xa02c3758},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      42,
   },
   { // Zork II - The Wizard of Frobozz (1986).rmp
      {0xe0240de6},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      43,
   },
   { // Zork III - The Dungeon Master (1986).rmp
      {0xa3fb554f},
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      44,
   },
};

char commands[] = {
   "nul\0"
   "|CPM\0"
   "RUN\"DISC\0"
   "RUN\"AXXI\0"
   "|CPM\n~~LOADER\0"
   "RUN\"BLOOD\0"
   "RUN\"CHESS\0"
   "RUN\"HIRISE\0"
   "RUN\"COBRA.\0"
   "MAHJONG\0"
   "CUTTHROA\0"
   "RUN\"DEAD\0"
   "ENCHANTE\0"
   "RUN\"GARFIELD\0"
   "RUN\"MENU\0"
   "RUN\"HEART\0"
   "RUN\"HELTER\0"
   "HITCHHIK\0"
   "INFIDEL\0"
   "RUN\"JT37\0"
   "RUN\"JUNGLE\0"
   "LEATHERG\0"
   "RUN\"MOLECULE\0"
   "RUN\"GAME\0"
   "RUN\"OCTOPLEX\0"
   "PLANETFA\0"
   "RUN\"ARCANA\n~~~~~~~~~~~~~L~QUIZ1`\0"
   "RUN\"SUNSET\0"
   "RUN\"REX\0"
   "RUN\"SECTE\0"
   "RUN\"INTRO\0"
   "SORCERER\0"
   "SPELLBRE\0"
   "RUN\"SPITFIRE\0"
   "RUN\"SPORT\0"
   "SUSPENDE\0"
   "RUN\"TYRANN\0"
   "RUN\"80DIAS\0"
   "WISHBRIN\0"
   "WITNESS\0"
   "RUN\"WORM\0"
   "RUN\"XANADU\0"
   "ZORK1\0"
   "ZORK2\0"
   "ZORK3\0"
};

char input_names[] = {
   "nul\0"
   "2 Players\0"
   "Roll Plane\0"
   "Bomb\0"
   "Break\0"
   "Gear\0"
   "Accelerate\0"
   "Mover Adso\0"
   "Decir Si\0"
   "Start / Dejar Objeto\0"
   "Girar Izquierda\0"
   "Girar Derecha\0"
   "Avanzar\0"
   "Decir No\0"
   "Send Adso Forward\0"
   "Answer Yes\0"
   "Start / Drop Object\0"
   "Turn Counterclockwise\0"
   "Turn Clockwise\0"
   "Move Forward\0"
   "Answer No\0"
   "Long Jump\0"
   "Short Jump\0"
   "Menu: Difficulty\0"
   "Change Speed\0"
   "Kick\0"
   "Punch\0"
   "Jump\0"
   "Carry\0"
   "Missile Ground\0"
   "Key 2\0"
   "Missile Air\0"
   "Key 1\0"
   "Start\0"
   "Carry And Jump\0"
   "Fire\0"
   "Joystick Select\0"
   "S / Start\0"
   "Bubble\0"
   "Start One Player\0"
   "[A]\0"
   "Captain\0"
   "Goliat\0"
   "Attack\0"
   "Crispin\0"
   "Speed Down\0"
   "Hand Brake\0"
   "Speed Up\0"
   "Change Pointer/Movement\0"
   "Crouch\0"
   "Information Load/Save\0"
   "Action/Press\0"
   "Look Up\0"
   "Look Down\0"
   "Run\0"
   "Walk\0"
   "Turbo\0"
   "Yes\0"
   "No\0"
   "Fire Left\0"
   "Pause\0"
   "Fire Right\0"
   "Select\0"
   "Down / Use\0"
   "Shoot\0"
   "Slow Down\0"
   "Back\0"
   "Invert Board\0"
   "Move First - Black Pieces\0"
   "Menu\0"
   "Select / Place\0"
   "2D / 3D\0"
   "Decrease Step\0"
   "Jet Pack\0"
   "Increase Step\0"
   "Rise\0"
   "Fall/Crouch\0"
   "Activate\0"
   "Turn Around\0"
   "Center View\0"
   "Pick-Up/Drop Object\0"
   "Jump / Enter Room\0"
   "Cancel Drill\0"
   "U Turn\0"
   "Drill\0"
   "Get/Drop Object\0"
   "Object Right\0"
   "Object Left\0"
   "Music / Sfx\0"
   "Fire 2\0"
   "Throw Bombs\0"
   "Kick / Action\0"
   "Key E\0"
   "Key 3\0"
   "Return / Confirm\0"
   "Change Character\0"
   "Drop Bomb\0"
   "Slow\0"
   "Fast\0"
   "Fire Rocket\0"
   "Swop\0"
   "Get And Jump\0"
   "Fire / Use Bed\0"
   "Password\0"
   "Key 0 / Menu\0"
   "Action\0"
   "Key 3 / Menu\0"
   "Game 2\0"
   "Game 1\0"
   "Toggle\0"
   "Joystick 1 Select\0"
   "Joystick 2 Select\0"
   "Press 1\0"
   "Press 4\0"
   "Open Door\0"
   "Take Slot 2\0"
   "Switch Music\0"
   "Take Slot 1\0"
   "Down/Take\0"
   "Drop Piece\0"
   "Throw Piece\0"
   "Checkpoint\0"
   "Restart\0"
   "Catch Multiple Hearts\0"
   "Push A Classmate\0"
   "Music Toggle\0"
   "Buy\0"
   "Pick Up\0"
   "Throw Bomb\0"
   "2\0"
   "1\0"
   "Coach Sub\0"
   "First Stage\0"
   "Second Stage\0"
   "Small Jump / Kick\0"
   "Start / Select Icon\0"
   "Action / Combo\0"
   "Long Jump / Punch\0"
   "Use\0"
   "Change Weapon\0"
   "Select Weapon\0"
   "Return\0"
   "Space\0"
   "Esc\0"
   "Time Left\0"
   "Jump Left\0"
   "Press 2\0"
   "Dog Attack\0"
   "Magic\0"
   "Take/Select Object\0"
   "Use Object\0"
   "Fireball\0"
   "Phase 2\0"
   "Phase 1\0"
   "Brakes\0"
   "Throttle Decrease\0"
   "Map\0"
   "Cockpick View\0"
   "Throttle Increase\0"
   "Throw Shot Or Jump\0"
   "Brake\0"
   "Menu: Arcade\0"
   "Menu: Expert\0"
   "Start Two Players\0"
   "Enter Wagon\0"
   "Special Features\0"
   "Flame\0"
   "Slot 1\0"
   "Slot 2\0"
   "Crouch/Walk\0"
   "Rest\0"
   "Change Step\0"
   "Change Angle\0"
   "Choose\0"
   "Camp / Rest\0"
   "Oui / Yes\0"
   "Camp\0"
   "Si\0"
   "Select Object\0"
   "Change Players\0"
   "Change Control\0"
   "Diagonal Jump\0"
};


// gen hash: 1739139292.4189138

