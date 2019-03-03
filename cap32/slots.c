/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich
   (c) Copyright 2004-2016 David Skywalker

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

/**
 * D_Skywalk: Imported from my 3DS pituka implementation
 *            http://david.dantoine.org/proyecto/26/
 */

#include "slots.h"
#include "crtc.h"
#include "tape.h"
#include "cart.h"
#include "z80.h"
#include "errors.h"

extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_FDC FDC;
extern t_VDU VDU;
extern t_GateArray GateArray;
extern t_PPI PPI;
extern t_PSG PSG;
extern t_drive driveA;
extern t_drive driveB;
extern t_z80regs z80;
extern uint8_t bit_values[8];
extern uint16_t MaxVSync;
extern t_flags1 flags1;
extern t_disk_format disk_format[MAX_DISK_FORMAT];
extern uint8_t *pbROM;

extern uint8_t *pbTapeImage;
extern uint8_t *pbGPBuffer;
extern uint8_t *pbRAM;
extern uint8_t *pbRAMbuffer;

extern FILE *pfileObject;

uint8_t *pbTapeImage = NULL;
uint8_t *pbTapeImageEnd = NULL;
uint8_t *pbSnaImage = NULL;


/**
 * file utils
 */
int file_size (int file_num)
{
   struct stat s;

   if (!fstat(file_num, &s)) {
      return s.st_size;
   } else {
      return 0;
   }
}


/**
 * SNA handlers
 */

/**
 * snapshot_load_mem:
 * @pBuffer: snapshot buffer
 * @bSize: snapshot memory size
 *
 * return 0 when is loaded successfully
 */
