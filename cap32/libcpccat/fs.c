/*        <<<<Last Modified: Thu Feb 08 15:08:10 1996>>>>
------------------------------------------------------------------------------

  =====
  CPCfs  --  f s . c  --  Manageing the Filesystem
  =====

  Version 0.85        (c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef __EMSCRIPTEN__
#include <ctype.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <zlib.h>
#include "fs.h"

#include "cpc_cat.h"

int  cpc_dsk_num_entry = 0;
char cpc_dsk_dirent[CPC_MAX_ENTRY][CPC_NAME_SIZE];

/****** Variables ******/
uchar  *blk_alloc = NULL;  /* Block-Allocation-Table, */
struct d_header disk_header;
DPB_list_entry *dpb_list_entry = NULL;
DPB_type *dpb = NULL;  /* pointer to current DPB */
DPB_list dpb_list;
int  BLKNR_SIZE   = 1;
int  BLKNR     = 16;

int  cur_trk = -1;    /* flag for no track loaded */
int  cur_hd = -1;
int  cur_blk = -1;
/* KT - added directory dirty flag - directory is written only if it has changed! */
int directory_dirty = 0;
/* KT - added track dirty flag - track is written only if it has changed! */
int track_dirty = 0;
uchar  *track = NULL;  /* 0x100 header + track data */
unsigned char filler;  /* copied from track header */
DirEntry *directory = (DirEntry*)NULL;
uchar  *block_buffer = NULL;  /* Buffer for 1024 or more bytes */
static int    imagefile = -1;    /* ... handle of imagefile */
static gzFile gz_imagefile = 0;
static int  gz_format = 0;

int image_type = 0;

static int my_open(const char* name)
{
   char *scan = strrchr(name, '.');
   gz_format = 0;
   if (scan && (!strcasecmp(scan, ".dsz")))
      gz_format = 1;

   if (gz_format)
   {
      gz_imagefile = gzopen(name, "rb+");
      if (! gz_imagefile) return -1; 
   }
   else
   {
	int fmode = O_RDONLY;
#ifdef _WIN32
 	fmode |=O_BINARY;
#endif
      imagefile = open(name, fmode);
      if (imagefile < 0) return -1;
   }
   return 0;
}

static int my_close(void)
{
  int ret_val = 0;
  if (gz_format)
  {
    if (gz_imagefile)
    {
      ret_val = gzclose(gz_imagefile);
      gz_imagefile = 0;
    }

  }
  else
  {
    if (imagefile >= 0)
    {
      ret_val = close(imagefile);
      imagefile = -1;
    }
  }
  return ret_val;
}

static int my_read(uchar* a_ptr, size_t a_size)
{  
  if (gz_format)
    return gzread( gz_imagefile, a_ptr, a_size);
  return read(imagefile, a_ptr, a_size);
}

static int my_lseek(long offs, int whence) 
{
  if (gz_format)
    return gzseek( gz_imagefile, offs, whence);
  return lseek(imagefile, offs, whence);
} 



void build_cpm_name_32(char *buf, int user, char *root, char *ext) {
/*   ^^^^^^^^^^^^^^^^^
The same function as <build_cpm_name> except, that <root> and <ext> are
considered as padded with ' ' (= ASCII 32) (as in <DirEntry>)
*/
int  j;

  *buf = 0;
  if (user==-2)  {strcpy(buf,"*:"); buf +=2;}
  if (user>=0)  buf += sprintf(buf,"%d:",user);

  memcpy(buf,root,8);
  j=7; while (buf[j] == ' ') {j--;}; j++;
  buf[j]='.'; j++;
  if (strncmp(ext,"   ",3)!=0) {
    memcpy(&buf[j],ext,3); j+=2;
    while (buf[j] == ' ') {j--;}; j++;
  }
  buf[j]=0;
}


bool tag_ok () {
/*   ^^^^^^ */

  /* KT - 27/04/2000 - Support for Extended disk image */
  if (strncmp("EXTENDED", (char *)disk_header.tag,8)==0) 
  {
    image_type = 1;
    return TRUE;
  }
  else
  if (strncmp("MV - CPC",(char*)disk_header.tag,8)==0)
  {
    image_type = 0;
    return TRUE;
  }
  
  return FALSE;
}

void alloc_block(int blk) {
/*   ^^^^^^^^^^^ */

  unsigned long blk_byte_offset;
  unsigned long blk_bit_offset;

  /* KT - if a disc was opened and the format was not detected correctly, then
  a invalid blk could be passed to this routine. Previously this attempted
  to read off the end of the array and cause an error. I've added this check
  to stop that from occuring and alerting this fact to the user */
  if ((blk<0) || (blk>=(dpb->DSM+1)))
  {
    /* invalid block */
    return;
  }

  blk_byte_offset = blk>>3;
  blk_bit_offset = blk & 0x07;

  blk_alloc[blk_byte_offset]|=(1<<blk_bit_offset);
}




