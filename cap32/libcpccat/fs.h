#ifndef CPC_FS_H
#define CPC_FS_H

typedef unsigned char bool;
typedef unsigned char uchar;
typedef unsigned short ushort;

# define TRUE      1
# define FALSE     0

#define EXTENTSIZE	16384	/* 16 kByte, constant for CP/M */
#define RECORDSIZE	128	/* 1/8 kByte, constant for CP/M */

#define DIRSEPARATOR '/'

extern	unsigned char filler;	/* copied from track header */

/****** CPC Filesystem ******/
/* size of block index in bytes - as stored in directory */
extern int	BLKNR_SIZE;
/* number of block index's that will fit into a directory entry */
extern int	BLKNR;

/************
  Structures
 ************/

struct d_header {	/* disk header of size 0x100 */
	uchar	tag[0x30];
		/*00-21	MV - CPC ... */
		/*22-2F	unused (0) */
	uchar	nbof_tracks;
		/*30	number of tracks (40) */
	uchar	nbof_heads;
		/*31	number of heads (1) 2 not yet supported by cpcemu */
	short	tracksize;	/* short must be 16bit integer */
		/*32-33	tracksize (including 0x100 bytes header)
			9 sectors * 0x200 bytes each + header = 0x1300 */
	uchar	unused[0xcc];
		/*34-FF	unused (0)*/
};

extern struct d_header disk_header;



struct s_info {		/* sector info, used in track header */
/*Sector-Information (for each sector):*/
	uchar	track;
		/*18+i	tracknumber	\				*/
	uchar	head;
		/*19+i	headnumber	 | Sector-ID-Information	*/
	uchar	sector;
		/*1A+i	sectornumber	 |				*/
	uchar	BPS;
		/*1B+i	BPS          	/				*/
	uchar	status1;
		/*1C+i	status 1 errorcode (0)*/
	uchar	status2;
		/*1D+i	status 2 errorcode (0)*/
	uchar	unused[2];
		/*1E+i,1F+i unused (0)*/
} ;

struct t_header  {	/* track header of size 0x100 */
	uchar	tag[0x10];
		/*00-0C	Track-Info\r\n*/
		/*0D-0F	unused (0)*/
	uchar	track;
		/*10	tracknumber (0 to number-of-tracks - 1)*/
	uchar	head;
		/*11	headnumber (0)*/
	uchar	unused[2];
		/*12-13	unused (0)*/
		/*Format-Track-Parameter:*/
	uchar	BPS;
		/*14	BPS (bytes per sector) (2 for 0x200 Bytes)*/
	uchar	SPT;
		/*15	SPT (sectors per track) (9, max. 18 possible)*/
	uchar	GAP3;
		/*16	GAP#3 Format (gap for formatting: 0x4E)*/
	uchar	filler;
		/*17	Filling-Byte (filler for formatting: 0xE5)*/
	struct s_info sector[29];
} ;


/****** Directory ******/

typedef struct {
	uchar	user;		/* actually a byte */
	uchar	root[8];	/* padded with space */
	uchar	ext[3];		/* ditto	     */
	uchar	name[13];	/* <root 8>+"."+<ext 3>+"\0" (for globbing) */
	uchar	rec;		/* size in 128 Byte records */

	int	attr;		/* bit array of size 11 (87654321rsa) */
	int	blk[16];	/* 16 or 8 indices of 1 or 2 byte */
	uchar	extent;		/* aka sorting criterion for dir entires */

/* only stored to write it back */
	uchar	unused[2];	/* used for internal CP/M purposes */

/* organisational */
	bool	first;		/* this entry is first */
	long	size;		/* length in Bytes, only set if <first> */
	int	next;		/* next entry for this file, -1 if last */
} DirEntry;

extern DirEntry *directory;


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
	/*uchar  ID;*/  /* Identifier */

	ushort SEC1_side1;/* 1. SECtor number (0, >1, >41h, >C1h) */
	ushort SEC1_side2;
	ushort SECS;/* number of sectors per track (8, >9) */
	ushort TRKS;/* number of tracks per side (>40, 80) */
	ushort HDS; /* number of heads per disk (>1, 2) */	
	ushort BPS; /* Bytes Per Sector (128, 256, >512, 1024) */
	
/* original Disk Parameter Block (> marks CPC defaults) */
	ushort SPT; /* records Per Track (18, 20, 30, 32, 34, >36, 40) */
	uchar  BSH; /* Block SHift ...      2^BSH = BLM+1 = Rec/Block */
	uchar  BLM; /* BLock Mask (>3/7, 4/15) */
	uchar  EXM; /* EXtent Mask (0, 1)   Number of Extents/Entry - 1 */
	ushort DSM; /* max. blocknumber = number of blocks - 1 */
	ushort DRM; /* DiRectory size - 1 (31, >63, 127) */
	uchar  AL0; /* \ DRM in binary (80h/0, >C0h/0, F0h/0) */
	uchar  AL1; /* / (1 bit=1 block, 11000000/00000000 = 2 blocks) */
	ushort CKS; /* ChecK recordS,nb of rec in dir (8, >16, 32) */
	ushort OFS; /* OFfSet, reserved tracks (1, >2, 3, 4) */

/* extended DPB info, automatically calculated */
	ushort BLS; /* BLock Size in bytes (>1024, 2048)*/
	bool   SYS; /* CP/M present in system tracks */
	ushort DBL; /* Directory BLocks = CKS/8 */

	BLOCK_ORDER	order;
	uchar	side0_hd;	/* if accessing side 0, use this head value */
	uchar	side1_hd;	/* if accessing side 1, use this head value */

	/* sector id skew */
	int		skew;

	int		num_extents; 	/* calculated from EXM*/

	unsigned char *label_data;	/* NULL = no label file, !=NULL = label file */
	unsigned long label_data_length;

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

/* KT - removed */
#if 0
/* DPB templates for SYSTEM, DATA, IBM, VORTEX, and user defined */
extern const int DPB_store_size;
typedef enum {	SYSTEM_DPB=0,
		DATA_DPB=1,
		IBM_DPB=2,
		VORTEX_DPB=3,
		USER_DPB=4
} DPB_Index_Type;
extern DPB_type DPB_store[];
#endif

extern DPB_type *dpb;	/* pointer to current DPB */

/* KT - added */
/* current dpb list entry */
extern DPB_list_entry *dpb_list_entry;
/* first dpb list entry in list */
extern DPB_list dpb_list;

DPB_list_entry *get_dpb_entry_from_format_name(const char *);

extern	uchar	*blk_alloc;	/* Block-Allocation-Table, points to entry, */
extern	uchar	*track;		/* 0x100 header + track data */
extern uchar	*block_buffer;	/* 1024 or more bytes for one block */


#endif