int snapshot_load_mem (uint8_t *sna_buffer, uint32_t buffer_size) {
   uint8_t val;
   int n;
   reg_pair port;
   uint32_t dwSnapSize, dwModel, dwFlags;
   t_SNA_header sh;

   if ((sna_buffer == NULL) || (buffer_size < sizeof(sh)))
      return ERR_SNA_SIZE;

   memcpy(&sh, sna_buffer, sizeof(sh));

   if (memcmp(sh.id, "MV - SNA", 8) != 0) // valid SNApshot image?
      return ERR_SNA_INVALID;

   dwSnapSize = sh.ram_size[0] + (sh.ram_size[1] * 256); // memory dump size
   dwSnapSize &= ~0x3f; // limit to multiples of 64
   if (!dwSnapSize)
      return ERR_SNA_SIZE;

   if (buffer_size > CPC.ram_size) { // memory dump size differs from current RAM size?
      uint8_t *pbTemp;

      pbTemp = (uint8_t*) malloc(dwSnapSize * 1024);
      if (pbTemp) {
         free(pbRAM);
         CPC.ram_size = dwSnapSize;
         pbRAM = pbTemp;
      } else
         return ERR_OUT_OF_MEMORY;
   }

   emulator_reset(false);
   if (sizeof(sh) + (dwSnapSize * 1024) > buffer_size)
      return ERR_SNA_INVALID;

   memcpy(pbRAM, (uint8_t*)(sna_buffer + sizeof(sh)), dwSnapSize * 1024); // read memory dump into CPC RAM

   // Z80
   _A = sh.AF[1];
   _F = sh.AF[0];
   _B = sh.BC[1];
   _C = sh.BC[0];
   _D = sh.DE[1];
   _E = sh.DE[0];
   _H = sh.HL[1];
   _L = sh.HL[0];
   _R = sh.R & 0x7f;
   _Rb7 = sh.R & 0x80; // bit 7 of R
   _I = sh.I;
   if (sh.IFF0)
     _IFF1 = Pflag;
   if (sh.IFF1)
     _IFF2 = Pflag;
   _IXh = sh.IX[1];
   _IXl = sh.IX[0];
   _IYh = sh.IY[1];
   _IYl = sh.IY[0];
   z80.SP.b.h = sh.SP[1];
   z80.SP.b.l = sh.SP[0];
   z80.PC.b.h = sh.PC[1];
   z80.PC.b.l = sh.PC[0];
   _IM = sh.IM; // interrupt mode
   z80.AFx.b.h = sh.AFx[1];
   z80.AFx.b.l = sh.AFx[0];
   z80.BCx.b.h = sh.BCx[1];
   z80.BCx.b.l = sh.BCx[0];
   z80.DEx.b.h = sh.DEx[1];
   z80.DEx.b.l = sh.DEx[0];
   z80.HLx.b.h = sh.HLx[1];
   z80.HLx.b.l = sh.HLx[0];
   // Gate Array
   port.b.h = 0x7f;
   for (n = 0; n < 17; n++) { // loop for all colours + border
      GateArray.pen = n;
      val = sh.ga_ink_values[n]; // GA palette entry
      z80_OUT_handler(port, val | (1 << 6));
   }
   val = sh.ga_pen; // GA pen
   z80_OUT_handler(port, (val & 0x3f));
   val = sh.ga_ROM_config; // GA ROM configuration
   z80_OUT_handler(port, (val & 0x3f) | (2 << 6));
   val = sh.ga_RAM_config; // GA RAM configuration
   z80_OUT_handler(port, (val & 0x3f) | (3 << 6));
   // CRTC
   port.b.h = 0xbd;
   for (n = 0; n < 18; n++) { // loop for all CRTC registers
      val = sh.crtc_registers[n];
      CRTC.reg_select = n;
      z80_OUT_handler(port, val);
   }
   port.b.h = 0xbc;
   val = sh.crtc_reg_select; // CRTC register select
   z80_OUT_handler(port, val);
   // ROM select
   port.b.h = 0xdf;
   val = sh.upper_ROM; // upper ROM number
   z80_OUT_handler(port, val);
   // PPI
   port.b.h = 0xf4; // port A
   z80_OUT_handler(port, sh.ppi_A);
   port.b.h = 0xf5; // port B
   z80_OUT_handler(port, sh.ppi_B);
   port.b.h = 0xf6; // port C
   z80_OUT_handler(port, sh.ppi_C);
   port.b.h = 0xf7; // control
   z80_OUT_handler(port, sh.ppi_control);
   // PSG
   PSG.control = PPI.portC;
   PSG.reg_select = sh.psg_reg_select;
   for (n = 0; n < 16; n++) // loop for all PSG registers
      SetAYRegister(n, sh.psg_registers[n]);

   if (sh.version > 1) { // does the snapshot have version 2 data?
      dwModel = sh.cpc_model; // determine the model it was saved for
      if (dwModel != CPC.model) { // different from what we're currently running?
         if (dwModel > 3) { // not one of the known models?
            emulator_reset(false);
            return ERR_SNA_CPC_TYPE;
         }
         CPC.model = dwModel;
         emulator_select_ROM();
      }
   }
   if (sh.version > 2) { // does the snapshot have version 3 data?
      FDC.motor = sh.fdc_motor;
      driveA.current_track = sh.drvA_current_track;
      driveB.current_track = sh.drvB_current_track;
      CPC.printer_port = sh.printer_data ^ 0x80; // invert bit 7 again
      PSG.AmplitudeEnv = sh.psg_env_step << 1; // multiply by 2 to bring it into the 0 - 30 range
      PSG.FirstPeriod = false;
      if (sh.psg_env_direction == 0x01) { // up
         switch (PSG.RegisterAY.EnvType) {
            case 4:
            case 5:
            case 6:
            case 7:
            case 13:
            case 14:
            case 15:
               PSG.FirstPeriod = true;
               break;
         }
      } else if (sh.psg_env_direction == 0xff) { // down
         switch (PSG.RegisterAY.EnvType) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 9:
            case 10:
            case 11:
               PSG.FirstPeriod = true;
               break;
         }
      }
      CRTC.addr = sh.crtc_addr[0] + (sh.crtc_addr[1] * 256);
      VDU.scanline = sh.crtc_scanline[0] + (sh.crtc_scanline[1] * 256);
      if (VDU.scanline > MaxVSync) {
         VDU.scanline = MaxVSync; // limit to max value
      }
      CRTC.char_count = sh.crtc_char_count[0];
      CRTC.line_count = sh.crtc_line_count;
      CRTC.raster_count = sh.crtc_raster_count;
      CRTC.hsw_count = sh.crtc_hsw_count;
      CRTC.vsw_count = sh.crtc_vsw_count;
      dwFlags = sh.crtc_flags[0] + (sh.crtc_flags[1] * 256);
      CRTC.flag_invsync = dwFlags & 1 ? 1 : 0; // vsync active?
      if (dwFlags & 2) { // hsync active?
         flags1.inHSYNC = 0xff;
         CRTC.flag_hadhsync = 1;
         if ((CRTC.hsw_count >= 3) && (CRTC.hsw_count < 7)) {
            CRTC.flag_inmonhsync = 1;
         }
      }
      CRTC.flag_invta = dwFlags & 0x80 ? 1 : 0; // in vertical total adjust?
      GateArray.hs_count = sh.ga_int_delay & 3;
      GateArray.sl_count = sh.ga_sl_count;
      z80.int_pending = sh.z80_int_pending;
   }

   return 0; // dump ok!
}

/**
 * snapshot_save_mem:
 * @pBuffer: snapshot buffer ready to use, is modified.
 * @bSize: allocated buffer size
 *
 * return 0 when is saved successfully
 */
