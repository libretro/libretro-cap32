/****************************************************************************
 *  Caprice32 libretro port
 *
 *  catalog is inspired on caprice-forever by Frédéric Coste (Fredouille)
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

#include "libretro-core.h"
#include "amsdos_catalog.h"
#include "format.h"

#define CAT_ENTRIES 16

//#define CATALOG_DEBUG
//#define CATALOG_DEBUG_VERBOSE
//#define CATALOG_DEBUG_ENTRIES

#define GET_CHAR(x) (x & 0x7F)

#define CATALOGUE_ENTRY_SIZE 32
#define CATALOGUE_NAME_SIZE 8
#define CATALOGUE_EXT_SIZE 3
#define CATALOGUE_FILENAME_SIZE (CATALOGUE_NAME_SIZE + CATALOGUE_EXT_SIZE)
#define CATALOGUE_ART_SIZE (1 + CATALOGUE_FILENAME_SIZE) // user + file + ext
#define CATALOGUE_ATTRIB 0xA

typedef union {
      unsigned char data[CATALOGUE_ENTRY_SIZE];
      struct {
         unsigned char user;
         union {
            char raw[CATALOGUE_FILENAME_SIZE];
            struct {
               char raw_name[CATALOGUE_NAME_SIZE]; // raw filename
               char raw_ext[CATALOGUE_EXT_SIZE];
            };
         } file;
         unsigned char extent;
         unsigned char unused[2];
         unsigned char size;
         unsigned char gap[16];
      };
} retro_catalog_entry_t;

catalogue_info_t catalogue;

t_sector *_find_sector(t_track *track, int sector_id)
{
    t_sector *sector_found = NULL;

    for (int idx = 0; idx < track->sectors; idx++)
    {
        if ( ((track->sector[idx].CHRN.sector_info & 0x0F) == sector_id)
            && (track->sector[idx].CHRN.sector_size == 2) )
        {
            sector_found = &track->sector[idx];
            break;
        }
    }

    return sector_found;
}

retro_catalog_entry_t * _get_entry(t_sector *sector, int position)
{
   unsigned int offset = position * sizeof(retro_catalog_entry_t);
   unsigned char *ptr = (unsigned char *) sector->data + offset;
   #ifdef CATALOG_DEBUG_VERBOSE
   printf("[CATALOG-NEW]: position(%02i): s[%03x,%03x] ${%02x%02x%02x%02x} (%lu)\n",
      position,
      offset,
      sector->size,
      *(ptr+0),
      *(ptr+1),
      *(ptr+2),
      *(ptr+3),
      sizeof(retro_catalog_entry_t)
   );
   #endif

   return (retro_catalog_entry_t *) ptr;
}

bool _is_valid_ext(const char * ext)
{
   // TODO: better check...
   // "BAS" Extension
   if (
      (GET_CHAR(ext[0]) == 'B') &&
      (GET_CHAR(ext[1]) == 'A') &&
      (GET_CHAR(ext[2]) == 'S')
   )
      return true;

   // "BIN" Extension
   if (
      (GET_CHAR(ext[0]) == 'B') &&
      (GET_CHAR(ext[1]) == 'I') &&
      (GET_CHAR(ext[2]) == 'N')
   )
      return true;

   // "   " Extension
   if (
      (GET_CHAR(ext[0]) == ' ') &&
      (GET_CHAR(ext[1]) == ' ') &&
      (GET_CHAR(ext[2]) == ' ')
   )
      return true;

   return false;
}

bool _is_valid_cpm(const char * ext)
{
   // TODO: better check...
   // "COM" Extension
   if (
      (GET_CHAR(ext[0]) == 'C') &&
      (GET_CHAR(ext[1]) == 'O') &&
      (GET_CHAR(ext[2]) == 'M')
   )
      return true;

   return false;
}

bool _is_valid_entry(const retro_catalog_entry_t * entry)
{
   unsigned char * entry_filename = (unsigned char *) entry->file.raw;

   if(!entry->size)
   {
      #ifdef CATALOG_DEBUG_VERBOSE
      printf("[CATALOG-NEW] >>> zero size!\n");
      #endif
      return false;
   }

   #ifdef CATALOG_DEBUG_ENTRIES
   printf("[CATALOG-NEW] >>> valid_entry: [");
   #endif

   for (int idx = 0; idx < CATALOGUE_FILENAME_SIZE; idx++)
   {
      if ( ((
         (GET_CHAR(*(entry_filename+idx)) >= 0x30 /*0*/) &&
         (GET_CHAR(*(entry_filename+idx)) <= 0x39 /*9*/)
      ) && (
         (GET_CHAR(*(entry_filename+idx)) >= 0x41 /*A*/) &&
         (GET_CHAR(*(entry_filename+idx)) <= 0x5a /*Z*/)
      )) || (
         (GET_CHAR(*(entry_filename+idx)) == 0x22 /*"*/)
      )) {
         #ifdef CATALOG_DEBUG_ENTRIES
         printf("@%02x\n", *(entry_filename+idx));
         #endif
         return false;
      }

      #ifdef CATALOG_DEBUG_ENTRIES
      printf("%c", GET_CHAR(*(entry_filename+idx)));
      #endif
   }

   #ifdef CATALOG_DEBUG_ENTRIES
   printf("]\n");
   #endif
   return true;
}