void free_block(int blk) {
/*   ^^^^^^^^^^ */

  unsigned long blk_byte_offset;
  unsigned long blk_bit_offset;

  if ((blk<0) || (blk>=(dpb->DSM+1)))
  {
    /* invalid block */
    return;
  }

  blk_byte_offset = blk>>3;
  blk_bit_offset = blk & 0x07;

  blk_alloc[blk_byte_offset]&=~(1<<blk_bit_offset);
}


bool is_free_block(int blk) {
/*   ^^^^^^^^^^^^^ */

  unsigned long blk_byte_offset;
  unsigned long blk_bit_offset;

  if ((blk<0) || (blk>=(dpb->DSM+1)))
  {
    /* invalid block */
    return FALSE;
  }

  blk_byte_offset = blk>>3;
  blk_bit_offset = blk & 0x07;

  return (bool)((blk_alloc[blk_byte_offset]&(1<<blk_bit_offset))==0);
}


/* KT - added support for different block orders */
void  calc_t_s_h(int blk,int *track, int *sector, int *head)
{
  int t,s,h;

  unsigned long byte_offset;
  unsigned long sector_offset;

  t = -1;
  s = -1;
  h = -1;

  /* calc byte offset from block index and bytes per block */
  byte_offset = blk*dpb->BLS;
  /* calc sector offset using sector size */
  sector_offset = byte_offset/dpb->BPS;

  /* calculate sector index within track by taking remainder after dividing by number of sectors
  per track (0-dpb->SECS). Sector ID is added on when sector is accessed! */
  s = sector_offset % dpb->SECS;

  /* calculate track offset by dividing by number of sectors */
  t = (sector_offset / dpb->SECS) + dpb->OFS;

  /* order system for blocks */
  switch (dpb->order)
  {
    /* track 0 side 0, track 1 side 0... track n side 0, track n side 1, track n-1 side 1 ... track 0 side 1 */
    case ORDER_CYLINDERS:
    {
      /* calc side */
      h = t/dpb->TRKS;
    
      /* odd side */
      if (h & 1)
      {
        /* tracks in reverse */
        t = dpb->TRKS - (t % dpb->TRKS);
      }
      else
      {
        /* tracks in order */
        t = t % dpb->TRKS;
      }
    }
    break;
    
    /* track 0 side 0, track 0 side 1, track 1 side 0, track 1 side 1... track n side 0, track n side 1*/
    case ORDER_SIDES:
    {
      /* calc side */
      h = t % dpb->HDS;
      /* calc track */
      t = t/dpb->HDS;
    }
    break;

    /* track 0 side 0, track 1 side 0... track n side 0, track 0 side 1, track 1 side 1....track n side 1 */
    case ORDER_EAGLE:
    {
      /* calc side */
      h = t / dpb->TRKS;

      /* calc track */
      t = t % dpb->TRKS;
    }
    break;
  }
  *track = t;
  *sector = s;
  *head = h;
}


#if 0
/* Calculate track, sector, and head number of the first sector of block
<blk>.

A quick picture:

    secs
     .------^-------.
     _______________
    / -2- - - - - >/|  } heads
   /______________/ | }
      / | -1- - - - - > | |
     :  |    | |
     :  |    | |
     :  |    | |
trks =  |  -4- - - - - -| |
     :  |    | |
     :  | -3- - - - - > | |
     :  |    | |
      \ |_______________|/


numbering scheme:
first secs, then heads, then tracks
    %s         /s%h    /s/h%t  has to be applied on running sec number

s = sec/trk, h = heads, t = tracks (here infinite)

*/


int trk_calc(int blk) {
/*  ^^^^^^^^ */
  return ((long)blk*dpb->BLS/dpb->BPS + dpb->OFS*dpb->SECS)
    / (dpb->SECS*dpb->HDS);
}

int sec_calc(int blk) {
/*  ^^^^^^^^
The returned number is not shifted by <sec_offset>! */
  return ((long)blk*dpb->BLS/dpb->BPS + dpb->OFS*dpb->SECS) % dpb->SECS;
}

int hd_calc(int blk) {
/*  ^^^^^^^ */
  return ((long)blk*dpb->BLS/dpb->BPS + dpb->OFS*dpb->SECS)
    / dpb->SECS % dpb->HDS;
}
#endif


/* TO BE FIXED ! */

int blk_calc(int hd, int trk, int sec) {
/*  ^^^^^^^^
Return the blocknumber of position <hd>, <trk>, <sec> or -1, if it is a
reserved position
*/
#if 0
  switch (dpb->order)
  {
    /* track 0 side 0, track 1 side 0... track n side 0, track n side 1, track n-1 side 1 ... track 0 side 1 */
    case ORDER_CYLINDERS:
    {



    }
    break;

    case ORDER_EAGLE:  
    {

    }
    break;

    case ORDER_SIDES:
    {



    }
    break;
  }
#endif  
  
  if (trk*dpb->HDS+hd < dpb->OFS) return -1;
  return ((long)sec + hd*dpb->SECS
    + trk*dpb->SECS*dpb->HDS
    - dpb->OFS*dpb->SECS)
    / (dpb->BLS/dpb->BPS);
}
  
