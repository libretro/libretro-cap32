#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "../cmocka/include/cmocka.h"
#include "libretro-common/include/utils/md5.h"

#include "cap32/slots.h"
#include "libretro-core.h"
#include "libretro/dsk/format.h"
#include "libretro/db/database.h"
#include "test-utils.h"

void test_loader(t_drive * drive, char * format_expected, char * loader_buffer)
{
   retro_format_info_t* test_format = NULL;
   test_format = format_get(drive);

   _loader_run(loader_buffer, test_format, drive);
   if (strlen(format_expected))
      assert_string_equal(test_format->label, format_expected);
}

int hextoi(char * str)
{
   char newstr[4];
   memcpy(newstr, str, 2);
   newstr[3]='\0';
   return strtol(newstr, NULL, 16);
}

void test_cmd(char * cmd, char * hash)
{
   uint8_t output[16];
   MD5_CTX ctx;

   MD5_Init(&ctx);
   MD5_Update(&ctx, cmd, strlen(cmd));
   MD5_Final(output, &ctx);
   for (int o = 0, c = 0; o < 16; o++, c+=2)
   {
      unsigned short n = hextoi(&hash[c]);
      assert_int_equal(output[o], n);
   }
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

   if (db_fail(hash))
      return 0;

   int result = load_dsk(&drive, file_path);
   memset(&game_configuration, 0, sizeof(game_cfg_t));

   if (file_check_flag(file_path, strlen(file_path), FLAG_BIOS_CPM, 5))
   {
      game_configuration.is_cpm = true;
   }

   // get database info
   db_info(hash);
   if (game_configuration.has_command) {
      strncpy(loader_buffer, game_configuration.loader_command, LOADER_MAX_SIZE);
   } else {
      test_loader(&drive, "", loader_buffer);
   }

   test_cmd(loader_buffer, result_string);

   return result;
}
