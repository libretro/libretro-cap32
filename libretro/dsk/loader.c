/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2021)
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

#include "libretro-core.h"
#include "dsk/format.h"
#include "dsk/amsdos_catalog.h"
#include "loader.h"
#include "cap32/slots.h"

extern t_drive driveA;

//#define LOADER_DEBUG

static bool loader_foreign[CAT_MAX_ENTRY];

bool _loader_launch(char * key_buffer, char * filename)
{
   if (game_configuration.is_cpm)
   {
      // CPM ROM boot do not need >RUN< prefix
      if(strncpy(key_buffer, filename, LOADER_MAX_SIZE) < 0)
         return false;
   }
   else if(snprintf(key_buffer, LOADER_MAX_SIZE, "RUN\"%s", filename) < 0)
   {
      printf("[LOADER] !!!! _loader_run: snprintf failed\n");
      return false;
   }

   #ifdef LOADER_DEBUG
   printf("[LOADER] launch: %s \n", key_buffer);
   #endif

   return true;
}

bool _loader_find_file (char * key_buffer, char * filename)
{
   for (int idx = 0; idx < catalogue.last_entry; idx++) {
      if (loader_foreign[idx] || memcmp(catalogue.dirent[idx].filename, filename, strlen(filename)) != 0)
         continue;

      return _loader_launch(key_buffer, catalogue.dirent[idx].filename);
   }

   // loop finished, file not found
   return false;
}

/* Probe only ordinary single-sided DATA disks. Unusual layouts keep the
 * existing filename heuristic rather than guessing their allocation scheme. */
static const unsigned char *loader_data_sector(t_drive *drive, unsigned logical)
{
   unsigned track_id = logical / 9;
   unsigned sector_id = 0xc1 + logical % 9;
   const unsigned char *data = NULL;
   if (track_id >= drive->tracks || track_id >= DSK_TRACKMAX)
      return NULL;
   t_track *track = &drive->track[track_id][0];
   if (track->sectors != 9)
      return NULL;
   for (unsigned i = 0; i < track->sectors; i++) {
      t_sector *sector = &track->sector[i];
      if (sector->CHRN.sector_info != sector_id)
         continue;
      if (data || sector->CHRN.cylinder != track_id || sector->CHRN.side ||
          sector->CHRN.sector_size != 2 || sector->size != 512 ||
          sector->flags[0] || sector->flags[1] || sector->weak_versions > 1)
         return NULL;
      data = sector->data;
   }
   return data;
}

static const unsigned char *loader_file_header(t_drive *drive, const char *filename)
{
   unsigned char name[11];
   const char *dot = strchr(filename, '.');
   if (!dot || dot - filename > 8 || strlen(dot + 1) > 3)
      return NULL;
   memset(name, ' ', sizeof(name));
   memcpy(name, filename, dot - filename);
   memcpy(name + 8, dot + 1, strlen(dot + 1));
   for (unsigned s = 0; s < 4; s++) {
      const unsigned char *directory = loader_data_sector(drive, s);
      if (!directory)
         return NULL;
      for (unsigned offset = 0; offset < 512; offset += 32) {
         const unsigned char *entry = directory + offset;
         unsigned i;
         /* User 0, first extent, nonempty file, ordinary allocation block. */
         if (entry[0] || entry[12] || entry[14] || !entry[15] || entry[16] < 2)
            continue;
         for (i = 0; i < 11; i++)
            if ((entry[i + 1] & 0x7f) != name[i])
               break;
         if (i != 11)
            continue;
         return loader_data_sector(drive, entry[16] * 2);
      }
   }
   return NULL;
}

static const unsigned char *loader_bin_header(t_drive *drive, const char *filename)
{
   const char *dot = strchr(filename, '.');
   const unsigned char *header;
   unsigned checksum = 0;
   if (!dot || strcasecmp(dot + 1, "BIN"))
      return NULL;
   header = loader_file_header(drive, filename);
   if (!header || header[18] != 2)
      return NULL;
   for (unsigned i = 0; i < 67; i++)
      checksum += header[i];
   return checksum == (unsigned)(header[67] | header[68] << 8) ? header : NULL;
}

