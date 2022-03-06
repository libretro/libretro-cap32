/****************************************************************************
 *  Caprice32 libretro port
 *
 *  code adapted from libcpccat, pituka, wiituka ported to retroarch by
 *   David Colmenero - D_Skywalk (2019-2021)
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

#include "../../cap32/cap32.h"
#include "catalog.h"

#define ENTRIES_PER_SECTOR 16
#define DSK_NAME_SIZE 8
#define DSK_EXT_SIZE 3

//#define CAT_DEBUG

char catalog_dirent[CAT_MAX_ENTRY][CAT_NAME_SIZE];
int catalog_entry = 0;

DSKEntry * _catalog_entry(unsigned char pos, unsigned short track_offset, t_drive *drive)
{
   int offset = pos * sizeof(DSKEntry);
   int entry = pos % ENTRIES_PER_SECTOR;
   int track = track_offset;

   unsigned int sector_size = drive->track[track][0].sector[0].size;
   unsigned int sector = offset / sector_size;

   #ifdef CAT_DEBUG
   printf("[LOADER] [CAT]: entry(%i): s[%u,%u] ==> %u a[%u,%u] (%lu)\n",
      pos,
      sector,
      sector_size,
      offset,
      entry,
      track,
      sizeof(DSKEntry)
   );
   #endif

   DSKEntry * archive_info = (DSKEntry *) drive->track[track][0].sector[sector].data;
   archive_info += entry;

   return archive_info;
}

bool _is_valid_extension(const char *ext)
{
   if (!strncasecmp(ext, "BAS", 3))
      return true;
   
   if (!strncasecmp(ext, "BIN", 3))
      return true;

   if (!strncmp(ext, "   ", 3))
      return true;

   return false;
}

bool _is_valid_filename(const char *name)
{
   if (strchr(name, '"') != NULL)
      return false;

   return true;
}

bool _catalog_build_name(char *name, char *raw_name, char *raw_ext)
{
   int i, j;

   if (!_is_valid_extension(raw_ext))
      return false;

   if (!_is_valid_filename(raw_name))
      return false;

   for (i = 0; i < DSK_NAME_SIZE; i++) {
      if (raw_name[i] == ' ')
         break;

      name[i] = raw_name[i];
   }

   name[i] = '.';
   i ++;

   // check extension
   for (j = 0; j < DSK_EXT_SIZE; j++, i++) {
      if (raw_ext[j] == ' ')
         break;

      name[i] = raw_ext[j];
   }

   name[i] = 0;

   // name > '.'
   if (i <= 1)
      return false;

   return true;
}

bool _catalog_exist_name(char * archive_name)
{
   for (int p = 0; p < catalog_entry; p++)
   {
      if(!strncmp(catalog_dirent[p], archive_name, CAT_NAME_SIZE))
         return true;
   }

   return false;
}

void _catalog_add(char * archive_name)
{
   if (_catalog_exist_name(archive_name))
      return;

   strncpy(catalog_dirent[catalog_entry], archive_name, CAT_NAME_SIZE);
   #ifdef CAT_DEBUG
   printf("[LOADER] [CAT]: dirent(%i) [%s]\n", catalog_entry, catalog_dirent[catalog_entry]);
   #endif

   catalog_entry ++;
}

void archive_init(unsigned short alloc_size, unsigned short track_offset, t_drive *drive)
{
   int i = 0;
   char raw_name[8 + 3];
   char tmp_name[13]; /* <raw_name 8>+"."+<raw_ext 3>+"\0" */

   for (i = 0; i <= alloc_size; i++)
   {
      const DSKEntry * archive_info = _catalog_entry(i, track_offset, drive);

      // ignore empty entries
      if (archive_info->user == 0xE5)
         continue;

      // filter filename/extension using mask
      for (int j = 0; j < 8 + 3; j++)
         raw_name[j] = archive_info->file.raw[j] & 0x7F;

      #ifdef CAT_DEBUG
      printf("[LOADER] [CAT]: entry[%i/%i] [%s]\n", i, alloc_size, raw_name);
      #endif

      if(!_catalog_build_name(tmp_name, raw_name, raw_name + 8))
         continue;

      #ifdef CAT_DEBUG
      printf("[LOADER] [CAT]: drive_data(%i/%i): t[%i][0] [%s]\n\n",
         i, alloc_size, track_offset,
         tmp_name
      );
      #endif

      _catalog_add(tmp_name);
   }
}
