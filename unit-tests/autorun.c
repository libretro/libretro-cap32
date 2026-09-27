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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>

#include "cmocka.h"
#include "cap32/slots.h"
#include "libretro/dsk/loader.h"
#include "libretro/dsk/amsdos_catalog.h"
#include "test-utils.h"

extern t_drive driveA;

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

/* --- TESTS CMOCKA DSK/SYSTEM CASES --- */

static void test_system_boot_is_not_catalogue(void **state) {
   generate_synthetic_disk("test_synth.dsk", true, true, 1);
   run_and_check("test_synth.dsk", 2, "DATA.BIN", false, "START.BIN", true, 2);
}

static void test_hidden_loader_keeps_hidden_attribute(void **state) {
   generate_synthetic_disk("test_synth.dsk", false, true, 2);
   run_and_check("test_synth.dsk", 2, "DATA.BIN", false, "START.BIN", true, 0);
}

static void test_ignore_boot_filename(void **state) {
   generate_synthetic_disk("test_synth.dsk", true, false, 3);
   run_and_check("test_synth.dsk", 2, "DATA.BIN", false, "START.BIN", false, 2);
}

static void test_fallback_track_without_art_leak(void **state) {
   generate_synthetic_disk("test_synth.dsk", false, false, 4);
   run_and_check("test_synth.dsk", 2, "DATA.BIN", false, "START.BIN", false, 1);
}

static void test_directory_art_preserved(void **state) {
   generate_synthetic_disk("test_synth.dsk", false, false, 5);
   run_and_check("test_synth.dsk", 1, "START.BIN", true, NULL, false, -1);
}

/* --- TESTS CMOCKA EDGE CASES --- */

static void test_edge_empty_unformatted_disk(void **state) {
   memset(&catalogue, 0, sizeof(catalogue_info_t));
   memset(&driveA, 0, sizeof(t_drive));

   generate_empty_disk("test_empty.dsk");
   dsk_load((char *)"test_empty.dsk", &driveA, 'A');

   char command[256];
   loader_run(command);

   // The emulator must not crash and should find 0 files
   assert_int_equal(catalogue.last_entry, 0);

   remove("test_empty.dsk");
}

static void test_edge_full_catalog_disk(void **state) {
   memset(&catalogue, 0, sizeof(catalogue_info_t));
   memset(&driveA, 0, sizeof(t_drive));
    
   generate_full_catalog_disk("test_full.dsk");
   dsk_load((char *)"test_full.dsk", &driveA, 'A');

   char command[256];
   loader_run(command);

   // The emulator must parse exactly 64 files without buffer overflows
   assert_int_equal(catalogue.last_entry, 64);

   remove("test_full.dsk");
}

static void test_edge_corrupt_disk_header(void **state) {
   memset(&catalogue, 0, sizeof(catalogue_info_t));
   memset(&driveA, 0, sizeof(t_drive));

   generate_corrupt_disk("test_corrupt.dsk");
   dsk_load((char *)"test_corrupt.dsk", &driveA, 'A');

   char command[256];
   loader_run(command);

   // The emulator must gracefully reject the file, not crash, and find 0 files
   assert_int_equal(catalogue.last_entry, 0);

   remove("test_corrupt.dsk");
}

/* --- CMocka bin selection test cases --- */

/* Default options initializer */
static autorun_opts_t default_opts() {
    autorun_opts_t opts = {
        .name0 = "DATA.BIN", .name1 = "START.BIN",
        .start0 = 0, .start1 = 0x4000,
        .load = 0x4000, .length = 128,
        .bad_header_idx = -1, .bad_block = false, .headerless = false,
        .system = false, .interleave = true, .hidden = false,
        .plus3_idx = -1, .bad_plus3_csum = false, .bad_signature = false,
        .empty_tracks = false
    };
    return opts;
}

static void test_prefer_entry_point(void **state) {
    autorun_opts_t opts = default_opts();
    check_autorun(opts, "START.BIN");
}

static void test_plus3_disk(void **state) {
    autorun_opts_t opts = default_opts();
    opts.name0 = "DISK."; 
    opts.name1 = "MENU.BAS";
    opts.plus3_idx = 0;
    check_autorun(opts, "MENU.BAS");
}

static void test_unknown_first_header(void **state) {
    autorun_opts_t opts = default_opts();
    opts.bad_header_idx = 0;
    check_autorun(opts, "DATA.BIN");
}


int main(void) {
   pbGPBuffer = (uint8_t*) malloc(128 * 1024 * sizeof(uint8_t)); // attempt to allocate the general purpose buffer

   const struct CMUnitTest tests[] = {
      cmocka_unit_test(basic_tests_success),
      cmocka_unit_test(cpm_tests_success),
      cmocka_unit_test(speedlock_tests_success),
      cmocka_unit_test(hexagon_tests_success),
      cmocka_unit_test(hidden_tests_success),
      cmocka_unit_test(test_system_boot_is_not_catalogue),
      cmocka_unit_test(test_hidden_loader_keeps_hidden_attribute),
      cmocka_unit_test(test_ignore_boot_filename),
      cmocka_unit_test(test_fallback_track_without_art_leak),
      cmocka_unit_test(test_directory_art_preserved),
      cmocka_unit_test(test_edge_empty_unformatted_disk),
      cmocka_unit_test(test_edge_full_catalog_disk),
      cmocka_unit_test(test_edge_corrupt_disk_header),
      cmocka_unit_test(test_prefer_entry_point),
      cmocka_unit_test(test_plus3_disk),
      cmocka_unit_test(test_unknown_first_header),
   };

   cmocka_run_group_tests(tests, NULL, NULL);
   free(pbGPBuffer);
}