/* KT - added common free data function */
void free_image_data(void)
{
  if (blk_alloc)
  {
    free(blk_alloc);  blk_alloc=NULL;
  }
  
  if (track)
  {
    free(track);    track=NULL;
  }
  
  if (directory)
  {
    free(directory);  directory=(DirEntry*)NULL;
  }
  
  if (block_buffer)
  {
    free(block_buffer);  block_buffer=(uchar*)NULL;          
  }
  
  *disk_header.tag = 0;
  dpb = NULL;

  my_close();
  
  cur_trk = -1;
  cur_blk = -1;
  directory_dirty = 0;
}



void abandonimage() 
{
  free_image_data();
}


/********
  Tracks
 ********/

/* KT - get offset to track data. */
unsigned long get_track_offset(int track, int side)
{
  switch (image_type)
  {
    /* standard disk image */
    case 0:
    {
      /* attempt to access a invalid track and side */
      if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
        return 0;

      return (((track*disk_header.nbof_heads) + side) * disk_header.tracksize) + 0x0100;
    }
    /* extended disk image */
    case 1:
    {
      int i;
      unsigned long offset;

      /* attempt to access a invalid track and side */
      if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
        return 0;

      /* unformatted track? */
      if (disk_header.unused[((track*disk_header.nbof_heads)+side)]==0)
        return 0;

      /* for extended disk image, the disk header contains
      size of each track / 256. Use this table to calculate
      the offset to the track */

      offset = 0;

      for (i=0; i<(track*disk_header.nbof_heads)+side; i++)
      {
        offset += (disk_header.unused[i]<<8);
      }

      return offset + 0x0100;
    }

    default:
      break;
  }
  
  return 0;
}

unsigned long get_track_size(int track, int side)
{
  switch (image_type)
  {
    /* standard disk image */
    case 0:
    {
      /* attempt to access a invalid track and side */
      if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
        return 0;
  
      /* return size of track */
      return disk_header.tracksize;
    }
    
    /* extended disk image */
    case 1:
    {
      /* attempt to access a invalid track and side */
      if ((track>=disk_header.nbof_tracks) || (side>=disk_header.nbof_heads))
        return 0;

      /* return track size */
      return ((disk_header.unused[(track*disk_header.nbof_heads)+side])<<8);
    }

    default:
      break;
  }

  return 0;
}

int  is_track_header_valid(unsigned char *track)
{
  struct t_header *track_header = (struct t_header*)track;

  /* header limited to 29 sectors per track */
  if (track_header->SPT>29)
    return 0;

  /* unlikely to have a track where the BPS is 16384 bytes! */ 
  if (track_header->BPS>6)
    return 0;


  return 1;
}

int validate_image(void)
{
  int t,h;
  /* header is already validated */
  for (h=0; h<disk_header.nbof_heads; h++)
  {
    for (t=0; t<disk_header.nbof_tracks; t++)
    {
      int track_offset = get_track_offset(t,h);

      if (track_offset>=0)
      {
        int n;

        int track_size;

        track_size =get_track_size(t,h);

        if (track_size!=0)
        {
          /* formatted track */
          n = my_lseek(track_offset,SEEK_SET);
          if (n == -1L) 
          {
            return 0;
          }

          n = my_read(track,track_size);
          if (n != track_size) 
          {
            return 0;
          }
        
          /* seeked to position ok, and read data */
                          
          /* check track header */
          if (!is_track_header_valid(track))
            return 0;
        }
      }
    }
  }

  return 1;

}



/* calculate the maximum size of a track from the disk image. Use this
to allocate a track buffer */
void malloc_track()
{
  unsigned long max_track_size;

  max_track_size = 0;

  switch (image_type)
  {
    case 0:
    {
      max_track_size = disk_header.tracksize;
    }
    break;

    case 1:
    {
    
      int t,h;

      for (h=0; h<disk_header.nbof_heads; h++)
      {
        for (t=0; t<disk_header.nbof_tracks; t++)
        {
          int track_size;

          track_size = get_track_size(t,h);

          if ((t==0) && (h==0))
          {
            max_track_size = track_size;
          }
          else
          {
            if (track_size>max_track_size)
            {
              max_track_size = track_size;
            }
          }
        }
      }
    }
    break;

    default:
      break;
  }

  track = (void *)malloc(max_track_size);
}

int read_track (int hd, int trk) 
{
/*  ^^^^^^^^^^ */
long  n, pos;
int    track_size;

  if (trk == cur_trk && hd == cur_hd) {
    return 0;
  }

  //printf("[rt(%d,%d)] ",hd,trk);

  track_size = get_track_size(trk,hd);
  
  if (track_size == 0)
    return -1;
  
  pos = get_track_offset(trk, hd);
    
  if (pos==0)
    return -1;

  n = my_lseek(pos,SEEK_SET);
  if (n == -1L) {
    //fprintf(stdout,"Image corrupt! I cannot position on track %d!", trk);
    abandonimage();
    return -1;
  }

  n = my_read(track,track_size);
  if (n != track_size) {
    //fprintf(stdout,"Image corrupt! I can only read %ld bytes " "of track %d (instead of %d bytes)!",
    //        n,trk,disk_header.tracksize);
    abandonimage();
    return -1;
  }
  cur_trk = trk;
  cur_hd  = hd;
  track_dirty = 0;
  return 0;
}


