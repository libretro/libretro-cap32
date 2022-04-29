/****************************************************************************
 *  Caprice32 libretro port
 *
 *  format.c is inspired on caprice-forever by Frédéric Coste (Fredouille)
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

#include "format.h"

//#define FORMAT_DEBUG
//#define FORMAT_DEBUG_VERBOSE

static const retro_format_info_t formats_list[] =
{
  // custom formats -- for compatibility with old loader
  { 10, 42, 1, 2, FORMAT_ID_DATA, 0, FORMAT_TYPE_AMSDOS_DATA, {"DATA_B"} },
  { 10, 42, 1, 2, FORMAT_ID_SYSTEM, 2, FORMAT_TYPE_AMSDOS_SYSTEM, {"SYSTEM_B"} },

  {  9, 42, 1, 2, FORMAT_ID_DATA, 0, FORMAT_TYPE_AMSDOS_DATA, {"DATA"} },
  {  9, 42, 1, 2, FORMAT_ID_SYSTEM, 2, FORMAT_TYPE_AMSDOS_SYSTEM, {"SYSTEM"} },
  { 10, 42, 1, 2, FORMAT_ID_IBM, 1, FORMAT_TYPE_AMSDOS_IBM, {"IBM"} },

   // unsupported formats
  { 10, 80, 2, 2, 0x10, 0, FORMAT_TYPE_ROMDOS_D10, {"D10"} },
  {  9, 80, 2, 2, 0x00, 0, FORMAT_TYPE_ROMDOS_D01, {"D01"} },

  {  0,  0, 0, 0, 0x00, 0, FORMAT_TYPE_UNKNOWN, {"UNK"} },
};

retro_format_info_t* format_get (t_drive *drive)
{
   t_track *first_track = &drive->track[0][0];
   t_sector *first_sector = &first_track->sector[0];

   retro_format_info_t * format_found = (retro_format_info_t*) &formats_list[5]; // unknown
   retro_format_info_t* cur_entry;

  for (cur_entry = (retro_format_info_t*) formats_list; cur_entry->type != FORMAT_TYPE_UNKNOWN; cur_entry++)
  {

      #ifdef FORMAT_DEBUG_VERBOSE
      printf("[NEW-FORMAT] >>> [%02u.<%u.%u.0x%02x.0x%02x]\n",
         cur_entry->sectors,
         cur_entry->tracks,
         cur_entry->sides,
         cur_entry->sector_size,
         cur_entry->sector_id
      );
      printf("                 [%02u. %u.%u.0x%02x.0x%02x]\n",
         first_track->sectors,
         drive->tracks,
         drive->sides + 1,
         first_track->sector[0].CHRN.sector_size,
         first_track->sector[0].CHRN.sector_info & 0xF0
      );
      #endif

      // check format
      if ((first_track->sectors <= cur_entry->sectors)
         && (drive->tracks <= cur_entry->tracks)
         && ((drive->sides + 1) <= cur_entry->sides)
         && (first_sector->CHRN.sector_size == cur_entry->sector_size)
         && ((first_sector->CHRN.sector_info & 0xF0) == cur_entry->sector_id) )
      {
         #ifdef FORMAT_DEBUG_VERBOSE
         printf("[NEW-FORMAT] >>> SELECTED %u\n", cur_entry->type);
         #endif
         format_found = cur_entry;
      }
   }

   #ifdef FORMAT_DEBUG
   if (format_found->type != FORMAT_TYPE_UNKNOWN)
      printf("[NEW-FORMAT] >>> [%u. %u.%u.0x%02x.0x%02x] [CAT: %u]\n",
         format_found->sectors,
         format_found->tracks,
         format_found->sides,
         format_found->sector_size,
         format_found->sector_id,
         format_found->catalogue_sector
      );
   else
      printf("[NEW-FORMAT] >>> [%u.<%u.%u.0x%02x.0x%02x] %u [%02x.%02x.%02x.%02x] ext(%u)\n",
         first_track->sectors,
         drive->tracks,
         drive->sides + 1,
         first_track->sector[0].CHRN.sector_size,
         first_track->sector[0].CHRN.sector_info & 0xF0,
         first_track->size,
         first_track->sector[0].CHRN.data[0],
         first_track->sector[0].CHRN.data[1],
         first_track->sector[0].CHRN.data[2],
         first_track->sector[0].CHRN.data[3],
         drive->extended
      );
   printf("[NEW-FORMAT] >>> format: \"%s\" matching disk.\n", format_found->label);
   #endif

   return format_found;
}

#define HEXAGON_TRACK_SIZE    0x1400
#define HEXAGON_TRACK_SIZE_V2 0x1800
#define HEXAGON_SECTOR_SIZE   0xA
#define HEXAGON_SECTOR_POS    0x8
#define HEXAGON_FILE_POS      0x1
#define HEXAGON_FILE_STR      "DISK"
#define HEXAGON_HEADER_POS    0x1C
#define HEXAGON_HEADER_STR    "HEXAGON"

bool _format_hexagon_check (t_track *first_track, int tracks, int sectors, int header_token, int file_token)
{
   if (first_track->size != tracks)
      return false;

   if (first_track->sectors != sectors)
      return false;

   t_sector * hexagon_sector = &first_track->sector[HEXAGON_SECTOR_POS];

   if (memcmp(
      hexagon_sector->data + header_token,
      HEXAGON_HEADER_STR,
      7
   ) != 0)
      return false;

   if (memcmp(
      hexagon_sector->data + file_token,
      HEXAGON_FILE_STR,
      4
   ) != 0)
      return false;

   #ifdef FORMAT_DEBUG
   printf("[LOADER] >>> protection: HEXAGON/DATA_B matching disk t(%i).\n", tracks);
   #endif

   return true;
}

bool format_probe_hexagon (t_drive *drive)
{
   t_track *first_track = &drive->track[0][0];

   bool result = _format_hexagon_check(
      first_track,
      HEXAGON_TRACK_SIZE_V2,
      HEXAGON_SECTOR_SIZE,
      HEXAGON_HEADER_POS,
      HEXAGON_FILE_POS
   );

   if (result)
      return true;

   #ifdef FORMAT_DEBUG
   printf("%i = %i, %i = %i\n",
      first_track->sectors, HEXAGON_SECTOR_SIZE,
      first_track->size, HEXAGON_TRACK_SIZE
   );
   #endif

   result = _format_hexagon_check(
      first_track,
      HEXAGON_TRACK_SIZE,
      HEXAGON_SECTOR_SIZE,
      HEXAGON_HEADER_POS,
      HEXAGON_FILE_POS
   );

   return result;
}