int snapshot_save_mem (uint8_t *sna_buffer, uint32_t buffer_size)
{
   t_SNA_header sh;
   int n;
   uint32_t dwFlags;

   if(buffer_size < sizeof(sh) + get_ram_size())
      return ERR_OUT_OF_MEMORY;

   memset(&sh, 0, sizeof(sh));

   memcpy(sh.id, "MV - SNA", sizeof(sh.id));

   sh.version = 3;

   /* Z80 */
   sh.AF[1] = _A;
   sh.AF[0] = _F;
   sh.BC[1] = _B;
   sh.BC[0] = _C;
   sh.DE[1] = _D;
   sh.DE[0] = _E;
   sh.HL[1] = _H;
   sh.HL[0] = _L;
   sh.R = (_R & 0x7f) | (_Rb7 & 0x80);
   sh.I = _I;
   if (_IFF1)
      sh.IFF0 = 1;
   if (_IFF2)
      sh.IFF1 = 1;
   sh.IX[1] = _IXh;
   sh.IX[0] = _IXl;
   sh.IY[1] = _IYh;
   sh.IY[0] = _IYl;
   sh.SP[1] = z80.SP.b.h;
   sh.SP[0] = z80.SP.b.l;
   sh.PC[1] = z80.PC.b.h;
   sh.PC[0] = z80.PC.b.l;
   sh.IM = _IM;
   sh.AFx[1] = z80.AFx.b.h;
   sh.AFx[0] = z80.AFx.b.l;
   sh.BCx[1] = z80.BCx.b.h;
   sh.BCx[0] = z80.BCx.b.l;
   sh.DEx[1] = z80.DEx.b.h;
   sh.DEx[0] = z80.DEx.b.l;
   sh.HLx[1] = z80.HLx.b.h;
   sh.HLx[0] = z80.HLx.b.l;
   // Gate Array
   sh.ga_pen = GateArray.pen;
   for (n = 0; n < 17; n++) { // loop for all colours + border
      sh.ga_ink_values[n] = GateArray.ink_values[n];
   }
   sh.ga_ROM_config = GateArray.ROM_config;
   sh.ga_RAM_config = GateArray.RAM_config;
   // CRTC
   sh.crtc_reg_select = CRTC.reg_select;
   for (n = 0; n < 18; n++) { // loop for all CRTC registers
      sh.crtc_registers[n] = CRTC.registers[n];
   }

   /* ROM select */
   sh.upper_ROM = GateArray.upper_ROM;

   /* PPI */
   sh.ppi_A = PPI.portA;
   sh.ppi_B = PPI.portB;
   sh.ppi_C = PPI.portC;
   sh.ppi_control = PPI.control;

   /* PSG */
   sh.psg_reg_select = PSG.reg_select;

   for (n = 0; n < 16; n++)
   {
      /* loop for all PSG registers */
      sh.psg_registers[n] = PSG.RegisterAY.Index[n];
   }

   sh.ram_size[0] = CPC.ram_size & 0xff;
   sh.ram_size[1] = (CPC.ram_size >> 8) & 0xff;
   /* version 2 info */
   sh.cpc_model = CPC.model;
   /* version 3 info */
   sh.fdc_motor = FDC.motor;
   sh.drvA_current_track = driveA.current_track;
   sh.drvB_current_track = driveB.current_track;
   sh.printer_data = CPC.printer_port ^ 0x80; // invert bit 7 again
   sh.psg_env_step = PSG.AmplitudeEnv >> 1; // divide by 2 to bring it into the 0 - 15 range
   if (PSG.FirstPeriod)
   {
      switch (PSG.RegisterAY.EnvType)
      {
         case 0:
         case 1:
         case 2:
         case 3:
         case 8:
         case 9:
         case 10:
         case 11:
            sh.psg_env_direction = 0xff; // down
            break;
         case 4:
         case 5:
         case 6:
         case 7:
         case 12:
         case 13:
         case 14:
         case 15:
            sh.psg_env_direction = 0x01; // up
            break;
      }
   }
   else
   {
      switch (PSG.RegisterAY.EnvType)
      {
         case 0:
         case 1:
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
         case 7:
         case 9:
         case 11:
         case 13:
         case 15:
            sh.psg_env_direction = 0x00; // hold
            break;
         case 8:
         case 14:
            sh.psg_env_direction = 0xff; // down
            break;
         case 10:
         case 12:
            sh.psg_env_direction = 0x01; // up
            break;
      }
   }
   sh.crtc_addr[0]       = CRTC.addr & 0xff;
   sh.crtc_addr[1]       = (CRTC.addr >> 8) & 0xff;
   sh.crtc_scanline[0]   = VDU.scanline & 0xff;
   sh.crtc_scanline[1]   = (VDU.scanline >> 8) & 0xff;
   sh.crtc_char_count[0] = CRTC.char_count;
   sh.crtc_line_count    = CRTC.line_count;
   sh.crtc_raster_count  = CRTC.raster_count;
   sh.crtc_hsw_count     = CRTC.hsw_count;
   sh.crtc_vsw_count     = CRTC.vsw_count;
   dwFlags               = 0;

   if (CRTC.flag_invsync) // vsync active?
      dwFlags |= 1;
   if (flags1.inHSYNC) // hsync active?
      dwFlags |= 2;
   if (CRTC.flag_invta) // in vertical total adjust?
      dwFlags |= 0x80;

   sh.crtc_flags[0] = dwFlags & 0xff;
   sh.crtc_flags[1] = (dwFlags >> 8) & 0xff;
   sh.ga_int_delay = GateArray.hs_count;
   sh.ga_sl_count = GateArray.sl_count;
   sh.z80_int_pending = z80.int_pending;

   memcpy(sna_buffer, &sh, sizeof(sh));
   memcpy(sna_buffer + sizeof(sh), pbRAM, CPC.ram_size*1024);

   return 0;

}


int snapshot_load (char *pchFileName)
{
   uint32_t size;

   if ((pfileObject = fopen(pchFileName, "rb")) != NULL)
   {
      size = file_size(fileno(pfileObject));
      if (size <= sizeof(t_SNA_header))
      { // the sna image should have at least the header...
         fclose(pfileObject);
         return ERR_SNA_INVALID;
      }

      pbSnaImage = (uint8_t *)malloc(size);

      if(!fread(pbSnaImage, size, 1, pfileObject)) { // read snapshot
         fclose(pfileObject);
         return ERR_SNA_INVALID;
      }
      return snapshot_load_mem(pbSnaImage, size);
   }

   return ERR_FILE_NOT_FOUND;
}


