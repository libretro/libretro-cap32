#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define KDB_AZERTY 1
#define KBD_MAX_ENTRIES 143

#define MOD_CPC_SHIFT   (0x01 << 8)
#define MOD_CPC_CTRL    (0x02 << 8)
#define MOD_EMU_KEY     (0x10 << 8)

typedef enum {
   CPC_0,
   CPC_1,
   CPC_2,
   CPC_3,
   CPC_4,
   CPC_5,
   CPC_6,
   CPC_7,
   CPC_8,
   CPC_9,
   CPC_A,
   CPC_B,
   CPC_C,
   CPC_D,
   CPC_E,
   CPC_F,
   CPC_G,
   CPC_H,
   CPC_I,
   CPC_J,
   CPC_K,
   CPC_L,
   CPC_M,
   CPC_N,
   CPC_O,
   CPC_P,
   CPC_Q,
   CPC_R,
   CPC_S,
   CPC_T,
   CPC_U,
   CPC_V,
   CPC_W,
   CPC_X,
   CPC_Y,
   CPC_Z,
   CPC_a,
   CPC_b,
   CPC_c,
   CPC_d,
   CPC_e,
   CPC_f,
   CPC_g,
   CPC_h,
   CPC_i,
   CPC_j,
   CPC_k,
   CPC_l,
   CPC_m,
   CPC_n,
   CPC_o,
   CPC_p,
   CPC_q,
   CPC_r,
   CPC_s,
   CPC_t,
   CPC_u,
   CPC_v,
   CPC_w,
   CPC_x,
   CPC_y,
   CPC_z,
   CPC_AMPERSAND,
   CPC_ASTERISK,
   CPC_AT,
   CPC_BACKQUOTE,
   CPC_BACKSLASH,
   CPC_CAPSLOCK,
   CPC_CLR,
   CPC_COLON,
   CPC_COMMA,
   CPC_CONTROL,
   CPC_COPY,
   CPC_CPY_DOWN,
   CPC_CPY_LEFT,
   CPC_CPY_RIGHT,
   CPC_CPY_UP,
   CPC_CUR_DOWN,
   CPC_CUR_LEFT,
   CPC_CUR_RIGHT,
   CPC_CUR_UP,
   CPC_CUR_ENDBL,
   CPC_CUR_HOMELN,
   CPC_CUR_ENDLN,
   CPC_CUR_HOMEBL,
   CPC_DBLQUOTE,
   CPC_DEL,
   CPC_DOLLAR,
   CPC_ENTER,
   CPC_EQUAL,
   CPC_ESC,
   CPC_EXCLAMATN,
   CPC_F0,
   CPC_F1,
   CPC_F2,
   CPC_F3,
   CPC_F4,
   CPC_F5,
   CPC_F6,
   CPC_F7,
   CPC_F8,
   CPC_F9,
   CPC_FPERIOD,
   CPC_GREATER,
   CPC_HASH,
   CPC_LBRACKET,
   CPC_LCBRACE,
   CPC_LEFTPAREN,
   CPC_LESS,
   CPC_LSHIFT,
   CPC_MINUS,
   CPC_PERCENT,
   CPC_PERIOD,
   CPC_PIPE,
   CPC_PLUS,
   CPC_POUND,
   CPC_POWER,
   CPC_QUESTION,
   CPC_QUOTE,
   CPC_RBRACKET,
   CPC_RCBRACE,
   CPC_RETURN,
   CPC_RIGHTPAREN,
   CPC_RSHIFT,
   CPC_SEMICOLON,
   CPC_SLASH,
   CPC_SPACE,
   CPC_TAB,
   CPC_UNDERSCORE,
   CPC_J0_UP,
   CPC_J0_DOWN,
   CPC_J0_LEFT,
   CPC_J0_RIGHT,
   CPC_J0_FIRE1,
   CPC_J0_FIRE2,
   CPC_J1_UP,
   CPC_J1_DOWN,
   CPC_J1_LEFT,
   CPC_J1_RIGHT,
   CPC_J1_FIRE1,
   CPC_J1_FIRE2,
   CPC_ES_NTILDE,
   CPC_ES_nTILDE,
   CPC_ES_PESETA,
   CPC_FR_eACUTE,
   CPC_FR_eGRAVE,
   CPC_FR_cCEDIL,
   CPC_FR_aGRAVE,
   CPC_FR_uGRAVE,
   CAP32_FPS,
   CAP32_JOY,
   CAP32_INCY,
   CAP32_DECY,
   CAP32_RENDER,
   CAP32_LOAD,
   CAP32_SAVE,
   CAP32_RESET,
   CAP32_AUTOFIRE,
   CAP32_INCFIRE,
   CAP32_DECFIRE,
   CAP32_SCREEN,
} CPC_KEYS;