static void loader_probe_foreign(t_drive *drive)
{
   memset(loader_foreign, 0, sizeof(loader_foreign));
   if (game_configuration.is_cpm || drive->sides)
      return;
   for (int i = 0; i < catalogue.last_entry; i++) {
      const unsigned char *header = loader_file_header(drive, catalogue.dirent[i].filename);
      unsigned checksum = 0;
      if (!header || memcmp(header, "PLUS3DOS\x1a", 9))
         continue;
      for (unsigned j = 0; j < 127; j++)
         checksum += header[j];
      loader_foreign[i] = (checksum & 0xff) == header[127];
   }
}

static int loader_prefer_startable_bin(t_drive *drive, retro_format_info_t *format, int first)
{
   if (game_configuration.is_cpm || format->type != FORMAT_TYPE_AMSDOS_DATA ||
       drive->sides || catalogue.track_listed_id != 0 ||
       catalogue.dirent[first].is_hidden)
      return first;
   const unsigned char *header = loader_bin_header(drive, catalogue.dirent[first].filename);
   if (!header || header[26] || header[27])
      return first;
   for (int i = first + 1; i < catalogue.last_entry; i++) {
      if (loader_foreign[i] || catalogue.dirent[i].is_hidden)
         continue;
      header = loader_bin_header(drive, catalogue.dirent[i].filename);
      if (!header)
         continue;
      unsigned load = header[21] | header[22] << 8;
      unsigned length = header[24] | header[25] << 8;
      unsigned start = header[26] | header[27] << 8;
      if (start && length && load + length <= 0x10000 &&
          start >= load && start < load + length)
         return i;
   }
   return first;
}

bool _loader_find (char * key_buffer, retro_format_info_t *format, t_drive *drive)
{
   if (catalogue.track_listed_id != format->catalogue_sector && catalogue.track_hidden_id != format->catalogue_sector)
      return false;

   int found = 0;
   int first_bas = -1;
   int first_spc = -1;
   int first_bin = -1;
   int cur_name_id = 0;

   for (int idx = 0; idx < catalogue.last_entry; idx++) {
      char* scan = strchr(catalogue.dirent[idx].filename, '.');
      if (loader_foreign[idx] || !scan)
         continue;

      #ifdef LOADER_DEBUG
      printf("[LOADER] CPM: %s [%u][%u]\n", catalogue.dirent[idx].filename, catalogue.dirent[idx].is_hidden, catalogue.entries_listed_found);
      #endif

      if (!strcasecmp(scan + 1, "BAS"))
      {
         if (first_bas == -1) first_bas = idx;
         found = true;
      }
      else if (!strcasecmp(scan + 1, ""))
      {
         if (first_spc == -1) first_spc = idx;
         found = true;
      }
      else if (!strcasecmp(scan + 1, "BIN"))
      {
         if (first_bin == -1) first_bin = idx;
         found = true;
      }
   }

   if (!found) {
      return false;
   }

   if (first_bas != -1) {
      cur_name_id = first_bas;

      #ifdef LOADER_DEBUG
      printf("[LOADER] FIND: first BAS EXT found at [%i] filename: %s \n", first_bas, catalogue.dirent[first_bas].filename);
      #endif
   }else if (first_spc != -1) {
      cur_name_id = first_spc;

      #ifdef LOADER_DEBUG
      printf("[LOADER] FIND: first EMPTY EXT found at [%i] filename: %s \n", first_spc, catalogue.dirent[first_spc].filename);
      #endif
   }else if (first_bin != -1) {
      cur_name_id = loader_prefer_startable_bin(drive, format, first_bin);

      #ifdef LOADER_DEBUG
      printf("[LOADER] FIND: first BIN EXT found at [%i] filename: %s \n", first_bin, catalogue.dirent[first_bin].filename);
      #endif
   }

   return _loader_launch(key_buffer, catalogue.dirent[cur_name_id].filename);
}