int snapshot_save (char *pchFileName)
{
   int error;
   uint32_t dwSnapSize;

   dwSnapSize = sizeof(t_SNA_header) + get_ram_size();
   pbSnaImage = (uint8_t*) malloc(dwSnapSize);
   if(!pbSnaImage)
      return ERR_OUT_OF_MEMORY;

   error = snapshot_save_mem(pbSnaImage, dwSnapSize);
   if(error) {
      free(pbSnaImage);
      pbSnaImage = NULL;
      return error;
   }

   if ((pfileObject = fopen(pchFileName, "wb")) != NULL) {
      if (fwrite(pbSnaImage, dwSnapSize, 1, pfileObject) != 1) { // write snapshot header
         fclose(pfileObject);
         return ERR_SNA_WRITE;
      }
      fclose(pfileObject);
   } else {
      return ERR_SNA_WRITE;
   }

   return 0;
}


/**
 * DSK handlers
 */
void dsk_eject (t_drive *drive)
{
   uint32_t track, side;
   uint32_t dwTemp;

   for (track = 0; track < DSK_TRACKMAX; track++)
   {
      /* loop for all tracks */
      for (side = 0; side < DSK_SIDEMAX; side++)
      {
         /* loop for all sides */
         if (drive->track[track][side].data) /* track is formatted? */
            free(drive->track[track][side].data); // release memory allocated for this track
      }
   }

   dwTemp = drive->current_track; // save the drive head position
   memset(drive, 0, sizeof(t_drive)); // clear drive info structure
   drive->current_track = dwTemp;
}


int dsk_load (char *pchFileName, t_drive *drive, char chID)
{
   int iRetCode;
   uint32_t dwTrackSize, track, side, sector, dwSectorSize, dwSectors;
   uint8_t *pbPtr, *pbDataPtr, *pbTempPtr, *pbTrackSizeTable;

   iRetCode = 0;
   dsk_eject(drive);
   if ((pfileObject = fopen(pchFileName, "rb")) != NULL)
   {
      if(!fread(pbGPBuffer, 0x100, 1, pfileObject)) { // read DSK header
         iRetCode = ERR_DSK_INVALID;
         goto exit;
      }
      pbPtr = pbGPBuffer;

      if (memcmp(pbPtr, "MV - CPC", 8) == 0) { // normal DSK image?
         drive->tracks = *(pbPtr + 0x30); // grab number of tracks
         if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
            drive->tracks = DSK_TRACKMAX; // limit to maximum
         }
         drive->sides = *(pbPtr + 0x31); // grab number of sides
         if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
            iRetCode = ERR_DSK_SIDES;
            goto exit;
         }
         dwTrackSize = (*(pbPtr + 0x32) + (*(pbPtr + 0x33) << 8)) - 0x100; // determine track size in bytes, minus track header
         drive->sides--; // zero base number of sides
         for (track = 0; track < drive->tracks; track++) { // loop for all tracks
            for (side = 0; side <= drive->sides; side++) { // loop for all sides
               if(!fread(pbGPBuffer+0x100, 0x100, 1, pfileObject)) { // read track header
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
               pbPtr = pbGPBuffer + 0x100;
               if (memcmp(pbPtr, "Track-Info", 10) != 0) { // abort if ID does not match
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
               dwSectorSize = 0x80 << *(pbPtr + 0x14); // determine sector size in bytes
               dwSectors = *(pbPtr + 0x15); // grab number of sectors
               if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
                  iRetCode = ERR_DSK_SECTORS;
                  goto exit;
               }
               drive->track[track][side].sectors = dwSectors; // store sector count
               drive->track[track][side].size = dwTrackSize; // store track size
               drive->track[track][side].data = (uint8_t *)malloc(dwTrackSize); // attempt to allocate the required memory
               if (drive->track[track][side].data == NULL) { // abort if not enough
                  iRetCode = ERR_OUT_OF_MEMORY;
                  goto exit;
               }
               pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
               pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
               for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
                  memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
                  memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
                  drive->track[track][side].sector[sector].size = dwSectorSize;
                  drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
                  pbDataPtr += dwSectorSize;
                  pbPtr += 8;
               }
               if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject)) { // read entire track data in one go
                  iRetCode = ERR_DSK_INVALID;
                  goto exit;
               }
            }
         }
         drive->altered = 0; // disk is as yet unmodified
      } else {
         if (memcmp(pbPtr, "EXTENDED", 8) == 0) { // extended DSK image?
            drive->tracks = *(pbPtr + 0x30); // number of tracks
            if (drive->tracks > DSK_TRACKMAX) {  // limit to maximum possible
               drive->tracks = DSK_TRACKMAX;
            }
            drive->random_DEs = *(pbPtr + 0x31) & 0x80; // simulate random Data Errors?
            drive->sides = *(pbPtr + 0x31) & 3; // number of sides
            if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
               iRetCode = ERR_DSK_SIDES;
               goto exit;
            }
            pbTrackSizeTable = pbPtr + 0x34; // pointer to track size table in DSK header
            drive->sides--; // zero base number of sides
            for (track = 0; track < drive->tracks; track++) { // loop for all tracks
               for (side = 0; side <= drive->sides; side++) { // loop for all sides
                  dwTrackSize = (*pbTrackSizeTable++ << 8); // track size in bytes
                  if (dwTrackSize != 0) { // only process if track contains data
                     dwTrackSize -= 0x100; // compensate for track header
                     if(!fread(pbGPBuffer+0x100, 0x100, 1, pfileObject)) { // read track header
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                     pbPtr = pbGPBuffer + 0x100;
                     if (memcmp(pbPtr, "Track-Info", 10) != 0) { // valid track header?
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                     dwSectors = *(pbPtr + 0x15); // number of sectors for this track
                     if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
                        iRetCode = ERR_DSK_SECTORS;
                        goto exit;
                     }
                     drive->track[track][side].sectors = dwSectors; // store sector count
                     drive->track[track][side].size = dwTrackSize; // store track size
                     drive->track[track][side].data = (uint8_t *)malloc(dwTrackSize); // attempt to allocate the required memory
                     if (drive->track[track][side].data == NULL) { // abort if not enough
                        iRetCode = ERR_OUT_OF_MEMORY;
                        goto exit;
                     }
                     pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
                     pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
                     for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
                        memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
                        memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
                        dwSectorSize = *(pbPtr + 0x1e) + (*(pbPtr + 0x1f) << 8); // sector size in bytes
                        drive->track[track][side].sector[sector].size = dwSectorSize;
                        drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
                        pbDataPtr += dwSectorSize;
                        pbPtr += 8;
                     }
                     if (!fread(pbTempPtr, dwTrackSize, 1, pfileObject)) { // read entire track data in one go
                        iRetCode = ERR_DSK_INVALID;
                        goto exit;
                     }
                  } else {
                     memset(&drive->track[track][side], 0, sizeof(t_track)); // track not formatted
                  }
               }
            }
            drive->altered = 0; // disk is as yet unmodified
         } else {
            iRetCode = ERR_DSK_INVALID; // file could not be identified as a valid DSK
         }
      }