int kbd_layout[KBD_MAX_ENTRIES][2] = {
   // US PC to CPC keyboard layout translation
    { CPC_0,          '0' },
    { CPC_1,          '1' },
    { CPC_2,          '2' },
    { CPC_3,          '3' },
    { CPC_4,          '4' },
    { CPC_5,          '5' },
    { CPC_6,          '6' },
    { CPC_7,          '7' },
    { CPC_8,          '8' },
    { CPC_9,          '9' },
    { CPC_A,          'A' },
    { CPC_B,          'B' },
    { CPC_C,          'C' },
    { CPC_D,          'D' },
    { CPC_E,          'E' },
    { CPC_F,          'F' },
    { CPC_G,          'G' },
    { CPC_H,          'H' },
    { CPC_I,          'I' },
    { CPC_J,          'J' },
    { CPC_K,          'K' },
    { CPC_L,          'L' },
    { CPC_M,          'M' },
    { CPC_N,          'N' },
    { CPC_O,          'O' },
    { CPC_P,          'P' },
    { CPC_Q,          'Q' },
    { CPC_R,          'R' },
    { CPC_S,          'S' },
    { CPC_T,          'T' },
    { CPC_U,          'U' },
    { CPC_V,          'V' },
    { CPC_W,          'W' },
    { CPC_X,          'X' },
    { CPC_Y,          'Y' },
    { CPC_Z,          'Z' },
    { CPC_a,          'a' },
    { CPC_b,          'b' },
    { CPC_c,          'c' },
    { CPC_d,          'd' },
    { CPC_e,          'e' },
    { CPC_f,          'f' },
    { CPC_g,          'g' },
    { CPC_h,          'h' },
    { CPC_i,          'i' },
    { CPC_j,          'j' },
    { CPC_k,          'k' },
    { CPC_l,          'l' },
    { CPC_m,          'm' },
    { CPC_n,          'n' },
    { CPC_o,          'o' },
    { CPC_p,          'p' },
    { CPC_q,          'q' },
    { CPC_r,          'r' },
    { CPC_s,          's' },
    { CPC_t,          't' },
    { CPC_u,          'u' },
    { CPC_v,          'v' },
    { CPC_w,          'w' },
    { CPC_x,          'x' },
    { CPC_y,          'y' },
    { CPC_z,          'z' },
    { CPC_AMPERSAND,  '&' },
    { CPC_ASTERISK,   '*' },
    { CPC_AT,         '@' },
    { CPC_BACKQUOTE,  '`' },
    { CPC_BACKSLASH,  '\\' },
    { CPC_CAPSLOCK,   -10 },
    { CPC_CLR,        -11      },
    { CPC_COLON,      ':' },
    { CPC_COMMA,      ',' },
    { CPC_CONTROL,    -12  },
    { CPC_COPY,       -13     },
    { CPC_CPY_DOWN,   -1  },
    { CPC_CPY_LEFT,   -1  },
    { CPC_CPY_RIGHT,  -1  },
    { CPC_CPY_UP,     -1  },
    { CPC_CUR_DOWN,   -2  },
    { CPC_CUR_LEFT,   -4  },
    { CPC_CUR_RIGHT,  -6 },
    { CPC_CUR_UP,     -8    },
    { CPC_CUR_ENDBL,  -14  },
    { CPC_CUR_HOMELN, -15 },
    { CPC_CUR_ENDLN,  -16  },
    { CPC_CUR_HOMEBL, -17 },
    { CPC_DBLQUOTE,   '"' },
    { CPC_DEL,        -18   },
    { CPC_DOLLAR,     '$' },
    { CPC_ENTER,      '\n' },
    { CPC_EQUAL,      '=' },
    { CPC_ESC,        '\e'  },
    { CPC_EXCLAMATN,  '!' },
    { CPC_F0,         -20 },
    { CPC_F1,         -21 },
    { CPC_F2,         -22 },
    { CPC_F3,         -23 },
    { CPC_F4,         -24 },
    { CPC_F5,         -25 },
    { CPC_F6,         -26 },
    { CPC_F7,         -27 },
    { CPC_F8,         -28 },
    { CPC_F9,         -29 },
    { CPC_FPERIOD,    '.' },
    { CPC_GREATER,    '>' },
    { CPC_HASH,       '#' },
    { CPC_LBRACKET,   '[' },
    { CPC_LCBRACE,    '{' },
    { CPC_LEFTPAREN,  '(' },
    { CPC_LESS,       '<' },
    { CPC_LSHIFT,     -19 },
    { CPC_MINUS,      '-' },
    { CPC_PERCENT,    '%' },
    { CPC_PERIOD,     '.' },
    { CPC_PIPE,       '|' },
    { CPC_PLUS,       '+' },
    { CPC_POUND,      '$' },
    { CPC_POWER,      '^' },
    { CPC_QUESTION,   '?' },
    { CPC_QUOTE,      '\'' },
    { CPC_RBRACKET,   ']' },
    { CPC_RCBRACE,    '}' },
    { CPC_RETURN,     '\n'  },
    { CPC_RIGHTPAREN, ')' },
    { CPC_RSHIFT,     -30  },
    { CPC_SEMICOLON,  ';' },
    { CPC_SLASH,      '/' },
    { CPC_SPACE,      ' ' },
    { CPC_TAB,        '\t' },
    { CPC_UNDERSCORE, '_'  },
    { CAP32_RESET,    -1 },
    { CAP32_FPS,      -1 },
    { CAP32_JOY,      -1 },
    { CAP32_INCY,     -1 },
    { CAP32_DECY,     -1 },
    { CAP32_RENDER,   -1 },
    { CAP32_LOAD,     -1 },
    { CAP32_SAVE,     -1 },
    { CAP32_RESET,    -1 },
    { CAP32_AUTOFIRE, -1 },
    { CAP32_INCFIRE   -1 },
    { CAP32_DECFIRE   -1 },
    { CAP32_SCREEN,   -1 },
    { MOD_CPC_CTRL|CPC_ENTER, -31 }
  };

