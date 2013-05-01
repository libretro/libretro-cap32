#ifndef VKBD_DEF_H
#define VKBD_DEF_H 1

#include "libretro.h"

typedef struct {
	char norml[NLETT];
	char shift[NLETT];
	int val;	
} Mvk;
#if 0
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
	{ "F0" ,"F0" ,RETROK_F10},
	{ "F11","F11" ,RETROK_F11},
	{ "F12","F12" ,RETROK_F12},

	{ " 1" ," !" , RETROK_1},//40
	{ " 2" ," \"" ,RETROK_2 },
	{ " 3" ," #"  ,RETROK_3 },
	{ " 4" ," $"  ,RETROK_4 },
	{ " 5" ," %"  ,RETROK_5 },
	{ " 6" ," &"  ,RETROK_6 },
	{ " 7" ," \'"  ,RETROK_7 },
	{ " 8" ," ("  ,RETROK_8 },
	{ " 9" ," )"  ,RETROK_9},
	{ " 0" ," _"  ,RETROK_0},

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

Mvk MVk[NPLGN*NLIGN*2]={

	{ "PG2","PG2" ,-2}, //0
	{ "Esc","Esc" ,27 },
	{ " q" ," Q"  ,113},
	{ " w" ," W"  ,119},
	{ " e" ," E"  ,101},
	{ " r" ," R"  ,114},
	{ " t" ," T"  ,116},
	{ " y" ," Y"  ,121},
	{ " u"," U"   ,117},
	{ " i"," I"   ,105},

	{ " o"," O"   ,111}, //10
	{ " p" ," P"  ,112},
	{ " a" ," A"  ,97},
	{ " s" ," S"  ,115},
	{ " d" ," D"  ,100},
	{ " f" ," F"  ,102},
	{ " g" ," G"  ,103},
	{ " h" ," H"  ,104},
	{ " j" ," J"  ,106},
	{ " k" ," K"  ,107},
	
	{ " l" ," L"  ,108},//20
	{ " z" ," Z"  ,122},
	{ " x" ," X"  ,120},
	{ " c" ," C"  ,99},
	{ " v" ," V"  ,118},
	{ " b" ," B"  ,98},
	{ " n" ," N"  ,110},
	{ " m"," M"   ,109},
	{ "F1" ,"F1"  ,282},
	{ "F2" ,"F2"  ,283},

	{ "F3" ,"F3"  ,284},//30
	{ "F4" ,"F4"  ,285},
	{ "F5" ,"F5"  ,286},
	{ "F6" ,"F6"  ,287},
	{ "F7" ,"F7"  ,288},
	{ "F8" ,"F8"  ,289},	
	{ "F9" ,"F9"  ,290},
	{ "F10","F10" ,291},
	{ "F11","F11" ,292},
	{ "F12","F12" ,283},

	{ " 1" ," !" , 49 },//40
	{ " 2" ," \"" ,50 },
	{ " 3" ," #"  ,51 },
	{ " 4" ," $"  ,52 },
	{ " 5" ," %"  ,53 },
	{ " 6" ," &"  ,54 },
	{ " 7" ," \'"  ,55 },
	{ " 8" ," ("  ,56 },
	{ " 9" ," )"  ,57},
	{ " 0" ," _"  ,48},

	{ "PG2","PG2" ,-2}, //50
	{ " `" ," ` " ,96}, 
	{ " -" ," =" , 45},
	{ " =" ," +" , 61},
	{ "Bks","Bks" , 8},
	{ "Scr","Scr" ,316},
	{ "Cpy","Cpy" ,315},
	{ "Ins","Ins" ,277},
	{ "Hme","Hme" ,278},
	{ "PgU","PgU" ,280},

	{ "Tab" ,"Tab", 9}, //60	
	{ " [" ,"  {" ,91},
	{ " ]" ,"  }" ,93},
	{ "Ent" ,"Ent",13},
	{ "Del" ,"Del",127},
	{ "End" ,"End",279},
	{ "PgD" ,"PgD",281},
	{ "SHF" ,"SHF",304},
	{ " \\" ," |" ,92},	
	{ " Dw" ," Dw",274},	
	
	{ "Cps","Cps" ,301}, //70
	{ " ;"," +"   ,59},
	{ " '"," @"   ,39},
	{ " #"," ~"   ,35},
	{ "Ctr" ,"Ctr",305},
	{ "Alt" ,"Alt",307},
	{ "Spc" ,"Spc",32},
	{ "Lft"," 4"  ,276},
	{ " Up"," 8"  ,273},
	{ "Rgt" ," 6" ,275},

	{ " ,"," <"   ,44},//80
	{ " ."," >"   ,46},
	{ " /"," ?"   ,47},
	{ " |" ," ^"  ,54},
	{ " Num"," Num",300},
	{ " /" ," /"  ,47},
	{ " *" ," *"  ,42},
	{ " -" ," -"  ,45},
	{ " Hme" ," 7",278},
	{ " ^"," Y"   ,94},

	{ " PgU"," U" ,280},//90
	{ " +"," I"   ,43},
	{ " 5" ," 5"  ,53},
	{ " 6" ,"Rgt" ,54},
	{ " 1" ,"End" ,49},
	{ "EXT","EXT" ,-6},
	{ "SNA" ,"SNA",-7},
	{ "DSK","DSK",-5},//switch DSK A/B
	{ "Col" ,"Col",-3},
	{ "Kbd" ,"Kbd",-4},

} ;

#endif