exit:
      fclose(pfileObject);
   }
   else
      iRetCode = ERR_FILE_NOT_FOUND;

   if (iRetCode != 0) // on error, 'eject' disk from drive
      dsk_eject(drive);

   return iRetCode;
}

int dsk_save (char *pchFileName, t_drive *drive, char chID)
{
   t_DSK_header dh;
   t_track_header th;
   uint32_t track, side, pos, sector;

   if ((pfileObject = fopen(pchFileName, "wb")) != NULL) {
      memset(&dh, 0, sizeof(dh));
      memcpy(dh.id, "EXTENDED CPC DSK File\r\nDisk-Info\r\n", sizeof(dh.id));
      strcpy(dh.unused1, "Caprice32\r\n");
      dh.tracks = drive->tracks;
      dh.sides = (drive->sides+1) | (drive->random_DEs); // correct side count and indicate random DEs, if necessary
      pos = 0;

      for (track = 0; track < drive->tracks; track++)
      { // loop for all tracks
         for (side = 0; side <= drive->sides; side++)
         { // loop for all sides
            if (drive->track[track][side].size) // track is formatted?
               dh.track_size[pos] = (drive->track[track][side].size + 0x100) >> 8; // track size + header in bytes
            pos++;
         }
      }

      if (!fwrite(&dh, sizeof(dh), 1, pfileObject))
      { // write header to file
         fclose(pfileObject);
         return ERR_DSK_WRITE;
      }

      memset(&th, 0, sizeof(th));
      memcpy(th.id, "Track-Info\r\n", sizeof(th.id));
      for (track = 0; track < drive->tracks; track++) { // loop for all tracks
         for (side = 0; side <= drive->sides; side++) { // loop for all sides
            if (drive->track[track][side].size) { // track is formatted?
               th.track = track;
               th.side = side;
               th.bps = 2;
               th.sectors = drive->track[track][side].sectors;
               th.gap3 = 0x4e;
               th.filler = 0xe5;
               for (sector = 0; sector < th.sectors; sector++) {
                  memcpy(&th.sector[sector][0], drive->track[track][side].sector[sector].CHRN, 4); // copy CHRN
                  memcpy(&th.sector[sector][4], drive->track[track][side].sector[sector].flags, 2); // copy ST1 & ST2
                  th.sector[sector][6] = drive->track[track][side].sector[sector].size & 0xff;
                  th.sector[sector][7] = (drive->track[track][side].sector[sector].size >> 8) & 0xff; // sector size in bytes
               }

               if (!fwrite(&th, sizeof(th), 1, pfileObject)) { // write track header
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
               if (!fwrite(drive->track[track][side].data, drive->track[track][side].size, 1, pfileObject)) { // write track data
                  fclose(pfileObject);
                  return ERR_DSK_WRITE;
               }
            }
         }
      }
      fclose(pfileObject);
   }
   else
      return ERR_DSK_WRITE; // write attempt failed

   return 0;
}

int dsk_format (t_drive *drive, int iFormat)
{
   uint32_t track, side;
   int iRetCode = 0;

   drive->tracks = disk_format[iFormat].tracks;
   if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
      drive->tracks = DSK_TRACKMAX; // limit to maximum
   }
   drive->sides = disk_format[iFormat].sides;
   if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
      iRetCode = ERR_DSK_SIDES;
      goto exit;
   }
   drive->sides--; // zero base number of sides
   for (track = 0; track < drive->tracks; track++)
   {
      /* loop for all tracks */

      for (side = 0; side <= drive->sides; side++)
      {
         /* loop for all sides */
         uint32_t dwTrackSize;
         uint32_t sector;
         uint32_t dwSectorSize = 0x80 << disk_format[iFormat].sector_size; // determine sector size in bytes
         uint32_t dwSectors = disk_format[iFormat].sectors;

         if (dwSectors > DSK_SECTORMAX)
         { // abort if sector count greater than maximum
            iRetCode = ERR_DSK_SECTORS;
            goto exit;
         }

         dwTrackSize = dwSectorSize * dwSectors; // determine track size in bytes, minus track header
         drive->track[track][side].sectors = dwSectors; /* store sector count */
         drive->track[track][side].size    = dwTrackSize; /* store track size */
         drive->track[track][side].data    = (uint8_t *)malloc(dwTrackSize); /* attempt to allocate the required memory */
         if (drive->track[track][side].data == NULL)
         {
            /* abort if not enough */
            iRetCode = ERR_OUT_OF_MEMORY;
            goto exit;
         }

         uint8_t *pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
         uint8_t *pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
         uint8_t CHRN[4];
         CHRN[0] = (uint8_t)track;
         CHRN[1] = (uint8_t)side;
         CHRN[3] = (uint8_t)disk_format[iFormat].sector_size;

         for (sector = 0; sector < dwSectors; sector++)
         {
            // loop for all sectors
            CHRN[2] = disk_format[iFormat].sector_ids[side][sector];
            memcpy(drive->track[track][side].sector[sector].CHRN, CHRN, 4); // copy CHRN
            drive->track[track][side].sector[sector].size = dwSectorSize;
            drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
            pbDataPtr += dwSectorSize;
         }
         memset(pbTempPtr, disk_format[iFormat].filler_byte, dwTrackSize);
      }
   }
   drive->altered = 1; // flag disk as having been modified

