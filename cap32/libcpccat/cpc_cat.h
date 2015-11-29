#ifndef CPC_CAT_H
#define CPC_CAT_H

# define CPC_MAX_ENTRY  64
# define CPC_NAME_SIZE  20

 extern int cpc_dsk_num_entry;
 extern char cpc_dsk_dirent[CPC_MAX_ENTRY][CPC_NAME_SIZE];
 extern int cpc_dsk_dir(char *filename);

#endif
