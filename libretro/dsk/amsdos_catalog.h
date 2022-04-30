/****************************************************************************
 *  Caprice32 libretro port
 *
 *  format.h is inspired on caprice-forever by Frédéric Coste (Fredouille)
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

#ifndef GFX_AMSDOSCATALOG_H__
#define GFX_AMSDOSCATALOG_H__

#include "cap32.h"

#define CAT_MAX_ENTRY 64
#define CAT_NAME_SIZE 20

typedef struct
{
   char filename[CAT_NAME_SIZE];
   bool is_hidden;
   bool is_readonly;
} amsdos_entry_t;

typedef struct {
   bool has_cat_art;
   bool probe_cpm;
   // normal entries
   int last_entry;
   amsdos_entry_t dirent[CAT_MAX_ENTRY];
   int entries_listed_found;
   int entries_hidden_found;
   int first_listed_dirent;
   int first_hidden_dirent;
   int track_hidden_id;
   int track_listed_id;
} catalogue_info_t;

extern catalogue_info_t catalogue;

int catalog_probe(t_drive *drive, unsigned char user);

#endif