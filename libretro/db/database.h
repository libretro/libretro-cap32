#include <libretro-core.h>
#include <retro_events.h>

#define DB_CLEAN 0xfff
#define DB_KEEP 0xffff
#define COMMAND_EMPTY -1

typedef struct {
   uint32_t hash_list[16];
   int btn_map[MAX_BUTTONS];
   int btn_name[MAX_BUTTONS];
   int loader_command;
} t_file_entry;

bool db_info(const uint32_t hash);
bool db_fail(const uint32_t hash);
bool db_clean(const uint32_t hash);