/* KT - added support for different block orders */
bool next_sector (int *hd, int *trk, int *sec) {
/*   ^^^^^^^^^^^
Assumes <sec> without offset. Answer TRUE if the <trk> or <hd> is changed. */

  int prev_hd;
  int prev_trk;

  prev_hd = *hd;
  prev_trk = *trk;
  
  switch (dpb->order)
  {
    case ORDER_CYLINDERS:
    {
      (*sec)++;

      if ((*sec) >= dpb->SECS)
      {
        (*sec) -= dpb->SECS;

        if ((*hd) & 1)
        {
          (*trk)--;

          if ((*trk)<0)
          {
            (*trk) = 1;
            (*hd)++;
          }
        }
        else
        {
          (*trk)++;

          if ((*trk)>dpb->TRKS)
          {
            (*trk) = dpb->TRKS-1;
            (*hd)++;
          }
        }
      }
    }
    break;

    case ORDER_SIDES:
    {
      (*sec)++;
      if (*sec >= dpb->SECS) 
      {
        *sec -= dpb->SECS;
        (*hd)++;
        if (*hd >= dpb->HDS) 
        {
          *hd = 0;
          (*trk)++;
        }
      }
    }
    break;

    case  ORDER_EAGLE:
    {
      (*sec)++;

      if ((*sec) >= dpb->SECS)
      {
        (*sec) -= dpb->SECS;

        (*trk)++;

        if ((*trk)>dpb->TRKS)
        {
          *trk = 0;
          (*hd)++;
        }
      }
    }
    break;


  }

  /* if head, track or head and track have changed, then force a write of track */
  if ((prev_hd!=(*hd)) || (prev_trk!=(*trk)))
  {
    return TRUE;
  }

  return FALSE;
}



#if 0
bool next_sector (int *hd, int *trk, int *sec) {
/*   ^^^^^^^^^^^
Assumes <sec> without offset. Answer TRUE if the <trk> or <hd> is changed. */
  (*sec)++;
  if (*sec >= dpb->SECS) {
    *sec -= dpb->SECS;
    (*hd)++;
    if (*hd >= dpb->HDS) {
      *hd = 0;
      (*trk)++;
    }
    return TRUE;
  }
  return FALSE;
}
#endif

/* given a sector id, search through sector id's in track header
to find the position of the sector in the track. This allows
for interleaved sectors */
int  get_sector_pos_in_track(int r,int h)
{
  struct t_header *track_header = (struct t_header*)track;
  struct s_info  *sector_info = &track_header->sector[0];
  int i;
  int spt;
  
  /* defines number of sector id's stored */
  spt = track_header->SPT;

  for (i=0; i<spt; i++)
  {
    /* match sector id */
    if (sector_info->sector==r)
    {
      /* match head value */
      if (sector_info->head == h)
      {
        return i;
      }
    }

    sector_info++;
  }

  /* sector not found */
  return -1;
}

int get_sector_data_offset_extended(int pos)
{
  struct t_header *track_header = (struct t_header*)track;
  struct s_info  *sector_info = &track_header->sector[0];
  int i;
  int sector_offset;

  sector_offset = 0;

  for (i=0; i<pos; i++)
  {
    int sector_size;

    sector_size = sector_info->unused[0] +
          (sector_info->unused[1]<<8);
  
    sector_info++;
    sector_offset += sector_size;
  }

  return sector_offset;
}

/* N value used in formatting and reading/writing data. This defines
the amount of data stored in the sector */
int  get_sector_size_from_n(int n)
{
  return (1<<n)<<7;
}

uchar  *get_sector_data_ptr(int r,int h)
{
   struct t_header *track_header = (struct t_header*)track;

   /* using sector id find sector pos in track */
   int pos = get_sector_pos_in_track(r,h);
   int sector_offset = 0;

   if (pos==-1)
      return NULL;

   switch (image_type)
   {
      case 0:
         {
            /* for standard disk images, N in the track header should
               define the largest sector size */
            int sector_size = get_sector_size_from_n(track_header->BPS);

            sector_offset = 0x0100 + pos*sector_size;
         }
         break;

      case 1:
         sector_offset = 0x0100 + get_sector_data_offset_extended(pos);
         break;

      default:
         return NULL;
   }

   return track+sector_offset;
}

uchar *read_block (int blk) {
/*    ^^^^^^^^^^
Read block <blk> into a buffer and return its address, or NULL on error */
int  trk, sec, hd;
int  filled = 0;
uchar *sector_ptr;


  if (blk==cur_blk) return block_buffer;

  if ((blk<0) || (blk>=(dpb->DSM+1)))
    return NULL;

  //printf("[rb(%d)] ",blk);

/*  trk = trk_calc (blk);
  sec = sec_calc (blk);
  hd  = hd_calc (blk);*/

  calc_t_s_h(blk, &trk, &sec, &hd);

  while (filled < dpb->BLS) 
  {
    int r,h;

    if (read_track(hd,trk)) 
      return NULL;

    r = sec;
    
    if (hd==0)
    {
      h = dpb->side0_hd;
      r += dpb->SEC1_side1;
    }
    else
    {
      h = dpb->side1_hd;
      r += dpb->SEC1_side2;
    }
    
    sector_ptr = get_sector_data_ptr(r,h);

    if (sector_ptr==NULL)
    {
      //fprintf(stdout, "Warning: cannot find sector, unable to read data!");
      return NULL;
    }
    
    memcpy(block_buffer+filled, sector_ptr, dpb->BPS);
        
    filled += dpb->BPS;
    next_sector (&hd,&trk,&sec);
  }
  cur_blk = blk;
  return block_buffer;
}


