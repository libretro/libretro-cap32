/* Copyright (C) 2018
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "retro_utils.h"

// Verify if file exists
bool file_exists(const char *filename)
{
   struct stat buf;
#ifdef VITA
	if (path_is_valid(filename) && !path_is_directory(filename))
#else
   if (stat(filename, &buf) == 0 &&
      (buf.st_mode & (S_IRUSR|S_IWUSR)) && !(buf.st_mode & S_IFDIR))
#endif
   {
      /* file points to user readable regular file */
      return true;
   }
   return false;
}

void path_join(char* out, const char* basedir, const char* filename)
{
   snprintf(out, RETRO_PATH_MAX, "%s%s%s", basedir, RETRO_PATH_SEPARATOR, filename);
}

char* path_join_dup(const char* basedir, const char* filename)
{
   size_t dirlen = strlen(basedir);
   size_t seplen = strlen(RETRO_PATH_SEPARATOR);
   size_t filelen = strlen(filename);
   char* result = (char*)malloc(dirlen + seplen + filelen + 1);
   strcpy(result, basedir);
   strcpy(result + dirlen, RETRO_PATH_SEPARATOR);
   strcpy(result + dirlen + seplen, filename);
   return result;
}

/**
 * D_Skywalk: Imported from my 3DS pituka implementation
 *            http://david.dantoine.org/proyecto/26/
 */

#ifdef _3DS
void* linearMemAlign(size_t size, size_t alignment);
void linearFree(void* mem);
#endif

void *retro_malloc(size_t size) {
   #ifdef _3DS
   return linearMemAlign(size, 0x80);
   #else
   return malloc(size);
   #endif
}

void retro_free(void * mem) {
   #ifdef _3DS
   linearFree(mem);
   #else
   free(mem);
   #endif
}
