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
#include "test-utils.h"

static void cpm_tests_success(void **state) {
   (void) state; /* unused */

   test_dsk("tests/cpm/Deadline (1982)(Infocom)[CPM].dsk", "|CPM", "SYSTEM");
   test_dsk("tests/cpm/Enchanter (1984)(Infocom)[CPM].dsk", "|CPM", "SYSTEM");
   test_dsk("tests/heroquest/Hero Quest (UK) (Face A) (128K) (2022) (CPM) (Hack).dsk", "|CPM", "SYSTEM");
   test_dsk("tests/cpm/Bestial Warrior (1989)(Dinamic Software)(es)[lightgun].dsk", "|CPM", "SYSTEM");

   // TODO: fails CPM BOOT
   //test_dsk("tests/cpm/Labyrinthe aux Cent Calculs, Le - Ecole (1989)(Retz)(fr)(Disk 1 Side A)[CPM] .dsk", "|CPM", "SYSTEM");

}

static void hexagon_tests_success(void **state) {
   (void) state; /* unused */

   test_dsk("tests/hexagon/cd.dsk", "RUN\"DISK.", "DATA_B");
   test_dsk("tests/hexagon/gng.dsk", "RUN\"DISK.", "D10");
   test_dsk("tests/hexagon/gauntlet.dsk", "RUN\"DISK.", "D10");
   test_dsk("tests/hexagon/Bigfoot (1988)(Codemasters).dsk", "RUN\"DISK.", "DATA_B");
}

static void speedlock_tests_success(void **state) {
   (void) state; /* unused */

   test_dsk("tests/speedlock-v1990/Back_To_The_Future_II__Side_A.dsk", "RUN\"DISC.BIN", "DATA");
   test_dsk("tests/speedlock-v1990/Back_to_the_Future-Speedlock.dsk", "RUN\"DISC.BIN", "DATA_B");
   test_dsk("tests/speedlock-v1990/Edd_The_Duck__ENGLISH.dsk", "RUN\"DISC.BIN", "DATA");
   test_dsk("tests/speedlock-v1990/Saint_Dragon__Side_A.dsk", "RUN\"DISC.BIN", "D10");
   test_dsk("tests/speedlock-v1990/Total_Recall__(UK_Retail)__ENGLISH.dsk", "RUN\"DISC.BIN", "DATA_B");
}

static void hidden_tests_success(void **state) {
   (void) state; /* unused */

   test_dsk("tests/hidden-files/Blagger (1985)(Amsoft)[cr].dsk", "RUN\"BLAGGER.BAS", "DATA");
}

static void basic_tests_success(void **state) {
   (void) state; /* unused */

   test_dsk("tests/Cauldron_2__ENGLISH.dsk", "RUN\"DISC.BIN", "DATA");
   test_dsk("tests/Abadia.dsk", "|CPM", "SYSTEM");
   test_dsk("tests/Bonanza_Bros - bad scroll.dsk", "RUN\"DISK.", "DATA_B");
   test_dsk("tests/Prince_of_Persia__(Release_DROSOFT)__ENGLISH.dsk", "RUN\"PRINCE.BIN", "D10");
   test_dsk("tests/Shovel Adventure JGN Edition.dsk", "RUN\"DISC.BIN", "DATA");
   test_dsk("tests/The_Shadows_of_Sergoth__ENGLISH-FRENCH-SPANISH__Side_A.dsk", "RUN\"DISC.BAS", "DATA");
}

int main(void) {
   pbGPBuffer = (uint8_t*) malloc(128 * 1024 * sizeof(uint8_t)); // attempt to allocate the general purpose buffer

   const struct CMUnitTest tests[] = {
      cmocka_unit_test(basic_tests_success),
      cmocka_unit_test(cpm_tests_success),
      cmocka_unit_test(speedlock_tests_success),
      cmocka_unit_test(hexagon_tests_success),
      cmocka_unit_test(hidden_tests_success),
   };

   cmocka_run_group_tests(tests, NULL, NULL);
   free(pbGPBuffer);
}
