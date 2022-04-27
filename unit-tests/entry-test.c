/****************************************************************************
 *  Caprice32 libretro unit-tests for clean-cpc-db
 *
 *   David Colmenero - D_Skywalk (2019-2022)
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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>

#include "cap32/slots.h"
#include "libretro-core.h"
#include "libretro/dsk/formats.h"
#include "libretro/dsk/loader.h"

extern uint8_t *pbGPBuffer;

void test_loader(t_drive * drive, char * loader_buffer)
{
   DPB_type *dpb = NULL;
   dpb = format_find(drive);
   _loader_run(loader_buffer, dpb, drive);
}

int test_dsk(char * filename_dsk)
{
   t_drive drive;
   char loader_buffer[LOADER_MAX_SIZE];

   memset(&drive, 0, sizeof(t_drive)); // clear disk
   memset(pbGPBuffer, 0, 128 * 1024 * sizeof(uint8_t)); // clear cache

   int result = dsk_load(filename_dsk, &drive, 'A');
   if (result != 0) {
      return result;
   }

   test_loader(&drive, loader_buffer);
   printf("AUTORUN-RESULT: [%s]\n", loader_buffer);
   return result;
}

int main(int argc, char* argv[]) {
   int result = 0;
   if (argc != 2) {
      printf("usage:\n\t%s <DSK>\n", argv[0]);
   }

   pbGPBuffer = (uint8_t*) malloc(128 * 1024 * sizeof(uint8_t)); // attempt to allocate the general purpose buffer
   loader_init();

   if((result = test_dsk(argv[1])) != 0) {
      printf("AUTORUN-ERROR: %i - '%s'\n", result, argv[1]);
   }

   free(pbGPBuffer);
   loader_clean();

   exit(result);
}
