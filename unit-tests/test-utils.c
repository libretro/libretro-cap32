#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"
#include "libretro-common/include/utils/md5.h"

#include "cap32/slots.h"
#include "libretro-core.h"
#include "libretro/dsk/format.h"
#include "libretro/dsk/loader.h"
#include "libretro/dsk/amsdos_catalog.h"
#include "libretro/db/database.h"
#include "test-utils.h"

//#define TEST_DEBUG

extern t_drive driveA;

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
   char newstr[3];
   memcpy(newstr, str, 2);
   newstr[2] = '\0';
   return strtol(newstr, NULL, 16);
}

void test_cmd(char * cmd, char * hash)
{
   uint8_t output[16];
   MD5_CTX ctx;

   MD5_Init(&ctx);
   MD5_Update(&ctx, cmd, strlen(cmd));
   MD5_Final(output, &ctx);

#ifdef TEST_DEBUG
   printf("MD5: ");

   for (int o = 0, c = 0; o < 16; o++, c+=2)
      printf("%02x", output[o]);

   printf("\n");
#endif

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

   #ifdef TEST_DEBUG
   printf("DSK: %s\n", filename_dsk);
   #endif

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

/*
 * CPC catalog tests
 */
/**
 * generate_synthetic_disk:
 * Generates a synthetic Amstrad CPC disk image (.dsk) on the filesystem 
 * to test specific catalog parsing behaviors and edge cases.
 * 
 * @param filepath   Destination path for the generated .dsk file.
 * @param is_system  If true, creates a System format disk (directory on track 2). 
 *                   If false, creates a Data format disk (directory on track 0).
 * @param is_hidden  If true, applies the hidden attribute (bit 7) to the second file entry.
 * @param tweak_case Defines specific edge-case scenarios:
 *                   1: Standard test (no special tweaks).
 *                   2: Standard test (no special tweaks).
 *                   3: Injects a fake directory entry ("FAKE    BAS") in a boot sector.
 *                   4: Leaves normal directory track empty and places a directory on track 1.
 *                   5: Removes the first file entry ("DATA    BIN"), leaving only "START.BIN".
 */
void generate_synthetic_disk(const char *filepath, bool is_system, bool is_hidden, int tweak_case) {
   uint8_t directory[2048];
   memset(directory, 0xe5, sizeof(directory));

   // Entry 0: DATA    BIN
   uint8_t entry0[32] = {0};
   memcpy(&entry0[1], "DATA    BIN", 11);
   entry0[15] = 1; entry0[16] = 2; 
   memcpy(&directory[0], entry0, 32);

   // Entry 1: START   BIN
   uint8_t entry1[32] = {0};
   memcpy(&entry1[1], "START   BIN", 11);
   entry1[15] = 1; entry1[16] = 3;
   if (is_hidden)
   {
      entry1[10] |= 128; // set hidden bit
   }
   memcpy(&directory[32], entry1, 32);

   // Disk header
   uint8_t dsk_header[256] = {0};
   memcpy(dsk_header, "MV - CPC", 8);
   dsk_header[48] = 40; dsk_header[49] = 1; 
   dsk_header[50] = (4864 & 0xFF); dsk_header[51] = (4864 >> 8);

   FILE *f = fopen(filepath, "wb");
   fwrite(dsk_header, 1, 256, f);

   for (int track = 0; track < 40; track++)
   {
      uint8_t track_header[256] = {0};
      memcpy(track_header, "Track-Info", 10);
      track_header[16] = track;
      track_header[20] = 2; track_header[21] = 9;

      for (int sector = 0; sector < 9; sector++)
      {
         int offset = 24 + sector * 8;
         track_header[offset] = track;
         track_header[offset + 1] = 0;
         track_header[offset + 2] = (is_system ? 0x41 : 0xc1) + sector;
         track_header[offset + 3] = 2;
      }
      fwrite(track_header, 1, 256, f);

      uint8_t data[4608] = {0};
      int dir_track = is_system ? 2 : 0;

      if (track == dir_track)
      {
         memcpy(data, directory, 2048);
      }

      /* Apply specific tweaks based on test case requirements */
      if (tweak_case == 3 && track == 0)
      {
         uint8_t fake[32] = {0};
         memcpy(&fake[1], "FAKE    BAS", 11);
         fake[15] = 1; fake[16] = 2;
         memcpy(&data[0], fake, 32); 
      }
      else if (tweak_case == 4)
      {
         // clear normal directory
         if (track == dir_track)
            memset(data, 0, 4608);

         // move it to track 1
         if (track == 1)
            memcpy(data, directory, 2048);

      }
      else if (tweak_case == 5 && track == dir_track)
      {
         memset(data, 0, 32); // erase first entry (DATA BIN)
      }

      fwrite(data, 1, 4608, f);
   }

   fclose(f);
}

/**
 * run_and_check:
 * Loads a disk image into drive A, triggers the autorun loader, and 
 * validates the resulting AMSDOS catalog state using CMocka assertions.
 * 
 * @param filepath       Path to the .dsk image to load.
 * @param expected_count Expected number of valid files found in the catalog.
 * @param name0          Expected filename for the first catalog entry (can be NULL).
 * @param hide0          Expected hidden status for the first catalog entry.
 * @param name1          Expected filename for the second catalog entry (can be NULL).
 * @param hide1          Expected hidden status for the second catalog entry.
 * @param expected_track Expected track number where the listed catalog was found (-1 to ignore).
 */
void run_and_check(const char *filepath, int expected_count, const char* name0, bool hide0, const char* name1, bool hide1, int expected_track) {
   /* Clean global state to prevent test contamination */
   memset(&catalogue, 0, sizeof(catalogue_info_t));
   memset(&driveA, 0, sizeof(t_drive));

   int res = dsk_load((char *) filepath, &driveA, 'A');
   assert_int_equal(res, 0);

   char command[256];
   loader_run(command);

   assert_int_equal(catalogue.last_entry, expected_count);

   if (expected_count > 0 && name0 != NULL)
   {
      assert_string_equal(catalogue.dirent[0].filename, name0);
      assert_int_equal(catalogue.dirent[0].is_hidden, hide0);
   }

   if (expected_count > 1 && name1 != NULL)
   {
      assert_string_equal(catalogue.dirent[1].filename, name1);
      assert_int_equal(catalogue.dirent[1].is_hidden, hide1);
   }

   if (expected_track >= 0)
   {
      assert_int_equal(catalogue.track_listed_id, expected_track);
   }
}

/**
 * generate_empty_disk:
 * Generates a structurally valid DSK file header but with completely empty tracks,
 * simulating an unformatted or blank disk.
 * 
 * @param filepath Destination path for the generated .dsk file.
 */
void generate_empty_disk(const char *filepath) {
   uint8_t dsk_header[256] = {0};
   memcpy(dsk_header, "MV - CPC", 8);
   dsk_header[48] = 40; dsk_header[49] = 1; 

   FILE *f = fopen(filepath, "wb");
   fwrite(dsk_header, 1, 256, f);

   for (int track = 0; track < 40; track++) {
      uint8_t track_header[256] = {0};
      memcpy(track_header, "Track-Info", 10);
      track_header[16] = track;
      // 0 sectors defined for this track
      fwrite(track_header, 1, 256, f);
      // No sector data written
   }

   fclose(f);
}

/**
 * generate_full_catalog_disk:
 * Generates a Data format disk where the directory track (track 0) 
 * contains the maximum allowed AMSDOS entries (64 files).
 * 
 * @param filepath Destination path for the generated .dsk file.
 */
void generate_full_catalog_disk(const char *filepath) {
   uint8_t directory[2048] = {0};

   // Fill all 64 entries (64 * 32 bytes = 2048 bytes)
   for (int i = 0; i < 64; i++) {
      uint8_t entry[32] = {0};
      entry[0] = 0x00; // User 0 (Valid entry)

      char filename[12];
      snprintf(filename, sizeof(filename), "F%02d     BIN", i);
      memcpy(&entry[1], filename, 11);

      entry[15] = 1; entry[16] = 2; // Dummy metadata
      memcpy(&directory[i * 32], entry, 32);
   }

   uint8_t dsk_header[256] = {0};
   memcpy(dsk_header, "MV - CPC", 8);
   dsk_header[48] = 40; dsk_header[49] = 1; 
   dsk_header[50] = (4864 & 0xFF); dsk_header[51] = (4864 >> 8);

   FILE *f = fopen(filepath, "wb");
   fwrite(dsk_header, 1, 256, f);

   for (int track = 0; track < 40; track++) {
      uint8_t track_header[256] = {0};
      memcpy(track_header, "Track-Info", 10);
      track_header[16] = track;
      track_header[20] = 2; track_header[21] = 9;

      for (int sector = 0; sector < 9; sector++) {
         int offset = 24 + sector * 8;
         track_header[offset] = track;
         track_header[offset + 1] = 0;
         track_header[offset + 2] = 0xc1 + sector; // Data format
         track_header[offset + 3] = 2;
      }
      fwrite(track_header, 1, 256, f);

      uint8_t data[4608] = {0};
      if (track == 0) {
         // Inject the 64-file directory into track 0
         memcpy(data, directory, 2048);
      }
      fwrite(data, 1, 4608, f);
   }

   fclose(f);
}

/**
 * generate_corrupt_disk:
 * Generates a file with random garbage and an invalid header to test 
 * parser resilience (fuzzing) against malformed or non-DSK files.
 * 
 * @param filepath Destination path for the generated file.
 */
void generate_corrupt_disk(const char *filepath) {
   FILE *f = fopen(filepath, "wb");
   uint8_t garbage[1024];

   for (int i = 0; i < 1024; i++) {
      garbage[i] = rand() % 256;
   }

   fwrite(garbage, 1, 1024, f);
   fclose(f);
}

/*
 * BIN selection
 */

/* Disk generator matching the Python logic */
void generate_autorun_disk(const char *filepath, autorun_opts_t opts) {
    uint8_t logical[40 * 9 * 512];
    memset(logical, 0, sizeof(logical));
    memset(logical, 0xe5, 2048); // Directory area

    const char* names[] = {opts.name0, opts.name1};
    uint16_t starts[] = {opts.start0, opts.start1};

    // We only process up to 2 files based on Python logic
    for (int i = 0; i < 2; i++) {
        if (!names[i]) continue;

        // Parse "NAME.EXT"
        char stem[9] = "        ";
        char ext[4] = "   ";
        const char *dot = strchr(names[i], '.');
        if (dot) {
            int stem_len = dot - names[i];
            memcpy(stem, names[i], stem_len > 8 ? 8 : stem_len);
            memcpy(ext, dot + 1, strlen(dot + 1) > 3 ? 3 : strlen(dot + 1));
        }

        // Create Directory Entry
        uint8_t entry[32] = {0};
        memcpy(&entry[1], stem, 8);
        memcpy(&entry[9], ext, 3);
        entry[15] = 2; entry[16] = i + 2;
        
        if (opts.bad_block && i == 0) entry[16] = 255;
        if (opts.hidden && i == 1) entry[10] |= 128;
        
        memcpy(&logical[i * 32], entry, 32);

        // Create AMSDOS Header
        uint8_t h[128] = {0};
        memcpy(&h[1], stem, 8);
        memcpy(&h[9], ext, 3);
        h[18] = 2;
        h[21] = opts.load & 0xff; h[22] = opts.load >> 8;
        h[24] = opts.length & 0xff; h[25] = opts.length >> 8;
        h[26] = starts[i] & 0xff; h[27] = starts[i] >> 8;
        
        uint16_t csum = 0;
        for (int j = 0; j < 67; j++) csum += h[j];
        h[67] = csum & 0xff; h[68] = csum >> 8;

        if (opts.bad_header_idx == i) h[67] ^= 1;
        if (opts.headerless && i == 0) memset(h, 0, 128);

        // PLUS3DOS injection
        if (opts.plus3_idx == i) {
            memset(h, 0, 128);
            memcpy(h, "PLUS3DOS\x1a", 9);
            h[9] = 1;
            h[11] = 0; h[12] = 1; // 256 bytes
            if (opts.bad_signature) h[8] = 0;
            
            uint16_t p3_csum = 0;
            for (int j = 0; j < 127; j++) p3_csum += h[j];
            h[127] = p3_csum & 0xff;
            
            if (opts.bad_plus3_csum) h[127] ^= 1;
        }

        // Copy header to logical track memory
        memcpy(&logical[(i + 2) * 1024], h, 128);
    }

    // Build physical DSK
    FILE *f = fopen(filepath, "wb");
    uint8_t result[256] = {0};
    memcpy(result, "MV - CPC", 8);
    
    if (opts.empty_tracks) {
        memcpy(result, "EXTENDED CPC DSK File\r\nDisk-Info\r\n", 34);
        result[48] = 99; // Tracks
    } else {
        result[48] = 40; 
    }
    result[49] = 1; 
    result[50] = 4864 & 0xff; result[51] = 4864 >> 8;
    
    if (opts.empty_tracks) {
        memset(&result[52], 19, 40); // Track sizes table
    }
    fwrite(result, 1, 256, f);

    int order[] = {0, 5, 1, 6, 2, 7, 3, 8, 4};
    int seq[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int *sectors = opts.interleave ? order : seq;

    for (int t = 0; t < 40; t++) {
        uint8_t h_track[256] = {0};
        memcpy(h_track, "Track-Info", 10);
        h_track[16] = t;
        h_track[20] = 2; h_track[21] = 9;
        
        for (int s = 0; s < 9; s++) {
            int offset = 24 + s * 8;
            h_track[offset] = t;
            h_track[offset + 2] = (opts.system ? 0x41 : 0xc1) + sectors[s];
            h_track[offset + 3] = 2;
        }
        fwrite(h_track, 1, 256, f);

        for (int s = 0; s < 9; s++) {
            int source = (t - (opts.system ? 2 : 0)) * 9 + sectors[s];
            if (source >= 0 && source < (40 * 9)) {
                fwrite(&logical[source * 512], 1, 512, f);
            } else {
                uint8_t empty[512] = {0};
                fwrite(empty, 1, 512, f);
            }
        }
    }
    
    // Fix extended disk sizes if needed
    if (opts.empty_tracks) {
        fseek(f, 256, SEEK_SET);
        for (int t = 0; t < 40; t++) {
            for (int s = 0; s < 9; s++) {
                long offset = 256 + (t * 4864) + 24 + (s * 8) + 6;
                fseek(f, offset, SEEK_SET);
                uint8_t size[2] = {0, 2}; // 512
                fwrite(size, 1, 2, f);
            }
        }
    }
    fclose(f);
}

/* Helper to execute and check autorun commands */
void check_autorun(autorun_opts_t opts, const char* expected_filename) {
    const char* filepath = "test_autorun.dsk";
    generate_autorun_disk(filepath, opts);
    
    memset(&catalogue, 0, sizeof(catalogue_info_t));
    memset(&driveA, 0, sizeof(t_drive));

    dsk_load((char *)filepath, &driveA, 'A');
    
    char command[256] = {0};
    loader_run(command);
    
    char expected_command[256];
    snprintf(expected_command, sizeof(expected_command), "RUN\"%s", expected_filename);
    
    assert_string_equal(command, expected_command);
    remove(filepath);
}