# define  CPC_MAX_KEY (CAP32_SCREEN+1)

   int cpc_kbd[CPC_MAX_KEY] = {
   // original CPC keyboard
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
       0x85 | MOD_CPC_SHIFT,   // CPC_A
       0x66 | MOD_CPC_SHIFT,   // CPC_B
       0x76 | MOD_CPC_SHIFT,   // CPC_C
       0x75 | MOD_CPC_SHIFT,   // CPC_D
       0x72 | MOD_CPC_SHIFT,   // CPC_E
       0x65 | MOD_CPC_SHIFT,   // CPC_F
       0x64 | MOD_CPC_SHIFT,   // CPC_G
       0x54 | MOD_CPC_SHIFT,   // CPC_H
       0x43 | MOD_CPC_SHIFT,   // CPC_I
       0x55 | MOD_CPC_SHIFT,   // CPC_J
       0x45 | MOD_CPC_SHIFT,   // CPC_K
       0x44 | MOD_CPC_SHIFT,   // CPC_L
       0x46 | MOD_CPC_SHIFT,   // CPC_M
       0x56 | MOD_CPC_SHIFT,   // CPC_N
       0x42 | MOD_CPC_SHIFT,   // CPC_O
       0x33 | MOD_CPC_SHIFT,   // CPC_P
       0x83 | MOD_CPC_SHIFT,   // CPC_Q
       0x62 | MOD_CPC_SHIFT,   // CPC_R
       0x74 | MOD_CPC_SHIFT,   // CPC_S
       0x63 | MOD_CPC_SHIFT,   // CPC_T
       0x52 | MOD_CPC_SHIFT,   // CPC_U
       0x67 | MOD_CPC_SHIFT,   // CPC_V
       0x73 | MOD_CPC_SHIFT,   // CPC_W
       0x77 | MOD_CPC_SHIFT,   // CPC_X
       0x53 | MOD_CPC_SHIFT,   // CPC_Y
       0x87 | MOD_CPC_SHIFT,   // CPC_Z
       0x85,                   // CPC_a
       0x66,                   // CPC_b
       0x76,                   // CPC_c
       0x75,                   // CPC_d
       0x72,                   // CPC_e
       0x65,                   // CPC_f
       0x64,                   // CPC_g
       0x54,                   // CPC_h
       0x43,                   // CPC_i
       0x55,                   // CPC_j
       0x45,                   // CPC_k
       0x44,                   // CPC_l
       0x46,                   // CPC_m
       0x56,                   // CPC_n
       0x42,                   // CPC_o
       0x33,                   // CPC_p
       0x83,                   // CPC_q
       0x62,                   // CPC_r
       0x74,                   // CPC_s
       0x63,                   // CPC_t
       0x52,                   // CPC_u
       0x67,                   // CPC_v
       0x73,                   // CPC_w
       0x77,                   // CPC_x
       0x53,                   // CPC_y
       0x87,                   // CPC_z
       0x60 | MOD_CPC_SHIFT,   // CPC_AMPERSAND
       0x35 | MOD_CPC_SHIFT,   // CPC_ASTERISK
       0x32,                   // CPC_AT
       0x26 | MOD_CPC_SHIFT,   // CPC_BACKQUOTE
       0x26,                   // CPC_BACKSLASH
       0x86,                   // CPC_CAPSLOCK
       0x20,                   // CPC_CLR
       0x35,                   // CPC_COLON
       0x47,                   // CPC_COMMA
       0x27 | MOD_CPC_CTRL,    // CPC_CONTROL
       0x11,                   // CPC_COPY
       0x02 | MOD_CPC_SHIFT,   // CPC_CPY_DOWN
       0x10 | MOD_CPC_SHIFT,   // CPC_CPY_LEFT
       0x01 | MOD_CPC_SHIFT,   // CPC_CPY_RIGHT
       0x00 | MOD_CPC_SHIFT,   // CPC_CPY_UP
       0x02,                   // CPC_CUR_DOWN
       0x10,                   // CPC_CUR_LEFT
       0x01,                   // CPC_CUR_RIGHT
       0x00,                   // CPC_CUR_UP
       0x02 | MOD_CPC_CTRL,    // CPC_CUR_ENDBL
       0x10 | MOD_CPC_CTRL,    // CPC_CUR_HOMELN
       0x01 | MOD_CPC_CTRL,    // CPC_CUR_ENDLN
       0x00 | MOD_CPC_CTRL,    // CPC_CUR_HOMEBL
       0x81 | MOD_CPC_SHIFT,   // CPC_DBLQUOTE
       0x97,                   // CPC_DEL
       0x70 | MOD_CPC_SHIFT,   // CPC_DOLLAR
       0x06,                   // CPC_ENTER
       0x31 | MOD_CPC_SHIFT,   // CPC_EQUAL
       0x82,                   // CPC_ESC
       0x80 | MOD_CPC_SHIFT,   // CPC_EXCLAMATN
       0x17,                   // CPC_F0
       0x15,                   // CPC_F1
       0x16,                   // CPC_F2
       0x05,                   // CPC_F3
       0x24,                   // CPC_F4
       0x14,                   // CPC_F5
       0x04,                   // CPC_F6
       0x12,                   // CPC_F7
       0x13,                   // CPC_F8
       0x03,                   // CPC_F9
       0x07,                   // CPC_FPERIOD
       0x37 | MOD_CPC_SHIFT,   // CPC_GREATER
       0x71 | MOD_CPC_SHIFT,   // CPC_HASH
       0x21,                   // CPC_LBRACKET
       0x21 | MOD_CPC_SHIFT,   // CPC_LCBRACE
       0x50 | MOD_CPC_SHIFT,   // CPC_LEFTPAREN
       0x47 | MOD_CPC_SHIFT,   // CPC_LESS
       0x25 | MOD_CPC_SHIFT,   // CPC_LSHIFT
       0x31,                   // CPC_MINUS
       0x61 | MOD_CPC_SHIFT,   // CPC_PERCENT
       0x37,                   // CPC_PERIOD
       0x32 | MOD_CPC_SHIFT,   // CPC_PIPE
       0x34 | MOD_CPC_SHIFT,   // CPC_PLUS
       0x30 | MOD_CPC_SHIFT,   // CPC_POUND
       0x30,                   // CPC_POWER
       0x36 | MOD_CPC_SHIFT,   // CPC_QUESTION
       0x51 | MOD_CPC_SHIFT,   // CPC_QUOTE
       0x23,                   // CPC_RBRACKET
       0x23 | MOD_CPC_SHIFT,   // CPC_RCBRACE
       0x22,                   // CPC_RETURN
       0x41 | MOD_CPC_SHIFT,   // CPC_RIGHTPAREN
       0x25 | MOD_CPC_SHIFT,   // CPC_RSHIFT
       0x34,                   // CPC_SEMICOLON
       0x36,                   // CPC_SLASH
       0x57,                   // CPC_SPACE
       0x84,                   // CPC_TAB
       0x40 | MOD_CPC_SHIFT,   // CPC_UNDERSCORE
       0x90,                   // CPC_J0_UP
       0x91,                   // CPC_J0_DOWN
       0x92,                   // CPC_J0_LEFT
       0x93,                   // CPC_J0_RIGHT
       0x94,                   // CPC_J0_FIRE1
       0x95,                   // CPC_J0_FIRE2
       0x60,                   // CPC_J1_UP
       0x61,                   // CPC_J1_DOWN
       0x62,                   // CPC_J1_LEFT
       0x63,                   // CPC_J1_RIGHT
       0x64,                   // CPC_J1_FIRE1
       0x65,                   // CPC_J1_FIRE2
       0xff,                   // CPC_ES_NTILDE
       0xff,                   // CPC_ES_nTILDE
       0xff,                   // CPC_ES_PESETA
       0xff,                   // CPC_FR_eACUTE
       0xff,                   // CPC_FR_eGRAVE
       0xff,                   // CPC_FR_cCEDIL
       0xff,                   // CPC_FR_aGRAVE
       0xff,                   // CPC_FR_uGRAVE
       0xff,                   // CAP32_FPS,
       0xff,                   // CAP32_JOY,
       0xff,                   // CAP32_INCY,
       0xff,                   // CAP32_DECY,
       0xff,                   // CAP32_RENDER,
       0xff,                   // CAP32_LOAD,
       0xff,                   // CAP32_SAVE,
       0xff,                   // CAP32_RESET,
       0xff,                   // CAP32_AUTOFIRE
       0xff,                   // CAP32_INCFIRE
       0xff,                   // CAP32_DECFIRE
       0xff                    // CAP32_SCREEN
  };

