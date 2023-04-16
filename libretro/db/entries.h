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
   { // 1942 (1986).rmp
      {0xab928e90},
      {RETROK_3, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // 1943 (1988).rmp
      {0x8769d0b1},
      {RETROK_RSHIFT, RETROK_RETURN, RETROK_2, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {0, 1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1},
      -1,
   },
   { // 3D Grand Prix (1985).rmp
      {0xe1e61287},
      {RETROK_SLASH, RETROK_SPACE, DB_KEEP, RETROK_SPACE, RETROK_EXCLAIM, RETROK_EXCLAIM, DB_KEEP, DB_KEEP, RETROK_QUOTE, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {2, 3, -1, -1, -1, -1, -1, -1, 4, 3, -1, -1, -1, -1},
      -1,
   },
   { // AMC - Astro Marine Corps.rmp
      {0xbee977b4, 0x8b94c44c},
      {RETROK_SPACE, RETROK_q, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_m, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Abadia Del Crimen (1988).rmp
      {0xaa05ed4a, 0xd37cf8e7},
      {RETROK_z, RETROK_s, DB_CLEAN, RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_k, RETROK_l, RETROK_a, RETROK_n, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {5, 6, -1, 7, -1, -1, 8, 9, 10, 11, -1, -1, -1, -1},
      0,
   },
   { // Abbey Of Crime (2017).rmp
      {0xbf188ce3, 0x6e5f98ff},
      {RETROK_z, RETROK_y, DB_CLEAN, RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_k, RETROK_l, RETROK_a, RETROK_n, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {12, 13, -1, 14, -1, -1, 15, 16, 17, 18, -1, -1, -1, -1},
      0,
   },
   { // Abu Simbel Profanation (1986).rmp
      {0x7037c06c},
      {RETROK_q, DB_CLEAN, DB_CLEAN, RETROK_2, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_a, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {19, -1, -1, -1, -1, -1, -1, -1, 20, -1, -1, -1, -1, -1},
      -1,
   },
   { // Addams Family (1991).rmp
      {0x4c117ec5},
      {DB_KEEP, RETROK_4, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 21, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // After Burner (1988).rmp
      {0x74458dd9},
      {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // After The War (1989).rmp
      {0x8b94c44c, 0x62d17d76},
      {RETROK_RALT, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {23, -1, -1, -1, -1, -1, -1, -1, 24, -1, -1, -1, -1, -1},
      -1,
   },
   { // Alien 8 (1985).rmp
      {0x160644e9},
      {RETROK_q, RETROK_2, RETROK_3, RETROK_0, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_a, RETROK_1, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {25, 26, -1, -1, -1, -1, 15, 16, 17, 26, -1, -1, -1, -1},
      -1,
   },
   { // Angel Nieto Pole 500.rmp
      {0x6d278d3e},
      {DB_KEEP, RETROK_DOWN, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Archon (1988).rmp
      {0x551da885},
      {RETROK_SPACE, RETROK_RIGHT, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_LEFT, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Archon II (1989).rmp
      {0x1fae9dcb},
      {RETROK_SPACE, RETROK_3, RETROK_0, RETROK_5, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_4, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Arkanoid (1987).rmp
      {0xe4bf51},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, RETROK_z, RETROK_x, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Army Moves (1986).rmp
      {0xe49fc694, 0x3db8fc89},
      {RETROK_RALT, RETROK_2, DB_KEEP, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {27, 28, -1, -1, -1, -1, -1, -1, 29, 30, -1, -1, -1, -1},
      -1,
   },
   { // Aspar Grand Prix Master (1988).rmp
      {0x8714bccc, 0x36c70444},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Asterix and the Magic Carpet (1988).rmp
      {0x392fbb50, 0x33e38c87, 0x31d37ba2},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      1,
   },
   { // Auf Wiedersehen Monty (1987).rmp
      {0x5c74e277, 0xa32c19fb},
      {RETROK_LCTRL, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Aventura Original (1989).rmp
      {0xb0c8cc9e},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      0,
   },
   { // BMX Simulator (1987).rmp
      {0x6cf1612c},
      {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_s, RETROK_f, RETROK_c, RETROK_z, RETROK_x, RETROK_f, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Barbarian (Palace Software) (1987).rmp
      {0xc761e738, 0x60e1f5f0},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Batman (1986).rmp
      {0x814d462},
      {RETROK_KP1, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {26, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, -1},
      -1,
   },
   { // Beach Buggy (1987).rmp
      {0x10bd9015},
      {204, DB_KEEP, DB_KEEP, RETROK_2, DB_CLEAN, DB_KEEP, DB_KEEP, DB_KEEP, 200, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {33, -1, 31, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Beach Volley (1989).rmp
      {0xb25430db},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_p, RETROK_i, RETROK_k, RETROK_j, RETROK_l, RETROK_p, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Beyond the Ice Palace (1988).rmp
      {0x2c2eb1e9},
      {RETROK_SPACE, RETROK_y, DB_KEEP, RETROK_y, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_p, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Boloncio (1986).rmp
      {0x5fc8032d},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_s, DB_CLEAN, DB_CLEAN, RETROK_KP1, RETROK_KP3, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, 34, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Bomb Jack (1986).rmp
      {0x4bf042ca},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, RETROK_q, RETROK_a, RETROK_n, RETROK_m, RETROK_x, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Boulder Dash (1985).rmp
      {0x46b5fe2c, 0xfed514e8, 0x474a9722, 0x5ebc61a1, 0x48fcdc1e},
      {RETROK_1, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Bruce Lee (1984).rmp
      {0xcf73ebd6},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Bubble Bobble 1-2.rmp
      {0x95d9f222, 0xdc214555},
      {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_q, RETROK_a, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {35, -1, -1, 36, -1, -1, -1, -1, 25, 37, -1, -1, -1, -1},
      -1,
   },
   { // Budokan - The Martial Spirit (1990).rmp
      {0x480c9e65},
      {DB_KEEP, RETROK_2, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 28, -1, -1, -1, -1, -1, -1, -1, 30, -1, -1, -1, -1},
      -1,
   },
   { // Camelot Warriors (1986).rmp
      {0x2669032b},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_j, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Carlos Sainz (1990).rmp
      {0x1913117d},
      {RETROK_a, RETROK_SPACE, RETROK_2, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_q, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {38, 39, -1, -1, -1, -1, -1, -1, 40, 39, -1, -1, -1, -1},
      -1,
   },
   { // Castle Master I-II.rmp
      {0x276db46d, 0x680a8d67, 0xd415b403},
      {RETROK_SPACE, RETROK_c, RETROK_i, RETROK_RETURN, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_a, RETROK_p, RETROK_l, RETROK_r, RETROK_w},
      {41, 42, 43, -1, -1, -1, -1, -1, 33, 44, 45, 46, 47, 48},
      -1,
   },
   { // Championship Sprint (1988).rmp
      {0x8564c2d7},
      {RETROK_SPACE, DB_KEEP, RETROK_1, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Chase H.Q (1989).rmp
      {0xac748c5b},
      {RETROK_n, RETROK_z, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_k, RETROK_l, RETROK_a, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {3, 2, -1, -1, -1, -1, -1, -1, -1, 49, -1, -1, -1, -1},
      -1,
   },
   { // Chibi Akuma's Episode 1 Invasion (v1.666).rmp
      {0xd851fde0, 0x5c3cfc2c},
      {RETROK_i, RETROK_RETURN, RETROK_p, DB_CLEAN, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_o, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {50, 1, 51, -1, -1, -1, -1, -1, 52, -1, -1, -1, -1, -1},
      -1,
   },
   { // Chuckie Egg (1985).rmp
      {0x12354538},
      {DB_KEEP, RETROK_1, DB_KEEP, RETROK_s, RETROK_a, RETROK_z, RETROK_COMMA, RETROK_PERIOD, RETROK_SPACE, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Chuckie Egg II (1985).rmp
      {0x11b9e71d},
      {DB_KEEP, RETROK_1, DB_KEEP, RETROK_p, RETROK_a, RETROK_z, RETROK_COMMA, RETROK_PERIOD, RETROK_SPACE, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Classic Invaders (1986).rmp
      {0xe698b5f6},
      {RETROK_a, DB_CLEAN, DB_CLEAN, RETROK_RETURN, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_RETURN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      0,
   },
   { // Clever and Smart (1987).rmp
      {0x3a5242b9},
      {RETROK_n, DB_KEEP, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_m, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {53, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Cobra - Loriciels.rmp
      {0x504d8d95},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      2,
   },
   { // Colossus Mahjong (1987).rmp
      {0xaf529a6a},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      3,
   },
   { // Comando Tracer (1988).rmp
      {0x261b785},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, 54, -1, -1, 55, -1, -1, -1, -1, -1},
      -1,
   },
   { // Combat School (1987).rmp
      {0x9e263bd7},
      {RETROK_SPACE, RETROK_RETURN, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Commando - Space Invasion (1985).rmp
      {0x791eae64, 0x3b5ab32},
      {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_s, RETROK_2, RETROK_w, RETROK_9, RETROK_0, RETROK_z, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {1, -1, -1, -1, -1, -1, -1, -1, 55, -1, -1, -1, -1, -1},
      -1,
   },
   { // Continental Circus (1989).rmp
      {0x7b2efe8b},
      {RETROK_SPACE, RETROK_k, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_z, RETROK_x, RETROK_o, RETROK_k, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {3, 56, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1},
      -1,
   },
   { // Corsarios (1988).rmp
      {0x4dca3178},
      {DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Cutthroats (1984).rmp
      {0x5d21b329},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      4,
   },
   { // Cybernoid I-II.rmp
      {0x94d17670, 0x2c70c790},
      {DB_KEEP, DB_KEEP, RETROK_3, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Dark Fusion (1988).rmp
      {0x1d1e61db},
      {RETROK_s, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Dark Side (1988).rmp
      {0x596a99e0},
      {RETROK_SPACE, RETROK_x, RETROK_i, RETROK_j, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_s, RETROK_p, RETROK_l, RETROK_r, RETROK_f},
      {41, 57, 43, 58, -1, -1, -1, -1, 33, 59, 45, 46, 60, 61},
      -1,
   },
   { // Deathsville (1986).rmp
      {0xb0dd3611},
      {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_RETURN, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_RETURN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {62, -1, -1, -1, -1, -1, -1, -1, 63, -1, -1, -1, -1, -1},
      -1,
   },
   { // Desperado (1987).rmp
      {0xcb00cc73},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Donkey Kong (1986).rmp
      {0x9b7c1f2},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_LEFTBRACKET, RETROK_RIGHTBRACKET, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, 36, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Dragon Ninja (1988).rmp
      {0x8c5e3ea2},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Driller (1988).rmp
      {0x24a54215, 0x73cff695},
      {RETROK_SPACE, RETROK_c, RETROK_i, RETROK_u, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_d, RETROK_p, RETROK_l, RETROK_r, RETROK_f},
      {41, 64, 43, 65, -1, -1, -1, -1, 33, 66, 45, 46, 60, 61},
      -1,
   },
   { // Dynamite Dan (1985).rmp
      {0x550ac9f9},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_RETURN, DB_CLEAN, DB_CLEAN, RETROK_a, RETROK_s, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Emilio Butragueno Futbol (1988).rmp
      {0x4f86b3c8},
      {DB_KEEP, DB_KEEP, RETROK_2, RETROK_0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Enchanter (1984).rmp
      {0xcca0759},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      5,
   },
   { // Exolon (1987).rmp
      {0x69219b4f},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_q, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Fernando Martin - Basket Master (1987).rmp
      {0x708351db, 0x471c5eac},
      {RETROK_3, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Freddy Hardest (1987).rmp
      {0x7a8fe41d, 0x874e9bb6},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Freddy Hardest in South Manhattan (1989).rmp
      {0x887d173f},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Gabrielle (1987).rmp
      {0x20dcbf76},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Galactic Plague (1984).rmp
      {0x674fbced},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_5, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Game Over (1987).rmp
      {0x4d723f39, 0x8d8e3122},
      {RETROK_RALT, RETROK_2, DB_KEEP, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {67, 28, -1, -1, -1, -1, -1, -1, 55, 30, -1, -1, -1, -1},
      -1,
   },
   { // Gauntlet (1986).rmp
      {0x7c16d340},
      {RETROK_KP3, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Ghost n Goblins (1986).rmp
      {0x8ceb4fa3},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_s, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Ghostbusters (1985).rmp
      {0xc8149ef6, 0xe71162a4},
      {RETROK_e, RETROK_1, RETROK_3, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {68, 30, 69, 70, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1},
      -1,
   },
   { // Ghouls n Ghosts (1989).rmp
      {0xc8d7f575},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_3, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Gnome Ranger (1987).rmp
      {0xb0a79287},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      6,
   },
   { // Goody (1987).rmp
      {0xbc911bd9, 0x795638dc},
      {RETROK_RETURN, DB_KEEP, RETROK_DELETE, RETROK_SPACE, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Goonies (1986).rmp
      {0xb6e80000},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_5, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_RSHIFT, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 71, -1, -1, -1, -1, -1},
      -1,
   },
   { // Gryzor (1987).rmp
      {0xa2b01221},
      {DB_KEEP, RETROK_SPACE, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Gunfright (1985).rmp
      {0xfe56bf61},
      {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_q, RETROK_v, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {55, -1, -1, -1, -1, -1, 15, 16, 17, -1, -1, -1, -1, -1},
      -1,
   },
   { // Hair Boy (2016).rmp
      {0xca1ccfa9},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_LEFT, RETROK_RIGHT, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Harrier Attack (1984).rmp
      {0x762a657},
      {RETROK_SPACE, RETROK_RETURN, RETROK_3, RETROK_2, RETROK_KP_PERIOD, RETROK_KP_ENTER, RETROK_z, RETROK_x, RETROK_KP0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {72, 70, 69, 28, -1, -1, 73, 74, 75, -1, -1, -1, -1, -1},
      -1,
   },
   { // Head over Hells (1986).rmp
      {0xc613efa0},
      {RETROK_KP1, RETROK_KP5, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_KP8, RETROK_SPACE, RETROK_SPACE, DB_KEEP, DB_KEEP},
      {26, 33, -1, -1, -1, -1, -1, -1, -1, 76, 77, 77, -1, -1},
      -1,
   },
   { // Hero Quest (1991).rmp
      {0xec279622, 0x257af370, 0xafe22434, 0x386ab47d},
      {DB_CLEAN, RETROK_0, DB_CLEAN, RETROK_SPACE, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_SPACE, RETROK_3, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, 78, -1, -1, -1, -1, -1, -1, 79, 80, -1, -1, -1, -1},
      -1,
   },
   { // Hitchhiker's Guide to the Galaxy, The (1984).rmp
      {0xa523527c},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      7,
   },
   { // Ice-Breaker (1990).rmp
      {0x55b67795},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_s, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Ikari Warriors (1986).rmp
      {0x36d943ea, 0xe7df0e60, 0x3e88f3f7},
      {RETROK_t, DB_KEEP, RETROK_2, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_3, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {81, -1, 82, 36, -1, -1, -1, -1, -1, 83, -1, -1, -1, -1},
      -1,
   },
   { // Infidel (1986).rmp
      {0xdc3c19fc},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      8,
   },
   { // Island of Dr Destructo (1987).rmp
      {0xaf33badf},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Ivan Ironman Super Off Road (1990).rmp
      {0xc3ce5121},
      {DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_4, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 84, -1, -1, -1, -1, -1, -1, -1, 85, -1, -1, -1, -1},
      -1,
   },
   { // Jack The Nipper I (1986).rmp
      {0xb06159a2},
      {RETROK_RETURN, RETROK_2, RETROK_m, RETROK_SPACE, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_0, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {86, 87, 88, -1, -1, -1, -1, -1, 55, 89, -1, -1, -1, -1},
      -1,
   },
   { // Jack The Nipper II (1987).rmp
      {0x2bec9bd},
      {RETROK_o, DB_KEEP, RETROK_m, RETROK_SPACE, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {25, -1, 88, -1, -1, 90, -1, -1, 55, -1, -1, -1, -1, -1},
      -1,
   },
   { // Jungle Book (1989).rmp
      {0xccc3a70b},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      9,
   },
   { // Klax (1990).rmp
      {0x96b53a41},
      {RETROK_a, DB_CLEAN, DB_CLEAN, RETROK_0, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {91, -1, -1, -1, -1, -1, -1, -1, 92, 40, -1, -1, -1, -1},
      -1,
   },
   { // Knight Lore (1984).rmp
      {0xa2d5c9db},
      {RETROK_q, RETROK_2, RETROK_3, RETROK_0, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_a, RETROK_1, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {25, 26, -1, -1, -1, -1, 15, 16, 17, 26, -1, -1, -1, -1},
      -1,
   },
   { // Lancelot (1988).rmp
      {0xc5ad3e3e},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      6,
   },
   { // Leather Goddesses of Phobos (1986).rmp
      {0xeb127e47},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      10,
   },
   { // Mad Mix Game (1988).rmp
      {0xc36d1efd, 0x97b8b3a9},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Manic Miner (1984).rmp
      {0xcd9dbf9c},
      {RETROK_RETURN, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Match Day II (1987).rmp
      {0x35c029d5},
      {RETROK_RETURN, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Mega Phoenix (1991).rmp
      {0xbd48490a},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Miss Input 1 (2019).rmp
      {0xe8d15029},
      {DB_CLEAN, RETROK_m, RETROK_2, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_o, RETROK_p, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, 93, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Miss Input 2 (2020).rmp
      {0x7c096b33},
      {DB_CLEAN, RETROK_m, RETROK_2, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_w, RETROK_e, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, 93, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Moon Cresta (1986).rmp
      {0x6abcc9b6},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_6, DB_CLEAN, DB_CLEAN, RETROK_z, RETROK_x, RETROK_RSHIFT, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Motos (1987).rmp
      {0x148447dc},
      {DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1},
      -1,
   },
   { // Mundial de Futbol (1990).rmp
      {0x6f132709},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, 94, -1, -1, -1, -1},
      -1,
   },
   { // Murder on the Atlantic (1985).rmp
      {0xb40c5cb2},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      11,
   },
   { // Mutan Zone (1989).rmp
      {0x62a4deae},
      {DB_KEEP, RETROK_1, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 95, -1, -1, -1, -1, -1, -1, -1, 96, -1, -1, -1, -1},
      -1,
   },
   { // Myth (1989).rmp
      {0x4d8464d3},
      {RETROK_j, RETROK_i, DB_CLEAN, RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_q, RETROK_w, RETROK_o, RETROK_i, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {97, -1, -1, 98, -1, -1, -1, -1, 99, 100, -1, -1, -1, -1},
      -1,
   },
   { // Nanako Descends to Hell (2009).rmp
      {0x6c2cdcea},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_SPACE, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 101, -1, -1, -1, -1, -1},
      -1,
   },
   { // Navy Moves (1988).rmp
      {0xacf62400, 0x9299697c},
      {RETROK_m, RETROK_2, DB_KEEP, RETROK_3, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {102, 28, -1, -1, -1, -1, -1, -1, 33, 30, -1, -1, -1, -1},
      -1,
   },
   { // Nemesis (1987).rmp
      {0xb35ed50},
      {RETROK_RETURN, RETROK_m, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {103, 93, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Oh Mummy (1984).rmp
      {0x2ff420da, 0x6abd6ce2},
      {DB_KEEP, DB_KEEP, RETROK_c, RETROK_p, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Operation Wolf (1988).rmp
      {0x7c6c1e2e},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Phantis (1989).rmp
      {0xa60eba03},
      {DB_KEEP, RETROK_1, DB_KEEP, RETROK_e, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 95, -1, -1, -1, -1, -1, -1, 33, 96, -1, -1, -1, -1},
      -1,
   },
   { // Phantomas Saga Infinity (2006).rmp
      {0x73d96734},
      {RETROK_DOWN, DB_CLEAN, DB_CLEAN, RETROK_s, DB_CLEAN, DB_CLEAN, RETROK_LEFT, RETROK_RIGHT, RETROK_UP, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {20, -1, -1, -1, -1, -1, -1, -1, 19, -1, -1, -1, -1, -1},
      -1,
   },
   { // Planetfall (1983).rmp
      {0x9af2061b},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      12,
   },
   { // Prince of Persia (1990).rmp
      {0xf964db5d, 0xa14a1e0f, 0xb9d14a6d, 0x18d230b1},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_ESCAPE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, 51, -1, -1, -1, -1, -1, 104, -1, -1, -1, -1},
      -1,
   },
   { // Pro Tennis (1985).rmp
      {0x25af0923},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Rainbow Islands (1989).rmp
      {0xd11aa568},
      {RETROK_a, DB_CLEAN, DB_CLEAN, RETROK_a, DB_CLEAN, DB_CLEAN, RETROK_k, RETROK_l, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Renegade (1987).rmp
      {0xdf16b5d3},
      {RETROK_LEFT, RETROK_DOWN, DB_KEEP, RETROK_SPACE, RETROK_w, RETROK_SPACE, RETROK_a, RETROK_d, RETROK_RIGHT, RETROK_DOWN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Rescate Atlantida (1989).rmp
      {0x39e92a63, 0x10712a0c},
      {RETROK_m, RETROK_1, DB_KEEP, RETROK_RETURN, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Rex (1989).rmp
      {0x6fbe410c},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      13,
   },
   { // Robocop (1989).rmp
      {0xb6017864, 0xda1142c4, 0xf70eee66},
      {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_1, RETROK_q, RETROK_a, RETROK_k, RETROK_l, RETROK_x, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Roland In The Caves - Pulga (1984).rmp
      {0x371eaac1, 0xf81c4337},
      {RETROK_BACKQUOTE, DB_KEEP, RETROK_a, RETROK_g, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_z, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, 105, -1, -1, -1, -1, -1},
      -1,
   },
   { // Roland On The Ropes - Fred (1984).rmp
      {0x324cfc6, 0xc3997a3d},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_5, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_RALT, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Roland in Time (1985).rmp
      {0xaca0e0f},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_n, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Rygar (1987).rmp
      {0x558abe44},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 106, -1, -1, -1, -1, -1},
      -1,
   },
   { // Sabre Wulf (1985).rmp
      {0x36596d2c},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_0, RETROK_r, RETROK_e, RETROK_q, RETROK_w, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Satan (1989).rmp
      {0xd66de504, 0xae64ea1d},
      {DB_CLEAN, RETROK_1, RETROK_2, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_2, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, 95, -1, -1, -1, -1, -1, -1, 33, 96, -1, -1, -1, -1},
      -1,
   },
   { // Savage (1988).rmp
      {0xca6ffd7a, 0xae64ea1d},
      {DB_CLEAN, RETROK_1, DB_CLEAN, DB_CLEAN, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_2, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, 84, -1, -1, -1, -1, -1, -1, 33, 107, -1, -1, -1, -1},
      -1,
   },
   { // Secte Noire, La (1990).rmp
      {0x95b94081},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      14,
   },
   { // Sgrizam (1986).rmp
      {0x8c8a37bd},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Shadow Dancer (1991).rmp
      {0x20e80d12},
      {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_0, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {108, -1, -1, -1, -1, -1, -1, -1, -1, 109, -1, -1, -1, -1},
      -1,
   },
   { // Shinobi (1989).rmp
      {0x6f1e3eb0, 0xbd6c0af},
      {RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Shinobu (2019).rmp
      {0x37ee404f},
      {DB_KEEP, DB_KEEP, RETROK_ESCAPE, RETROK_g, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Sir Fred (1986).rmp
      {0x92345cde},
      {RETROK_z, DB_CLEAN, DB_CLEAN, RETROK_4, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_m, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {110, -1, -1, -1, -1, -1, -1, -1, 111, -1, -1, -1, -1, -1},
      -1,
   },
   { // Skate Crazy (1988).rmp
      {0xef7871dc, 0x5ac9f3f4},
      {RETROK_SPACE, RETROK_1, DB_KEEP, RETROK_p, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 84, -1, 51, -1, -1, -1, -1, -1, 107, -1, -1, -1, -1},
      -1,
   },
   { // Smash TV (1991).rmp
      {0xc18a84f9},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Sol Negro (1989).rmp
      {0xb0540671},
      {DB_KEEP, RETROK_1, DB_KEEP, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 95, -1, -1, -1, -1, -1, -1, -1, 96, -1, -1, -1, -1},
      -1,
   },
   { // Solomons Key (1986).rmp
      {0x29671a87},
      {RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {112, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Sorcerer (1984)(Infocom)[CPM].rmp
      {0x636a30d8},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      15,
   },
   { // Space Hawks - Aguilas Del Espacio (1984).rmp
      {0xa041e903},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Spellbreaker (1985).rmp
      {0xd519f3ce},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      16,
   },
   { // Stormlord II (1990).rmp
      {0x375d4353},
      {RETROK_q, RETROK_1, DB_KEEP, RETROK_LALT, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {25, 84, -1, 51, -1, -1, -1, -1, -1, 107, -1, -1, -1, -1},
      -1,
   },
   { // Super Skweek (1990).rmp
      {0x1993c727, 0x7a97eed},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_KP1, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Suspended (1983).rmp
      {0x819d6ed7},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      17,
   },
   { // Tapper (1986).rmp
      {0xb699edb3},
      {DB_CLEAN, RETROK_a, DB_CLEAN, RETROK_s, RETROK_o, RETROK_k, RETROK_z, RETROK_x, RETROK_p, RETROK_e, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, 113, -1, -1, -1, -1, -1, -1, 33, 114, -1, -1, -1, -1},
      -1,
   },
   { // Target Renegade (1988).rmp
      {0xe4154a1f},
      {DB_KEEP, DB_KEEP, RETROK_ESCAPE, RETROK_1, RETROK_q, RETROK_a, RETROK_k, RETROK_l, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, 51, 36, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Techno Cop (1988).rmp
      {0xcbdafd77},
      {RETROK_z, DB_KEEP, DB_KEEP, RETROK_SPACE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_1, RETROK_BACKQUOTE, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Teenage Mutant Hero Turtles (1990).rmp
      {0xca4f0b5, 0x8dd0cf27},
      {RETROK_s, RETROK_RSHIFT, DB_KEEP, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {103, 115, -1, -1, -1, -1, -1, -1, 33, 116, -1, -1, -1, -1},
      -1,
   },
   { // Thanatos (1986).rmp
      {0x827e83ac, 0x8d64a322, 0xf9fce391, 0x59d6aef1},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_4, RETROK_a, RETROK_z, RETROK_n, RETROK_m, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, 117, -1, -1, -1, -1, -1},
      -1,
   },
   { // Three Weeks in Paradise (1985).rmp
      {0x60a4a40f},
      {DB_KEEP, RETROK_1, DB_KEEP, RETROK_a, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 118, -1, -1, -1, -1, -1, -1, -1, 119, -1, -1, -1, -1},
      -1,
   },
   { // Total Eclipse I-II (1988).rmp
      {0x41f8051c, 0xd8e6113b},
      {RETROK_SPACE, RETROK_h, RETROK_i, RETROK_u, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_0, RETROK_r, RETROK_p, RETROK_l, RETROK_s, RETROK_a},
      {41, 120, 43, 65, -1, -1, -1, -1, 33, 121, 45, 46, 122, 123},
      -1,
   },
   { // Trantor The Last Stormtrooper (1987).rmp
      {0x3365f862},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Tres Luces de Glaurung (1986).rmp
      {0x3a5242b9},
      {RETROK_n, DB_KEEP, DB_KEEP, RETROK_0, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_m, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {53, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // Turbo Girl (1988).rmp
      {0x77c7c536, 0x1e737e6c},
      {RETROK_SPACE, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Vera Cruz Affair, The (1986).rmp
      {0xd5a4c3f2, 0x4f6b8ae6, 0xd276b5bc},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      0,
   },
   { // Victory Road (1988).rmp
      {0xaf4b1161},
      {RETROK_z, RETROK_2, DB_CLEAN, RETROK_1, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_SPACE, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {81, 107, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1},
      -1,
   },
   { // WEC Le Mans (1988).rmp
      {0x54c40e54},
      {RETROK_SPACE, DB_CLEAN, DB_CLEAN, RETROK_2, RETROK_q, RETROK_a, RETROK_o, RETROK_p, RETROK_q, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Warlock (1987).rmp
      {0xa1b83838},
      {DB_CLEAN, DB_CLEAN, DB_CLEAN, RETROK_LSHIFT, RETROK_UP, RETROK_DOWN, RETROK_LEFT, RETROK_RIGHT, RETROK_RALT, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Way Of The Exploding Fist (1985).rmp
      {0x6fda459c},
      {DB_KEEP, DB_KEEP, RETROK_CAPSLOCK, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, RETROK_TAB, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, 124, -1, -1, -1, -1, -1, -1, 125, -1, -1, -1, -1},
      -1,
   },
   { // West Bank (1985).rmp
      {0x361c4a1e},
      {RETROK_2, DB_CLEAN, DB_CLEAN, RETROK_0, DB_CLEAN, DB_CLEAN, RETROK_1, RETROK_3, RETROK_2, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN, DB_CLEAN},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Winter Games (1986).rmp
      {0xba91af38, 0x4794cfb0},
      {DB_KEEP, RETROK_a, RETROK_RSHIFT, RETROK_RETURN, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Wishbringer (1985).rmp
      {0x5332c3ad},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      18,
   },
   { // Witness, The (1984).rmp
      {0xb16409a2},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      19,
   },
   { // Wonderboy (1987).rmp
      {0xb1026135},
      {204, RETROK_BACKSPACE, RETROK_ESCAPE, RETROK_1, DB_CLEAN, DB_KEEP, DB_KEEP, DB_KEEP, 200, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, 93, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Xenon (1988).rmp
      {0x91f26a80, 0x34cd836c},
      {DB_KEEP, DB_KEEP, DB_KEEP, RETROK_2, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      -1,
   },
   { // Yie Ar Kung-Fu.rmp
      {0x651cd158},
      {RETROK_KP0, RETROK_KP7, DB_KEEP, RETROK_1, RETROK_KP8, RETROK_KP2, RETROK_KP4, RETROK_KP6, RETROK_KP0, RETROK_KP9, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {106, 126, -1, -1, -1, -1, -1, -1, 106, 126, -1, -1, -1, -1},
      -1,
   },
   { // Zork I - The Great Underground Empire (1986).rmp
      {0xa02c3758},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      20,
   },
   { // Zork II - The Wizard of Frobozz (1986).rmp
      {0xe0240de6},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      21,
   },
   { // Zork III - The Dungeon Master (1986).rmp
      {0xa3fb554f},
      {DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP, DB_KEEP},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      22,
   },
};

char commands[] = {
   "|CPM\0"
   "|CPM\n~~LOADER\0"
   "RUN\"COBRA.\0"
   "MAHJONG\0"
   "CUTTHROA\0"
   "ENCHANTE\0"
   "RUN\"MENU\0"
   "HITCHHIK\0"
   "INFIDEL\0"
   "RUN\"JUNGLE\0"
   "LEATHERG\0"
   "RUN\"GAME\0"
   "PLANETFA\0"
   "RUN\"REX\0"
   "RUN\"SECTE\0"
   "SORCERER\0"
   "SPELLBRE\0"
   "SUSPENDE\0"
   "WISHBRIN\0"
   "WITNESS\0"
   "ZORK1\0"
   "ZORK2\0"
   "ZORK3\0"
};

char input_names[] = {
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
   "S / Start\0"
   "Bubble\0"
   "Start One Player\0"
   "[A]\0"
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
   "Fire Left\0"
   "Pause\0"
   "Fire Right\0"
   "Select\0"
   "Down / Use\0"
   "Shoot\0"
   "Slow Down\0"
   "Decrease Step\0"
   "Jet Pack\0"
   "Increase Step\0"
   "Rise\0"
   "Fall/Crouch\0"
   "Pick-Up/Drop Object\0"
   "Jump / Enter Room\0"
   "Cancel Drill\0"
   "U Turn\0"
   "Drill\0"
   "Throw Bombs\0"
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
   "Key 0 / Menu\0"
   "Action\0"
   "Key 3 / Menu\0"
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
   "Music Toggle\0"
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
   "Time Left\0"
   "Jump Left\0"
   "Attack\0"
   "Press 2\0"
   "Dog Attack\0"
   "Magic\0"
   "Take/Select Object\0"
   "Use Object\0"
   "Fireball\0"
   "Menu: Arcade\0"
   "Menu: Expert\0"
   "Enter Wagon\0"
   "Special Features\0"
   "Flame\0"
   "Slot 1\0"
   "Slot 2\0"
   "Crouch/Walk\0"
   "Rest\0"
   "Change Step\0"
   "Change Angle\0"
   "Change Players\0"
   "Change Control\0"
   "Diagonal Jump\0"
};


// gen hash: 1681665135.9645612

