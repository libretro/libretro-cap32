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

#include "../../cap32/cap32.h"

typedef enum
{
   ORDER_SIDES,
   ORDER_CYLINDERS,
   ORDER_EAGLE
} BLOCK_ORDER;

/****** Disk Parameter Block ******/

typedef struct {
/* extended DPB info, needed for format, in DPB_store */
   /* KT - removed */
   /*unsigned char  ID;*/  /* Identifier */

   unsigned short SEC1_side1;/* 1. SECtor number (0, >1, >41h, >C1h) */
   unsigned short SEC1_side2;
   unsigned short SECS;/* number of sectors per track (8, >9) */
   unsigned short TRKS;/* number of tracks per side (>40, 80) */
   unsigned short HDS; /* number of heads per disk (>1, 2) */   
   unsigned short BPS; /* Bytes Per Sector (128, 256, >512, 1024) */
   
/* original Disk Parameter Block (> marks CPC defaults) */
   unsigned short SPT; /* records Per Track (18, 20, 30, 32, 34, >36, 40) */
   unsigned char  BSH; /* Block SHift ...      2^BSH = BLM+1 = Rec/Block */
   unsigned char  BLM; /* BLock Mask (>3/7, 4/15) */
   unsigned char  EXM; /* EXtent Mask (0, 1)   Number of Extents/Entry - 1 */
   unsigned short DSM; /* max. blocknumber = number of blocks - 1 */
   unsigned short DRM; /* DiRectory size - 1 (31, >63, 127) */
   unsigned char  AL0; /* \ DRM in binary (80h/0, >C0h/0, F0h/0) */
   unsigned char  AL1; /* / (1 bit=1 block, 11000000/00000000 = 2 blocks) */
   unsigned short CKS; /* ChecK recordS,nb of rec in dir (8, >16, 32) */
   unsigned short OFS; /* OFfSet, reserved tracks (1, >2, 3, 4) */

/* extended DPB info, automatically calculated */
   unsigned short BLS; /* BLock Size in bytes (>1024, 2048)*/
   unsigned char  SYS; /* CP/M present in system tracks - bool: true/false */
   unsigned short DBL; /* Directory BLocks = CKS/8 */

   BLOCK_ORDER   order;
   unsigned char   side0_hd;   /* if accessing side 0, use this head value */
   unsigned char   side1_hd;   /* if accessing side 1, use this head value */

   /* sector id skew */
   int      skew;

   int      num_extents;    /* calculated from EXM*/

   char *label;   /* label dsk type */
} DPB_type;

/* KT - added link list of DPB's */
typedef struct DPB_list_entry
{
	/* ident */
	char	*ident;
	/* format description */
	char	*description;
	/* dpb for this format */
	DPB_type dpb;
	/* pointer to next entry in list */
	struct DPB_list_entry *next;
} DPB_list_entry;

typedef struct DPB_list
{
	/* pointer to first entry in list */
	DPB_list_entry *first;
} DPB_list;

#define DSK_TYPE_DATA    0x1C
#define DSK_TYPE_SYSTEM  0x41

void formats_init();
void formats_clean();
bool sector_exists(t_track *track, unsigned short sector);
bool format_hexagon_protection(t_drive *drive);
DPB_type *format_find (t_drive *drive);