/***********
  Directory
 ***********/


/* local definitions for glob_cpm_* */

#define GLOB_ENV_MAX  3
int  glob_env = 0;
  /* determines which global variable-set should <glob_cpm_*> use, */
  /* necessary for nested globs */

/* local definitions for update_directory */

struct pair {uchar en; uchar ex;};

int cmp_pair(struct pair *x, struct pair *y) {
  if (x->ex < y->ex) return -1;
  if (x->ex > y->ex) return 1;
  return 0;
}

void update_directory(int set_dirty) {
/*   ^^^^^^^^^^^^^^^^
(Re-)Organizes the directory structure (Packing the name, making a linked
list) */

int  i, j;

/* a dynamic array of (entry,extent) pairs */
struct pair  *map;

  if (set_dirty)
  {
    directory_dirty = 1;
  }


  //printf("[ud] ");
  map = (struct pair*)malloc(sizeof(DirEntry)*(dpb->DRM+1));

  /* KT - added memory fail check */
  if (map==NULL)
    return;

/****** packing a name of kind "FOO  BAR" to "FOO.BAR\0" ******/
  for (i=0;i<=dpb->DRM;i++) {
    if (directory[i].user == 0xE5) continue;
    build_cpm_name_32((char*)directory[i].name, -1,
          (char*)directory[i].root,
          (char*)directory[i].ext);
  }


/****** organizing the directory structure as linked list ******/

/* set entries in the directory to "not visited"
<size> = 0 : never visit; <size> = -1 : not yet visited */
  for (i=0;i<=dpb->DRM;i++) {
    if (directory[i].user == 0xE5) /*NOT <filler>, E5 is required*/
      directory[i].size = 0;  /* never visit empty entries */
    else
      directory[i].size = -1; /* not visited */
    directory[i].first = FALSE;
    directory[i].next  = -1;
  };

/* scan the entries */
  for (i=0;i<=dpb->DRM;i++) {
    if (directory[i].size > -1) continue;

/* reset the map */
    for (j=0;j<=dpb->DRM;j++) {map[j].en=(uchar)j;map[j].ex=(uchar)0xFF;}

/* fill the map with <extent> from the directory */
    map[i].ex = directory[i].extent;
    for (j=0;j<=dpb->DRM;j++) {
      if ((directory[j].size == -1) &&
          (directory[j].user == directory[i].user) &&
          (i!=j) &&
          (strcmp((char*)directory[i].name,
            (char*)directory[j].name)==0)) {
          map[j].ex = directory[j].extent;
          directory[j].size = 0;
      }
    }
/* sort the map according to map[].ex, not necessary in most cases */
    qsort(map,dpb->DRM+1,sizeof(struct pair),
      (int(*)(const void*,const void*))cmp_pair);

/* fill <first>, <size> and <next> from the map */
    directory[map[0].en].first = TRUE;
    j=1;
    while (map[j].ex < 0xFF) {
      directory[map[j-1].en].next = map[j].en;
      j++;
    }
    directory[map[j-1].en].next = -1;

/* the filesize located in the first fileentry can be calculated from the
<extent> and <rec> fields of the last fileentry */

    /* KT: changed calculation to use extent number, and block size */
    directory[map[0].en].size =
      ((long)directory[map[j-1].en].extent * EXTENTSIZE) 
      + (directory[map[j-1].en].rec * RECORDSIZE);

    
  } /* for i */

  free(map); map=NULL;
}


