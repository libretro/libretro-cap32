/****************************************************************************
 *  Caprice32 libretro port
 *
 *  code adapted from libcpc, ported to retroarch by
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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "formats.h"

DPB_list formats_list;
DPB_type dsk_type_system = {
   0x041, /* SEC1 side 1 */
   0x041, /* SEC1 side 2 */
   9,  /* SECS */
   40,  /* TRKS */
   1,  /* HDS */
   512,  /* BPS */

   36,  /* SPT */
   3,  /* BSH */
   7,  /* BLM */
   0,  /* EXM */
   170,  /* DSM */
   63,  /* DRM */
   0xC0,  /* AL0 */
   0x00,  /* AL1 */
   16,  /* CKS */
   2,  /* OFS */
   0,0,0,
   ORDER_SIDES,0,0,5,0,NULL
};

DPB_type dsk_type_system_big = {
   0x041, /* SEC1 side 1 */
   0x041, /* SEC1 side 2 */
   10,  /* SECS */
   40,  /* TRKS */
   1,  /* HDS */
   512,  /* BPS */

   36,  /* SPT */
   3,  /* BSH */
   7,  /* BLM */
   0,  /* EXM */
   170,  /* DSM */
   63,  /* DRM */
   0xC0,  /* AL0 */
   0x00,  /* AL1 */
   16,  /* CKS */
   2,  /* OFS */
   0,0,0,
   ORDER_SIDES,0,0,5,0,NULL
};


DPB_type dsk_type_data = {
   0x0c1, /* SEC1 side 1 */
   0x0c1, /* SEC1 side 2 */
   9,  /* SECS */
   40,  /* TRKS */
   1,  /* HDS */
   512,  /* BPS */

   36,  /* SPT */
   3,  /* BSH */
   7,  /* BLM */
   0,  /* EXM */
   179,  /* DSM */
   63,  /* DRM */
   0xC0,  /* AL0 */
   0x00,  /* AL1 */
   16,  /* CKS */
   0,  /* OFS */
   0,0,0,
   ORDER_SIDES,0,0,5,0,NULL
};


DPB_type dsk_type_data_big = {
   0x0c1, /* SEC1 side 1 */
   0x0c1, /* SEC1 side 2 */
   10,  /* SECS */
   40,  /* TRKS */
   1,  /* HDS */
   512,  /* BPS */

   36,  /* SPT */
   3,  /* BSH */
   7,  /* BLM */
   0,  /* EXM */
   179,  /* DSM */
   63,  /* DRM */
   0xC0,  /* AL0 */
   0x00,  /* AL1 */
   16,  /* CKS */
   0,  /* OFS */
   0,0,0,
   ORDER_SIDES,0,0,5,0,NULL
};

// FIXME: remove
char * _upper(char *s)  {
  char  *p=s;
  while (*s)
  {
    *s = toupper(*s);
    s++;
  }
  return p;
}

void _dpb_list_add_entry(DPB_list *list, DPB_list_entry *entry)
{
   DPB_list_entry *cur_entry;

   /* if list empty add as first item */
   if (list->first==NULL)
   {
      list->first = entry;
   } else {
      /* add to end of list */
      cur_entry = list->first;

      while (cur_entry->next!=NULL) {
         cur_entry = cur_entry->next;
      }
      cur_entry->next = entry;
   }
}

void _dpb_list_add_item(DPB_list *list,char *description, char *ident, DPB_type *dpb)
{
   DPB_list_entry *entry = (DPB_list_entry *) malloc(sizeof(DPB_list_entry));
   if (entry!=NULL) {
      if (description==NULL) {
         entry->description = NULL;
      } else {
         entry->description = (char *) malloc(strlen(description) + 1);
         if (entry->description!=NULL) {
            strcpy(entry->description, description);
         }
      }

      if (ident == NULL) {
         entry->ident = NULL;
      } else {
         entry->ident = (char *) malloc(strlen(ident) + 1);
         if (entry->ident!=NULL) {
            strcpy(entry->ident, ident);
            _upper(entry->ident);
         }
      }
      /* copy dpb */
      memcpy(&entry->dpb, dpb, sizeof(DPB_type));

      /* if cylinders is 40, but used in a 80 track drive, then the drive
      head is automatically double stepped */
      /* want to use cylinders from .def because this is good for discs
      where only 60 tracks are used */
      entry->next = NULL;
   }

   _dpb_list_add_entry(list, entry);
}

void formats_init() {
   formats_list.first = NULL;
   _dpb_list_add_item(&formats_list,"data", "data", &dsk_type_data);
   // fix big data format (ex: Prehistorik II)
   _dpb_list_add_item(&formats_list,"data_b", "data_b", &dsk_type_data_big);

   /* system */
   _dpb_list_add_item(&formats_list,"system", "system", &dsk_type_system);
   _dpb_list_add_item(&formats_list,"system_b", "system_b", &dsk_type_system_big);
}