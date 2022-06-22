/****************************************************************************
 *  Caprice32 libretro unit-tests
 *
 *   David Colmenero - D_Skywalk (2019-2021)
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

#include "../cmocka/include/cmocka.h"
#include "libretro/dsk/loader.h"
#include "../unit-tests/test-utils.h"
#include "test-db.h"

char * basedir = NULL;
char rompath[RETRO_PATH_MAX];
game_cfg_t game_configuration;


static void cleancpcdb_tests(void **state) {
   (void) state; /* unused */

   uint32_t db_size = sizeof(cleancpcdb) / sizeof(t_test_entry);
   for (int n = 0; n < db_size; n++)
   {
      // clean config
      memset(&game_configuration, 0, sizeof(game_cfg_t));

      // prepare entry
      t_test_entry * entry = &cleancpcdb[n];

      // prepare rom path
      path_join(rompath, basedir, entry->filepath);

      // run test
      test_dsk_hashed(rompath, entry->cmdhash, entry->hash);
   }
}

int main(int argc, char* argv[]) {
   if (argc != 2) {
      printf("usage:\n\t%s <path-to-roms>\n", argv[0]);
      exit(1);
   }

   basedir = argv[1];

   pbGPBuffer = (uint8_t*) malloc(128 * 1024 * sizeof(uint8_t)); // attempt to allocate the general purpose buffer

   const struct CMUnitTest tests[] = {
      cmocka_unit_test(cleancpcdb_tests),
   };

   cmocka_run_group_tests(tests, NULL, NULL);
   free(pbGPBuffer);
}
