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

   return true;
}

bool _loader_find_file (char * key_buffer, char * filename)
{
   for (int idx = 0; idx < catalogue.last_entry; idx++) {
      if (memcmp(catalogue.dirent[idx].filename, filename, strlen(filename)) != 0)
         continue;

      return _loader_launch(key_buffer, catalogue.dirent[idx].filename);
   }

   // loop finished, file not found
   return false;
}

bool _loader_find (char * key_buffer, retro_format_info_t *format)
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
      if (!scan)
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

   if (first_bas != -1)
      cur_name_id = first_bas;
   else if (first_spc != -1)
      cur_name_id = first_spc;
   else if (first_bin != -1)
      cur_name_id = first_bin;

   return _loader_launch(key_buffer, catalogue.dirent[cur_name_id].filename);
}

bool _loader_one_listed(char * key_buffer)
{
   if (!game_configuration.is_cpm && catalogue.entries_listed_found != 1)
      return false;

   if (game_configuration.is_cpm && (catalogue.entries_listed_found != 1 && catalogue.entries_hidden_found != 1))
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

   if (_loader_cpm(key_buffer, format))
      return;

   // first we try to find classic run filenames
   if (_loader_find_file(key_buffer, "DISC"))
      return;

   if (_loader_find_file(key_buffer, "DISK"))
      return;

   if (_loader_find_file(key_buffer, "JEU.BAS"))
      return;

   if (_loader_one_listed(key_buffer))
      return;

   if (_loader_hidden(key_buffer, format))
      return;

   #ifdef LOADER_DEBUG
   printf("[  LOADER  ] finally trying with bas/bin/dot files\n");
   #endif

   if(!_loader_find(key_buffer, format))
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