int cpc_get_key_from_ascii(char ascii)
{
  int index;
  for (index = 0; index < KBD_MAX_ENTRIES; index++) {
   if (kbd_layout[index][1] == ascii) return kbd_layout[index][0];
  }
  return -1;
}

char kbd_feedbuf[255];
int kbd_feedbuf_pos;
bool kbd_runcmd=false;
int kbd_mode = 0;

extern void play_tape();

static uint8_t bit_values2[8] = {
   0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

extern uint8_t keyboard_matrix[16];

void kbd_buf_feed(char *s) {
	strcpy(kbd_feedbuf, s);
	kbd_feedbuf_pos=0;
	kbd_runcmd=true;
	//printf("cmd:%s\n",s);
}

void kbd_buf_clean(){
   kbd_runcmd=false;
   memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix));
}

void kbd_buf_update() {

	static int old=0;

	//handle special case for playtape
	if( kbd_feedbuf[kbd_feedbuf_pos]=='^' ) {
		kbd_feedbuf_pos++;
		play_tape();
		return;
	}

	if( (kbd_feedbuf[kbd_feedbuf_pos]!=0) && old==0) {

     	int cpc_key = cpc_kbd[cpc_get_key_from_ascii(kbd_feedbuf[kbd_feedbuf_pos])];
//		printf("kpress:%2x (%x,%x) ,",cpc_key,cpc_key >> 4,cpc_key & 7);

		if(cpc_key & MOD_CPC_SHIFT)
			keyboard_matrix[0x25 >> 4] &= ~bit_values2[0x25 & 7]; // key needs to be SHIFTed

		keyboard_matrix[(unsigned char)cpc_key >> 4] &= ~bit_values2[(unsigned char)cpc_key & 7]; // key is being held down
		old=kbd_feedbuf[kbd_feedbuf_pos];

	}
	else if( (kbd_feedbuf[kbd_feedbuf_pos]!=0)  && old!=0){

		int cpc_key = cpc_kbd[cpc_get_key_from_ascii(old)];//&0xff;
//		printf("krelea:%2x \n",cpc_key);

		if(cpc_key & MOD_CPC_SHIFT)
			keyboard_matrix[0x25 >> 4] |= bit_values2[0x25 & 7]; // make sure key is unSHIFTed

		keyboard_matrix[(unsigned char)cpc_key >> 4] |= bit_values2[(unsigned char)cpc_key & 7];

		old=0;
		kbd_feedbuf_pos++;
	}
	else if(kbd_feedbuf[kbd_feedbuf_pos]=='\0')
   {
      kbd_buf_clean();
   }

}

