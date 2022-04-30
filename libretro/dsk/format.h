/****************************************************************************
 *  Caprice32 libretro port
 *
 *  format.h is inspired on caprice-forever by Frédéric Coste (Fredouille)
 *    and adapted to libretro by David Colmenero - D_Skywalk (2019-2021)
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

#ifndef GFX_FORMAT_H__
#define GFX_FORMAT_H__

#include "cap32.h"

// formats info: http://www.cpcwiki.eu/index.php/ParaDOS

#define FORMAT_ID_DATA   0xC0
#define FORMAT_ID_SYSTEM 0x40
#define FORMAT_ID_IBM    0x00

typedef enum {
   FORMAT_TYPE_UNKNOWN       = 0,
   FORMAT_TYPE_AMSDOS_DATA   = 1,
   FORMAT_TYPE_AMSDOS_SYSTEM = 2,
   FORMAT_TYPE_AMSDOS_IBM    = 3,
   FORMAT_TYPE_ROMDOS_D10    = 4,
   FORMAT_TYPE_ROMDOS_D01    = 5,
} retro_format_type_t;

typedef struct {
   unsigned short sectors; // max number of sectors per track (8, >9)
   unsigned short tracks; // max number of tracks per side (>40, 80)
   unsigned short sides; // max number of sides (1/2)
   unsigned char sector_size; // Bytes Per Sector [SECTOR_CHRN_N]
   unsigned char sector_id; // Sector ID Mask [SECTOR_CHRN_R]
   unsigned char catalogue_sector; // sector catalogue expected
   retro_format_type_t type; // format ID
   char label[10]; // format string
} retro_format_info_t;

retro_format_info_t* format_get (t_drive *drive);
bool format_probe_hexagon (t_drive *drive);

#endif