bool _is_catalogue_art(unsigned char * entry_info)
{
   int idx = 1;
   for (; idx < sizeof(retro_catalog_entry_t); idx++)
   {
      // check valid filename / extension
      if (
         (GET_CHAR(*(entry_info+idx)) >= 0x20 /*SPACE*/) &&
         (GET_CHAR(*(entry_info+idx)) <= 0x5a /*Z*/)
      )
         continue;
      else
         break;
   }

   if (idx >= CATALOGUE_ART_SIZE)
      return false;

   if (!catalogue.has_cat_art)
   {
      #ifdef CATALOG_DEBUG
      printf("[CATALOG-NEW] >>> possible catArt, marked at [%u/%u] {%02x} (%lu)\n",
         idx,
         CATALOGUE_ART_SIZE,
         GET_CHAR(*(entry_info+idx)),
         sizeof(retro_catalog_entry_t)
      );
      #endif
      catalogue.has_cat_art = true;
   }

   return true;
}

bool __catalog_build_name(char *name, const char *raw_name, const char *raw_ext)
{
   int i, j;

   for (i = 0; i < CATALOGUE_NAME_SIZE; i++) {
      if (GET_CHAR(raw_name[i]) == ' ')
         break;

      name[i] = GET_CHAR(raw_name[i]);
   }

   name[i] = '.';
   i ++;

   // check extension
   for (j = 0; j < CATALOGUE_EXT_SIZE; j++, i++) {
      if (GET_CHAR(raw_ext[j]) == ' ')
         break;

      name[i] = GET_CHAR(raw_ext[j]);
   }

   name[i] = 0;

   // name > '.'
   if (i <= 1)
      return false;

   #ifdef CATALOG_DEBUG_ENTRIES
   printf("[CATALOG-NEW] >>> build_name OK!: [%s]\n", name);
   #endif

   return true;
}

bool __catalog_exist_name(char * archive_name)
{
   for (int idx = 0; idx < catalogue.last_entry; idx++)
   {
      if(!strncmp(catalogue.dirent[idx].filename, archive_name, CAT_NAME_SIZE))
         return true;
   }

   return false;
}

void __catalog_add(char * archive_name, int track_id, bool is_hidden)
{
   if (__catalog_exist_name(archive_name))
      return;

   strncpy(catalogue.dirent[catalogue.last_entry].filename, archive_name, CAT_NAME_SIZE);
   catalogue.dirent[catalogue.last_entry].is_hidden = is_hidden;

   if (is_hidden)
   {
      if (!catalogue.first_hidden_dirent)
      {
         catalogue.track_hidden_id = track_id;
         catalogue.first_hidden_dirent = catalogue.last_entry;
      }

      catalogue.entries_hidden_found++;
   } else {
      if (!catalogue.entries_listed_found)
      {
         catalogue.track_listed_id = track_id;
         catalogue.first_listed_dirent = catalogue.last_entry;
      }

      catalogue.entries_listed_found++;
   }

   #ifdef CATALOG_DEBUG_ENTRIES
   printf("[CATALOG-NEW]: dirent(%i) [%s][%u%u]\n",
      catalogue.last_entry,
      catalogue.dirent[catalogue.last_entry].filename,
      catalogue.dirent[catalogue.last_entry].is_hidden,
      catalogue.dirent[catalogue.last_entry].is_readonly
   );
   #endif

   catalogue.last_entry ++;
}

