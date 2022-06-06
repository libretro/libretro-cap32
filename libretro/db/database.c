#include <libretro-core.h>
#include <retro_events.h>

#include "db-clean.h"
#include "db-fail.h"
#include "entries.h"

// #define DEBUG_DATABASE

extern t_button_cfg btnPAD[MAX_PADCFG];

static unsigned char get_keybind (unsigned int key, unsigned char position)
{
   switch (key)
   {
      case DB_CLEAN:
         #ifdef DEBUG_DATABASE
         printf("[keybind-clean] k:%u p:%u = %u\n", key, position, btnPAD[0].buttons[position]);
         #endif
         return 0xff;

      case DB_KEEP:
         #ifdef DEBUG_DATABASE
         printf("[keybind-keep] k:%u p:%u = %u\n", key, position, btnPAD[0].buttons[position]);
         #endif
         return btnPAD[0].buttons[position];

      default:
         #ifdef DEBUG_DATABASE
         printf("[keybind-cfg] k:%u p:%u = %u\n", key, position, get_cpckey(key));
         #endif
         return get_cpckey(key);
   }
}

static const char * string_find(const char * data, size_t data_leng, unsigned int position)
{
   for(size_t n = 0; n < (data_leng - 1); n++)
   {
      if (!position)
         return &data[n];

      if (data[n] == '\0')
         position --;
   }

   return NULL;
}

static void command_add(unsigned int command_position)
{
   const char * command = string_find(commands, sizeof(commands), command_position);

   if (!command || strlen(command) > LOADER_MAX_SIZE)
      return;

   strcpy(game_configuration.loader_command, command);
   game_configuration.has_command = true;
}

static void database_entry(t_file_entry * entry)
{
   if (entry->btn_map[0])
   {
      for (int n = 0; n < MAX_BUTTONS; n++)
      {
         game_configuration.btn_config.buttons[n] = get_keybind(entry->btn_map[n], n);
      }
      game_configuration.has_btn = true;
   }

   if (entry->loader_command != COMMAND_EMPTY)
   {
      command_add(entry->loader_command);
   }
}

bool db_info(const uint32_t hash)
{
   uint32_t db_size = sizeof(dbinfo) / sizeof(t_file_entry);
   for (int n = 0; n < db_size; n++)
   {
      for(int i = 0; dbinfo[n].hash_list[i]; i++)
      {
         if (dbinfo[n].hash_list[i] == hash)
         {
            database_entry(&dbinfo[n]);
            #ifdef _UNITTEST_DEBUG
            printf("[  ENTRY!  ] --- crc: 0x%x cmd: %i\n", hash, game_configuration.has_command);
            #endif
            return true;
         }
      }
   }
   return false;
}

bool db_fail(const uint32_t hash)
{
   uint32_t db_size = sizeof(dbfail) / sizeof(uint32_t);
   for (int n = 0; n < db_size; n++)
   {
      if(dbfail[n] == hash)
      {
         return true;
      }
   }

   return false;
}

bool db_clean(const uint32_t hash)
{
   uint32_t db_size = sizeof(dbclean) / sizeof(uint32_t);
   for (int n = 0; n < db_size; n++)
   {
      if(dbclean[n] == hash)
      {
         return true;
      }
   }

   return false;
}
