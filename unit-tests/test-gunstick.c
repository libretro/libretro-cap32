/* Native core test: compile with -Ilibretro-common/include -Ilibretro -Icap32
 * (and -ldl on Linux), then run with the shared core path as argument.
 * Exercises the real PSG input path with synthetic pixels and two guns. */
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
SYM(t_lightgun*,gun);SYM(t_lightgun_cfg*,lightgun_cfg);SYM(t_CPC*,CPC);SYM(t_PPI*,PPI);SYM(t_PSG*,PSG);SYM(retro_video_t*,retro_video);
SYM(uint32_t*,colours);SYM(uint8_t*,keyboard_matrix);memset(keyboard_matrix,255,16);
*(retro_log_printf_t*)dlsym(h,"log_cb")=logfn;*(retro_input_state_t*)dlsym(h,"input_state_cb")=input;
uint32_t *pixels=calloc(EMULATION_SCREEN_WIDTH*EMULATION_SCREEN_HEIGHT,4);*(uint32_t**)dlsym(h,"video_buffer")=pixels;
void(*setup)(retro_video_depth_t)=dlsym(h,"video_setup");void(*prepare)(lightgun_type)=dlsym(h,"lightgun_prepare");void(*device)(unsigned,unsigned)=dlsym(h,"retro_set_controller_port_device");void(*update)(void)=dlsym(h,"gunstick_emulator_update");uint8_t(*readport)(reg_pair)=dlsym(h,"z80_IN_handler");
lightgun_cfg->guntype=LIGHTGUN_TYPE_GUNSTICK;colours[11]=0xffffff;colours[0]=0x666666;setup(DEPTH_24BPP);device(0,RETRO_DEVICE_AMSTRAD_LIGHTGUN);device(1,RETRO_DEVICE_AMSTRAD_LIGHTGUN);
PPI->control=0x10;PSG->control=0x40;PSG->reg_select=14;PSG->RegisterAY.Index[7]=0;
reg_pair port;port.w.l=0xf400;
xs[0]=-16384;xs[1]=16384;pressed[0]=pressed[1]=true;update();assert(calls[0]&&calls[1]);assert(gun[0].x<gun[1].x);
pixels[gun[0].y*EMULATION_SCREEN_WIDTH+gun[0].x]=colours[11];pixels[gun[1].y*EMULATION_SCREEN_WIDTH+gun[1].x]=0x111111;
CPC->keyboard_line=9;assert(readport(port)==0xed);CPC->keyboard_line=6;assert(readport(port)==0xef);
pressed[0]=false;pressed[1]=true;update();CPC->keyboard_line=9;assert((readport(port)&0x10)!=0);CPC->keyboard_line=6;assert((readport(port)&0x10)==0);
pixels[gun[1].y*EMULATION_SCREEN_WIDTH+gun[1].x]=colours[11];assert(readport(port)==0xed);
off[1]=true;pressed[1]=false;update();assert(gun[1].x==-1&&!gun[1].pressed);assert(readport(port)==0xff);
off[1]=false;xs[1]=32767;ys[1]=32767;update();assert(gun[1].x==EMULATION_SCREEN_WIDTH-1&&gun[1].y==EMULATION_SCREEN_HEIGHT-1);
xs[1]=-32768;update();assert(gun[1].x==-1);xs[1]=0;ys[1]=0;
retro_video->screen_crop=true;setup(DEPTH_24BPP);update();assert(gun[1].x==EMULATION_CROP+(retro_video->screen_render_width-1)/2);
device(1,RETRO_DEVICE_NONE);update();CPC->keyboard_line=6;assert(readport(port)==0xff);assert(!gun[1].pressed);device(1,RETRO_DEVICE_AMSTRAD_LIGHTGUN);
for(int depth=DEPTH_16BPP;depth<=DEPTH_24BPP;depth++){if(depth!=DEPTH_16BPP&&depth!=DEPTH_24BPP)continue;setup(depth);colours[11]=depth==DEPTH_16BPP?0xffff:0xffffff;colours[0]=depth==DEPTH_16BPP?0x632c:0x666666;prepare(LIGHTGUN_TYPE_GUNSTICK);pressed[1]=true;update();int n=gun[1].y*EMULATION_SCREEN_WIDTH+gun[1].x;memset(pixels,0x11,EMULATION_SCREEN_WIDTH*EMULATION_SCREEN_HEIGHT*4);if(depth==DEPTH_16BPP)((uint16_t*)pixels)[n]=0xffff;else pixels[n]=0xffffff;CPC->keyboard_line=6;assert(readport(port)==0xed);}
/* Crosshairs may overlap: neither may remain in the sensor framebuffer. */
void(*draw)(void)=dlsym(h,"lightgun_draw");void(*restore)(void)=dlsym(h,"lightgun_restore");
xs[0]=xs[1]=ys[0]=ys[1]=0;pressed[0]=pressed[1]=false;update();
size_t bytes=EMULATION_SCREEN_WIDTH*EMULATION_SCREEN_HEIGHT*4;
uint8_t *before=malloc(bytes);memcpy(before,pixels,bytes);draw();restore();assert(!memcmp(before,pixels,bytes));free(before);
/* Grey targets must work in the active pixel format too. */
colours[0]=0x666666;prepare(LIGHTGUN_TYPE_GUNSTICK);pressed[0]=true;update();pixels[gun[0].y*EMULATION_SCREEN_WIDTH+gun[0].x]=colours[0];CPC->keyboard_line=9;assert(readport(port)==0xed);
pressed[0]=true;update();pixels[gun[0].y*EMULATION_SCREEN_WIDTH+gun[0].x]=0;assert(readport(port)==0xef);pixels[gun[0].y*EMULATION_SCREEN_WIDTH+gun[0].x]=colours[0];assert(readport(port)==0xed);
/* Bright coloured target flashes must work without accepting dark colours. */
for (int depth=DEPTH_8BPP; depth<=DEPTH_24BPP; depth++) {
 setup(depth);
 uint32_t grey=depth==DEPTH_8BPP?0x6d:depth==DEPTH_16BPP?0x632c:0x666666;
 uint32_t white=depth==DEPTH_8BPP?0xff:depth==DEPTH_16BPP?0xffff:0xffffff;
 uint32_t yellow=depth==DEPTH_8BPP?0xd9:depth==DEPTH_16BPP?0xce6c:0xcccc66;
 uint32_t darkred=depth==DEPTH_8BPP?0x60:depth==DEPTH_16BPP?0x6000:0x660000;
 colours[0]=grey;colours[11]=white;prepare(LIGHTGUN_TYPE_GUNSTICK);
 for(unsigned p=0;p<2;p++) {
  pressed[p]=true;update();CPC->keyboard_line=p?6:9;
  int n=gun[p].y*EMULATION_SCREEN_WIDTH+gun[p].x;
  uint32_t samples[]={0,darkred,yellow,grey,white};
  for(unsigned i=0;i<5;i++) {
   update();
   if(depth==DEPTH_8BPP)((uint8_t*)pixels)[n]=samples[i];
   else if(depth==DEPTH_16BPP)((uint16_t*)pixels)[n]=samples[i];
   else pixels[n]=samples[i];
   assert(readport(port)==(i<2?0xef:0xed));
  }
 }
}
puts("PASS: black/dark red rejected, yellow/grey/white detected on both ports in RGB332/RGB565/XRGB8888");
prepare(LIGHTGUN_TYPE_NONE);assert(!CPC->gun_IN);assert(!gun[0].pressed&&!gun[1].pressed);
puts("PASS: independent positions/triggers/hits through PSG, release/offscreen, bounds/crop, detach, RGB565/XRGB8888, disable");free(pixels);return 0;}