exit:
   if (iRetCode != 0) // on error, 'eject' disk from drive
      dsk_eject(drive);
   return iRetCode;
}


/**
 * TAPE handlers
 */

void tape_eject (void)
{
   free(pbTapeImage);
   pbTapeImage = NULL;
}

void play_tape(void)
{
	if (pbTapeImage) {
		if (CPC.tape_play_button) {
			CPC.tape_play_button = 0;
		} else {
			CPC.tape_play_button = 0x10;
		}
	}
}

int tape_insert (char *pchFileName)
{
   long lFileSize;
   int iBlockLength;
   uint8_t bID;
   uint8_t *pbPtr, *pbBlock;

   tape_eject();

   if ((pfileObject = fopen(pchFileName, "rb")) == NULL)
      return ERR_FILE_NOT_FOUND;

   if(!fread(pbGPBuffer, 10, 1, pfileObject)) { // read CDT header
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   pbPtr = pbGPBuffer;

   if (memcmp(pbPtr, "ZXTape!\032", 8) != 0)
   {
      /* valid CDT file? */
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }

   if (*(pbPtr + 0x08) != 1)
   {
      /* major version must be 1 */
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }

   lFileSize = file_size(fileno(pfileObject)) - 0x0a;

   if (lFileSize <= 0)
   { // the tape image should have at least one block...
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   pbTapeImage = (uint8_t *)malloc(lFileSize+6);
   *pbTapeImage = 0x20; // start off with a pause block
   *(uint16_t *)(pbTapeImage+1) = 2000; // set the length to 2 seconds
   if(!fread(pbTapeImage+3, lFileSize, 1, pfileObject)) { // append the entire CDT file
      fclose(pfileObject);
      return ERR_TAP_INVALID;
   }
   fclose(pfileObject);
   *(pbTapeImage+lFileSize+3) = 0x20; // end with a pause block
   *(uint16_t *)(pbTapeImage+lFileSize+3+1) = 2000; // set the length to 2 seconds

   #ifdef DEBUG_TAPE
   fputs("--- New Tape\r\n", pfoDebug);
   #endif
   pbTapeImageEnd = pbTapeImage + lFileSize+6;
   pbBlock = pbTapeImage;
   bool bolGotDataBlock = false;
   while (pbBlock < pbTapeImageEnd)
   {
      bID = *pbBlock++;
      switch(bID)
      {
         case 0x10: // standard speed data block
            iBlockLength = *(uint16_t *)(pbBlock+2) + 4;
            bolGotDataBlock = true;
            break;
         case 0x11: // turbo loading data block
            iBlockLength = (*(uint32_t *)(pbBlock+0x0f) & 0x00ffffff) + 0x12;
            bolGotDataBlock = true;
            break;
         case 0x12: // pure tone
            iBlockLength = 4;
            bolGotDataBlock = true;
            break;
         case 0x13: // sequence of pulses of different length
            iBlockLength = *pbBlock * 2 + 1;
            bolGotDataBlock = true;
            break;
         case 0x14: // pure data block
            iBlockLength = (*(uint32_t *)(pbBlock+0x07) & 0x00ffffff) + 0x0a;
            bolGotDataBlock = true;
            break;
         case 0x15: // direct recording
            iBlockLength = (*(uint32_t *)(pbBlock+0x05) & 0x00ffffff) + 0x08;
            bolGotDataBlock = true;
            break;
         case 0x20: // pause
            if ((!bolGotDataBlock) && (pbBlock != pbTapeImage+1)) {
               *(uint16_t *)pbBlock = 0; // remove any pauses (execept ours) before the data starts
            }
            iBlockLength = 2;
            break;
         case 0x21: // group start
            iBlockLength = *pbBlock + 1;
            break;
         case 0x22: // group end
            iBlockLength = 0;
            break;
         case 0x23: // jump to block
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 2;
            break;
         case 0x24: // loop start
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 2;
            break;
         case 0x25: // loop end
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 0;
            break;
         case 0x26: // call sequence
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = (*(uint16_t *)pbBlock * 2) + 2;
            break;
         case 0x27: // return from sequence
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = 0;
            break;
         case 0x28: // select block
            return ERR_TAP_UNSUPPORTED;
            iBlockLength = *(uint16_t *)pbBlock + 2;
            break;
         case 0x30: // text description
            iBlockLength = *pbBlock + 1;
            break;
         case 0x31: // message block
            iBlockLength = *(pbBlock+1) + 2;
            break;
         case 0x32: // archive info
            iBlockLength = *(uint16_t *)pbBlock + 2;
            break;
         case 0x33: // hardware type
            iBlockLength = (*pbBlock * 3) + 1;
            break;
         case 0x34: // emulation info
            iBlockLength = 8;
            break;
         case 0x35: // custom info block
            iBlockLength = *(uint32_t *)(pbBlock+0x10) + 0x14;
            break;
         case 0x40: // snapshot block
            iBlockLength = (*(uint32_t *)(pbBlock+0x01) & 0x00ffffff) + 0x04;
            break;
         case 0x5A: // another tzx/cdt file
            iBlockLength = 9;
            break;

         default: // "extension rule"
            iBlockLength = *(uint32_t *)pbBlock + 4;
      }

#ifdef DEBUG_TAPE
      fprintf(pfoDebug, "%02x %d\r\n", bID, iBlockLength);
#endif

      pbBlock += iBlockLength;
   }

   if (pbBlock != pbTapeImageEnd)
   {
      tape_eject();
      return ERR_TAP_INVALID;
   }

   Tape_Rewind();

   return 0;
}


int tape_insert_voc (char *pchFileName)
{
   long lFileSize, lOffset, lInitialOffset, lSampleLength;
   int iBlockLength;
   uint8_t *pbPtr, *pbTapeImagePtr, *pbVocDataBlock, *pbVocDataBlockPtr;
   bool bolDone;

   tape_eject();

   if ((pfileObject = fopen(pchFileName, "rb")) == NULL)
      return ERR_FILE_NOT_FOUND;

   if(!fread(pbGPBuffer, 26, 1, pfileObject)) { // read VOC header
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }
   pbPtr = pbGPBuffer;

   if (memcmp(pbPtr, "Creative Voice File\032", 20) != 0)
   {
      // valid VOC file?
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }
   lOffset =
   lInitialOffset = *(uint16_t *)(pbPtr + 0x14);
   lFileSize = file_size(fileno(pfileObject));

   if ((lFileSize-26) <= 0)
   { // should have at least one block...
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }

   #ifdef DEBUG_TAPE
   fputs("--- New Tape\r\n", pfoDebug);
   #endif
   iBlockLength = 0;
   lSampleLength = 0;
   uint8_t bSampleRate = 0;
   bolDone = false;
   while ((!bolDone) && (lOffset < lFileSize))
   {
      fseek(pfileObject, lOffset, SEEK_SET);
      if(!fread(pbPtr, 16, 1, pfileObject)) { // read block ID + size
         fclose(pfileObject);
         return ERR_TAP_BAD_VOC;
      }
#ifdef DEBUG_TAPE
      fprintf(pfoDebug, "%02x %d\r\n", *pbPtr, *(uint32_t *)(pbPtr+0x01) & 0x00ffffff);
#endif

      switch(*pbPtr)
      {
         case 0x0: // terminator
            bolDone = true;
            break;
         case 0x1: // sound data
            iBlockLength = (*(uint32_t *)(pbPtr+0x01) & 0x00ffffff) + 4;
            lSampleLength += iBlockLength - 6;

            if ((bSampleRate) && (bSampleRate != *(pbPtr+0x04)))
            { // no change in sample rate allowed
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }

            bSampleRate = *(pbPtr+0x04);

            if (*(pbPtr+0x05) != 0)
            { // must be 8 bits wide
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            break;
         case 0x2: // sound continue
            iBlockLength = (*(uint32_t *)(pbPtr+0x01) & 0x00ffffff) + 4;
            lSampleLength += iBlockLength - 4;
            break;
         case 0x3: // silence
            iBlockLength = 4;
            lSampleLength += *(uint16_t *)(pbPtr+0x01) + 1;
            if ((bSampleRate) && (bSampleRate != *(pbPtr+0x03))) { // no change in sample rate allowed
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            bSampleRate = *(pbPtr+0x03);
            break;
         case 0x4: // marker
            iBlockLength = 3;
            break;
         case 0x5: // ascii
            iBlockLength = (*(uint32_t *)(pbPtr+0x01) & 0x00ffffff) + 4;
            break;
         default:
            fclose(pfileObject);
            return ERR_TAP_BAD_VOC;
      }
      lOffset += iBlockLength;
   }
   #ifdef DEBUG_TAPE
   fprintf(pfoDebug, "--- %ld bytes\r\n", lSampleLength);
   #endif

   uint32_t dwTapePulseCycles = 3500000L / (1000000L / (256 - bSampleRate)); // length of one pulse in ZX Spectrum T states
   uint32_t dwCompressedSize = lSampleLength >> 3; // 8x data reduction
   if (dwCompressedSize > 0x00ffffff)
   {
      /* we only support one direct recording block right now */
      fclose(pfileObject);
      return ERR_TAP_BAD_VOC;
   }

   pbTapeImage = (uint8_t *)malloc(dwCompressedSize+1+8+6);

   if (pbTapeImage == NULL)
   {
      /* check if the memory allocation has failed */
      fclose(pfileObject);
      return ERR_OUT_OF_MEMORY;
   }
   *pbTapeImage = 0x20; // start off with a pause block
   *(uint16_t *)(pbTapeImage+1) = 2000; // set the length to 2 seconds

   *(pbTapeImage+3)             = 0x15; // direct recording block
   *(uint16_t *)(pbTapeImage+4) = (uint16_t)dwTapePulseCycles; // number of T states per sample
   *(uint16_t *)(pbTapeImage+6) = 0; // pause after block
   *(pbTapeImage+8)             = lSampleLength & 7 ? lSampleLength & 7 : 8; // bits used in last byte
   *(uint32_t *)(pbTapeImage+9) = dwCompressedSize & 0x00ffffff; // data length
   pbTapeImagePtr               = pbTapeImage + 12;

   lOffset = lInitialOffset;
   bolDone = false;
   uint32_t dwBit = 8;
   uint8_t bByte = 0;
   unsigned iBytePos;
   while ((!bolDone) && (lOffset < lFileSize))
   {
      fseek(pfileObject, lOffset, SEEK_SET);
      if(!fread(pbPtr, 1, 1, pfileObject)) { // read block ID
         fclose(pfileObject);
         return ERR_TAP_BAD_VOC;
      }

      switch(*pbPtr)
      {
         case 0x0: // terminator
            bolDone = true;
            break;
         case 0x1: // sound data
            if(!fread(pbPtr, 3+2, 1, pfileObject)) { // get block size and sound info
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            iBlockLength   = (*(uint32_t *)(pbPtr) & 0x00ffffff) + 4;
            lSampleLength  = iBlockLength - 6;
            pbVocDataBlock = (uint8_t *)malloc(lSampleLength);

            if (pbVocDataBlock == NULL)
            {
               fclose(pfileObject);
               tape_eject();
               return ERR_OUT_OF_MEMORY;
            }

            if(!fread(pbVocDataBlock, lSampleLength, 1, pfileObject)) {
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            pbVocDataBlockPtr = pbVocDataBlock;

            for (iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
            {
               uint8_t bVocSample = *pbVocDataBlockPtr++;

               dwBit--;

               if (bVocSample > VOC_THRESHOLD)
                  bByte |= bit_values[dwBit];

               if (!dwBit)
               {
                  /* got all 8 bits? */
                  *pbTapeImagePtr++ = bByte;
                  dwBit = 8;
                  bByte = 0;
               }
            }
            free(pbVocDataBlock);
            break;
         case 0x2: // sound continue
            if(!fread(pbPtr, 3, 1, pfileObject)) { // get block size
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            iBlockLength = (*(uint32_t *)(pbPtr) & 0x00ffffff) + 4;
            lSampleLength = iBlockLength - 4;
            pbVocDataBlock = (uint8_t *)malloc(lSampleLength);
            if (pbVocDataBlock == NULL)
            {
               fclose(pfileObject);
               tape_eject();
               return ERR_OUT_OF_MEMORY;
            }
            if(!fread(pbVocDataBlock, lSampleLength, 1, pfileObject)) {
               fclose(pfileObject);
               return ERR_TAP_BAD_VOC;
            }
            pbVocDataBlockPtr = pbVocDataBlock;
            for (iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
            {
               uint8_t bVocSample = *pbVocDataBlockPtr++;
               dwBit--;
               if (bVocSample > VOC_THRESHOLD)
                  bByte |= bit_values[dwBit];
               if (!dwBit)
               { // got all 8 bits?
                  *pbTapeImagePtr++ = bByte;
                  dwBit = 8;
                  bByte = 0;
               }
            }
            free(pbVocDataBlock);
            break;
         case 0x3: // silence
            iBlockLength = 4;
            lSampleLength = *(uint16_t *)(pbPtr) + 1;
            for (iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
            {
               dwBit--;
               if (!dwBit)
               { // got all 8 bits?
                  *pbTapeImagePtr++ = bByte;
                  dwBit = 8;
                  bByte = 0;
               }
            }
            break;
         case 0x4: // marker
            iBlockLength = 3;
            break;
         case 0x5: // ascii
            iBlockLength = (*(uint32_t *)(pbPtr) & 0x00ffffff) + 4;
            break;
      }
      lOffset += iBlockLength;
   }
   fclose(pfileObject);

   *pbTapeImagePtr = 0x20; // end with a pause block
   *(uint16_t *)(pbTapeImagePtr+1) = 2000; // set the length to 2 seconds

   pbTapeImageEnd = pbTapeImagePtr + 3;

   Tape_Rewind();

   return 0;
}
