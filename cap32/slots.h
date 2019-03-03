/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef SLOTS_H
#define SLOTS_H

#include <stdint.h>
#include <stdbool.h>

#include "cap32.h"

int snapshot_load_mem (uint8_t *sna_buffer, uint32_t buffer_size);
int snapshot_save_mem (uint8_t *sna_buffer, uint32_t buffer_size);
int snapshot_load (char *pchFileName);
int snapshot_save (char *pchFileName);

void dsk_eject (t_drive *drive);
int dsk_load (char *pchFileName, t_drive *drive, char chID);
int dsk_save (char *pchFileName, t_drive *drive, char chID);
int dsk_format (t_drive *drive, int iFormat);

void tape_eject (void);
void play_tape(void);
int tape_insert (char *pchFileName);
int tape_insert_voc (char *pchFileName);

#endif
