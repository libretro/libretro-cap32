/****************************************************************************
 *  Caprice32 libretro port
 *
 *  code adapted from libcpccat, ported to retroarch by
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

//#define CAT_DEBUG 1

char catalog_dirent[CAT_MAX_ENTRY][CAT_NAME_SIZE];
int catalog_entry = 0;

CatalogEntry * _catalog_init(unsigned short alloc_size) {
   CatalogEntry *catalog = (CatalogEntry *) malloc(sizeof(CatalogEntry) * (alloc_size + 1));
   if (catalog == NULL) {
      return NULL;
   }

   for (int i = 0; i <= alloc_size; i++) {
      catalog[i].valid = false;
      memset(catalog[i].name, 0, 13);
   }

   #ifdef CAT_DEBUG
   printf("  archive: created %u entries.\n", alloc_size);
   #endif

   return catalog;
}

DSKEntry * _catalog_entry(unsigned char pos, t_drive *drive)
{
   int offset = pos * sizeof(DSKEntry);
   int entry = pos % ENTRIES_PER_SECTOR;

   unsigned int sector_size = drive->track[0][0].sector[0].size;
   unsigned int sector = offset / sector_size;

   #ifdef CAT_DEBUG
   printf("    entry(%i): s[%u,%u] ==> %u a[%u] (%lu)\n",
      pos,
      sector,
      sector_size,
      offset,
      entry,
      sizeof(DSKEntry)
   );
   #endif

   DSKEntry * archive_info = (DSKEntry *) drive->track[0][0].sector[sector].data;
   archive_info += entry;

   return archive_info;
}

bool _is_valid_extension(const char *ext) {
   if (! strcasecmp(ext, "BAS"))
      return true;
   
   if (! strcasecmp(ext, "BIN"))
      return true;

   if (! strncmp(ext, "   ", 3))
      return true;

   return false;
}

void _catalog_build_name(CatalogEntry * cat)
{
   int  i, j;
   char *buf = cat->name;
   char *name = cat->raw_name;
   char *ext = cat->raw_ext;


   if(!_is_valid_extension(ext))
      return;

   for (i = 0; i < DSK_NAME_SIZE; i++) {
      if (name[i] == ' ')
         break;

      buf[i] = name[i];
   }

   buf[i] = '.';
   i ++;

   // check extension
   for (j = 0; j < DSK_EXT_SIZE; j++, i++) {
      if (ext[j] == ' ')
         break;

      buf[i] = ext[j];
   }

   buf[i] = 0;

   // name > '.'
   if (i > 1)
      cat->valid = true;
}

bool _catalog_exist_name(char * archive_name) {
   for (int p = 0; p < catalog_entry; p++) {
      if(!strncmp(catalog_dirent[p], archive_name, CAT_NAME_SIZE))
         return true;
   }

   return false;
}

void _catalog_add(char * archive_name) {
   if (_catalog_exist_name(archive_name))
      return;

   strncpy(catalog_dirent[catalog_entry], archive_name, CAT_NAME_SIZE);
   printf("dirent(%i) [%s]\n", catalog_entry, catalog_dirent[catalog_entry]);

   catalog_entry ++;
}

void archive_init(unsigned short alloc_size, t_drive *drive)
{
   int i = 0;
   CatalogEntry *catalog = NULL;

   catalog = _catalog_init(alloc_size);
   if (catalog == NULL) {
      return;
   }

   for (i = 0; i <= alloc_size; i++) {
      DSKEntry * archive_info = _catalog_entry(i, drive);

      // ignore empty entries
      if (archive_info->user == 0xE5)
         continue;

      // filter filename/extension using mask
      for (int j = 0; j < 8; j++)
         catalog[i].raw_name[j] = archive_info->raw_name[j] & 0x7F;

      for (int j=0; j < 3; j++)
         catalog[i].raw_ext[j] = archive_info->raw_ext[j] & 0x7F;

      _catalog_build_name(&catalog[i]);

      if (!catalog[i].valid)
         continue;

      #ifdef CAT_DEBUG
      printf("  drive_data(%i/%i): t[0][0] [%s] v[%u]\n\n",
         i, alloc_size,
         catalog[i].name,
         catalog[i].valid
      );
      #endif

      _catalog_add(catalog[i].name);
   }

   // free allocated info
   free(catalog);
}