void _add_hidden_entry(const char * filename, int track_id)
{
   char tmp_name[CAT_NAME_SIZE];

   if (!__catalog_build_name(tmp_name, filename, filename + 8))
      return;

   __catalog_add(tmp_name, track_id, true);
}

void _probe_track(t_track *track, unsigned char user, int track_id)
{
   char tmp_name[CAT_NAME_SIZE];

   for (int sector_idx = 1; sector_idx <= 4; sector_idx++)
   {
      t_sector *sector = _find_sector(track, sector_idx);
      if (!sector || (!sector->size))
         continue;

      #ifdef CATALOG_DEBUG_VERBOSE
      printf("[CATALOG-NEW]: Sector [%02i] \n", sector_idx);
      #endif

      // todo: declared size
      int total_entries = sector->size / sizeof(retro_catalog_entry_t);
      for (int entry = 0; entry < total_entries; entry++)
      {
         const retro_catalog_entry_t * archive_info = _get_entry(sector, entry);

         if (archive_info->user != user)
         {
            // printf("[CATALOG-NEW]: invalid user [%x != %x]\n", archive_info->user, user);
            continue;
         }

         if(_is_catalogue_art((unsigned char *) archive_info->data))
            continue;

         if(!_is_valid_entry(archive_info))
         {
            continue;
         }

         if (game_configuration.is_cpm)
         {
            if (!_is_valid_cpm(archive_info->file.raw_ext))
            {
               continue;
            }
         } else if (!_is_valid_ext(archive_info->file.raw_ext))
         {
            continue;
         }

         // hidden or CATArt detected
         if ((archive_info->data[CATALOGUE_ATTRIB] & 0x80) || (catalogue.has_cat_art))
         {
            _add_hidden_entry(archive_info->file.raw, track_id);
            continue;
         }

         if(!__catalog_build_name(tmp_name, archive_info->file.raw_name, archive_info->file.raw_ext))
         {
            continue;
         }

         #ifdef CATALOG_DEBUG
         printf("[CATALOG-NEW]: entry(%i/%i): s[%i] [%s]\n\n",
            catalogue.last_entry,
            CAT_MAX_ENTRY,
            sector_idx,
            tmp_name
         );
         #endif

         __catalog_add(tmp_name, track_id, false);
      }
   }
}

void _find_entries(t_drive *drive, t_track *catalog_track, unsigned char user)
{
   bool entries_found = false;
   t_track *current_track = NULL;

   // parse tracks from 0 to 2 trying to detect valid entries
   for (int track_idx = 0; (track_idx <= 2) && !entries_found; track_idx++)
   {
      current_track = &drive->track[track_idx][0]; // Side = 0

      // ignore missing or empty tracks
      if (!current_track->size)
         continue;

      #ifdef CATALOG_DEBUG_VERBOSE
      printf("[CATALOG-NEW]: Track [%02i] \n", track_idx);
      #endif

      _probe_track(current_track, user, track_idx);
   }


}

void _prepare_catalog(t_drive *drive, unsigned char user)
{
   t_sector *first_sector = &drive->track[0][0].sector[0];
   t_track *catalog_track = NULL;

   unsigned char format_id = first_sector->CHRN.sector_info & 0xF0;

   switch (format_id)
   {
      case FORMAT_ID_DATA:
         catalog_track = &drive->track[0][0];
         break;
   
      case FORMAT_ID_SYSTEM:
         catalog_track = &drive->track[2][0];
         catalogue.probe_cpm = true;
         break;

      case FORMAT_ID_IBM:
         catalog_track = &drive->track[1][0];
         break;

      default:
         printf("[CATALOG-ERR]: format [%02x] not supported\n", format_id);
         return;
   }

   _find_entries(drive, catalog_track, user);
}

int catalog_probe(t_drive *drive, unsigned char user)
{
   // clean base catalogue
   memset(&catalogue, 0, sizeof(catalogue_info_t));

   _prepare_catalog(drive, user);
   return catalogue.last_entry;
}
