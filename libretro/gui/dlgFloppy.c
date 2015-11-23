/*
	modded for libretro-frodo
*/

/*
  Hatari - dlgFloppy.c

  This file is distributed under the GNU Public License, version 2 or at
  your option any later version. Read the file gpl.txt for details.
*/
const char DlgFloppy_fileid[] = "Hatari dlgFloppy.c : " __DATE__ " " __TIME__;

#include <assert.h>

#include "dialog.h"
#include "sdlgui.h"
#include "file.h"

#define GET_DRIVE(code) ((code)&0x0F)

#define Log_AlertDlg printf
#define LOG_INFO stderr
#define LOG_ERROR stderr

static const char * const pszDiskImageNameExts[] =
{
	".d64",
	".d71",
	".d80",
	".d82",
	".g64",
	".g41",
	".t64",
	".x64",
	".tap",
	".prg",
	".p00",
	".crt",
	".zip",
	NULL
};

#define MAX_FLOPPYDRIVES 4

char szDiskZipPath[MAX_FLOPPYDRIVES][FILENAME_MAX]={ {'\0'},{'\0'}, {'\0'},{'\0'}};
char szDiskFileName[MAX_FLOPPYDRIVES][FILENAME_MAX]={ {'\0'},{'\0'}, {'\0'},{'\0'}};
char szDiskImageDirectory[FILENAME_MAX]={'\0'};

#define FLOPPYDLG_EJECTA      3
#define FLOPPYDLG_BROWSEA     4
#define FLOPPYDLG_DISKA       5
#define FLOPPYDLG_EJECTB      7
#define FLOPPYDLG_BROWSEB     8
#define FLOPPYDLG_DISKB       9
#define FLOPPYDLG_EJECT2      11
#define FLOPPYDLG_BROWSE2     12
#define FLOPPYDLG_DISK2       13
#define FLOPPYDLG_EJECT3      15
#define FLOPPYDLG_BROWSE3     16
#define FLOPPYDLG_DISK3       17
#define FLOPPYDLG_IMGDIR      19
#define FLOPPYDLG_BROWSEIMG   20
#define FLOPPYDLG_ATTACH2FLIPLIST   22
#define FLOPPYDLG_EXIT        23


/* The floppy disks dialog: */
static SGOBJ floppydlg[] =
{
	{ SGBOX, 0, 0, 0,0, 64,20, NULL },
	{ SGTEXT, 0, 0, 25,1, 12,1, "Floppy disks" },
	{ SGTEXT, 0, 0, 2,3, 8,1, "DF8:" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 46,3, 7,1, "Eject" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 54,3, 8,1, "Browse" },
	{ SGTEXT, 0, 0, 3,4, 58,1, NULL },
	{ SGTEXT, 0, 0, 2,6, 8,1, "DF9:" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 46,6, 7,1, "Eject" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 54,6, 8,1, "Browse" }
,
	{ SGTEXT, 0, 0, 3,7, 58,1, NULL },
	{ SGTEXT, 0, 0, 2,9, 8,1, "DF10:" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 46,9, 7,1, "Eject" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 54,9, 8,1, "Browse" },
	{ SGTEXT, 0, 0, 3,10, 58,1, NULL },
	{ SGTEXT, 0, 0, 2,12, 8,1, "DF11:" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 46,12, 7,1, "Eject" },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 54,12, 8,1, "Browse" },

	{ SGTEXT, 0, 0, 3,13, 58,1, NULL },
	{ SGTEXT, 0, 0, 2,14, 32,1, "Default floppy images directory:" },
	{ SGTEXT, 0, 0, 3,15, 58,1, NULL },
	{ SGBUTTON,  SG_EXIT/*0*/, 0, 54,14, 8,1, "Browse" },

	{ SGTEXT, 0, 0, 3,16, 58,1, NULL },	
	{ SGCHECKBOX, 0, 0, 3,17, 25,1, "Attach to Fliplist" },
	{ SGBUTTON, SG_EXIT/*SG_DEFAULT*/, 0, 22,18, 24,1, "Back to main menu" },
	{ -1, 0, 0, 0,0, 0,0, NULL }
};

#define DLGMOUNT_A       2
#define DLGMOUNT_B       3
#define DLGMOUNT_CANCEL  4

