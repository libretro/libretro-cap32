#ifndef VKBD_DEF_H
#define VKBD_DEF_H 1

typedef struct {
	char norml[NLETT];
	char shift[NLETT];
	int val;	
} Mvk;

#if 1
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
	{ " #"," ~"   ,RETROK_HASH},
	{ "Ctr" ,"Ctr",RETROK_LCTRL},
	{ "Alt" ,"Alt",RETROK_LALT},
	{ "Spc" ,"Spc",RETROK_SPACE},
	{ "Lft"," 4"  ,RETROK_LEFT},
	{ " Up"," 8"  ,RETROK_UP},
	{ "Rgt" ," 6" ,RETROK_RIGHT},

	{ " ,"," <"   ,RETROK_COMMA},//80
	{ " ."," >"   ,RETROK_PERIOD},
	{ " /"," ?"   ,RETROK_SLASH},
	{ " |" ," ^"  ,RETROK_BACKSLASH|(3 << 16)},
	{ " Num"," Num",RETROK_NUMLOCK},
	{ " /" ," /"  ,RETROK_KP_DIVIDE},
	{ " *" ," *"  ,RETROK_KP_MULTIPLY},
	{ " -" ," -"  ,RETROK_KP_MINUS},
	{ " Hme" ," 7",RETROK_HOME},
	{ " ^"," Y"   ,RETROK_CARET},

	{ " PgU"," U" ,RETROK_PAGEUP},//90
	{ " +"," I"   ,RETROK_PLUS},
	{ " 5" ," 5"  ,RETROK_5},
	{ " 6" ,"Rgt" ,RETROK_6},
	{ "P/S" ,"P/S" ,-8},
	{ "EXT","EXT" ,-6},
	{ "SNA" ,"SNA",-7},
	{ "DSK","DSK",-5},//switch DSK A/B
	{ "Col" ,"Col",-3},
	{ "Kbd" ,"Kbd",-4},
} ;
#else

