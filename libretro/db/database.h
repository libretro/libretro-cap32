#include <libretro-core.h>
#include <retro_events.h>

#define DB_CLEAN 0xfff
#define DB_KEEP 0xffff
#define COMMAND_EMPTY 0

typedef struct {
   int btn_map[MAX_BUTTONS];
   int btn_name[MAX_BUTTONS];
} t_player_btn;

typedef struct {
   uint32_t hash_list[16];
   t_player_btn player_1;
   t_player_btn player_2;
   int loader_command;
} t_file_entry;

bool db_info(const uint32_t hash);
bool db_fail(const uint32_t hash);
bool db_clean(const uint32_t hash);