/* The "Alert"-dialog: */
static SGOBJ alertdlg[] =
{
	{ SGBOX, 0, 0, 0,0, 40,6, NULL },
	{ SGTEXT, 0, 0, 3,1, 30,1, "Insert last created disk to?" },
	{ SGBUTTON, SG_EXIT/*0*/, 0, 3,4, 10,1, "Drive A:" },
	{ SGBUTTON, SG_EXIT/*0*/, 0, 15,4, 10,1, "Drive B:" },
	{ SGBUTTON, SG_EXIT/*SG_CANCEL*/, 0, 27,4, 10,1, "Cancel" },
	{ -1, 0, 0, 0,0, 0,0, NULL }
};

/*-----------------------------------------------------------------------*/
/**
 * Set floppy image to be ejected
 */
const char* Floppy_SetDiskFileNameNone(int Drive)
{
	assert(Drive >= 0 && Drive < MAX_FLOPPYDRIVES);
	szDiskFileName[Drive][0] = '\0';
	return szDiskFileName[Drive];
}

/*-----------------------------------------------------------------------*/
/**
 * Set given floppy drive image file name and handle
 * different image extensions.
 * Return corrected file name on success and NULL on failure.
 */
const char* Floppy_SetDiskFileName(int Drive, const char *pszFileName, const char *pszZipPath)
{
	char *filename;
	int i;

	/* setting to empty or "none" ejects */
	if (!*pszFileName || strcasecmp(pszFileName, "none") == 0)
	{
		return Floppy_SetDiskFileNameNone(Drive);
	}
	/* See if file exists, and if not, get/add correct extension */
	if (!File_Exists(pszFileName))
		filename = File_FindPossibleExtFileName(pszFileName, pszDiskImageNameExts);
	else
		filename = strdup(pszFileName);
	if (!filename)
	{
		Log_AlertDlg((const char *)LOG_INFO, "Image '%s' not found", pszFileName);
		return NULL;
	}
#if 0
	/* If we insert a disk into Drive A, should we try to put disk 2 into drive B? */
	if (Drive == 0 && ConfigureParams.DiskImage.bAutoInsertDiskB)
	{
		/* Attempt to make up second filename, eg was 'auto_100a' to 'auto_100b' */
		char *szDiskBFileName = Floppy_CreateDiskBFileName(filename);
		if (szDiskBFileName)
		{
			/* recurse with Drive B */
			Floppy_SetDiskFileName(1, szDiskBFileName, pszZipPath);
			free(szDiskBFileName);
		}
	}
#endif

	/* validity checks */
	assert(Drive >= 0 && Drive < MAX_FLOPPYDRIVES);
	for (i = 0; i < MAX_FLOPPYDRIVES; i++)
	{
		if (i == Drive)
			continue;
		/* prevent inserting same image to multiple drives */
		if (strcmp(filename, /*ConfigureParams.DiskImage.*/szDiskFileName[i]) == 0)
		{
			Log_AlertDlg((const char *)LOG_ERROR, "ERROR: Cannot insert same floppy to multiple drives!");
			return NULL;
		}
	}

	/* do the changes */
	if (pszZipPath)
		strcpy(szDiskZipPath[Drive], pszZipPath);
	else
		szDiskZipPath[Drive][0] = '\0';
	strcpy(szDiskFileName[Drive], filename);
	free(filename);
	//File_MakeAbsoluteName(ConfigureParams.DiskImage.szDiskFileName[Drive]);
	return szDiskFileName[Drive];
}



/**
 * Let user browse given disk, insert disk if one selected.
 */
static void DlgDisk_BrowseDisk(char *dlgname, int drive, int diskid)
{
	char *selname, *zip_path;
	const char *tmpname, *realname;

	assert(drive >= 0 && drive < MAX_FLOPPYDRIVES);
	if (szDiskFileName[drive][0])
		tmpname = szDiskFileName[drive];
	else
		tmpname = szDiskImageDirectory;

	selname = SDLGui_FileSelect(tmpname, &zip_path, false);
	if (!selname)
		return;

	if (File_Exists(selname))
	{
		realname = Floppy_SetDiskFileName(drive, selname, zip_path);
		if (realname)
			File_ShrinkName(dlgname, realname, floppydlg[diskid].w);
	}
	else
	{
		Floppy_SetDiskFileNameNone(drive);
		dlgname[0] = '\0';
	}
	if (zip_path)
		free(zip_path);
	free(selname);
}