bool _loader_one_listed(char * key_buffer)
{
   #ifdef LOADER_DEBUG
   printf("[  LOADER  ] ONE LISTED: CPM(%i), ENTRIES(%i), HIDDEN(%i)\n", game_configuration.is_cpm, catalogue.entries_listed_found, catalogue.entries_hidden_found);
   #endif

   if (!game_configuration.is_cpm && catalogue.entries_listed_found != 1)
      return false;

   if (game_configuration.is_cpm && (catalogue.entries_listed_found != 1 && catalogue.entries_hidden_found != 1))
      return false;

   if (loader_foreign[catalogue.first_listed_dirent])
      return false;
   return _loader_launch(key_buffer, catalogue.dirent[catalogue.first_listed_dirent].filename);
}

bool _loader_hidden(char * key_buffer, retro_format_info_t *format)
{
   #ifdef LOADER_DEBUG
   printf("[  LOADER  ] >>> hidden[%u, %u] [%u == %u]\n",
      catalogue.entries_listed_found,
      catalogue.entries_hidden_found,
      catalogue.track_hidden_id,
      format->catalogue_sector
   );
   #endif

   if (catalogue.entries_listed_found || catalogue.entries_hidden_found != 1)
      return false;

   if (catalogue.track_hidden_id != format->catalogue_sector)
      return false;

   #ifdef LOADER_DEBUG
   printf("[  LOADER  ] >>> using hidden\n");
   #endif

   if (loader_foreign[catalogue.first_hidden_dirent])
      return false;
   return _loader_launch(key_buffer, catalogue.dirent[catalogue.first_hidden_dirent].filename);
}

bool _loader_cpm(char * key_buffer, retro_format_info_t *format)
{
   #ifdef LOADER_DEBUG
   printf("[  LOADER  ] >>> CPM [%u] [%u,%u] [%u == %u]\n", catalogue.probe_cpm, catalogue.entries_listed_found, catalogue.entries_hidden_found, catalogue.track_listed_id, format->catalogue_sector);
   #endif

   if (!catalogue.probe_cpm || catalogue.entries_listed_found || catalogue.entries_hidden_found)
      return false;

   #ifdef LOADER_DEBUG
   printf("[  LOADER  ] >>> using CPM [%u,%u,%u]\n", catalogue.probe_cpm, catalogue.entries_listed_found, catalogue.entries_hidden_found);
   #endif
   strcpy(key_buffer, "|CPM");

   return true;
}

void _loader_failed (char * key_buffer, bool is_system)
{
   if (game_configuration.is_cpm)
   {
      strcpy(key_buffer, "DIR");
      return;
   }
   else if (is_system)
   {
      strcpy(key_buffer, "|CPM");
      return;
   }

   // usefull to user see catalogue files (or run DSK protections)
   strcpy(key_buffer, "CAT");
}

void _loader_run(char * key_buffer, retro_format_info_t *format, t_drive *current_drive)
{
   memset(key_buffer, 0, LOADER_MAX_SIZE);

   catalog_probe(current_drive, 0);
   loader_probe_foreign(current_drive);

   if (_loader_cpm(key_buffer, format))
      return;

   // first we try to find classic run filenames
   if (_loader_find_file(key_buffer, "DISC.")) // DISC.*
      return;

   if (_loader_find_file(key_buffer, "DISC")) // DISC*.*
      return;

   if (_loader_find_file(key_buffer, "DISK.")) // DISK.*
      return;

   if (_loader_find_file(key_buffer, "JEU.BAS"))
      return;

   if (_loader_find_file(key_buffer, "ELITE.BAS"))
      return;

   if (_loader_one_listed(key_buffer))
      return;

   if (_loader_hidden(key_buffer, format))
      return;

   #ifdef LOADER_DEBUG
   printf("[  LOADER  ] finally trying with bas/bin/dot files\n");
   #endif

   if(!_loader_find(key_buffer, format, current_drive))
   {
      _loader_failed(key_buffer, format->type == FORMAT_TYPE_AMSDOS_SYSTEM);
   }
}

void loader_run (char * key_buffer)
{
   t_drive *current_drive = &driveA; 

   retro_format_info_t* test_format = NULL;
   test_format = format_get(current_drive);

   _loader_run(key_buffer, test_format, current_drive);
}
