#ifndef VKBD_DEF_H
#define VKBD_DEF_H 1

#include "libretro.h"

typedef struct {
	char norml[NLETT];
	char shift[NLETT];
	int val;	
} Mvk;

Mvk MVk[NPLGN*NLIGN*2]={

	{ "PG2","PG2" ,-2}, //0
	{ "Esc","Esc" ,RETROK_ESCAPE },
	{ " q" ," Q"  ,RETROK_q},
	{ " w" ," W"  ,RETROK_w},
	{ " e" ," E"  ,RETROK_e},
	{ " r" ," R"  ,RETROK_r},
	{ " t" ," T"  ,RETROK_t},
	{ " y" ," Y"  ,RETROK_y},
	{ " u"," U"   ,RETROK_u},
	{ " i"," I"   ,RETROK_i},

	{ " o"," O"   ,RETROK_o}, //10
	{ " p" ," P"  ,RETROK_p},
	{ " a" ," A"  ,RETROK_a},
	{ " s" ," S"  ,RETROK_s},
	{ " d" ," D"  ,RETROK_d},
	{ " f" ," F"  ,RETROK_f},
	{ " g" ," G"  ,RETROK_g},
	{ " h" ," H"  ,RETROK_h},
	{ " j" ," J"  ,RETROK_j},
	{ " k" ," K"  ,RETROK_k},
	
	{ " l" ," L"  ,RETROK_l},//20
	{ " z" ," Z"  ,RETROK_z},
	{ " x" ," X"  ,RETROK_x},
	{ " c" ," C"  ,RETROK_c},
	{ " v" ," V"  ,RETROK_v},
	{ " b" ," B"  ,RETROK_b},
	{ " n" ," N"  ,RETROK_n},
	{ " m"," M"   ,RETROK_m},
	{ "F1" ,"F1"  ,RETROK_F1},
	{ "F2" ,"F2"  ,RETROK_F2},

	{ "F3" ,"F3"  ,RETROK_F3},//30
	{ "F4" ,"F4"  ,RETROK_F4},
	{ "F5" ,"F5"  ,RETROK_F5},
	{ "F6" ,"F6"  ,RETROK_F6},
	{ "F7" ,"F7"  ,RETROK_F7},
	{ "F8" ,"F8"  ,RETROK_F8},	
	{ "F9" ,"F9"  ,RETROK_F9},
	{ "F10","F10" ,RETROK_F10},
	{ "F11","F11" ,RETROK_F11},
	{ "F12","F12" ,RETROK_F12},

	{ " 1" ," !" , RETROK_1},//40
	{ " 2" ," \"" ,RETROK_2 },
	{ " 3" ," 3"  ,RETROK_3 },
	{ " 4" ," $"  ,RETROK_4 },
	{ " 5" ," %"  ,RETROK_5 },
	{ " 6" ," ^"  ,RETROK_6 },
	{ " 7" ," &"  ,RETROK_7 },
	{ " 8" ," *"  ,RETROK_8 },
	{ " 9" ," ("  ,RETROK_9},
	{ " 0" ," )"  ,RETROK_0},

	{ "PG2","PG2" ,-2}, //50
	{ " `" ," ` " ,-1}, 
	{ " -" ," _" , RETROK_MINUS},
	{ " =" ," +" , RETROK_EQUALS},
	{ "Bks","Bks" ,RETROK_BACKSPACE},
	{ "Scr","Scr" ,RETROK_PRINT},
	{ "Pse","Pse" ,RETROK_PAUSE},
	{ "Ins","Ins" ,RETROK_HELP},
	{ "Hme","Hme" ,RETROK_HOME},
	{ "PgU","PgU" ,RETROK_PAGEUP},

	{ "Tab" ,"Tab",RETROK_TAB}, //60	
	{ " [" ,"  {" ,RETROK_LEFTBRACKET},
	{ " ]" ,"  }" ,RETROK_RIGHTBRACKET},
	{ "Ent" ,"Ent",RETROK_RETURN},
	{ "Del" ,"Del",RETROK_DELETE},
	{ "End" ,"End",RETROK_END},
	{ "PgD" ,"PgD",RETROK_PAGEDOWN},
	{ "SHF" ,"SHF",RETROK_LSHIFT},
	{ " \\" ," |" ,RETROK_BACKSLASH},	
	{ " Dw" ," Dw",RETROK_DOWN},	
	
	{ "Cps","Cps" ,RETROK_CAPSLOCK}, //70
	{ " ;"," :"   ,RETROK_SEMICOLON},
	{ " '"," @"   ,RETROK_QUOTE},
	{ " #"," ~"   ,43},
	{ "Ctr" ,"Ctr",RETROK_LCTRL},
	{ "Alt" ,"Alt",RETROK_LALT},
	{ "Spc" ,"Spc",RETROK_SPACE},
	{ "Lft"," 4"  ,RETROK_LEFT},
	{ " Up"," 8"  ,RETROK_UP},
	{ "Rgt" ," 6" ,RETROK_RIGHT},

	{ " ,"," <"   ,RETROK_COMMA},//80
	{ " ."," >"   ,RETROK_PERIOD},
	{ " /"," ?"   ,RETROK_SLASH},
	{ " |" ," ^"  ,54},
	{ " Num"," Num",69},
	{ " /" ," /"  ,RETROK_KP_DIVIDE},
	{ " *" ," *"  ,RETROK_KP_MULTIPLY},
	{ " -" ," -"  ,RETROK_KP_MINUS},
	{ " Hme" ," 7",RETROK_HOME},
	{ " ^"," Y"   ,RETROK_CARET},

	{ " PgU"," U" ,RETROK_PAGEUP},//90
	{ " +"," I"   ,RETROK_PLUS},
	{ " 5" ," 5"  ,RETROK_5},
	{ " 6" ,"Rgt" ,RETROK_6},
	{ " 1" ,"End" ,RETROK_1},
	{ "EXT","EXT" ,-6},
	{ "SNA" ,"SNA",-7},
	{ "DSK","DSK",-5},//switch DSK A/B
	{ "Col" ,"Col",-3},
	{ "Kbd" ,"Kbd",-4},

} ;

#endif