/**
 * Let user browse given directory, set directory if one selected.
 */
static void DlgDisk_BrowseDir(char *dlgname, char *confname, int maxlen)
{
	char *str, *selname;

	selname = SDLGui_FileSelect(confname, NULL, false);
	if (!selname)
		return;

	strcpy(confname, selname);
	free(selname);

	str = strrchr(confname, PATHSEP);
	if (str != NULL)
		str[1] = 0;
	File_CleanFileName(confname);
	File_ShrinkName(dlgname, confname, maxlen);
}


/**
 * Ask whether new disk should be inserted to A: or B: and if yes, insert.
 */
static void DlgFloppy_QueryInsert(char *namea, int ida, char *nameb, int idb, const char *path)
{
	const char *realname;
	int diskid, dlgid;
	char *dlgname;

	SDLGui_CenterDlg(alertdlg);

int but;

do{        
	but=SDLGui_DoDialog(alertdlg, NULL);

	{
		if(but== DLGMOUNT_A){
			dlgname = namea;
			dlgid = ida;
			diskid = 0;
                }
		else if(but == DLGMOUNT_B){
			dlgname = nameb;
			dlgid = idb;
			diskid = 1;
                }
		
	}
        gui_poll_events();
}
while (but != DLGMOUNT_CANCEL && but != DLGMOUNT_A  && but != DLGMOUNT_B && but != SDLGUI_QUIT
	        && but != SDLGUI_ERROR && !bQuitProgram);


	realname = Floppy_SetDiskFileName(diskid, path, NULL);
	if (realname)
		File_ShrinkName(dlgname, realname, floppydlg[dlgid].w);
}

/**
 * Show and process the floppy disk image dialog.
 */
