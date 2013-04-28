
PS3DEV := /usr/local/ps3dev
CELL_SDK := /root/.wine/drive_c/usr/local/cell

platform = ps3

ifeq ($(platform), ps3)
   TARGET := cap32_libretro_ps3.a
   CC = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
   CXX = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-g++.exe
   AR = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-ar.exe
   SHARED :=  
   ZLIB_DIR = ../utils/zlib/
   ZLIB_OBJECTS	:= $(ZLIB_DIR)adler32.o $(ZLIB_DIR)compress.o $(ZLIB_DIR)crc32.o $(ZLIB_DIR)deflate.o \
		$(ZLIB_DIR)gzclose.o $(ZLIB_DIR)gzlib.o $(ZLIB_DIR)gzread.o\
		$(ZLIB_DIR)gzwrite.o $(ZLIB_DIR)infback.o $(ZLIB_DIR)inffast.o $(ZLIB_DIR)inflate.o \
		$(ZLIB_DIR)trees.o $(ZLIB_DIR)inftrees.o $(ZLIB_DIR)uncompr.o $(ZLIB_DIR)uncompr.o $(ZLIB_DIR)zutil.o 
   CFLAGS += -DSDL_BYTEORDER=SDL_BIG_ENDIAN -DMSB_FIRST -DBYTE_ORDER=BIG_ENDIAN  -DBYTE_ORDER=BIG_ENDIAN -DWORDS_BIGENDIAN=1\
	 -D__CELLOS_LV2 -DPS3PORT=1 -D__PS3__ -I$(ZLIB_DIR)

endif

ifeq ($(DEBUG), 1)
   CFLAGS += -O0 -g
else
   CFLAGS += -O3
endif

EMU = ../cap32/capold-4.2.0

CORE_SRCS :=$(EMU)/z80.o $(EMU)/tape.o $(EMU)/psg.o $(EMU)/fdc.o $(EMU)/crtc.o $(EMU)/cap32.o 

BUILD_APP = $(ZLIB_OBJECTS)  $(CORE_SRCS) 


HINCLUDES := -I./$(EMU) -I./$(EMU)/include -I../libretro  

OBJECTS := $(BUILD_APP) ../libretro/libretro-cap32.o ../libretro/cap32-mapper.o ../libretro/vkbd.o \
	../libretro/graph.o ../libretro/diskutils.o ../libretro/fontmsx.o  
	
DEFINES += -DALIGN_DWORD

CFLAGS += $(DEFINES) -DRETRO=1  -O3 -finline-functions -ffast-math -fomit-frame-pointer -funsigned-char -s -fPIC
#-funroll-loops  -fsigned-char  -std=gnu99 \
#	-Wno-strict-prototypes -ffast-math -fomit-frame-pointer -fno-strength-reduce  -fno-builtin -finline-functions -s -fPIC
#-std=gnu++0x
CXXFLAGS  +=	$(CFLAGS) 
CPPFLAGS += $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJECTS)
#	$(CC) $(fpic) $(SHARED) $(INCLUDES) -o $@ $(OBJECTS) -lm -lpthread -lz 
	$(AR) rcs $@ $(OBJECTS) 
%.o: %.c
	$(CC) $(CFLAGS) $(HINCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean

