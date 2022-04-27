#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "../cmocka/include/cmocka.h"

#include "cap32/slots.h"
#include "libretro-core.h"
#include "libretro/dsk/formats.h"
#include "libretro/db/database.h"
#include "test-utils.h"

void test_loader(t_drive * drive, char * format_expected, char * loader_buffer)
{
   DPB_type *dpb = NULL;
   dpb = format_find(drive);
   _loader_run(loader_buffer, dpb, drive);
   if (strlen(format_expected))
      assert_string_equal(dpb->label, format_expected);
}

int load_dsk(t_drive * drive, char * filename_dsk)
{
   memset(drive, 0, sizeof(t_drive)); // clear disk
   memset(pbGPBuffer, 0, 128 * 1024 * sizeof(uint8_t)); // clear cache

   int result = dsk_load(filename_dsk, drive, 'A');
   assert_int_equal(result, 0);

   return result;
}

int test_dsk(char * file_path, char * result_string, char * format_expected)
{
   t_drive drive;
   char loader_buffer[LOADER_MAX_SIZE];

   int result = load_dsk(&drive, file_path);
   test_loader(&drive, format_expected, loader_buffer);

   assert_string_equal(loader_buffer, result_string);
   return result;
}

int test_dsk_hashed(char * file_path, char * result_string, uint32_t file_hash)
{
   t_drive drive;
   char loader_buffer[LOADER_MAX_SIZE];

   // check crc32
   uint32_t hash = get_hash(file_path);
   assert_int_equal(hash, file_hash);

   int result = load_dsk(&drive, file_path);

   // get database info
   get_database(hash);
   if (game_configuration.has_command) {
      strncpy(loader_buffer, game_configuration.loader_command, LOADER_MAX_SIZE);
   } else {
      test_loader(&drive, "", loader_buffer);
   }

   assert_string_equal(loader_buffer, result_string);

   return result;
}