void get_directory() 
{
/*   ^^^^^^^^^^^^^ */
int  i,j,off;
uchar  *buf;
int  mask;

  //printf("[rd] ");
  for (i=0; i<=dpb->DRM; i++)
  {
    directory[i].user = 0x0e5;
  }
  
/* reading the directory data */
  for (i=0;i<=dpb->DRM;i++) {
    buf = read_block((signed)i*32/dpb->BLS);

    if (buf==0)
      break;

    off = i*32 % dpb->BLS;
/* user, name, ... */
    directory[i].user  = buf[off+0];
    for (j=0;j<8;j++) directory[i].root[j] = (buf[off+j+1] & 0x7F);
    for (j=0;j<3;j++) directory[i].ext[j]  = (buf[off+j+9] & 0x7F);   
    directory[i].name[0]  = 0;
          
    directory[i].extent  = buf[off+12];
    directory[i].unused[0]  = buf[off+13];
    directory[i].unused[1]  = buf[off+14];
    directory[i].rec  = buf[off+15];

/* attributes */
    mask=0x1;
    directory[i].attr = 0;
    for (j=11;j>0;j--) {
      if (buf[off+j]&0x80) directory[i].attr |= mask;
      mask <<= 1;
    }

/* block pointer */
    for (j=0;j<16;j++) directory[i].blk[j] = 0;
    if (BLKNR_SIZE==1) {
      for (j=0;j<16;j++) {
        directory[i].blk[j] = buf[off+16+j];
      }
    } else if (BLKNR_SIZE==2) {
      for (j=0;j<8;j++) {
        directory[i].blk[j] = (buf[off+16+2*j]
              + (buf[off+17+2*j]<<8));
      }
    }
  }

  update_directory(0);
  /*for (i=0;i<dpb->DBL;i++) alloc_block(i,0);*/ /* 0 is not correct! */
        
  /* marking all blocks as free */
  for (j=0;j<=dpb->DSM;j++) free_block(j);

  {
    /* mark allocated blocks from AL1, AL0 as used */
    /* AL0 and AL1 make a bitmask. AL0 is bits 15..8, AL1 is
    bits 7..0. If a bit is set, a block is allocated. Block indexs
    go from 0-15, bits 15..0 */
    unsigned long bit_mask;

    bit_mask = (dpb->AL0<<8) | dpb->AL1;

    for (i=0; i<16; i++)
    {
      /* is bit set? */
      if (bit_mask & 0x08000)
      {
        /* yes, alloc block */
        alloc_block(i);
      }
      /* shift for next bit */
      bit_mask = bit_mask<<1;

    }
  }

  /* mark blocks used by files as allocated */
  cpc_dsk_num_entry = 0;
  for (i=0;i<=dpb->DRM;i++) 
  {
    if ((directory[i].user!=0xE5) && (directory[i].first)) {
      strncpy((char *)cpc_dsk_dirent[cpc_dsk_num_entry], (char *)directory[i].name, CPC_NAME_SIZE);
      cpc_dsk_num_entry++;
      if (cpc_dsk_num_entry >= CPC_MAX_ENTRY) break;
    }
    for (j=0;j<BLKNR;j++) 
    {
      if (directory[i].user!=0xE5)
      {
        /* KT - changed. will quit out at the first zero block index found */
        if (directory[i].blk[j])
        {
          alloc_block(directory[i].blk[j]);
        }
        else
          break;
      }
    }
  }
}


/*******
  Image
 *******/

int  get_bit_count_from_mask(unsigned long mask)
{
  int bit_count;

  bit_count = 0;

  while ((mask & 1)!=0)
  {
    bit_count++;
    mask = mask>>1;
  }

  return bit_count;
}


/* at this point we should have a format which matches the disk image format */
/* TRKS in DPB is not good. In 22disk this defines the physical disk structure. */
void update_dpb(DPB_type *dpb, uchar *track) {
/*   ^^^^^^^^^^
Determine the extended DPB data out of <dpb> and the sample track <track>.
Complete the extension parts of <dpb>. <track> must be read in first!
*/
  int BLKNR_MAX;
  int sectors_per_dir;

  dpb->BLS  = 1 << (dpb->BSH + 7); /* or 2^BSH*128 */
  
  /* KT - already present in format description */
#if 0
/* an image must exist, do not call form <format>! */
  dpb->SEC1 = ((struct t_header*)track)->sector[0].sector;
  dpb->SECS = ((struct t_header*)track)->SPT;
  
  /* KT - Removed because they are already specified */
  /* the next two elements should already be set */
  dpb->TRKS = disk_header.nbof_tracks;
  dpb->HDS  = disk_header.nbof_heads;
#endif

  dpb->SYS  = (dpb->OFS>0) && (*(track+0x100)) != filler;
  dpb->DBL  = 32 * (dpb->DRM+1) / dpb->BLS; /* or often CKS/8 */

  /* KT - Removed because this is already specified */
#if 0
  /* DSM will be specified, so no need to calculate */

  dpb->DSM = (dpb->TRKS*dpb->HDS*dpb->SECS) / (dpb->BLS/dpb->BPS);
/* subtract reserved tracks */
  dpb->DSM -= dpb->OFS * dpb->SECS / (dpb->BLS/dpb->BPS); 
  dpb->DSM--;
#endif

  /* KT - Added because it is no longer stored! */
  dpb->SPT = (dpb->SECS*dpb->BPS)/RECORDSIZE;
  
  sectors_per_dir = (((dpb->DRM+1)<<5)+(dpb->BPS-1))/dpb->BPS;
  sectors_per_dir = sectors_per_dir * dpb->BPS;
  
  dpb->CKS = sectors_per_dir/RECORDSIZE;


   if (dpb->DSM>=256) {
/* 2 byte pointer and 8 pointer per entry max */
    BLKNR_SIZE = 2;
    BLKNR_MAX = 8;
  }
  else
  {
/* 1 byte pointer and 16 pointer per entry max */
    BLKNR_SIZE = 1;
    BLKNR_MAX = 16;
  }
  
  /* if the extent mask is a real mask, then we can calculate the number of 1 bits in it.
  From this we can calculate the number of extents */
  /* the function will only work if EXM is 1,3,7,15.... i.e. if bit 3 is set, bits 2,1 and 0 must be set */
  /* it will not handle isolated bits! e.g. EXM=8*/
  /* get number of extents from exm bit mask */
  dpb->num_extents = 1<<get_bit_count_from_mask(dpb->EXM);


  /* I hope this should correctly get the number of blocks per directory entry */
  /* a directory entry can only describe a maximum of 16384 bytes */
  {
    unsigned long max_size;

    /* calculate the max size based on the max number of blocks that
    can be stored in the directory entry. The max number of blocks
    is dependant on DSM. If DMS<=255, then 16 blocks can be stored,
    else 8 blocks can be stored */
    max_size = BLKNR_MAX * dpb->BLS;

    /* divide max size by EXM+1. If this is greater than 16384,
    calculate the number of blocks we can store */
    if ((max_size/dpb->num_extents)>EXTENTSIZE)
    {
      BLKNR = (EXTENTSIZE*dpb->num_extents)/dpb->BLS;
    }
    else
    {
      /* set to max allowed */
      BLKNR = BLKNR_MAX;
    }
  }
}   