//FIXME VIRTULAL KBD HANDLE
extern int SHIFTON;

void vkbd_key(int key,int pressed){

	//printf("key(%x)=%x shift:%d\n",key,pressed,SHIFTON);
	if(pressed){

		if(SHIFTON==1)
			keyboard_matrix[0x25 >> 4] &= ~bit_values2[0x25 & 7]; // key needs to be SHIFTed

		keyboard_matrix[(unsigned char)key >> 4] &= ~bit_values2[(unsigned char)key & 7]; // key is being held down

	}
	else {
		if(SHIFTON==1)
			keyboard_matrix[0x25 >> 4] |= bit_values2[0x25 & 7]; // make sure key is unSHIFTed

		keyboard_matrix[(unsigned char)key >> 4] |= bit_values2[(unsigned char)key & 7];

	}

}

void kbd_update_table(int lang) {
   switch (lang)
   {
      case KDB_AZERTY:
         kbd_layout[CPC_A][1] = 'Q';
         kbd_layout[CPC_Q][1] = 'A';
         kbd_layout[CPC_W][1] = 'Z';
         kbd_layout[CPC_Z][1] = 'W';
         kbd_layout[CPC_M][1] = '*';
         kbd_layout[CPC_ASTERISK][1] = 'M';
         kbd_layout[CPC_HASH][1] = '"';
         kbd_layout[CPC_DBLQUOTE][1] = '#';
         cpc_kbd[CPC_0] = 0x40 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_1] = 0x80 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_2] = 0x81 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_3] = 0x71 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_4] = 0x70 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_5] = 0x61 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_6] = 0x60 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_7] = 0x51 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_8] = 0x50 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_9] = 0x41 | MOD_CPC_SHIFT;
         cpc_kbd[CPC_HASH] = 0x71; // CPC_DBLQUOTE
         kbd_mode = KDB_AZERTY;
         break;

      default:
         if (kbd_mode != KDB_AZERTY)
            return;

         kbd_layout[CPC_A][1] = 'A';
         kbd_layout[CPC_Q][1] = 'Q';
         kbd_layout[CPC_W][1] = 'W';
         kbd_layout[CPC_Z][1] = 'Z';
         kbd_layout[CPC_M][1] = 'M';
         kbd_layout[CPC_ASTERISK][1] = '*';
         kbd_layout[CPC_HASH][1] = '#';
         kbd_layout[CPC_DBLQUOTE][1] = '"';
         cpc_kbd[CPC_0] = 0x40;
         cpc_kbd[CPC_1] = 0x80;
         cpc_kbd[CPC_2] = 0x81;
         cpc_kbd[CPC_3] = 0x71;
         cpc_kbd[CPC_4] = 0x70;
         cpc_kbd[CPC_5] = 0x61;
         cpc_kbd[CPC_6] = 0x60;
         cpc_kbd[CPC_7] = 0x51;
         cpc_kbd[CPC_8] = 0x50;
         cpc_kbd[CPC_9] = 0x41;
         cpc_kbd[CPC_HASH] = 0x71 | MOD_CPC_SHIFT;
         kbd_mode = 0;
         break;
   }
}

