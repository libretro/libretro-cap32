#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "../cmocka/include/cmocka.h"

#include "cap32/slots.h"
#include "libretro-core.h"
#include "libretro/dsk/formats.h"
#include "test-utils.h"

void test_loader(t_drive * drive, char * format_expected, char * loader_buffer)
{
   DPB_type *dpb = NULL;
   dpb = format_find(drive);
   _loader_run(loader_buffer, dpb, drive);
   assert_string_equal(dpb->label, format_expected);
}

int test_dsk(char * filename_dsk, char * result_string, char * format_expected)
{
   t_drive drive;
   char loader_buffer[LOADER_MAX_SIZE];

   memset(&drive, 0, sizeof(t_drive)); // clear disk
   memset(pbGPBuffer, 0, 128 * 1024 * sizeof(uint8_t)); // clear cache

   int result = dsk_load(filename_dsk, &drive, 'A');
   assert_int_equal(result, 0);
   test_loader(&drive, format_expected, loader_buffer);
   assert_string_equal(loader_buffer, result_string);
   return result;
}
