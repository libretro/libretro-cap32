#ifndef CPC_CAT_H
#define CPC_CAT_H

#define CPC_MAX_ENTRY  64
#define CPC_NAME_SIZE  20

#define DSK_TYPE_DATA    0x1C
#define DSK_TYPE_SYSTEM  0x41

extern int cpc_dsk_num_entry;
extern int cpc_dsk_type;
extern char cpc_dsk_dirent[CPC_MAX_ENTRY][CPC_NAME_SIZE];
extern int cpc_dsk_dir(char *filename);

#endif