int sector_exists(uchar *track, ushort SEC1)
{
   struct t_header *track_header = (struct t_header *)track;
   int i;
  struct s_info *sector_info = &track_header->sector[0];
  int spt;

  spt = track_header->SPT;

  /* check if SEC1 exists in sectors available on track 0 */
  for (i=0; i<spt; i++)
  {
    if (sector_info->sector == SEC1)
    {
      return 1;
    }

    sector_info++;
  }

  return 0;
}

/* KT - added select format from those available */
int  select_format(uchar *track)
{
  struct t_header *track_header = (struct t_header *)track;
  int spt;
  DPB_list_entry *cur_entry;
  DPB_list_entry *dpb_entry_found = NULL;
  int found_count = 0;
  spt = track_header->SPT;

  /* go through list of supported formats */
  cur_entry = dpb_list.first;

  while (cur_entry!=NULL)
  {
    /* check if first sector exists in list of sectors in track */
    if (sector_exists(track, cur_entry->dpb.SEC1_side1))
    {
      /* sector exists */
      
      /* correct sectors per track? */
      if (spt == cur_entry->dpb.SECS)
      {
        /* correct number of sides? */
        if (disk_header.nbof_heads==cur_entry->dpb.HDS) 
        {
          if (disk_header.nbof_tracks  >=40 /*== cur_entry->dpb.TRKS*/)
          {
            // found a format that appears to match the format on the disc
            if (dpb_entry_found==NULL)
            {
              dpb_entry_found = cur_entry;
            }
            
            //fprintf(stdout, "Found format \"%s\" matching disk image format", cur_entry->ident);
            found_count++;
          }
        }

      }

    }

    cur_entry = cur_entry->next;
  }

  /* no entry found */
  if (dpb_entry_found==NULL)
  {
    /* format not recognised */
  
    /* no format found */
    return -1;
  }

  if (found_count==1)
  {
    dpb = &dpb_entry_found->dpb;
    dpb_list_entry = dpb_entry_found;

    /* found it - update it */
    update_dpb(dpb,track);

    return 0;
  }

  /* too many formats to choose from */
  return -2;
}

void 
initialise(void)
{
  /* KT - initialise some data */
  
  cur_trk = -1;    
  cur_hd = -1;
  cur_blk = -1;
  directory_dirty = 0;
  image_type = 0;
  *disk_header.tag = 0;
  track = NULL;
  blk_alloc = NULL;
  directory = NULL;
  block_buffer = NULL;
  imagefile = -1;
  gz_imagefile = 0;
  gz_format = 0;
}

/********
  Blocks
 ********/

int get_free_block() {
/*  ^^^^^^^^^^^^^^ */
static int next = 0;
int  i;

  if (next > dpb->DSM) next = 0;
/* try to allocate next block, if there was a previos one (next!=0) */
  if ((next != 0) && (is_free_block(next))) return next++;
/* try to find the first free block */
  for (i=dpb->DBL;i<=dpb->DSM;i++) {
    if (is_free_block(i)) return i;
  }
  return -1;
}


/*****************
  FS Maintenance
 *****************/

