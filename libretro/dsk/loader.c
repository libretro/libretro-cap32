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

#include "loader.h"
#include "formats.h"
#include "catalog.h"

extern DPB_list formats_list;
extern t_drive driveA;

extern char catalog_dirent[CAT_MAX_ENTRY][CAT_NAME_SIZE];
extern int catalog_entry;

void loader_init () {
   formats_init();
}

bool _loader_find (char * text) {
   int found = 0;
   int first_bas = -1;
   int first_spc = -1;
   int first_bin = -1;
   int cur_name_id = 0;

   for (int index = 0; index < catalog_entry; index++) {
      char* scan = strchr(catalog_dirent[index], '.');
      if (!scan)
         continue;

      if (!strcasecmp(scan + 1, "BAS")) {
         if (first_bas == -1) first_bas = index;
         found = true;
      } else if (!strcasecmp(scan + 1, "")) {
         if (first_spc == -1) first_spc = index;
         found = true;
      } else
      if (!strcasecmp(scan + 1, "BIN")) {
         if (first_bin == -1) first_bin = index;
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

   // check added to avoid warning on gcc >= 8
   if(snprintf(text, LOADER_MAX_SIZE, "RUN\"%s", catalog_dirent[cur_name_id]) < 0) {
      printf("autoload: snprintf failed\n");
      return false;
   }

   return true;
}

void _loader_failed (char * text, bool cpc_dsk_system, bool is_hexagon) {
   if (cpc_dsk_system) {
      strcpy(text, "|CPM");
      return;
   }

   if (is_hexagon) {
      strcpy(text, "RUN\"DISK");
      return;
   }

   // usefull to user see catalogue files (or run DSK protections)
   strcpy(text, "CAT");
}

void loader_run (char * key_buffer) {
   DPB_type *dpb = NULL;
   t_drive *current_drive = &driveA; 

   memset(key_buffer, 0, LOADER_MAX_SIZE);
   dpb = format_find(current_drive);

   if (dpb == NULL) {
      printf("[LOADER] FORMAT NOT FOUND.\n");
      strcpy(key_buffer, "CAT");
      return;
   }

   archive_init(dpb->DRM, dpb->OFS, current_drive);
   if(!_loader_find(key_buffer)) {
      _loader_failed(
         key_buffer,
         dpb->SEC1_side1 == DSK_TYPE_SYSTEM,
         format_hexagon_protection(current_drive)
      );
   }
}