Mvk MVk[NPLGN*NLIGN*2]={

	{ " 1" ," !" , RETROK_1 },//0
	{ " 2" ," \"" ,RETROK_2 },
	{ " 3" ," 3"  ,RETROK_3 },
	{ " 4" ," $"  ,RETROK_4 },
	{ " 5" ," %"  ,RETROK_5 },
	{ " 6" ," ^"  ,RETROK_6 },
	{ " 7" ," &"  ,RETROK_7 },
	{ " 8" ," *"  ,RETROK_8 },
	{ " 9" ," ("  ,RETROK_9 },
	{ " 0" ," )"  ,RETROK_0 },

	{ " q" ," Q"  ,RETROK_q}, //10
	{ " w" ," W"  ,RETROK_w},
	{ " e" ," E"  ,RETROK_e},
	{ " r" ," R"  ,RETROK_r},
	{ " t" ," T"  ,RETROK_t},
	{ " y" ," Y"  ,RETROK_y},
	{ " u" ," U"  ,RETROK_u},
	{ " i" ," I"  ,RETROK_i},
	{ " o" ," O"  ,RETROK_o},
	{ " p" ," P"  ,RETROK_p},

	{ " a" ," A"  ,RETROK_a}, //20
	{ " s" ," S"  ,RETROK_s},
	{ " d" ," D"  ,RETROK_d},
	{ " f" ," F"  ,RETROK_f},
	{ " g" ," G"  ,RETROK_g},
	{ " h" ," H"  ,RETROK_h},
	{ " j" ," J"  ,RETROK_j},
	{ " k" ," K"  ,RETROK_k},	
	{ " l" ," L"  ,RETROK_l},
	{ " ;" ," :"  ,RETROK_SEMICOLON},

	{ " z" ," Z"  ,RETROK_z},//30
	{ " x" ," X"  ,RETROK_x},
	{ " c" ," C"  ,RETROK_c},
	{ " v" ," V"  ,RETROK_v},
	{ " b" ," B"  ,RETROK_b},
	{ " n" ," N"  ,RETROK_n},
	{ " m"," M"   ,RETROK_m},
	{ " ,"," <"   ,RETROK_COMMA},
	{ " ."," >"   ,RETROK_COLON},
	{ " /" ," /"  ,RETROK_SLASH},

	{ "PG2","PG2" ,-2}, //40
	{ "Esc","Esc" ,RETROK_ESCAPE},
	{ "F1" ,"F1"  ,RETROK_F1},
	{ "F3" ,"F3"  ,RETROK_F3},
	{ "F5" ,"F5"  ,RETROK_F5},
	{ "F7" ,"F7"  ,RETROK_F7},
	{ "C=" ,"c=" , RETROK_RCTRL},
	{ "Spc" ,"Spc",RETROK_SPACE},
	{ "<-" ,"<-"  ,RETROK_BACKSPACE},
	{ "Ent" ,"Ent",RETROK_RETURN},


	{ "DEL" ,"DEL" ,RETROK_DELETE}, //50
	{ "UP" ,"F3"   ,RETROK_UP},
	{ "CLR" ,"CLR" ,RETROK_CLEAR},
	{ "F5" ,"F5"  ,RETROK_F5},
	{ "SHL" ,"F6"  ,RETROK_LSHIFT},
	{ " @ " ," @ " , RETROK_AT},
	{ " ^ " ," ^ "  ,RETROK_CARET},	
	{ " : " ," : "  ,RETROK_COLON},
	{ " = "," = " ,RETROK_EQUALS},
	{ "SHR","SHR" ,RETROK_RSHIFT},


	{ "LF" ,"LF" , RETROK_LEFT },//60
	{ "Ent" ,"Ent" ,RETROK_RETURN},
	{ "RG" ,"RG"  ,RETROK_RIGHT},
	{ " 4" ," $"  ,RETROK_4 },
	{ " 5" ," %"  ,RETROK_5 },
	{ " 6" ," ^"  ,RETROK_6 },
	{ " 7" ," &"  ,RETROK_7 },
	{ " 8" ," *"  ,RETROK_8 },
	{ " 9" ," ("  ,RETROK_9 },
	{ " 0" ," )"  ,RETROK_0},

	{ "HOM","HOM" ,RETROK_HOME}, //70
	{ "DW" ,"DW" ,RETROK_DOWN}, 
	{ " - " ," - " , RETROK_MINUS},
	{ "CTL" ,"CTL" , RETROK_LCTRL},
	{ " * "," * " ,RETROK_RIGHTBRACKET},
	{ " + "," + " ,RETROK_PLUS},
	{ "AMP","AMP" ,RETROK_AMPERSAND},
	{ "Ins","Ins" ,RETROK_INSERT},
	{ "Hme","Hme" ,RETROK_HOME},
	{ "PgU","PgU" ,RETROK_PAGEUP},

	{ "Tab" ,"Tab",RETROK_TAB}, //80	
	{ " [" ,"  {" ,RETROK_LEFTBRACKET},
	{ " ]" ,"  }" ,RETROK_RIGHTBRACKET},
	{ "Ent" ,"Ent",RETROK_RETURN},
	{ "Del" ,"Del",RETROK_DELETE},
	{ "R/S" ,"End",RETROK_END},
	{ "c= " ,"c= ",RETROK_RCTRL},
	{ "SHL" ,"SHL",RETROK_LSHIFT},
	{ "SHR" ,"SHR" ,RETROK_RSHIFT},	
	{ "SPC" ,"SPC",RETROK_SPACE},	
	
	{ "PG1","PG1" ,-2},//90
	{ "JOY","JOY"   ,-14},
	{ "GUI","GUI"  ,-13},
	{ "CTR" ,"CTR" ,-12},
	{ "R/S" ,"R/S" ,-11},
	{ "SHI" ,"SHI" ,-10},
	{ "VFL","VFL",-5},
	{ "COL" ,"COL",-3},
	{ "Ent" ,"Ent",RETROK_RETURN},
	{ "KBD" ,"KBD",-4},

} ;
#endif

#endif
