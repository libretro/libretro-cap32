
/* scancode key to CPC scan code mapping table */
const int keyboard_translation[512] =
{
	/* ST,  PC Code */
	-1,    /* 0 */ 	-1,    /* 1 */ 	-1,    /* 2 */ 	-1,    /* 3 */ 	-1,    /* 4 */ 	-1,    /* 5 */ 	-1,    /* 6 */ 	-1,    /* 7 */
	0x97,  /* CPCDEL SDLK_BACKSPACE=8 */
	0x84,  /* CPC SDLK_TAB=9 */
	-1,    /* 10 */ -1,    /* 11 */
	0x47,  /* CPC_CLR SDLK_CLEAR = 12 */
	0x06,  /* CPC SDLK_RETURN = 13 */
	-1,    /* 14 */	-1,    /* 15 */	-1,    /* 16 */ -1,    /* 17 */	-1,    /* 18 */
	-1,    /* SDLK_PAUSE = 19 */
	-1,    /* 20 */	-1,    /* 21 */	-1,    /* 22 */	-1,    /* 23 */	-1,    /* 24 */	-1,    /* 25 */	-1,    /* 26 */
	0x82,  /*CPC  SDLK_ESCAPE = 27 */
	-1,    /* 28 */	-1,    /* 29 */	-1,    /* 30 */	-1,    /* 31 */
	0x57,  /* CPC SDLK_SPACE = 32 */
	0x80,    /*CPCSH  SDLK_EXCLAIM = 33 */
	-1,    /* SDLK_QUOTEDBL = 34 */
	0x71,  /* CPC SDLK_HASH = 35 */
	0x70,    /*CPC  SDLK_DOLLAR = 36 */
	-1,    /* 37 */
	0x60,    /*CPC_AMPERSAND SDLK_AMPERSAND = 38 */
	0x51,  /* CPC SDLK_QUOTE = 39 */
	0x50,  /*CPC SDLK_LEFTPAREN = 40 */
	0x41,  /*CPC  SDLK_RIGHTPAREN = 41 */
	0x35,    /*CPC_ASTERISK SDLK_ASTERISK = 42 */
	0x34,  /* CPC SDLK_PLUS = 43 */
	0x47,  /* CPC SDLK_COMMA = 44 */
	0x31,  /* CPC SDLK_MINUS = 45 */
	0x34,  /* CPC SDLK_PERIOD = 46 */
	0x36,  /* CPC SDLK_SLASH = 47 */
         0x40,                   // CPC_0
         0x80,                   // CPC_1
         0x81,                   // CPC_2
         0x71,                   // CPC_3
         0x70,                   // CPC_4
         0x61,                   // CPC_5
         0x60,                   // CPC_6
         0x51,                   // CPC_7
         0x50,                   // CPC_8
         0x41,                   // CPC_9
	 0x35,    /*CPC SDLK_COLON = 58 */
	0x34,  /*CPC SDLK_SEMICOLON = 59 */
	0x47,  /*CPCSH SDLK_LESS = 60 */
	0x31,  /*CPCSH SDLK_EQUALS = 61 */
	0x37 ,  /*CPCSH SDLK_GREATER  = 62 */
	0x36,    /*CPCSH SDLK_QUESTION = 63 */
	0x32,    /*CPC SDLK_AT = 64 */
	-1,    /* 65 */  /* Skip uppercase letters */
	-1,    /* 66 */	-1,    /* 67 */	-1,    /* 68 */	-1,    /* 69 */	-1,    /* 70 */	-1,    /* 71 */	-1,    /* 72 */	-1,    /* 73 */	-1,    /* 74 */
	-1,    /* 75 */	-1,    /* 76 */ -1,    /* 77 */	-1,    /* 78 */	-1,    /* 79 */	-1,    /* 80 */	-1,    /* 81 */	-1,    /* 82 */	-1,    /* 83 */
	-1,    /* 84 */	-1,    /* 85 */	-1,    /* 86 */	-1,    /* 87 */	-1,    /* 88 */	-1,    /* 89 */	-1,    /* 90 */
	0x21,  /*CPC SDLK_LEFTBRACKET = 91 */
	0x26,  /* CPC SDLK_BACKSLASH = 92 */     /* Might be 0x60 for UK keyboards */
	0x23,  /* CPC SDLK_RIGHTBRACKET = 93 */
	0x30,  /* CPC SDLK_CARET = 94 */
	0x40,    /*CPCSH SDLK_UNDERSCORE = 95 */
	0x26,  /* CPCSH SDLK_BACKQUOTE = 96 */
         0x85,                   // CPC_a 97
         0x66,                   // CPC_b 98
         0x76,                   // CPC_c 99
         0x75,                   // CPC_d 100
         0x72,                   // CPC_e 101
         0x65,                   // CPC_f 102 
         0x64,                   // CPC_g 103  
         0x54,                   // CPC_h 104
         0x43,                   // CPC_i 105
         0x55,                   // CPC_j 106
         0x45,                   // CPC_k 107
         0x44,                   // CPC_l 108
         0x46,                   // CPC_m 109
         0x56,                   // CPC_n 110
         0x42,                   // CPC_o 111
         0x33,                   // CPC_p 112
         0x83,                   // CPC_q 113 
         0x62,                   // CPC_r 114
         0x74,                   // CPC_s 115
         0x63,                   // CPC_t 116
         0x52,                   // CPC_u 117
         0x67,                   // CPC_v 118
         0x73,                   // CPC_w 119
         0x77,                   // CPC_x 120
         0x53,                   // CPC_y 121
         0x87,                   // CPC_z 122
	-1,    /* 123 */	-1,    /* 124 */	-1,    /* 125 */	-1,    /* 126 */
	 0x97,  /*CPC SDLK_DELETE = 127 */
	/* End of ASCII mapped keysyms */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 128-143*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 144-159*/
	0x0d, 0x0c, 0x1a, 0x28, 0x27, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 160-175*/
	-1, -1, -1, -1, 0x0D, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 176-191*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 192-207*/
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x0C, /* 208-223*/
	-1, -1, -1, -1, 0x28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* 224-239*/
	-1, -1, -1, -1, -1, -1, 0x27, -1, -1, -1, -1, -1, 0x1A, -1, -1, -1, /* 240-255*/
	/* Numeric keypad: */
	 0x40,                   // CPC_0   /* SDLK_KP0 = 256 */
	0x80,                   // CPC_1    /* SDLK_KP1 = 257 */
	 0x81,                   // CPC_2    /* SDLK_KP2 = 258 */
	 0x71,                   // CPC_3    /* SDLK_KP3 = 259 */
	0x70, // CPC_4      /* SDLK_KP4 = 260 */
	 0x61,                   // CPC_5    /* SDLK_KP5 = 261 */
	0x60,                   // CPC_6   /* SDLK_KP6 = 262 */
	 0x51,                   // CPC_7 /* SDLK_KP7 = 263 */
	 0x50,                   // CPC_8    /* SDLK_KP8 = 264 */
	0x41,                   // CPC_9   /* SDLK_KP9 = 265 */
	0x37,                   // CPC_PERIOD    /* SDLK_KP_PERIOD = 266 */
	 0x36,                   // CPC_SLASH,    /* SDLK_KP_DIVIDE = 267 */
	   -1,    /* SDLK_KP_MULTIPLY = 268 */
	 0x31,    /*CPC SDLK_KP_MINUS = 269 */
	 0x34,    /*CPCSH SDLK_KP_PLUS = 270 */
	 0x22,    /* CPC SDLK_KP_ENTER = 271 */
	0x31,    /*CPC SH SDLK_KP_EQUALS = 272 */
	/* Arrows + Home/End pad */
	0x00,                   // CPC_CUR_UP    /* SDLK_UP = 273 */
	0x02,                   // CPC_CUR_DOWN    /* SDLK_DOWN = 274 */
	0x01,                   // CPC_CUR_RIGHT    /* SDLK_RIGHT = 275 */
	0x10,                   // CPC_CUR_LEFT    /* SDLK_LEFT = 276 */
	-1,    /* SDLK_INSERT = 277 */
	 0x10,    /*CPC_CTRL SDLK_HOME = 278 */
	-1,    /* SDLK_END = 279 */
	-1,    /* SDLK_PAGEUP = 280 */
	-1,    /* SDLK_PAGEDOWN = 281 */
	/* Function keys */
         0x15,                   // CPC_F1
         0x16,                   // CPC_F2
         0x05,                   // CPC_F3
         0x24,                   // CPC_F4
         0x14,                   // CPC_F5
         0x04,                   // CPC_F6
         0x12,                   // CPC_F7
         0x13,                   // CPC_F8
         0x03,                   // CPC_F9
         0x17,                   // CPC_F0
	-1,      /* SDLK_F11 = 292 */
	-1,      /* SDLK_F12 = 293 */
	-1,    /* SDLK_F13 = 294 */
	-1,      /* SDLK_F14 = 295 */
	-1,      /* SDLK_F15 = 296 */
	-1,      /* 297 */	-1,      /* 298 */	-1,      /* 299 */
	/* Key state modifier keys */
	-1,      /* SDLK_NUMLOCK = 300 */
	0x86,    /* CPC SDLK_CAPSLOCK = 301 */
	-1,    /* SDLK_SCROLLOCK = 302 */
	0x25,                   // CPC_RSHIFT    /* SDLK_RSHIFT = 303 */
	0x25,                   // CPC_LSHIFT/* SDLK_LSHIFT = 304 */
	0x27,                   // CPC_CONTROL    /* SDLK_RCTRL = 305 */
	0x27,                   // CPC_CONTROL   /* SDLK_LCTRL = 306 */
	-1,    /* SDLK_RALT = 307 */
	-1,    /* SDLK_LALT = 308 */
	-1,      /* SDLK_RMETA = 309 */
	-1,      /* SDLK_LMETA = 310 */
	-1,      /* SDLK_LSUPER = 311 */
	-1,      /* SDLK_RSUPER = 312 */
	-1,      /* SDLK_MODE = 313 */     /* "Alt Gr" key */
	-1,      /* SDLK_COMPOSE = 314 */
	/* Miscellaneous function keys */
	0x11,                   // CPC_COPY    /* SDLK_HELP = 315 */
	-1,    /* SDLK_PRINT = 316 */
	-1,      /* SDLK_SYSREQ = 317 */
	-1,      /* SDLK_BREAK = 318 */
	-1,      /* SDLK_MENU = 319 */
	0x30,      /* CPC SDLK_POWER = 320 */
	-1,      /* SDLK_EURO = 321 */
	-1     /* SDLK_UNDO = 322 */
};

