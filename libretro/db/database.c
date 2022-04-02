#include <libretro-core.h>
#include <retro_events.h>

#include "entries.h"

extern t_button_cfg btnPAD[MAX_PADCFG];

unsigned char get_keybind (unsigned int key, unsigned char position)
{
   switch (key)
   {
      case DB_CLEAN:
         return 0xff;

      case DB_KEEP:
         return btnPAD[0].buttons[position];

      default:
         return get_cpckey(key);
   }
}

void database_entry(t_file_entry * entry)
{
   if (entry->btn_map[0])
   {
      for (int n = 0; n < MAX_BUTTONS; n++)
      {
         game_configuration.btn_config.buttons[n] = get_keybind(entry->btn_map[n], n);
      }
      game_configuration.has_btn = true;
   }

   if (entry->loader_command[0])
   {
      strncpy(game_configuration.loader_command, entry->loader_command, LOADER_MAX_SIZE);
      game_configuration.has_command = true;
   }
}

bool get_database(const uint32_t hash)
{
   uint32_t db_size = sizeof(database) / sizeof(t_file_entry);
   for (int n = 0; n < db_size; n++)
   {
      for(int i = 0; database[n].hash_list[i]; i++)
      {
         if (database[n].hash_list[i] == hash)
         {
            database_entry(&database[n]);
            return true;
         }
      }
   }

   return false;
}