int 
open_image(char *name, int auto_detect) 
{
/*  ^^^^^^^^^^
alloc track buffer, blk_alloc, buffer, read directory */

int  n;
int result;

  /* open file */
  if (my_open(name) < 0) {
    return -1;
  }
  n = my_read((uchar*)&disk_header,0x100);
  if (n!=0x100) {
    return -1;
  }
  if (!tag_ok()) {
    //fprintf(stdout,"\"%s\" is not a DSK image!",name);
    return -1;
  }
  if ((disk_header.nbof_heads<1) || (disk_header.nbof_tracks<1)) {
    //fprintf(stdout,"--==>>> open_image: \"%s\"",name);
    abandonimage();
    return -1;
  }

  /* added function to malloc block which is size of the biggest track */
  malloc_track();

  /* check malloc succeded */
  if (track==NULL)
  {
    abandonimage();
    return -1;
  }



  if (!validate_image())
  {
    //fprintf(stdout, "Image file is corrupted!");
    abandonimage();
    return -1;
  }

  

/* set up varaibles */
  filler = 0xE5;

  /* determine disk format */
  if (read_track(0,0))
  {
    abandonimage();
    return -1;
  }



  /* auto-detect image format */
  if (auto_detect)
  {
    
    result = select_format(track);

    if (result==-2)
    {
      //fprintf(stdout, "Multiple formats found!");
      abandonimage();
      return -1;
    }

    if (result==-1)
    {
      //fprintf(stdout, disc_format_not_recognised_string);
      abandonimage();
      return -1;
    }
  }
  else
  {
    /* found it - update it */
    update_dpb(dpb,track);
  }

  if (dpb==NULL)
  {
    //fprintf(stdout, disc_format_not_recognised_string);
    abandonimage();
    return -1;
  }



/* calculate number of blocks and allocate memory */
  blk_alloc = (uchar *)malloc(((dpb->DSM+1)+7)>>3);

  /* KT - added memory allocate check */
  if (blk_alloc==NULL)
  {
    abandonimage();

    return -1;
  }

  directory = (DirEntry*)malloc(sizeof(DirEntry)*(dpb->DRM+1));

  /* KT - added memory allocate check */
  if (directory==NULL)
  {
    abandonimage();
    return -1;
  }

/* allocate block buffer */
  block_buffer = (uchar*)malloc(dpb->BLS);
    
  if (block_buffer==NULL)
  {
    abandonimage();
    return -1;
  }

/* get directory information */
  get_directory();

  return 0;
}

DPB_type system_dpb=
  {  
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

DPB_type data_dpb = 
  {  
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

char *upper(char *s)  {
/*    ^^^^^ */
char  *p=s;

  while (*s) 
  {
    *s = toupper(*s);
    s++;
  }


  return p;
}
void  dpb_list_add_entry(DPB_list *list, DPB_list_entry *entry)
{
  DPB_list_entry *cur_entry;

  /* if list empty add as first item */
  if (list->first==NULL)
  {
    list->first = entry;
  }
  else
  {
    /* add to end of list */
    cur_entry = list->first;

    while (cur_entry->next!=NULL)
    {
      cur_entry = cur_entry->next;
    }

    cur_entry->next = entry;
  }
}
void  dpb_list_add_item(DPB_list *list,char *description, char *ident, DPB_type *dpb)
{
  DPB_list_entry *entry = (DPB_list_entry *)malloc(sizeof(DPB_list_entry));

  if (entry!=NULL)
  {
    if (description==NULL)
    {
      entry->description = NULL;
    }
    else
    {
      entry->description = (char *)malloc(strlen(description)+1);

      if (entry->description!=NULL)
      {
        strcpy(entry->description, description);
      }
    }

    if (ident==NULL)
    {
      entry->ident = NULL;
    }
    else
    {
      entry->ident = (char *)malloc(strlen(ident)+1);

      if (entry->ident!=NULL)
      {
        strcpy(entry->ident, ident);
        upper(entry->ident);
      }
    }


    /* copy dpb */
    memcpy(&entry->dpb, dpb, sizeof(DPB_type));

    /* if cylinders is 40, but used in a 80 track drive, then the drive
    head is automatically double stepped */
    /* want to use cylinders from .def because this is good for discs
    where only 60 tracks are used */
#if 0
    /* The 22disk definition uses CYLINDERS to define, from what I can
    see the physical number of tracks on the disc media. So, you can
    have a 80 track 5.25" drive reading a 40 track format with
    or without double step - calculate a decent TRKS from DSM
    which specifies the total number of blocks on the disc. */
    {
      DPB_type *dpb = &entry->dpb;

      dpb->BLS  = 1 << (dpb->BSH + 7); /* or 2^BSH*128 */

      /* calculate the number of tracks from DSM */
      dpb->TRKS = (((dpb->DSM+1)*dpb->BLS)+(dpb->OFS*dpb->SECS*dpb->BPS) + ((dpb->DRM+1)<<5))/(dpb->SECS*dpb->BPS*dpb->HDS);
    }
#endif

    entry->next = NULL;
  }

  dpb_list_add_entry(list, entry);
}


static void
cpc_dsk_init()
{
static int loc_initialized = 0;
  if (loc_initialized) return;

  dpb_list.first = NULL;

  initialise();

  /* data */
  //dpb_list_add_item(&dpb_list,"Data Format", "data", &data_dpb);
  dpb_list_add_item(&dpb_list,"data", "data", &data_dpb);
  
  /* system */
  //dpb_list_add_item(&dpb_list,"System Format", "system", &system_dpb);
  dpb_list_add_item(&dpb_list,"system", "system", &system_dpb);

  loc_initialized = 1;
}

//#define DEBUG
int
cpc_dsk_dir(char *filename)
{
  cpc_dsk_init();

  cpc_dsk_num_entry = 0;
  int error = open_image(filename, 1);
  abandonimage();
#ifdef DEBUG
  int index;
  for (index = 0; index < cpc_dsk_num_entry; index++) {
    fprintf(stdout, "[%d] -> '%s'\n", index, cpc_dsk_dirent[index]);
  }
#endif
  return error;
}

