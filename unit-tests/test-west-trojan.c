/* Native core test: compile with -Ilibretro-common/include -Ilibretro -Icap32
 * (and -ldl on Linux), then run with the shared core path as argument.
 * Exercises West Phaser and Trojan Phazer through the real PSG input path. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>
#include "libretro.h"
#include "libretro-core.h"
#include "cap32.h"
#include "z80.h"
#include "gfx/video.h"
#include "retro_gun.h"
#include "lightgun/lightgun.h"
static int16_t xs[2],ys[2];static bool pressed[2],off[2];static unsigned calls[2];
static int16_t input(unsigned p,unsigned d,unsigned i,unsigned id){assert(p<2);if(d==RETRO_DEVICE_MOUSE)return 1;if(d!=RETRO_DEVICE_LIGHTGUN)return 0;calls[p]++;switch(id){case RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X:return xs[p];case RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y:return ys[p];case RETRO_DEVICE_ID_LIGHTGUN_TRIGGER:return pressed[p];case RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN:return off[p];}return 0;}
static void logfn(enum retro_log_level l,const char*f,...){}
#define SYM(type,name) type name=dlsym(h,#name);assert(name)
int main(int argc,char**argv){assert(argc==2);void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);assert(h);
SYM(t_lightgun*,gun);SYM(t_lightgun_cfg*,lightgun_cfg);SYM(t_CPC*,CPC);SYM(t_PPI*,PPI);SYM(t_PSG*,PSG);
SYM(uint32_t*,colours);SYM(uint8_t*,keyboard_matrix);memset(keyboard_matrix,255,16);
*(retro_log_printf_t*)dlsym(h,"log_cb")=logfn;*(retro_input_state_t*)dlsym(h,"input_state_cb")=input;
uint32_t *pixels=calloc(EMULATION_SCREEN_WIDTH*EMULATION_SCREEN_HEIGHT,4);*(uint32_t**)dlsym(h,"video_buffer")=pixels;
void(*setup)(retro_video_depth_t)=dlsym(h,"video_setup");void(*prepare)(lightgun_type)=dlsym(h,"lightgun_prepare");void(*device)(unsigned,unsigned)=dlsym(h,"retro_set_controller_port_device");uint8_t(*readport)(reg_pair)=dlsym(h,"z80_IN_handler");
lightgun_cfg->guntype=LIGHTGUN_TYPE_GUNSTICK;colours[11]=0xffffff;colours[0]=0x666666;setup(DEPTH_24BPP);device(0,RETRO_DEVICE_AMSTRAD_LIGHTGUN);device(1,RETRO_DEVICE_AMSTRAD_LIGHTGUN);
PPI->control=0x10;PSG->control=0x40;PSG->reg_select=14;PSG->RegisterAY.Index[7]=0;
reg_pair port;port.w.l=0xf400;
t_CRTC *CRTC=dlsym(h,"CRTC");t_VDU *VDU=dlsym(h,"VDU");
void (*phaser_update)(void)=dlsym(h,"phaser_emulator_update");
for(int depth=DEPTH_8BPP;depth<=DEPTH_24BPP;depth++) {
 setup(depth);memset(pixels,255,EMULATION_SCREEN_WIDTH*EMULATION_SCREEN_HEIGHT*4);CPC->scr_bpp=depth==DEPTH_8BPP?8:depth==DEPTH_16BPP?16:32;prepare(LIGHTGUN_TYPE_WEST_PHASER);
 assert(!CPC->gun_CRTC && !CPC->gun_OUT);
 CPC->keyboard_line=9;gun[0].x=160;gun[0].y=100;gun[0].pressed=false;
 CPC->scr_base=pixels;CPC->scr_pos=(uint32_t*)((uint8_t*)pixels+192*(CPC->scr_bpp/8));
 VDU->flag_drawing=1;VDU->scrln=99;assert(readport(port)==0xff);
 VDU->scrln=101;assert(readport(port)==0xfe);
 memset(pixels,0,EMULATION_SCREEN_WIDTH*EMULATION_SCREEN_HEIGHT*4);assert(readport(port)==0xff);
 memset(pixels,255,EMULATION_SCREEN_WIDTH*EMULATION_SCREEN_HEIGHT*4);
 VDU->flag_drawing=0;assert(readport(port)==0xff);VDU->flag_drawing=1;
 gun[0].pressed=true;assert(readport(port)==0xde);
 CPC->scr_pos=(uint32_t*)((uint8_t*)pixels+144*(CPC->scr_bpp/8));assert(readport(port)==0xdf);
 CPC->keyboard_line=6;assert(readport(port)==0xff);
 CPC->keyboard_line=9;off[0]=true;pressed[0]=true;phaser_update();assert(readport(port)==0xdf);
 off[0]=false;pressed[0]=false;phaser_update();assert(!gun[0].pressed);
 device(0,RETRO_DEVICE_NONE);assert(readport(port)==0xff);device(0,RETRO_DEVICE_AMSTRAD_LIGHTGUN);
 prepare(LIGHTGUN_TYPE_TROJAN_PHAZER);assert(CPC->gun_CRTC && !CPC->gun_OUT);
 CPC->model=CPC_MODEL_PLUS;gun[0].x=160;gun[0].y=100;gun[0].state=GUN_SHOOT;gun[0].pressed=true;
 CPC->scr_pos=(uint32_t*)((uint8_t*)pixels+160*(CPC->scr_bpp/8));VDU->scrln=100;
 CRTC->addr=0x3000;CRTC->char_count=19;CRTC->registers[16]=0;CRTC->registers[17]=0;
 assert(readport(port)==0xef);CPC->gun_CRTC();assert(CRTC->registers[16]==0x30 && CRTC->registers[17]==19);
 gun[0].state=GUN_PREPARE;CRTC->char_count++;CPC->gun_CRTC();assert(CRTC->registers[17]==19);
 CPC->keyboard_line=6;assert(readport(port)==0xff);CPC->keyboard_line=9;
 CPC->model=CPC_MODEL_6128;assert(readport(port)==0xff);gun[0].state=GUN_SHOOT;CPC->gun_CRTC();assert(CRTC->registers[17]==19);
 CPC->model=CPC_MODEL_PLUS;gun[0].x=-1;CPC->gun_CRTC();assert(CRTC->registers[17]==19);
 device(0,RETRO_DEVICE_NONE);assert(readport(port)==0xff);device(0,RETRO_DEVICE_AMSTRAD_LIGHTGUN);
}
prepare(LIGHTGUN_TYPE_NONE);assert(!CPC->gun_IN && !CPC->gun_CRTC && !CPC->gun_OUT);
free(pixels);puts("PASS: West sensor/trigger separation, raster transitions, Trojan latch and Plus gating, offscreen/detach, all pixel formats");return 0;}
