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


/* Loading of Plus range cartridge files (.cpr)
   David Skywalker - libretro port CPC+ code
   based on the new cpc caprice core by Colin Pitrat - https://github.com/ColinPitrat/caprice32

   The file format is RIFF (Resource Interchange File Format) as described
   here:
    - http://www.cpcwiki.eu/index.php/Format:CPR_CPC_Plus_cartridge_file_format
    - https://en.wikipedia.org/wiki/Resource_Interchange_File_Format
*/

#include "cart.h"
#include "errors.h"

// TODO: remove log
#ifdef DEBUG_CART
#define ERR(x) fprintf(stderr, x "\n")
#define LOG(x, val) printf(x "\n", val)
#else
#define ERR(x)
#define LOG(x, val)
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))

#define CPR_HEADER_SIZE 12
#define CPR_CHUNK_ID_SIZE 4
#define CPR_CHUNK_HEADER_SIZE 8

#define CPR_PAGES          32
#define CPR_PAGE_SIZE      16 * 1024
#define CPR_FILE_MAX_SIZE 768 * 1024
#define CPR_MAX_HEADER_SIZE 260
#define CPR_MAX_CART_SIZE (CPR_PAGES * CPR_PAGE_SIZE) + CPR_MAX_HEADER_SIZE

extern uint8_t* pbROMlo;

uint8_t* pbCartridgeImage = NULL;
uint8_t* pbCartridgePages[CPR_PAGES] = { NULL };

// FIXME: compatible MSB?
uint32_t extractChunkSize(const uint8_t *pChunk)
{
   return (uint32_t) (pChunk[4]) +
         ((uint32_t) (pChunk[5]) << 8) +
         ((uint32_t) (pChunk[6]) << 16) +
         ((uint32_t) (pChunk[7]) << 24);
}

void cpr_eject (void)
{
   int i;

   for(i = 0; i < CPR_PAGES; i++)
      pbCartridgePages[i] = NULL;

   if(pbCartridgeImage)
      free(pbCartridgeImage);
   pbCartridgeImage = NULL;
}

int cpr_init()
{
   int i;

   pbCartridgeImage = (uint8_t*) malloc(CPR_MAX_CART_SIZE * sizeof(uint8_t));
   if (pbCartridgeImage == NULL)
   {
      return ERR_OUT_OF_MEMORY;
   }

   memset(pbCartridgeImage, 0, CPR_MAX_CART_SIZE);

   // prepare cart page pointers
   for(i = 0; i < CPR_PAGES; i++) {
      pbCartridgePages[i] = &pbCartridgeImage[i * CPR_PAGE_SIZE];
   }

   return 0;
}

int cpr_fload (const char* pchFileName)
{
   FILE *pfile;
   uint8_t* pBuffer;

   if ((pfile = fopen(pchFileName, "rb")) == NULL)
   {
      LOG("cpr_load, Error: file not found %s ",  pchFileName);
      return ERR_FILE_NOT_FOUND;
   }

   pBuffer = (uint8_t*) malloc(CPR_FILE_MAX_SIZE * sizeof(uint8_t));
   if (pBuffer == NULL)
   {
      fclose(pfile);
      return ERR_OUT_OF_MEMORY;
   }

   if(!fread(pBuffer, sizeof(uint8_t), CPR_FILE_MAX_SIZE, pfile))
   {
      free(pBuffer);
      fclose(pfile);
      return ERR_CPR_INVALID;
   }

   LOG("cpr_load file: %s ",  pchFileName);
   fclose(pfile);

   int result = cpr_load(pBuffer);

   free(pBuffer);
   return result;
}

int cpr_load(const uint8_t* pbCtBuffer)
{
   const uint8_t *pbPtr = pbCtBuffer;

   // check ROM
   if (memcmp(pbPtr, "RIFF", 4) != 0) // RIFF file
   {
      ERR("Cartridge file is not a RIFF file");
      return ERR_CPR_INVALID;
   }

   if (memcmp(pbPtr + 8, "AMS!", 4) != 0) // CPR file
   {
      ERR("Cartridge file is not a CPR file");
      return ERR_CPR_INVALID;
   }

   uint32_t totalSize = extractChunkSize(pbPtr);
   LOG("CPR size: %u ", totalSize);

   if (totalSize > CPR_MAX_CART_SIZE)
   {
      LOG("Cartridge invalid size - max: %u", CPR_MAX_CART_SIZE);
      return ERR_CPR_INVALID;
   }

   // prepare new CART
   cpr_eject();
   int result = cpr_init();
   if(result)
      return result;

   uint32_t chunkSize = 0;
   uint8_t chunkId[CPR_CHUNK_ID_SIZE+1];
   uint32_t offset = CPR_HEADER_SIZE;
   uint32_t cartOffset = 0;

   // pointer to CHUNK_HEADER
   pbPtr += CPR_HEADER_SIZE;

   while(offset < totalSize) {
      offset += CPR_CHUNK_HEADER_SIZE;

      memcpy(chunkId, pbPtr, CPR_CHUNK_ID_SIZE);
      chunkId[CPR_CHUNK_ID_SIZE] = '\0';

      chunkSize = extractChunkSize(pbPtr);
      //printf("Chunk '%s' at offset %u of size %u \n", chunkId, offset, chunkSize);

      // pointer to CHUNK_DATA
      pbPtr += CPR_CHUNK_HEADER_SIZE;

      // Normal chunk size is 16kB
      // If smaller, it must be filled with 0 up to this limit
      // If bigger, what is after must be ignored
      uint32_t chunkKept = MIN(chunkSize, CPR_PAGE_SIZE);
      // If chunk size is not even, there's a pad byte at the end of it
      if (chunkKept % 2 != 0)
         chunkKept++;

      // A chunk can be empty (observed on some CPR files)
      if(chunkKept > 0)
      {
         memcpy(&pbCartridgeImage[cartOffset], pbPtr, chunkKept);
         pbPtr += chunkKept;
         if(chunkKept < CPR_PAGE_SIZE) {
            // TODO: use the chunkId to identify the cartridge page to set (cbXX with XX between 00 and 31)
            // This would require intializing the whole to 0 before instead of filling what remains at the end
            // Not sure if there are some CPR with unordered pages but this seems to be allowed in theory
            // memset(&pbCartridgeImage[cartOffset+chunkKept], 0, CPR_PAGE_SIZE-chunkKept);
         } else if(chunkKept < chunkSize) {
            ERR("This chunk is bigger than the max allowed size !!!");
            pbPtr += chunkSize-chunkKept;
         }
         cartOffset += CPR_PAGE_SIZE;
         offset += chunkSize;
      }
   }

   LOG("Final offset: %u", offset);
   LOG("Final cartridge offset: %u", cartOffset);

   pbROMlo = (uint8_t *) &pbCartridgeImage[0];

   return 0;
}