void DlgFloppy_Main(void)
{
	int but, i;
	char *newdisk;
	char dlgname[MAX_FLOPPYDRIVES][64], dlgdiskdir[64];

	SDLGui_CenterDlg(floppydlg);

	/* Set up dialog to actual values: */
 const char *name;

 floppydlg[FLOPPYDLG_ATTACH2FLIPLIST].state &= ~SG_SELECTED;

 name = file_system_get_disk_name(8); /* Filename */
 if (!name)dlgname[0][0] = '\0';
 else File_ShrinkName(dlgname[0], name,floppydlg[FLOPPYDLG_DISKA].w);
 floppydlg[FLOPPYDLG_DISKA].txt = dlgname[0];

 name = file_system_get_disk_name(9); /* Filename */
 if (!name)dlgname[1][0] = '\0';
 else File_ShrinkName(dlgname[1], name,floppydlg[FLOPPYDLG_DISKB].w);
 floppydlg[FLOPPYDLG_DISKB].txt = dlgname[1];

 name = file_system_get_disk_name(10); /* Filename */
 if (!name)dlgname[2][0] = '\0';
 else File_ShrinkName(dlgname[2], name,floppydlg[FLOPPYDLG_DISK2].w);
 floppydlg[FLOPPYDLG_DISK2].txt = dlgname[2];

 name = file_system_get_disk_name(11); /* Filename */
 if (!name)dlgname[3][0] = '\0';
 else File_ShrinkName(dlgname[3], name,floppydlg[FLOPPYDLG_DISK3].w);
 floppydlg[FLOPPYDLG_DISK3].txt = dlgname[3];

	/* Default image directory: */
	File_ShrinkName(dlgdiskdir,szDiskImageDirectory,
	                floppydlg[FLOPPYDLG_IMGDIR].w);
	floppydlg[FLOPPYDLG_IMGDIR].txt = dlgdiskdir;


	/* Draw and process the dialog */
	do
	{       
		but = SDLGui_DoDialog(floppydlg, NULL);
		switch (but)
		{
		 case FLOPPYDLG_EJECTA:                         /* Eject disk in drive A: */
			Floppy_SetDiskFileNameNone(0);
			dlgname[0][0] = '\0';
			file_system_detach_disk(GET_DRIVE(8));

			break;
		 case FLOPPYDLG_BROWSEA:                        /* Choose a new disk A: */
			DlgDisk_BrowseDisk(dlgname[0], 0, FLOPPYDLG_DISKA);

			if (strlen(szDiskFileName[0]) > 0){

			int drivetype;

			printf("load (%s)-",szDiskFileName[0]);
			resources_get_int_sprintf("Drive%iType", &drivetype, GET_DRIVE(8));
			printf("(Drive%iType)\n",drivetype);

			cartridge_detach_image(-1);
			tape_image_detach(1);
//			file_system_detach_disk(GET_DRIVE(8));

			if(File_DoesFileExtensionMatch(szDiskFileName[0],"CRT"))
				cartridge_attach_image(CARTRIDGE_CRT, szDiskFileName[0]);
			else {
//FIXME
/*
				if(File_DoesFileExtensionMatch(szDiskFileName[0],"D81") && drivetype!=1581)
						resources_set_int_sprintf("Drive%iType", 1581,  GET_DRIVE(8));
				else if (drivetype!=1542 && !File_DoesFileExtensionMatch(szDiskFileName[0],"D81"))
						resources_set_int_sprintf("Drive%iType", 1542,  GET_DRIVE(8));
*/			

				if (floppydlg[FLOPPYDLG_ATTACH2FLIPLIST].state & SG_SELECTED){
					file_system_detach_disk(GET_DRIVE(8));
					printf("Attach to flip list\n");
					file_system_attach_disk(8, szDiskFileName[0]);
					fliplist_add_image(8)	;
				}
				else {
					printf("autostart\n");
					autostart_autodetect(szDiskFileName[0], NULL, 0, AUTOSTART_MODE_RUN);
				}

			}

			}

			break;
		 case FLOPPYDLG_EJECTB:                         /* Eject disk in drive B: */
			Floppy_SetDiskFileNameNone(1);
			dlgname[1][0] = '\0';
			file_system_detach_disk(GET_DRIVE(9));

			break;
		case FLOPPYDLG_BROWSEB:                         /* Choose a new disk B: */
			DlgDisk_BrowseDisk(dlgname[1], 1, FLOPPYDLG_DISKB);

			if (strlen(szDiskFileName[1]) > 0){
				
			file_system_detach_disk(GET_DRIVE(9));
     		file_system_attach_disk(9, szDiskFileName[1]);
	
			}

		 case FLOPPYDLG_EJECT2:                         /* Eject disk in drive A: */
			Floppy_SetDiskFileNameNone(2);
			dlgname[2][0] = '\0';
			file_system_detach_disk(GET_DRIVE(10));
			break;
		 case FLOPPYDLG_BROWSE2:                        /* Choose a new disk A: */
			DlgDisk_BrowseDisk(dlgname[2], 0, FLOPPYDLG_DISK2);

			if (strlen(szDiskFileName[2]) > 0){
					//strcpy(prefs->DrivePath[2], szDiskFileName[2]);
			}

			break;
		 case FLOPPYDLG_EJECT3:                         /* Eject disk in drive B: */
			Floppy_SetDiskFileNameNone(3);
			dlgname[3][0] = '\0';
			file_system_detach_disk(GET_DRIVE(11));
			break;
		case FLOPPYDLG_BROWSE3:                         /* Choose a new disk B: */
			DlgDisk_BrowseDisk(dlgname[3], 1, FLOPPYDLG_DISKB);

			if (strlen(szDiskFileName[3]) > 0){

//					strcpy(prefs->DrivePath[3], szDiskFileName[3]);
			}

			break;
		 case FLOPPYDLG_BROWSEIMG:
			DlgDisk_BrowseDir(dlgdiskdir,szDiskImageDirectory,floppydlg[FLOPPYDLG_IMGDIR].w);
			break;
/*
		 case FLOPPYDLG_CREATEIMG:
			newdisk = DlgNewDisk_Main();
			if (newdisk)
			{
				DlgFloppy_QueryInsert(dlgname[0], FLOPPYDLG_DISKA,
						      dlgname[1], FLOPPYDLG_DISKB,
						      newdisk);
				free(newdisk);
			}
			break;
*/
		}
                gui_poll_events();
	}
	while (but != FLOPPYDLG_EXIT && but != SDLGUI_QUIT
	        && but != SDLGUI_ERROR && !bQuitProgram);

/*
	if (floppydlg[FLOPPYDLG_AUTOSTART].state & SG_SELECTED){

			if(!ThePrefs.Emul1541Proc){
					prefs->Emul1541Proc = !prefs->Emul1541Proc;
			}
	}
	else {
			if(ThePrefs.Emul1541Proc){
					prefs->Emul1541Proc = !prefs->Emul1541Proc;
			}	

	}
*/

}
